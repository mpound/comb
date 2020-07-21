/* gf.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"

#define WIDFTR 2.35482
static struct {
    float a[3], sigsq;
} gfcom_1;


#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* gf.c */
static double gauss_ P_((int *nx));
static double chisq_ P_((int *nfrom, int *nto));
static void matinv_ P_((double *array, int *norder));
#undef P_

/* Table of constant values */

static int c__1 = CHAN;
static int c__3 = VEL;
static double c_b7 = 0.;

void gf_()
{
    /* System generated locals */
    int i__1, i__2, i__3;
    float r__1;
    double d__1;

    /* Local variables */
    float area, beta[3];
    float gfun, fwhm;
    int i, j, k;
    float alpha[9]	/* was [3][3] */, hfact;
    int nchan;
    float fpeak, z;
    int nfree;
    float sigma[3];
    extern double chisq_();
    float deriv[3];
    float anorm[9]	/* was [3][3] */, fperr;
    double array[9]	/* was [3][3] */;
    double gauss_();
    int nfrom;
    int npass;
    float fwerr;
    double rfrom, rtemp;
    float z2, aa[3], flamda;
    double chiold;
    double chisqr;
    static int csigsv;
    /* Subroutine */ void matinv_();
    int nterms;
    int nto;
    double rto;
    int nsn1;
    static char title[][5] = {" ch ", "MHz ", "Km/s"};
    char *titlePtr;
#include "gf.dc.h"


/*   gf=Gaussian Fit performs a chi**2 fit of the data in stack 1 to */
/*   		gauss = a(1)*exp(-.5*((n-a(2))/a(3))**2) */

/* This Gaussian fit program is taken from Bevington's CURFIT (prog 11-5).  */
/* array definitions: a(j)=Gaussian parameter (amplitude, mean, & dispersi on)*/
/* 		     aa(j)=copy of a(j) used in error analysis */
/* 		  sigma(j)=1 sigma error in a(j) */
/* 		array(j,k)=modified curvature matrix and its inverse. */
/* 		  deriv(j)=partial derivatives of current Gaussian at chan i */

#include "gf.ec.h"

    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }
    pltoch_(h1, h2, &nfrom, &nto);
/*  set reasonable starting values */
    i = (nfrom + nto) / 2;
    gfcom_1.a[0] = stk_[0].stak[i - 1];
    gfcom_1.a[1] = (float) i;
    i__1 = nto;
    for (i = nfrom; i <= i__1; ++i) {
	if (gfcom_1.a[0] >= stk_[0].stak[i - 1]) {
	    goto L5;
	}
	gfcom_1.a[0] = stk_[0].stak[i - 1];
	gfcom_1.a[1] = (float) i;
L5:
	;
    }
/*Widfl will =0 if a width was given. In that case the width should not be
*/
/* varied. */
    if (*widfl != 0) {
	gfcom_1.a[2] = (nto - nfrom + 1) / (float)4.7096400000000003;
	nterms = 3;
    } else {
	gfcom_1.a[2] = (tran_(&main_.htype, &c__1, width) - tran_(&
		main_.htype, &c__1, &c_b7)) / WIDFTR;
	sigma[2] = (float)0.;
	nterms = 2;
    }
    nchan = nto - nfrom + 1;
    nfree = nchan - nterms;
    if (nfree <= 0) {
	warn_("not enough channels");
	stk_[0].fwhm = FDRAGON;
	stk_[0].fwerr = FDRAGON;
	stk_[0].fpeak = FDRAGON;
	stk_[0].fperr = FDRAGON;
	stk_[0].ftmax = FDRAGON;
	stk_[0].fterr = FDRAGON;
	stk_[0].area = FDRAGON;
	stk_[0].chisq =FDRAGON;
	stk_[0].nfree = FDRAGON;
	return;
    }
    if (stk_[0].rms != (float)0.) {
/* Computing 2nd power */
	r__1 = stk_[0].rms;
	gfcom_1.sigsq = r__1 * r__1;
    } else {
	i__1 = (int) gfcom_1.a[1];
	gfcom_1.sigsq = (float)4. / (stk_[0].fwid * wt_(&i__1, &c__1));
/* This gives the expected rms**2. */
    }
    npass = 1;
    flamda = (float).001;
/* get chisq for the initial guess. */
    chisqr = chisq_(&nfrom, &nto);
/*This is the beginning of the main loop for following the gradient of chisq.*/
/* save current chisquare and clear the alpha and beta matrices. */
L20:
    chiold = chisqr;
    memset(alpha, 0, 36);
    memset(beta, 0, 12);
    memcpy(aa, gfcom_1.a, 12);
