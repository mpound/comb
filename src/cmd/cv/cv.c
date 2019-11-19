/*
 * Convolve the data in stack 1 with a pattern in stack 3 putting the result
 * in stack 1.  Stack 1 is effectively extended with enough zeros to compute
 * the first and last channels by limiting the range of the inner product
 * to remain within the limits of stack 1.  No normalization is done and
 * it is assumed that expch3 is the center of the convolving function.
 */

#include <stdio.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../error/C.h"

void cv()
{
	float *tempst;		/* Pointer to temp storage for the result */
	int tsize;		/* size of storage array */
	int n1, n3;		/* Number of chans in stacks 1 & 3 */
	int i1, i2;		/* Range of inner product */
	int ch, i;		/* Current chan, offset for inner product */
	int e3;			/* expected (center) chan in stack 3 */
	int maxi2;		/* max value of i2 determined from stk 3 */

#include "cv.dc.h"

#include "cv.ec.h"

	if (coreFull[0] == 0) {
	    error_("cv: No data in stack 1");
	}
	if (coreFull[2] == 0) {
	    error_("cv: No data in stack 3");
	}

	/* Set up things for plot */
	main_.pltd = 0;

	n1 = stk_[0].numpt;
	tsize = n1 * sizeof(float);
	tempst = (float *)SafeAlloc(tsize, "cv array");
	memset(tempst, 0, tsize);
	/* Start off with i1 at its minimum value as determined by stack 3.
	 * It will be increased to zero as we approach the last channel.
	 * Correct e3 for zero based chan numbers and round to the nearest
	 * channel.  Maxi2 and i1 are calculated from stack 3, but the stack
	 * 1 limits are applied as we go through the channels.
	 */
	i1 = -(e3 = stk_[2].expch - .500001);
	maxi2 = stk_[2].numpt - e3 - 1;
	for(ch = 0; ch < n1; ch++) {
	    if(ch - i1 >= n1)		/* Don't go beyond last chan */
		i1 = ch - n1 + 1;
	    if(ch <= maxi2)		/* i2 starts at 0 and grows to maxi2 */
		i2 = ch;
	    /* Do the inner product for this channel */
	    for(i = i1; i <= i2; i++) {
		tempst[ch] += stk_[0].stak[ch - i] * stk_[2].stak[e3 + i];
	    }
	}
	main_.pltd = 0;
	/* Move the result into stack 1 */
	memcpy(stk_[0].stak, tempst, tsize);
	SafeFree((char *)tempst);
	/* Correct the expected ch for non integral center chan in stack 3 */
	stk_[0].expch -= e3 + 1 - stk_[2].expch;
}
