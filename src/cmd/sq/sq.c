/* sq.f -- translated by f2c (version of 16 May 1991  13:06:06).
   Bug fix by A. Stark 30 Aug 2000
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"

#define bcopy(s, d, n) memcpy(d, s, n)
#define bzero(s, n) memset(s, 0, n)

/* Table of constant values */

static int c__1 = 1;

/* Subroutine */ 
void sq_()
{
    /* System generated locals */
    int i__1, i__2;
    double d__1, d__2;

/* Center Channel about which the expansion occurs */
    double exch;

/* Channel number in the new shifted spectrum. */
    int n;

/*Channel numbers in the old spectrum corresponding to chan edges in the new.*/
    double c1, c2;
    double ctol;		/* Min width (in old) for a chan to be used */
    int n1, n2, no;

/* Channels in the new spectrum, not shifted into final place. */
    int nb, ne, nn;

/* The amt to add to chans in new to get final chan #. (nfinal=new+ishift) */
    int ishift;

/* Temporary storage for the new spectrum and its weights */
    float tempst[MAXCHN];
    short nwts;
    short itmplc[NWEIGHTS];
    float tempwt[NWEIGHTS];
    float totwt;
    double sum;
    double weight;

/* The formula for squish is: nold-exch=(new-exch)*wnool where exch is the */
/* channel about which the expansion occurs. */

/* Get the arguments */
#include "sq.dc.h"

#include "sq.ec.h"
    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }
    if (*wnool < .01 || *wnool > 100.) {
	error_("unreasonable width ratio");
    }

/* Set up things for plot */
    main_.pltd = 0;
/* Set up the center of expansion */
    if (*hflg == 0) {
	exch = tran_(&main_.htype, &c__1, h);
    } else {
	exch = stk_[0].expch;
    }
/*Calculate the ends of the new spectrum. Don't use a chan that has <0.4 of*/
/* the weight of a full chan. */
    nb = round((.5 - exch) / *wnool + exch + .4);
    ne = round((stk_[0].numpt + .5 - exch) / *wnool + exch - .4);
/* Calculate ishift. First put exch at the center of the final spectrum. */
    ishift = round(MAXCHN / 2 + .5 - exch);
/* Now get rid of any unused chans at the end of a MAXCHN spectrum */
    i__2 = MAXCHN - ne;
    if(i__2 > ishift)
	ishift = i__2;
/* Finally elim unused chans at the beginning */
    i__2 = 1 - nb;
    if(i__2 < ishift)
	ishift = i__2;
/* Limit nb & ne to the range [1,MAXCHN] */
    i__2 = 1 - ishift;
    if(nb < i__2)
	nb = i__2;
    i__2 = MAXCHN - ishift;
    if(ne > i__2)
	ne = i__2;
/* Clear the wts storage */
    bzero(tempwt, sizeof(tempwt));
    bzero(itmplc, sizeof(itmplc));
    nwts = 0;
/* Don't bother with old channels which contribute less than .5% */
    ctol = 0.005 * *wnool;
/* Step through the new chans. */
    i__1 = ne;
    for (nn = nb; nn <= i__1; ++nn) {
/* Find the edges of the new chan in the old spectrum. Avoid tiny slivers */
	c1 = (nn - .5 - exch) * *wnool + exch;
	c2 = c1 + *wnool;
	n1 = round(c1 + ctol);
	if(n1 < 1)
	    n1 = 1;
	n2 = round(c2 - ctol);
	if(n2 > stk_[0].numpt)
	    n2 = stk_[0].numpt;
/* Calculate the value and weight of the new chan */
	totwt = 0.;
	sum = 0.;
	i__2 = n2;
	for (no = n1; no <= i__2; ++no) {
/* Computing MIN */
	    d__1 = no + .5;
/* Computing MAX */
	    d__2 = no - (float).5;
	    weight = wt_(&no, &c__1) * (DMIN(d__1,c2) - DMAX(d__2,c1));
	    totwt += weight;
	    sum += stk_[0].stak[no - 1] * weight;
/* L90: */
	}
	n = nn + ishift;
	if (totwt < (float)1.e-8) {
	    tempst[n - 1] = 0.0;
	} else {
	    tempst[n - 1] = sum / totwt;
	}
	pkwts_(&totwt, &n, tempwt, itmplc, &nwts);
/* L100: */
    }
/* Move the results into place */
    bcopy(tempwt, stk_[0].wght, sizeof(tempwt));
    bcopy(itmplc, stk_[0].locwt, sizeof(itmplc));
    stk_[0].nwts = nwts;
    stk_[0].numpt = n;
    bcopy(tempst, stk_[0].stak, stk_[0].numpt * sizeof(float));
/* 	if(numpt1.lt.nsize) call clear(stak1(numpt1+1),(nsize-numpt1)*4) */
/* Adjust the header values */
    stk_[0].expch = (stk_[0].expch - exch) / *wnool + exch + ishift;
    if (stk_[0].refch != (float)-9999.) {
	stk_[0].refch = (stk_[0].refch - exch) / *wnool + exch + ishift;
    }
    stk_[0].fwid *= *wnool;
    if (*wnool > 1.) {
	stk_[0].rms /= sqrt(*wnool);
    }
} /* sq_ */

