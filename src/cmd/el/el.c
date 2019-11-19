/* el.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

/*
 * Wed Mar 13 12:29:30 PST 1996
 * MWP - Changed so it can work on any core stack. 
 */
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"

/* el.c*/
void elim_(/*n1, n2*/);

void el_()
{
    /* Local variables */
    int n1, n2;
    extern int stfull_();

#include "el.dc.h"
#include "el.ec.h"

    if( *stn < 1 || *stn >= FRSTONDISK)
	error("Can only eliminate channels from stacks in stacks 1 thru %d",
		(int)FRSTONDISK-1);
    if (coreFull[*stn-1] == 0) {
	error("No data in stack %d",*stn);
    }
    if(*unit == 0) {
	pltoch_(h1, h2, &n1, &n2);
    } else {
	/* This is a bad hack, but I don't have time to fix it correctly now */
	int savehtype = main_.htype;
	noint_();
	main_.htype = *unit;
	pltoch_(h1, h2, &n1, &n2);
	main_.htype = savehtype;
	yesint_();
    }
    elim_(n1, n2, *stn-1);
} /* el_ */


/*
 * elim_(start,end,stack) was originally fortran, so it operates on elements
 * start-1  to end-1 of stk_[stack].
 */

void elim_(n1, n2, stack)
int n1, n2, stack;
{
    /* Local variables */
    int i;
    double avg, cnt;

/*eliminate channels n1 through n2 in stack 1 by replacing them by the avg */
/*of the chans on either end of the range. If n1 is 1 or n2 = numpt1, then */
/* use the other as the replacement value. */
    if (n1 > 1) {
	cnt = 1.;
	avg = stk_[stack].stak[n1 - 2];
    } else {
	cnt = 0.;
	avg = 0.;
    }
    if (n2 < stk_[stack].numpt) {
	cnt += 1.;
	avg += stk_[stack].stak[n2];
    }
    if (cnt <= 0.) {
	error_("can't eliminate all channels");
    }
    avg /= cnt;
    for (i = n1; i <= n2; ++i) {
	stk_[stack].stak[i - 1] = avg;
    }
    main_.pltd = 0;
} /* elim_ */

