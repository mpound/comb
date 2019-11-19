/* cc.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"

/* Table of constant values */

static int c__1 = 1;
static int c__2 = 2;
static int c__3 = 3;

/* Subroutine */ void cc_()
{

    /* System generated locals */
    double d__1;

    float texpch;

#include "cc.dc.h"
#include "cc.ec.h"
    /* Store old value of expch for updating refch later */
    texpch = stk_[0].expch;

    switch ((int)(*iactn + 1)) {
	case 1:  goto L100;
	case 2:  goto L200;
	case 3:  goto L300;
    }
    error_("Bad action");
/* compute new center channel from new frequency */
L100:
    stk_[0].expch = tran_(&c__2, &c__1, nwf);
    stk_[0].freq = *nwf;
    goto L1000;
/* compute new center channel from new velocity */
L200:
    stk_[0].expch = tran_(&c__3, &c__1, nwv);
    stk_[0].vlsr = *nwv;
    goto L1000;
/* Correct an error in a receiver oscillator given an equivalent amount to
 */
/* shift the line in velocity.  Save the shift in expch1 in rsdm(1) */
L300:
    stk_[0].rsdm[0] -= stk_[0].expch;
    d__1 = stk_[0].vlsr - *oe;
    stk_[0].expch = tran_(&c__3, &c__1, &d__1);
    stk_[0].rsdm[0] += stk_[0].expch;
    goto L1000;
/* update refch */
L1000:
    if (stk_[0].refch != (float)-9999.) {
	stk_[0].refch = stk_[0].refch + stk_[0].expch - texpch;
    }
} /* cc_ */
