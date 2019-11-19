/* pltoch.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include <stdio.h>
#include "../main/C.h"
#include "../stacks/C.h"
#include "C.h"

/* Table of constant values */

static int c__1 = 1;

/* Pltoch takes an unordered range of real*8 numbers in plot units and
 *returns an ordered pair of integer channel numbers limited to the range of
 * stack 1 channels.
 */
void pltchr_(double *h1, double *h2, double *r1, double *r2)
{
    /* Local variables */
    double rt;

    /* functions */
    extern double tran_();

    if (main_.htype == 4) {
	warn_("Converting from temperature to channels");
    }
    *r1 = tran_(&main_.htype, &c__1, h1);
    *r2 = tran_(&main_.htype, &c__1, h2);
    if (*r2 < *r1) {
	rt = *r1;
	*r1 = *r2;
	*r2 = rt;
    }
    /* Limit values to the range of stack 1 channels */
    if(*r1 < .501) {
	*r1 = .501;
	if(*r2 < .501)
	    *r2 = .501;
    }
    if(*r2 > stk_[0].numpt + .499) {
	*r2 = stk_[0].numpt + .499;
	if(*r1 > stk_[0].numpt + .499)
	    *r1 = stk_[0].numpt + .499;
    }
} /* pltchr_ */

void pltoch_(double *h1, double *h2, int *n1, int *n2)
{
    /* Local variables */
    double r1, r2;
    extern int round();

    pltchr_(h1, h2, &r1, &r2);
    *n1 = round(r1);
    *n2 = round(r2);
}
