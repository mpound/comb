/* li.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"


#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* li.c */
static void bsln_ P_((int *nslope, int *modisp));
static void eqsol_ P_((int *n, double *a, int *m, double *x));
static void eqval_ P_((int *m, double *bb, double *xp, double *ycomp));
#undef P_

/* Table of constant values */

static int c__1 = 1;
static double c_b6 = 0.;
static float c_b8 = (float)1e-4;

void li_()
{
#include "li.dc.h"

    /* Local variables */
    int modisp;

    if (*nordr > 1) {
	*nordr = 1;
    }
#include "li.ec.h"
    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }

    if((modisp = *iacton)==2) {
	if (coreFull[1]==0)
		error_("No data in stack 2");
    }

    if (*widfl == 0) {
	stk_[0].exwid = tran_(&main_.htype, &c__1, wid) - tran_(&main_.htype,
		 &c__1, &c_b6);
    }
    bsln_((int *)nordr, &modisp);
    main_.pltd = 0;
} /* li_ */


static void bsln_(nslope, modisp)
int *nslope, *modisp;
{
    /* System generated locals */
    int i__1, i__2;
    double d__1;

    /* Local variables */
    int nbeg;
    int nref1, nsig1, nsig2, nref2, i, j, m;
    double a, b, c, scale, y;
    double cwrat, a2, ycomp;
    int m2, mm[16], ms;
    double aa[272]	/* was [16][17] */, bb[30], ci, xc, xp, xx, xy, bb2, 
	    offset;
    /* Subroutine */ int plchns_();
    int mp1, no2, nen, ipj;
    double xxc;

/*     2-23-81 modified for modisp=2 - aas */

/*  baseline removal by least squares - uses points i if */
/*     i.gt.0  and  i.le.numpt1  and  ibit(i,nuse).ne.0 */
/*  kbsln=0,1,2 will additionally cause no points,points within */
/*  nwid/2 of sig,within nwid/2 of sig or ref to be eliminated */
/*  baseline removed for all points between 1 and numpt1 */

/* curfi declarations: */

/*  m = polynomial  order */
/* modisp = 1 evaluates b and replaces y by best fit curve */
/* modisp = 0 evaluates b and replaces y by residuals */
/* modisp = 2 same as modisp=0, but also removes the polynomial from stack 2 */

/*  current dimensions are for polynomial order 15 */
/*  a(max,max+1),b(2*max-2),mm(max) where max = nslope+1 */

    if (*modisp == 2) {
	if (stk_[0].ibsln != stk_[1].ibsln || ifdif_(&stk_[0].rmval, &
		stk_[1].rmval, &c_b8) || stk_[0].indx != stk_[1].indx || 
		stk_[0].factt != stk_[1].factt || stk_[0].nprc != stk_[1].nprc) {
	    goto L20;
	}
	i__1 = stk_[0].indx;
	for (i = 1; i <= i__1; ++i) {
	    j = indxsn_(&i);
	    if (stk_[0].nstk[j - 1] != stk_[1].nstk[j - 1]) {
		goto L20;
	    }
	}
    }
    goto L30;
L20:
    warn_("Stacks 1&2 not compatable for baseline removal");
L30:
    if (*nslope < 0) {
	return;
    }
    if (*nslope > 15) {
	error_("Baseline order too high");
    }
/* Restrict baseline operations to the plotted range if a plot exists. */
    plchns_(&nbeg, &nen);
/* Set the new ibsln parameters. */
    stk_[0].ibsln = MAX((int) stk_[0].ibsln,*nslope);
/* If stack 2 is to be modified also set the chan width ratio. */
    if (*modisp == 2) {
	stk_[1].ibsln = stk_[0].ibsln;
	if (stk_[0].fwid < (float)1e-6) {
	    error_("Bad stack1 in bsln");
	}
	cwrat = stk_[1].fwid / stk_[0].fwid;
    }
/* use c as a convenient temporary while calculating the excluded chans. 
*/
    if (stk_[0].exwid > (float).9) {
	c = stk_[0].exwid / (float)2.;
    } else {
	c = (float)-1e3;
    }
    d__1 = stk_[0].expch - c;
    nsig1 = irond_(&d__1);
    d__1 = stk_[0].expch + c;
    nsig2 = irond_(&d__1);
    d__1 = stk_[0].refch - c;
    nref1 = irond_(&d__1);
    d__1 = stk_[0].refch + c;
    nref2 = irond_(&d__1);
/* If the order is higher than 1, use the more complicated routine. */
    if (*nslope > 1) {
	goto L300;
    }
    c = (float)0.;
    xc = (float)0.;
    xxc = (float)0.;
    y = (float)0.;
    xy = (float)0.;
    i__1 = nen;
    for (i = nbeg; i <= i__1; ++i) {
	if (ibit_(use_.nuse, &i) == 0) {
	    goto L80;
	}
	if (i >= nsig1 && i <= nsig2) {
	    goto L80;
	}
	if (i >= nref1 && i <= nref2) {
	    goto L80;
	}
	ci = (double) i;
	c += (float)1.;
	xc += ci;
	xxc += ci * ci;
	y += stk_[0].stak[i - 1];
	xy += i * stk_[0].stak[i - 1];
L80:
	;
    }
    if (c <= (double) (*nslope)) {
	error_("Too few points to remove baseline");
    }
/*  a,b are usual coefficients for y=ax+b */
    a = (y * xc - xy * c) / (xc * xc - c * xxc);
    b = (xc * xy - xxc * y) / (xc * xc - c * xxc);
    offset = y / c;
    if (*nslope == 1) {
	offset = a * stk_[0].expch + b;
    }
    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	if (*modisp != 1) {
	    goto L199;
	}
	if (*nslope == 1) {
	    stk_[0].stak[i - 1] = a * i + b;
	} else {
	    stk_[0].stak[i - 1] = offset;
	}
	goto L200;
L199:
	if (*nslope == 1) {
	    stk_[0].stak[i - 1] -= a * i + b;
	} else {
	    stk_[0].stak[i - 1] -= offset;
	}
L200:
	;
    }
    if (*modisp == 1) {
	goto L290;
    }
    stk_[0].rmval += offset;
    if (*modisp != 2) {
	goto L290;
    }
    a2 = a * cwrat;
    bb2 = b + a * stk_[0].expch - a2 * stk_[1].expch;
    i__1 = stk_[1].numpt;
    for (i = 1; i <= i__1; ++i) {
	if (*nslope == 1) {
	    stk_[1].stak[i - 1] -= a2 * i + bb2;
	} else {
	    stk_[1].stak[i - 1] -= offset;
	}
    }
    stk_[1].rmval += offset;
