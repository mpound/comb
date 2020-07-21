/* lf.c converted from pf.c - CLM 2001 May 17 */
/*   Trys to fit a Lorentzian to the data */

/* pf.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/
/* added MAXITERS to bug out if fit is not converging
 * -- MWP Thu Jun 27 11:11:44 PDT 1996
 */

#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"

#define MAXITERS 1000  /* maximum iterations before quitting */
static struct {
    float a[3];
} pfcom_1;

/* Table of constant values */

static int c__1 = CHAN;
static int c__3 = VEL;
static int iteration;


#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* lf.c */
static void gridls_ P_((float *y, float *sigmay, int *nfrom, int *nto, int *nterms, int *mode, float *deltaa, float *sigmaa, double *chisqr));
static double lorent_ P_((int *nx));
static double fchisq_ P_((float *stak1, float *sigmay, int *nfrom, int *nto, int *nfree, int *mode, float *yfit));
#undef P_

void lf_()
{
    /* System generated locals */
    int i__1;
    double d__1;

    /* Local variables */
    float diff, area;
    float fwhm;
    int i;
    extern double lorent_();
    float fpeak;
    int nchan;
    float hfact;
    int nfree;
    float fperr;
    int nfrom;
    int npass;
    float fwerr;
    float rfrom, rtemp;
    float sigmaa[3], deltaa[3];
    double chisqr;
    float chtest;
    float sig[MAXCHN];
    int nto;
    float rto;
    int nsn1;

    static int csigsv;
    static char title[][5] = {" ch ", "MHz ", "Km/s"};
    char *titlePtr;


/* 	lf = lorentzian fit; performs a chisquared fit of the data in stack1 */
/* 	to lorent=a(3)/(((nch - a(1))**2 / a(2)**2) + 1) */

/* 	This fit program is from Bevington's GRIDLS (p212) */
/* 	array definitions: a(j) = lorentzian parameters */
/* 	                   sigmaa(j)=one sigma errors in a(j) */
/*                          deltaa(j)=increments for a in chi-squared */
/*                          grid search */
/* 	                   sig(i)=one sigma noise in data, for weighting */

#include "lf.dc.h"
#include "lf.ec.h"

    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }
	iteration=0;
    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	sig[i - 1] = stk_[0].rms;
    }
    pltoch_(h1, h2, &nfrom, &nto);

/*     Set reasonable starting values and increments */

    i = (nfrom + nto) / 2;
    pfcom_1.a[2] = stk_[0].stak[i - 1];
    pfcom_1.a[0] = (float) i;
    i = (nto - nfrom) / 2;
    pfcom_1.a[1] = (float) i;
    deltaa[2] = pfcom_1.a[2] / (float)5.;
    deltaa[0] = pfcom_1.a[1] / (float)5.;
    deltaa[1] = deltaa[0];
    nchan = nto - nfrom + 1;
    nfree = nchan - 3;
    if (nfree <= 0) {
	error_("not enough channels");
    }
    npass = 1;
    chtest = stk_[0].rms * nchan;

/*     Begin loop for following gradient of chisquared. */

L200:
    gridls_(stk_[0].stak, sig, &nfrom, &nto, &c__3, &c__1, deltaa, sigmaa, &
	    chisqr);
    ++npass;
    diff = (chtest - chisqr) / chisqr;
    diff = DABS(diff);
    if (diff < (float).001) {
	goto L100;
    }
    chtest = chisqr;
    goto L200;
L100:

	if(stk_[0].fwhm == FDRAGON) {
		printf("Lorentzian fit did not converge\n");
		return;
	}
/* Carry out requested action on stack 1 */

    switch ((int)(*iacton + 1)) {
	case 1:  goto L280;
	case 2:  goto L260;
	case 3:  goto L270;
    }

/*  Put lorentzian function in stack 1 and overlay it on data plot. */

L260:
    if (main_.pltd == 0) {
	combex_("pl fhr:");
    }
    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	stk_[0].stak[i - 1] = lorent_(&i);
    }
    combex_("pl ov:oh:ovl:");
    goto L280;

/*  Subtract the lorentzian function from the data in stack 1 */

L270:
    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	stk_[0].stak[i - 1] -= lorent_(&i);
    }
    main_.pltd = 0;
