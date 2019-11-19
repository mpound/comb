/* rm.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"

void rm_()
{
    /* System generated locals */
    int i__1;
    float r__1;

    /* Local variables */
    int chnl;
    float ssbn;
    float count;
    float rms;
#include "rm.dc.h"

#include "rm.ec.h"
    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }
    count = (float)0.;
    rms = (float)0.;
    i__1 = stk_[0].numpt;
    for (chnl = 1; chnl <= i__1; ++chnl) {
	if (ibit_(use_.nuse, &chnl) == 0) {
	    goto L10;
	}
/* Computing 2nd power */
        r__1 = stk_[0].stak[chnl - 1];
	rms += r__1 * r__1;
	count += 1;
L10:
	;
    }
    if (count <= (float)2.) {
	error_("Not enough channels in stack 1");
    }
    rms = sqrt(rms / (count - 2));
/* put rms value in stack1 */
    stk_[0].rms = rms;
/* calculate equivalent single sideband noise temp of system */
    ssbn = rms * sqrt(stk_[0].time * stk_[0].fwid * (float)1e6) / 2;
    if (*dp != 0) {
	return;
    }
    printf("rms = %.3g  system noise temp = %.1f\n", rms, ssbn);
} /* rm_ */
