/* in.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"

/* Table of constant values */

static int c__1 = CHAN;
static int c__3 = VEL;
static float c_b26 = (float).501;
static float c_b27 = (float)1e-5;
static float c_b28 = (float).001;

static float clevel[2]; /* Clip levels - mwp 8/21/96 */

void in_()
{
    /* Initialized data */

    static char title[][5] = {" ch ", "MHz ", "Km/s"};
    char *titlePtr;

    /* System generated locals */
    double d__1;

    /* Local variables */
    float cench, areak, hfact, fwhm;
    extern double integ_();
    float numch;
    float erarea, peakch;
    static int csigsv, esigsv;

#include "in.dc.h"
#include "in.ec.h"

    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }
    clevel[0]=DABS(*clip0);
    clevel[1]=DABS(*clip1);
    stk_[0].area = integ_(h1, h2, &stk_[0].tmax, &stk_[0].vcent, &stk_[0].vpeak, &stk_[0].fwhm);
    hfact = rtran_(&c__1, &main_.htype);
    numch = (*h2 - *h1) / hfact;
    if (numch < 0) {
	numch = -numch;
    }
    if (numch < 1) {
	numch = 1;
    }
    erarea = stk_[0].rms * sqrt(numch) * hfact;
    stk_[0].arerr = erarea;
    if (*ndp == 0 && stk_[0].area != FDRAGON) {
	areak = stk_[0].area * rtran_(&c__3, &main_.htype);
/*
 * this is not a true FWHM but the dispersion (2nd moment) 
 * for a gaussian profile, they differ by sqrt(8*ln(2))   
 */ 
	fwhm = stk_[0].fwhm * rtran_(&c__3, &main_.htype);
	d__1 = (double) stk_[0].vcent;
	cench = tran_(&c__3, &main_.htype, &d__1);
	d__1 = (double) stk_[0].vpeak;
	peakch = tran_(&c__3, &main_.htype, &d__1);
/*
 * Write a heading if this is the first call in this cmd line 
 */
	if (cmnd_.cmdsig != csigsv) {
	    titlePtr = title[main_.htype - 1];
	    printf(
	"Stack  From(%s) To (%s) Tmax       Area(K*%s)    Centroid(%s) Peak   Disp.(%s)\n",
		titlePtr, titlePtr, titlePtr, titlePtr, titlePtr);
	    csigsv = cmnd_.cmdsig;
	}
	printf("%5d %9.3f %9.3f %7.3f %9.3f +/- %-7.3f %9.3f %9.3f ",
	    stk_[0].numst,*h1,*h2, stk_[0].tmax, areak, erarea, cench, peakch);
 	printf((fwhm==FDRAGON)?"  .DR\n":"%9.3f\n",fwhm);
    } else {
	if (stk_[0].area == FDRAGON && esigsv != cmnd_.cmdsig) {
	    warn_("Integration is completely outside of stack");
	    esigsv = cmnd_.cmdsig;
	}
    }

} /* in_() */
#include "in.uc.h"

/*============================================================================*/
/* integrate stack 1 between h1 and h2(horiz plot units) and return result in */
/* deg K-km/s.  Max temperature is put in tmax. Velocity of centroid is       */
/* is put in cenvel and velocity of peak in vpeak.                            */
/*============================================================================*/