L290:
    if (*nslope <= 1) {
	return;
    }

L300:
    no2 = (nen + nbeg) / 2;
    scale = sqrt((nen - nbeg) / (float)2.);
    m = *nslope + 1;
    mp1 = m + 1;
    m2 = (m << 1) - 2;
    i__1 = m;
    for (i = 1; i <= i__1; ++i) {
	i__2 = mp1;
	for (j = 1; j <= i__2; ++j) {
	    aa[i + (j << 4) - 17] = (float)0.;
	}
    }
    i__2 = m2;
    for (i = 1; i <= i__2; ++i) {
	bb[i - 1] = (float)0.;
    }
    i__2 = nen;
    for (i = nbeg; i <= i__2; ++i) {
	if (ibit_(use_.nuse, &i) == 0) {
	    goto L6;
	}
	if (i >= nsig1 && i <= nsig2) {
	    goto L6;
	}
	if (i <= nref2 && i >= nref1) {
	    goto L6;
	}
	xp = (float) (i - no2) / scale;
	xx = xp;
	bb[0] += xp;
	aa[(mp1 << 4) - 16] += stk_[0].stak[i - 1];
	aa[0] += (float)1.;
	i__1 = m;
	for (j = 2; j <= i__1; ++j) {
	    aa[j + (mp1 << 4) - 17] += stk_[0].stak[i - 1] * xp;
	    xp *= xx;
	    bb[j - 1] += xp;
	}
	i__1 = m2;
	for (j = mp1; j <= i__1; ++j) {
	    xp *= xx;
	    bb[j - 1] += xp;
	}
L6:
	;
    }
    if (aa[0] <= (double) (*nslope)) {
	error_("Too few points to remove baseline");
    }
    ms = 2;
    i__2 = m;
    for (i = 1; i <= i__2; ++i) {
	i__1 = m;
	for (j = ms; j <= i__1; ++j) {
	    ipj = i + j - 2;
	    aa[i + (j << 4) - 17] = bb[ipj - 1];
	}
	ms = 1;
    }
    eqsol_(&m, aa, mm, bb);
    i__2 = stk_[0].numpt;
    for (i = 1; i <= i__2; ++i) {
	xp = (float) (i - no2) / scale;
	eqval_(&m, bb, &xp, &ycomp);
	if (*modisp != 1) {
	    goto L9;
	}
	stk_[0].stak[i - 1] = ycomp;
	goto L8;
L9:
	stk_[0].stak[i - 1] -= ycomp;
L8:
	;
    }
    if (*modisp == 1) {
	return;
    }
    xp = (stk_[0].expch - no2) / scale;
    eqval_(&m, bb, &xp, &offset);
    stk_[0].rmval += offset;
    if (*modisp != 2) {
	return;
    }
