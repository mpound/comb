/* rs.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"

/*  rescale data rs */
void rs_()
{
    /* Local variables */
    int i;

    /* System generated locals */
    int i__1;

#include "rs.dc.h"

#include "rs.ec.h"
    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }

    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	  stk_[0].stak[i - 1] = *set + *factr * stk_[0].stak[i - 1];
    }

    stk_[0].factt = *factr * stk_[0].factt;
    stk_[0].rms *= ABS(*factr);
    main_.pltd = 0;

} /* rs_ */