/* evaluate the alpha and beta matrices. */
    i__1 = nto;
    for (i = nfrom; i <= i__1; ++i) {
/* compute the current gaussian function and its derivatives. */
	z = (i - gfcom_1.a[1]) / gfcom_1.a[2];
	z2 = z * z;
	if (z2 > (float)16.) {
	    memset(deriv, 0, 12);
	    gfun = (float)0.;
	} else {
	    deriv[0] = exp(z2 * (float)-.5);
	    gfun = gfcom_1.a[0] * deriv[0];
	    deriv[0] = deriv[0];
	    deriv[1] = gfun * z / gfcom_1.a[2];
	    deriv[2] = deriv[1] * z;
	}
/* add to beta and alpha matrices */
	i__2 = nterms;
	for (j = 1; j <= i__2; ++j) {
	    beta[j - 1] += (stk_[0].stak[i - 1] - gfun) * deriv[j - 1];
	    i__3 = j;
	    for (k = 1; k <= i__3; ++k) {
		alpha[j + k * 3 - 4] += deriv[j - 1] * deriv[k - 1];
	    }
	}
    }
/* construct and invert modified curvature matrix. */
L71:
    i__1 = nterms;
    for (j = 1; j <= i__1; ++j) {
	i__2 = j - 1;
	for (k = 1; k <= i__2; ++k) {
	    anorm[j + k * 3 - 4] = sqrt(alpha[j + j * 3 - 4] * alpha[k + k * 
		    3 - 4]);
	    if (anorm[j + k * 3 - 4] == (float)0.) {
		anorm[j + k * 3 - 4] = (float)1e-20;
		if (alpha[j + j * 3 - 4] == (float)0.) {
		    alpha[j + j * 3 - 4] = (float)1e-10;
		}
		if (alpha[k + k * 3 - 4] == (float)0.) {
		    alpha[k + k * 3 - 4] = (float)1e-10;
		}
	    }
	    anorm[k + j * 3 - 4] = anorm[j + k * 3 - 4];
	    array[j + k * 3 - 4] = alpha[j + k * 3 - 4] / anorm[j + k * 3 - 4]
		    ;
	    array[k + j * 3 - 4] = array[j + k * 3 - 4];
	}
	array[j + j * 3 - 4] = flamda + (float)1.;
	anorm[j + j * 3 - 4] = alpha[j + j * 3 - 4];
    }
    matinv_(array, &nterms);
/* Update the a coeff by adding alpha*beta */
    i__1 = nterms;
    for (j = 1; j <= i__1; ++j) {
	gfcom_1.a[j - 1] = aa[j - 1];
	i__2 = nterms;
	for (k = 1; k <= i__2; ++k) {
	    gfcom_1.a[j - 1] += beta[k - 1] * array[j + k * 3 - 4] / anorm[j 
		    + k * 3 - 4];
	}
    }
/* if chisquare increased, increase flamda and try again. */
    chisqr = chisq_(&nfrom, &nto);
    if (chisqr > chiold) {
	if (flamda > (float)1e3) {
	    goto L120;
	}
	flamda *= (float)10.;
	goto L71;
    }
/* end of main loop. */
    if (gfcom_1.a[1] < (float) nfrom || gfcom_1.a[1] > (float) nto || gfcom_1.a[
	    2] > (float) (nto - nfrom)) {
	goto L120;
    }
    if (chiold > chisqr + (float).001 || npass < 3) {
	++npass;
	flamda /= (float)10.;
	goto L20;
    }
    goto L130;
L120:
    printf("Gauss fit not converging\n");
    main_.test=1;
    stk_[0].fwhm = FDRAGON;
    stk_[0].fwerr = FDRAGON;
    stk_[0].fpeak = FDRAGON;
    stk_[0].fperr = FDRAGON;
    stk_[0].ftmax = FDRAGON;
    stk_[0].fterr = FDRAGON;
    stk_[0].area = FDRAGON;
    stk_[0].chisq =FDRAGON;
    stk_[0].nfree = FDRAGON;
    return;

/* evaluate the uncertainties. */
L130:
    i__1 = nterms;
    for (j = 1; j <= i__1; ++j) {
	sigma[j - 1] = sqrt(gfcom_1.sigsq * array[j + j * 3 - 4] / alpha[j + 
		j * 3 - 4]);
/* L133: */
    }
/* cary out any requested action on stack 1. */
    switch ((int)(*iacton + 1)) {
	case 1:  goto L280;
	case 2:  goto L260;
	case 3:  goto L270;
    }
/* put Gaussian function in stack 1 and overlay it on the data plot. */
L260:
    if (main_.pltd == 0) {
	combex_("pl fhr:");
    }
    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	stk_[0].stak[i - 1] = gauss_(&i);
/* L265: */
    }
    combex_("pl ov:oh:ovl:");
    goto L280;
/* Subtract the Gaussian function from the data in stack 1. */
L270:
    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	stk_[0].stak[i - 1] -= gauss_(&i);
    }
    main_.pltd = 0;