/* evaulate fit for stak 2 */
    i__2 = stk_[1].numpt;
    for (i = 1; i <= i__2; ++i) {
	xp = (cwrat * (i - stk_[1].expch) + stk_[0].expch - no2) / scale;
	eqval_(&m, bb, &xp, &ycomp);
	stk_[1].stak[i - 1] -= ycomp;
    }
    stk_[1].rmval += offset;

/* end of curfi */

} /* bsln_ */

static void eqsol_(n, a, m, x)
int *n;
double *a;
int *m;
double *x;
{
    /* System generated locals */
    int i__1, i__2, i__3;
    double d__1;

    /* Local variables */
    double amax;
    int i, j;
    double zmult;
    int ip, nn, mmm;


    /* Parameter adjustments */
    --x;
    --m;
    a -= 17;

    /* Function Body */
    nn = *n + 1;
    i__1 = *n;
    for (i = 1; i <= i__1; ++i) {
	m[i] = 1;
	amax = a[i + 16];
	i__2 = *n;
	for (j = 2; j <= i__2; ++j) {
	    if ((d__1 = a[i + (j << 4)], ABS(d__1)) - ABS(amax) <= 0.) {
		goto L2;
	    } else {
		goto L100;
	    }
L100:
	    amax = a[i + (j << 4)];
	    m[i] = j;
L2:
	    ;
	}
	i__2 = nn;
	for (j = 1; j <= i__2; ++j) {
	    a[i + (j << 4)] /= amax;
	}
	i__2 = *n;
	for (ip = 1; ip <= i__2; ++ip) {
	    if (ip - i != 0) {
		goto L50;
	    } else {
		goto L5;
	    }
L50:
	    mmm = m[i];
	    zmult = a[ip + (mmm << 4)];
	    i__3 = nn;
	    for (j = 1; j <= i__3; ++j) {
		if (j - mmm != 0) {
		    goto L9;
		} else {
		    goto L8;
		}
L8:
		a[ip + (j << 4)] = (float)0.;
		goto L6;
L9:
		a[ip + (j << 4)] -= zmult * a[i + (j << 4)];
L6:
		;
	    }
L5:
	    ;
	}
    }
    i__2 = *n;
    for (i = 1; i <= i__2; ++i) {
	mmm = m[i];
	x[mmm] = a[i + (nn << 4)];
    }
    return;
} /* eqsol_ */

static void eqval_(m, bb, xp, ycomp)
int *m;
double *bb, *xp, *ycomp;
{
    int j;


    /* Parameter adjustments */
    --bb;

    /* Function Body */
    *ycomp = bb[*m];
    for (j = *m - 1; j >= 1; --j) {
	*ycomp = *ycomp * *xp + bb[j];
    }
} /* eqval_ */