L280:

/*  Calculate stack header information */

    hfact = rtran_(&c__1, &c__3);
    stk_[0].fwhm = pfcom_1.a[1] * (float)2. * hfact;
    stk_[0].fwerr = sigmaa[1] * (float)2. * hfact;
    d__1 = (double) pfcom_1.a[0];
    stk_[0].fpeak = tran_(&c__1, &c__3, &d__1);
    stk_[0].fperr = sigmaa[0] * hfact;
    stk_[0].ftmax = pfcom_1.a[2];
    stk_[0].fterr = sigmaa[2];
    stk_[0].area = pfcom_1.a[2] * (float).66666666666666663 * stk_[0].fwhm;
    stk_[0].chisq=chisqr;
    stk_[0].nfree=nfree;
    if (*dpfl != 0) {
	return;
    }

/*  Calculate information for printing */

    hfact = rtran_(&c__1, &main_.htype);
    fwhm = pfcom_1.a[1] * (float)2. * hfact;
    fwerr = sigmaa[1] * (float)2. * hfact;
    d__1 = (double) pfcom_1.a[0];
    fpeak = tran_(&c__1, &main_.htype, &d__1);
    fperr = sigmaa[0] * hfact;
    area = pfcom_1.a[2] * (float).66666666666666663 * fwhm;

/*  Do we need a header? */

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
 	printf("Lorentzian fit from %.2f to %.2f %s\nscan # tpeak(k) ",
	    rfrom, rto, titlePtr);
	printf("err(k) center(%s) err fwhm(%s) err area(K%s) chisq nfree\n",
	    titlePtr, titlePtr, titlePtr);
    }
    if (stk_[0].nstk[0] < -32000) {
	nsn1 = stk_[0].nstk[2];
    } else {
	nsn1 = stk_[0].nstk[0];
    }
	printf("%5d %7.3f %7.3f %9.3f %7.3f %7.3f %6.3f %9.4f %9.1f %4d\n",
	    nsn1, stk_[0].ftmax, stk_[0].fterr,fpeak,fperr,fwhm,fwerr,area,
	    chisqr,nfree);
} /* pf_ */


static void gridls_(y, sigmay, nfrom, nto, nterms, mode, deltaa, 
	sigmaa, chisqr)