double integ_(h1, h2, tmax, cenvel, vpeak, fwhm)
double *h1, *h2;
float *tmax, *cenvel, *vpeak;
float *fwhm;
{
    /* System generated locals */
    float r__1, r__2, r__3;
    double ret_val, d__1;

    /* Local variables */
    float area;
    int nbeg, nend;
    float temp;
    int i;
    float harea;
    float parea[MAXCHN+1];
    double r1, r2;
    float extra1, extra2;
    static int csigsv;

    /* variables for fwhm. - mh 6/25/96 */
    double r1save, rcent; /* save result for calculation at the end of integ_() */
    float area2;

/* convert to channels */
    pltchr_(h1, h2, &r1, &r2);
/* pltochr sorts r1 and r2 and then limits them.  If they are out of order, 
 * the complete integration is outside of the spectrum 
 */
    if (r1 > r2) {
	ret_val = FDRAGON;
	*tmax = FDRAGON;
	*cenvel = FDRAGON;
	*fwhm = FDRAGON;
	return ret_val;
    }
/* if either r1 or r2 has its limited value, the integration goes outside of
 * the stack 
 */
    r__1 = r1;
    r__2 = r2;
    r__3 = stk_[0].numpt + (float).499;
    if (! ifdif_(&r__1, &c_b26, &c_b27) || ! ifdif_(&r__2, &r__3, &c_b28)) {
	if (csigsv != cmnd_.cmdsig) {
	    warn_("Integration extends outside of stack");
	    csigsv = cmnd_.cmdsig;
	}
    }
    nbeg = irondd_(&r1); /* nearest integer to r1 */
    nend = irondd_(&r2); /* nearest integer to r2 */
    r1save = r1;
/*
 * first take care of missing fractional chans at 
 * the beginning and the end.
 * extra1 and extra2 are <= 0. 
 */
    extra1 = nbeg - .5 - r1;
    extra2 = r2 - (nend + .5);

    area = 0.0;
    /* get fractional area at beginning */
    /* apply clip level for area        */
    if(DABS(stk_[0].stak[nbeg-1])>=clevel[0]*stk_[0].rms) {
     if (DABS(stk_[0].stak[nbeg - 1]) <= MAXVALUE) {
	area = extra1 * stk_[0].stak[nbeg - 1];
     } else {
	area = extra1 * MAXVALUE;
     }
    }
    *tmax = (float)-1e38;
/* for the moment, keep track of channel of peak temp in vpeak */
    for (i = nbeg; i <= nend; ++i) {
	parea[i - 1] = area; /* parea is the sum of the area up to the
			      * current channel. It will be used in
			      * finding the centroid velocity */	

     /* apply clip level for area */
     if(DABS(stk_[0].stak[i-1])>=clevel[0]*stk_[0].rms) {
	if (DABS(stk_[0].stak[i - 1]) <= MAXVALUE) {
	    area += stk_[0].stak[i - 1];
	} else {
	    area += (float)1e20;
	}
	if (stk_[0].stak[i - 1] > *tmax) {
	    *tmax = stk_[0].stak[i - 1];
	    *vpeak = (float) i;
	}
     } /*else {printf ("ignored channel %d value %f\n",i - 1,stk_[0].stak[i-1]);}*/
    }
    parea[i - 1] = area;

    /* get fractional area at end       */
    /* apply clip level for area        */
    if(DABS(stk_[0].stak[nend-1])>=clevel[0]*stk_[0].rms) {
     if (DABS(stk_[0].stak[nend - 1]) <= MAXVALUE) {
	area += extra2 * stk_[0].stak[nend - 1];
     } else {
	area += extra2 * MAXVALUE;
     }
    }
    if (*tmax > MAXVALUE) {
	*tmax = MAXVALUE;
    }
    harea = area / (float)2.;

/*  Find the channel where the partial area exceeds half the total */ 
    for (i = nbeg + 1; i <= nend; ++i) {
	if (parea[i - 1] >= harea) {
	    goto L210;
	}
    }
L210:
    ret_val = area * rtran_(&c__1, &c__3);
    temp = parea[i - 1] - parea[i - 2];
    if (temp != (float)0.) { /* split the difference */
	r1 = i - (float).5 - (parea[i - 1] - harea) / temp;
    } else {
	r1 = i - 1;
    }
/*
 * The centroid velocity is at the channel where half the area is
 * above and half is below 
 */
    *cenvel = tran_(&c__1, &c__3, &r1);
    d__1 = (double) (*vpeak);
    *vpeak = tran_(&c__1, &c__3, &d__1);

    /**** second moment  - mh 6/25/96 ****/
/*
 * first take care of missing fractional chans at 
 * the beginning and the end.
 * extra1 and extra2 are <= 0. 
 */
    area2=0.;
    rcent = r1;
    r1 = r1save; /* r1save is the 1st channel in the integration */

    /* get fractional area at beginning */
    /* Only add to area2 if channel value greater than clip level -mwp */
    if(DABS(stk_[0].stak[nbeg-1])>=clevel[1]*stk_[0].rms) {
      if (DABS(stk_[0].stak[nbeg - 1]) <= MAXVALUE) {
	area2 = extra1 * stk_[0].stak[nbeg - 1] * (r1 - rcent) * (r1 - rcent);
      } else {
	area2 = extra1 * MAXVALUE * (r1 - rcent) * (r1 - rcent);
      }
    } 
    for (i = nbeg; i <= nend; ++i) {
       if(DABS(stk_[0].stak[i-1])>=clevel[1]*stk_[0].rms) {
	if (DABS(stk_[0].stak[i - 1]) <= MAXVALUE) {
		float dv=((float)i-1.-rcent);
	    area2 += stk_[0].stak[i - 1]*((float)i-1.-rcent)*((float)i-1.-rcent);
	} else {
	    area2 += (float)1e20 ;
	}
       }
    }

    /* get fractional area at end */
    if(DABS(stk_[0].stak[nend-1])>=clevel[1]*stk_[0].rms) {
      if (DABS(stk_[0].stak[nend - 1]) <= MAXVALUE) {
	area2 += extra2 * stk_[0].stak[nend - 1] * (r2  - rcent) * (r2 - rcent);
      } else {
	area2 += extra2 * MAXVALUE * (r2 - rcent) * (r2 - rcent);
      }
    }
    /* normalize to get FWHM */

     area2 = (area <= 0. || area2 < 0.) ?  FDRAGON : sqrt( area2 / DABS(area) );

/* NOTE: This quantity is NOT the FWHM. For a Gaussian, 
 *        FWHM = sqrt(8*ln(2))*[2nd moment] 
 *  For non-Gaussian, it is not defined!  -- mwp 
 */
    *fwhm = (area2 == FDRAGON) ? FDRAGON: (area2 * rtran_(&c__1, &c__3));

    return ret_val;
} /* integ_ */
