/* 
 * Given maps at two adjacent IRAS bands make Temperature
 * and Tau maps from them.
 * Adapted from modifications of pop at ipac done by
 * rick@ipac.caltech.edu  (Rick Ebert, 1987)
 * 
 * modified for use with ImageOp() and im() and documented. 
 * Wed Jan 18 09:51:45 EST 1995  -- MWP
 *
 * All tables included herein are taken from from IRAS Exp. Supp. 1986 p. VI-26.
 * see also: Langer etal , 1989, ApJ, 337, 355.
 * 
 */

# include "../stacks/C.h"
#include "C.h"
#include "../graphics/C.h"
#include "../main/dragon.h"
#include <math.h>
#include <string.h>
#define bcopy(s, d, n) memcpy(d, s, n)
#define bzero(s, n) memset(s, 0, n)
#define DEBUG 0
#if DEBUG
#include <stdio.h>
#endif

static float banddelta[4] = {13.48E12, 5.16E12, 2.58E12, 1.00E12};
/* banddeltas from IRAS Exp Supp, page X-13 */
static float bandwave[4] = { 1.2E-5, 2.5E-5, 6.0E-5, 1.0E-4	};


#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* taut.c */
static int locate P_((double x, float table[], int n));
static float interp P_((int i, double x, float array1[], float array2[]));
static int GetBand P_((struct of_imagehdr *ihp));
#undef P_

/* These values are used by the local subroutines and ImageOp() */
int lband, sband;
double ccfact, tempfact;

void taut(sim , lim, power, oper)
int sim, lim, oper;	/* image number of short and long wavelength input */
double power;		/* power law for optical depth */
                        /*oper = which operation TEMP, CC, TAU */

{
	struct of_imagehdr *sihp, *lihp, *tauihp, *tempihp;
	double bpfact, waverat;

	/* Do a bunch of checks of the input images */
	lihp = imageHdr[lim];
	sihp = imageHdr[sim];
	/* Note, the band numbers used here are IRAS band numbers in [1,4] */
	lband = GetBand(lihp);
	sband = GetBand(sihp);

	switch(oper) { /* these checks depend on the operation being performed */
	case TEMP:
	   if(lband < sband) 
		error("Long Band < Short Band. Switch the input images.\n");
	   if(lband - sband != 1)
		error("Input images must be adjacent IRAS bands");
	   if(lihp->naxis < 3 || sihp->naxis < 3 ||
		lihp->naxis3 != 1 || sihp->naxis3 != 1 ||
		strcmp(lihp->ctype3, sihp->ctype3) ||
		strcmp(lihp->ctype3, "LAMBDA") )
			error("Axis 3 must be 'LAMBDA' and NAXIS3 must be 1");
	   if(strcmp(lihp->bunit, sihp->bunit) ||
		(strcmp(lihp->bunit, "W/M2/SR") &&
		 strcmp(lihp->bunit, "W/M2") &&
		 strcmp(lihp->bunit, "JY/SR") &&
		 strcmp(lihp->bunit, "MJY/SR") &&
		 strcmp(lihp->bunit, "MJY") &&
		 strcmp(lihp->bunit, "JY")) )
			error("BUNIT must be W/M2/SR, W/M2, [M]JY/SR, or [M]JY");
	   break;
	case CC:
	case TAU:
	   if(lihp->naxis < 3 || sihp->naxis < 3 ||
		lihp->naxis3 != 1 || sihp->naxis3 != 1 ||
		strcmp(lihp->ctype3, sihp->ctype3) ||
		strcmp(lihp->ctype3, "LAMBDA") )
			error("Axis 3 must be 'LAMBDA' and NAXIS3 must be 1");
	   if( (strcmp(sihp->bunit, "W/M2/SR") &&
		 strcmp(sihp->bunit, "W/M2") &&
		 strcmp(sihp->bunit, "JY/SR") &&
		 strcmp(sihp->bunit, "MJY/SR") &&
		 strcmp(sihp->bunit, "MJY") &&
		 strcmp(sihp->bunit, "JY")) )
			error("BUNIT of Flux image must be W/M2/SR, W/M2, [M]JY/SR, or [M]JY");
	   if(strcmp(lihp->bunit, "K") && strncmp(lihp->bunit, "KELVIN",6) )
			error("BUNIT of Temperature image must be K or KELVIN");
	   break;
	default: /* this should never happen */
		error_("Internal error: no operation! Consult a programmer!");
		break;
	}

	/* bpfact is the ratio of band passes. */
	if(strcmp(sihp->bunit,"W/M2/SR") == 0 ||
	    strcmp(sihp->bunit,"W/M2") == 0 ) {
	/* bpfact converts W/M2 to JY */
		bpfact = banddelta[lband - 1] / banddelta[sband-1];
		ccfact = 1.0E26 / banddelta[sband - 1];
	} else if(strcmp(sihp->bunit,"MJY") == 0 ||
                  strcmp(sihp->bunit,"MJY/SR") == 0 ) {
		     ccfact = 1.0E6;
		     bpfact = 1.0; /* for temperature, pixels are divided, so Mega
				    * cancels out */
		  }
	        else 
		     ccfact = bpfact = 1.0;

	(void)printf(" Opacity power law = %.3f\n",power);
	waverat = pow((bandwave[sband-1]/bandwave[lband - 1]), power);

	/* tempfact is the multiplicative factor to be applied to each pixel
	 * before  looking up temperature */
	tempfact = bpfact / waverat;
	return;
}


