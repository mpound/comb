#include <stdio.h>
#include "C.h"
#include "../main/C.h"
#include "../stacks/C.h"

void pkwts_(float *totwt, int *ichan, float *tempwt, short *itmplc, short *nwts)
{
    /* System generated locals */
    float r__1;

    /* Local variables */
    float diff;
    int i;
    int isave;
    float smdiff;

/*Pack weights into the condensed weight array. This subroutine must be ca
lled*/
/* in order of increasing chan number. The arguments are: */
/* 	totwt	the weight for the current channel. */
/* 	ichan	the current chan number. */
/* 	tempwt	a working array of 15 reals to hold the new weights. */
/* 	itmplc	15 integers to hold the new locwt`s. */
/* 	nwts	integer*2 will be set to the number of discrete weights. */
/* 		nwts must be initialized to 0 before the first call. */
/* 	The working arrays should also be cleared before the first call. */
/* Check for first call */
/* If totwt is within 2% of previous stored weight, no action is needed. 
*/
    /* Parameter adjustments */
    --itmplc;
    --tempwt;

    /* Function Body */
    r__1 = *totwt * (float)2e-2;
    if (*nwts != 0 && ! ifdif_(totwt, &tempwt[*nwts], &r__1)) {
	return;
    }
    if (*nwts < 15) {
	++(*nwts);
	goto L300;
    }

/*At this point we have run out of space in the weight array. First we wil
l*/
/* find the adjacent pair of weights which is closest. These will then be 
*/
/* replaced by their average to make room for the new value. The first */
/* thing to consider is the new weight and the last one in the array. */

    smdiff = (r__1 = tempwt[15] - *totwt, DABS(r__1));
    isave = 15;
    for (i = 1; i <= 14; ++i) {
	diff = (r__1 = tempwt[i + 1] - tempwt[i], DABS(r__1));
	if (diff < smdiff) {
	    smdiff = diff;
	    isave = i;
	}
/* L210: */
    }
/* Now combine the two closest weights. */
    if (isave == 15) {
	tempwt[15] = (tempwt[15] + *totwt) / (float)2.;
	return;
    }
    tempwt[isave] = (tempwt[isave] + tempwt[isave + 1]) / (float)2.;
/* Close up the weight array if necessary */
    for (i = isave + 1; i <= 14; ++i) {
	tempwt[i] = tempwt[i + 1];
	itmplc[i + 1] = itmplc[i + 2];
/* L240: */
    }
/* Finally put in the new weight. */
L300:
    tempwt[*nwts] = *totwt;
    itmplc[*nwts] = *ichan;
} /* pkwts_ */

double wt_(int *nchan, int *istack)
{
    register struct of_stk *stk = &stk_[*istack - 1];
    int i;

    for (i = 1; i < stk->nwts; ++i) {
	if (*nchan < stk->locwt[i]) {
	    break;
	}
    }
    return(stk->wght[i - 1]);
} /* wt_ */