L280:
/*  calculate stack header information */
/*  get factor to translate chans into velocity. */
/*RWW 2-4-92 Change so that header contains same htype as printed */
/*    hfact = rtran_(&c__1, &c__3); */
    hfact = rtran_(&c__1, &main_.htype);
    stk_[0].fwhm = gfcom_1.a[2] * WIDFTR * hfact;
    stk_[0].fwerr = sigma[2] * WIDFTR * hfact;
    d__1 = (double) gfcom_1.a[1];
    stk_[0].fpeak = tran_(&c__1, &main_.htype, &d__1);
    stk_[0].fperr = sigma[1] * hfact;
    stk_[0].ftmax = gfcom_1.a[0];
    stk_[0].fterr = sigma[0];
    stk_[0].area = gfcom_1.a[0] * (float)1.064467 * stk_[0].fwhm;
	chisqr /= (float)nfree; /* reduced Chisq. So gf and pf
				 * return the same quantity 
				 *  MWP 5/23/96 */
    stk_[0].chisq = chisqr;
    stk_[0].nfree = nfree;
    if (*dpfl != 0) {
	return;
    }
/*  calculate information for printing. */
    hfact = rtran_(&c__1, &main_.htype);
    fwhm = gfcom_1.a[2] * WIDFTR * hfact;
    fwerr = sigma[2] * WIDFTR * hfact;
    d__1 = (double) gfcom_1.a[1];
    fpeak = tran_(&c__1, &main_.htype, &d__1);
    fperr = sigma[1] * hfact;
    area = gfcom_1.a[0] * (float)1.064467 * fwhm;
/* Do we need a header? */
    if (cmnd_.cmdsig != csigsv) {
	csigsv = cmnd_.cmdsig;
	d__1 = (double) nfrom;
	rfrom = tran_(&c__1, &main_.htype, &d__1);
	d__1 = (double) nto;
	rto = tran_(&c__1, &main_.htype, &d__1);
	if (rto < rfrom) {
	    rtemp = rfrom;
	    rfrom = rto;
	    rto = rtemp;
	}
	titlePtr = title[main_.htype - 1];
	printf("Gaussian fit from %.2f to %.2f %s\n", rfrom, rto, titlePtr);
	printf("scan # tpeak(K) err(K) center(%s) err  fwhm(%s) err",
		titlePtr, titlePtr);
	printf("  area(K%s) chisq nfree\n", titlePtr);
    }
    if (stk_[0].nstk[0] < -32000) {
	nsn1 = stk_[0].nstk[2];
    } else {
	nsn1 = stk_[0].nstk[0];
    }
    printf("%5d %7.3f %7.3f %9.3f %7.3f %7.3f %6.3f %9.4f %9.1f %4d\n",
	nsn1,stk_[0].ftmax,stk_[0].fterr,fpeak,fperr,fwhm,fwerr,area,chisqr,
	nfree);
   
} /* gf_ */


/*Function to calculate the current Gaussian function for the given channel.*/

static double gauss_(nx)
int *nx;
{
    /* System generated locals */
    float ret_val, r__1;

    /* Builtin functions */
    double exp();

    /* Local variables */
    float arg;

/* Computing 2nd power */
    r__1 = (*nx - gfcom_1.a[1]) / gfcom_1.a[2];
    arg = r__1 * r__1 * (float)-.5;
    if (arg < (float)-50.) {
	ret_val = (float)0.;
	return ret_val;
    } else {
	ret_val = gfcom_1.a[0] * exp(arg);
    }
    return ret_val;
} /* gauss_ */

/* Function to calculate chi square over the region to be fit. */
static double chisq_(nfrom, nto)
int *nfrom, *nto;
{
    /* System generated locals */
    int i__1;
    float r__1;
    double ret_val;

    /* Local variables */
    int i;
    extern double gauss_();
    double chi2;

    chi2 = 0.;
    i__1 = *nto;
    for (i = *nfrom; i <= i__1; ++i) {
/* Computing 2nd power */
	r__1 = stk_[0].stak[i - 1] - gauss_(&i);
	chi2 += r__1 * r__1;
    }
    ret_val = chi2 / gfcom_1.sigsq; /* This is not reduced chisq */
    return ret_val;
} /* chisq_ */

/* Matrix inversion subroutine. */
static void matinv_(array, norder)
double *array;
int *norder;
{
    /* System generated locals */
    int i__1, i__2, i__3;

    /* Local variables */
    double amax;
    int i, j, k;

/* amax is Bevington's term, I am using it to hold the diagonal element. 
*/
    /* Parameter adjustments */
    array -= 4;

    /* Function Body */
    i__1 = *norder;
    for (k = 1; k <= i__1; ++k) {
	amax = array[k + k * 3];
	if (ABS(amax) < 1e-7) {
	    error_("matinv can't invert matrix");
	}
	i__2 = *norder;
	for (i = 1; i <= i__2; ++i) {
	    if (i != k) {
		array[i + k * 3] = -array[i + k * 3] / amax;
	    }
/* L70: */
	}
	i__2 = *norder;
	for (i = 1; i <= i__2; ++i) {
	    i__3 = *norder;
	    for (j = 1; j <= i__3; ++j) {
		if (i != k && j != k) {
		    array[i + j * 3] += array[i + k * 3] * array[k + j * 3];
		}
/* L79: */
	    }
/* L80: */
	}
	i__2 = *norder;
	for (j = 1; j <= i__2; ++j) {
	    if (j != k) {
		array[k + j * 3] /= amax;
	    }
/* L90: */
	}
	array[k + k * 3] = (float)1. / amax;
/* L100: */
    }
} /* matinv_ */