#define ASIZE 51          /* length of color correction tables */

int r2temp(pic1, pic2, factor, band, rpic)
/* pic1 - short band flux image
 * pic2 - long band flux image
 * factor is bandpass factor 
 * rpic - output image
 */
double pic1, pic2, factor;  
float *rpic;
int band;
{
/*
 * aratio: the ratio of the flux densities before color-correction, as a function
 *         temperature.  aratio[0] = 12/25, aratio[1] = 25/60, aratio[2] = 60/100.
 *         Temperatures are as given in temp[].
 *         This is the quantity "R" in Langer etal 1989 eqn A5.
 */
	static float aratio[3][ASIZE] =  {
		/* 12 micron/25 micron*/
		{4.345, 4.172, 4.086, 3.944, 3.666, 2.891, 2.545, 2.036, 1.692, 1.272,
		0.785, 0.734, 0.684, 0.633, 0.583, 0.534, 0.486, 0.438, 0.392, 0.347,
		0.304, 0.263, 0.224, 0.188, 0.154, 0.124, 0.097, 0.073, 0.053, 0.036,
		0.023, 0.018, 0.014, 0.010, 0.007, 0.005, 0.003, 0.002, 0.001, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	        /* 25 micron/60 micron*/
		{6.050, 5.931, 5.872, 5.773, 5.578, 5.005, 4.730, 4.287, 3.950, 3.478,
		2.780, 2.693, 2.602, 2.506, 2.407, 2.304, 2.196, 2.084, 1.967, 1.845,
		1.719, 1.589, 1.455, 1.317, 1.176, 1.034, 0.892, 0.751, 0.614, 0.484,
		0.363, 0.307, 0.256, 0.208, 0.165, 0.127, 0.095, 0.067, 0.045, 0.028,
		0.016, 0.008, 0.003, 0, 0, 0, 0, 0, 0, 0, 0},
		/* 60 micron/100 micron*/
		{3.350, 3.327, 3.316, 3.297, 3.259, 3.145, 3.088, 2.995, 2.920, 2.810,
		2.630, 2.606, 2.580, 2.553, 2.523, 2.491, 2.457, 2.420, 2.381, 2.338,
		2.291, 2.240, 2.184, 2.124, 2.057, 1.983, 1.901, 1.810, 1.709, 1.595,
		1.468, 1.400, 1.326, 1.249, 1.168, 1.082, 0.993, 0.898, 0.801, 0.700,
		0.597, 0.493, 0.391, 0.3252, 0.2881, 0.2263, 0.1877, 0.13202, 0.1005,
		0.0597, 0.0392}
	};	

/*
 * asiz: index of last meaningful number in aratio vector (e.g. 12/25 cuts off 
 *       at 60 K).
 */
	static int asiz[3] = {
		39,43,51	};

/* temp: the temperatures in Kelvin */
	static float temp[ASIZE] = {
		10000, 5000, 4000, 3000, 2000, 1000, 800, 600, 500, 400, 300, 290,
		280, 270, 260, 250, 240, 230, 220, 210, 200, 190, 180, 170, 160,
		150, 140, 130, 120, 110, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55,
		50, 45, 40, 37, 35, 32, 30, 27, 25, 22, 20
	};	


	int index;
	double ratio;

	ratio = (pic1 / pic2) * factor;
#if DEBUG
	fprintf(stderr,"r2temp: pic1 = %f pic2 = %f factor = %f ratio = %f\n", pic1, pic2, factor, ratio);
#endif /*DEBUG*/
	index = locate(ratio, aratio[band-1], asiz[band-1]);
	if(index == -1 || index == asiz[band-1] - 1){
		*rpic = FDRAGON;
		return(1);  /* Pixel is blank */
	} else
		*rpic = interp(index, ratio, aratio[band-1], temp);
#if DEBUG 
	fprintf(stderr,"r2temp: ratio = %f, result = %f\n",ratio, *rpic);
#endif  /*DEBUG*/
	return(0); /* Pixel is valid */

}

/*
 * cc  Perform colour correction on IRAS flux maps
 * given flux map and temperature map
 * using the table included in the code.
 * rick@ipac.caltech.edu  (Rick Ebert, 1987)
 */

#define  KSIZE 51

int cc(pic1, pic2, mfactor, band, rpic)
double  pic1, pic2, mfactor; /* pic1 -flux, pic2 - temperature */
float *rpic;                 /* mfactor = ccfact - units correction */
int band;                    /* rpic - output image, band - band number of pic1 */
{
/*
 * kfact: the color-correction factor to multiply the image pic1 by.
 *        kfact[0] = 12 micron , kfact[1] = 25 micron
 *        kfact[2] = 60 micron , kfact[3] = 100 micron
 *        As with aratio[], the second index is matched to temperature
 *        in temp[]. 
 */
	static float kfact[4][KSIZE] =  {
		/* 12 micron */
		{1.45, 1.43, 1.42, 1.41, 1.38, 1.27, 1.22, 1.15, 1.09, 1.01,
		0.92, 0.91, 0.90, 0.89, 0.88, 0.87, 0.86, 0.85, 0.85, 0.84,
		0.83, 0.83, 0.83, 0.83, 0.84, 0.85, 0.87, 0.90, 0.94, 1.01,
		1.12, 1.19, 1.28, 1.39, 1.54, 1.74, 2.01, 2.40, 2.97, 3.86,
		5.35, 8.09, 13.79, 0,   0,    0,    0,    0,    0 ,   0,
		0},
		/* 25 micron */
		{1.41, 1.40, 1.40, 1.39, 1.38, 1.34, 1.32, 1.29, 1.26, 1.22,
		1.15, 1.15, 1.14, 1.13, 1.12, 1.11, 1.09, 1.08, 1.07, 1.06,
		1.04, 1.02, 1.01, 0.99, 0.97, 0.95, 0.93, 0.91, 0.89, 0.86,
		0.84, 0.83, 0.83, 0.82, 0.81, 0.81, 0.81, 0.82, 0.83, 0.86,
		0.90, 0.97, 1.08, 0,    0,    0,    0,    0,    0 ,   0,
		0},
		/* 60 micron */
		{1.32, 1.32, 1.31, 1.31, 1.31, 1.29, 1.28, 1.27, 1.26, 1.24,
		1.21, 1.21, 1.20, 1.20, 1.19, 1.19, 1.18, 1.18, 1.17, 1.16,
		1.16, 1.15, 1.14, 1.13, 1.12, 1.11, 1.09, 1.08, 1.06, 1.04,
		1.02, 1.01, 1.00, 0.99, 0.97, 0.96, 0.95, 0.94, 0.93, 0.92,
		0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00,
		1.01},
		/* 100 micron */
		{1.09, 1.09, 1.09, 1.09, 1.09, 1.08, 1.08, 1.08, 1.08, 1.08,
		1.07, 1.07, 1.07, 1.07, 1.07, 1.07, 1.07, 1.07, 1.07, 1.06,
		1.06, 1.06, 1.06, 1.06, 1.06, 1.05, 1.05, 1.05, 1.04, 1.04,
		1.04, 1.03, 1.03, 1.03, 1.02, 1.02, 1.01, 1.01, 1.00, 1.00,
		0.99, 0.98, 0.98, 0.98, 0.97, 0.97, 0.97, 0.98, 0.98, 0.99,
		1.00}
	};	


/*
 * ksiz: index of last meaningful number in kfact vector 
*/
	static int ksiz[4] = {
		43, 43, KSIZE, KSIZE	};

/* temp: the temperatures in Kelvin */
	static float temp[KSIZE] = {
		10000, 5000, 4000, 3000, 2000, 1000, 800, 600, 500, 400, 300, 290,
		280, 270, 260, 250, 240, 230, 220, 210, 200, 190, 180, 170, 160,
		150, 140, 130, 120, 110, 100, 95, 90, 85, 80, 75, 70, 65, 60, 55,
		50, 45, 40, 37, 35, 32, 30, 27, 25, 22, 20
	};	


	int index;

/* locate the nearest temperature to the currect pic2 pixel */
	index = locate(pic2, temp, ksiz[band - 1]);

/* blank it or do the correction */
	if(index == -1 || index == ksiz[band - 1] - 1){
		*rpic = FDRAGON;
		return(1);  /* Pixel is blank */
	} else 
		*rpic = pic1 * mfactor / interp(index, pic2, temp, kfact[band - 1]);
#if DEBUG
 	fprintf(stderr,"cc: flux = %e, temp = %e, result = %e\n", pic1, pic2, *rpic);
#endif  /*DEBUG*/
	return(0); /* Pixel is valid */

}

/*  taum:  Calculate a Tau (opacity) map from the color corrected input image1
 *  and the temperature map image2
 *
 *  opacity (rpic) is Flux(lambda)*(lambda^3)*[exp(hc/(lambda*k*T))-1]
 *
 * rick@ipac.caltech.edu  (Rick Ebert, 1987)
 */

#define  ACONST  3.962348E19 /* ????? MWP */
#define  HCK	 1.4388E4 /* hc/k (micron^{-1}) */

int taum(pic1, pic2, band, rpic)
double  pic1, pic2;
float *rpic;
int band;
{
	double expo, t;
/*                     wavelength,  wavelength^3  (micron units) */
	static double lambda = 0.0, lambda3 = 0.0; 

		switch(band){
		case 1:
			lambda = 12.;
			lambda3 = 1.728E3;
			break;
		case 2:
			lambda = 25.;
			lambda3 = 1.5625E4;
			break;
		case 3:
			lambda = 60.;
			lambda3 = 2.16E5;
			break;
		case 4:
			lambda = 100.;
			lambda3 = 1.0E6;
			break;

		default:
			perror("taum: Bad band number on input image");
			exit(1);
		}


	expo = HCK / lambda / pic2;
	if(expo > 80.) /* why 80 ? MWP */
		return(1);
	t  = exp(expo);
	*rpic  = pic1 * lambda3 / ACONST * (t - 1.);
#if DEBUG
	fprintf(stderr,"lambda=%f, expo=%f, t=%f, ",lambda,expo,t);
	fprintf(stderr,"taum: flux = %e, temp = %e, result = %e\n", pic1, pic2, *rpic);
#endif /*DEBUG*/
	return(0); /* Pixel is valid */

}

/* Locate: Given an array, table, of length n, and given a value x
 * returns a value, base, such that x is between table[base] and
 * table[base+1].  Table must be monotonic, either increasing or
 * decreasing.  base = -1 or base = n-1 is returned when x is
 * out of range.
 *
 * From W.H.Press et.al.,Numerical Recipes, Cambridge U. Press (1986)p90
 * Translated to C from fortran by R. Ebert (1987)
 * rick@ipac.caltech.edu
 *
 * cleaned up by rww 3/89
 * 
 */

static int locate(x, table, n)
double x;
float table[];
int n;
{
	int base, center, upper;
	int ascend;

	base = -1;
	upper = n;
	ascend = table[n-1] > table[0];

	while(upper - base > 1) {
		center = (upper + base) /2; /* compute a midpoint */

#if DEBUG
		fprintf(stderr,"Center = %d, Base = %d, x = %f, Table[center] = %f\n",
			center, base, x, table[center]);
#endif /*DEBUG*/


		if((x > table[center]) == ascend)
			base = center;
		else
			upper = center;

	}
	return(base);

}

/*
 * interp  Interpolate a value from a table.
 * The value of index has already been determined by a call to
 * locate
 */

static float interp(i, x, array1, array2)
int i;
double x;
float array1[], array2[];
{
	float y;

	y = ((array2[i+1] - array2[i]) * ( x - array1[i]) /
	    (array1[i+1] - array1[i])) + array2[i];

#if 0
	fprintf(stderr,"Interp: given %f, returning %f\n",x ,y);
#endif /*0*/
	return(y);
}

static int GetBand(ihp)
struct of_imagehdr *ihp;
{
	int band;

	for(band = 0; band < 4; band++) {
		if( !cifdif(ihp->crval3, bandwave[band], bandwave[band] / 100.))
			return(band + 1);
	}
	error("Input image %s doesn't look like an IRAS band",ihp->fname);
}