float *y, *sigmay;
int *nfrom, *nto, *nterms, *mode;
float *deltaa, *sigmaa;
double *chisqr;
{
    /* System generated locals */
    int i__1, i__2;
    double d__1;

    /* Local variables */
    float free, save, yfit[MAXCHN];
    int i, j;
    extern double lorent_();
    int nchan;
    float delta;
    int nfree;
    float fn;
    double chisq1, chisq2, chisq3;
    extern double fchisq_();

/*       Deltaa values are modified by the program. */

    /* Parameter adjustments */
    --sigmaa;
    --deltaa;
    --sigmay;
    --y;

    /* Function Body */
    nchan = *nto - *nfrom + 1;
    nfree = nchan - *nterms;
    free = (float) nfree;
    if (free <= (float)0.) {
	goto L100;
    }
    i__1 = *nterms;
    for (j = 1; j <= i__1; ++j) {

/*  Evaluate chi square at first two search points */

	i__2 = *nto;
	for (i = *nfrom; i <= i__2; ++i) {
	    yfit[i - 1] = lorent_(&i);
	}
	chisq1 = fchisq_(stk_[0].stak, &sigmay[1], nfrom, nto, &nfree, mode, 
		yfit);
	fn = (float)0.;
	delta = deltaa[j];
L41:
        iteration++;
        if(iteration>MAXITERS) {
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
        }
	pfcom_1.a[j - 1] += delta;
	i__2 = *nto;
	for (i = *nfrom; i <= i__2; ++i) {
	    yfit[i - 1] = lorent_(&i);
	}
	chisq2 = fchisq_(&y[1], &sigmay[1], nfrom, nto, &nfree, mode, yfit);
	if ((d__1 = chisq1 - chisq2) < 0.) {
	    goto L51;
	} else if (d__1 == 0) {
	    goto L41;
	} else {
	    goto L61;
	}

/*   reverse direction if chisquared is increasing */

L51:
	delta = -(double)delta;
	pfcom_1.a[j - 1] += delta;
	i__2 = *nto;
	for (i = *nfrom; i <= i__2; ++i) {
	    yfit[i - 1] = lorent_(&i);
	}
	save = chisq1;
	chisq1 = chisq2;
	chisq2 = save;

/*  Increment a(j) until chisq increases */

L61:
	iteration++;
	if(iteration>MAXITERS) {
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
	}
	fn += (float)1.;
	pfcom_1.a[j - 1] += delta;
	i__2 = *nto;
	for (i = *nfrom; i <= i__2; ++i) {
	    yfit[i - 1] = lorent_(&i);
	}
	chisq3 = fchisq_(stk_[0].stak, &sigmay[1], nfrom, nto, &nfree, mode, 
		yfit);
	if (chisq3 >= chisq2) {
	    goto L81;
	}
	chisq1 = chisq2;
	chisq2 = chisq3;
	goto L61;

/*  Find minimum of lorentzian defined by last three points */
L81:
	delta *= (float)1. / ((chisq1 - chisq2) / (chisq3 - chisq2) + (float)
		1.) + (float).5;
	pfcom_1.a[j - 1] -= delta;
	sigmaa[j] = deltaa[j] * sqrt((float)2. / (free * (chisq3 - chisq2 * (
		float)2. + chisq1)));
	deltaa[j] = deltaa[j] * fn / (float)3.;
    }

/*  Evaluate fit and chisq for final parameters */
    i__1 = *nfrom;
    i__2 = *nto;
    for (i = 1; i__2 < 0 ? i >= i__1 : i <= i__1; i += i__2) {
	yfit[i - 1] = lorent_(&i);
    }
    *chisqr = fchisq_(stk_[0].stak, &sigmay[1], nfrom, nto, &nfree, mode, yfit)
	    ;
L100:
    return;
} /* gridls_ */


/*  Function to calculate the current lorentzian for the given channel */

static double lorent_(nx)
int *nx;
{
    /* System generated locals */
    float ret_val, r__1;

    /* Local variables */
    float q, xi;


/* local variables */

    xi = *nx - pfcom_1.a[0];
/* Computing 2nd power */
    r__1 = xi / pfcom_1.a[1];
    q = r__1 * r__1;
    ret_val = pfcom_1.a[2] / ((float)1. + q);
    return ret_val;
} /* lorent_ */

static double fchisq_(stak1, sigmay, nfrom, nto, nfree, mode, yfit)
float *stak1, *sigmay;
int *nfrom, *nto, *nfree, *mode;
float *yfit;
{
    /* System generated locals */
    int i__1;
    float r__1;
    double ret_val;

    /* Local variables */
    float free;
    int i;
    double chisq, weight;

/*  Bevington p194 */

    /* Parameter adjustments */
    --yfit;
    --sigmay;
    --stak1;

    /* Function Body */
    chisq = 0.;
    if (*nfree > 0) {
	goto L20;
    }
    ret_val = (float)0.;
    goto L40;

/*  Accumulate chi squared */

L20:
    i__1 = *nto;
    for (i = *nfrom; i <= i__1; ++i) {
	if (*mode < 0) {
	    goto L22;
	} else if (*mode == 0) {
	    goto L27;
	} else {
	    goto L29;
	}
L22:
	if ((r__1 = stak1[i]) < (float)0.) {
	    goto L25;
	} else if (r__1 == 0) {
	    goto L27;
	} else {
	    goto L23;
	}
L23:
	weight = (float)1. / stak1[i];
	goto L30;
L25:
	weight = (float)1. / (-(double)stak1[i]);
	goto L30;
L27:
	weight = (float)1.;
	goto L30;
L29:
/* Computing 2nd power */
	r__1 = sigmay[i];
	weight = (float)1. / (r__1 * r__1);
L30:
/* Computing 2nd power */
	r__1 = stak1[i] - yfit[i];
	chisq += weight * (r__1 * r__1);
    }

/*   Divide by number of degrees of freedom */

    free = (float) (*nfree);
    ret_val = chisq / free;
L40:
    return ret_val;
} /* fchisq_ */

