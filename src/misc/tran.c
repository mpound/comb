/* tran.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include "../main/C.h"
#include "../stacks/C.h"

#define C 299792.458

/* tran - translate a given value from one unit measure to another
 * 	Tran takes as arguments a source unit measure (CHAN, FREQ, or VEL), a
 * 	destination unit measure, and a value.  The value is assumed to be
 *	expressed in the source units.  Tran returns the same value as it would
 * 	be represented in the destination units.  It works by changing the
 * 	value into CHAN and then changing it into the desired type.
 */
double tran_(int *ufrom, int *uto, double *value)
{
    /* System generated locals */
    double ret_val;

/* skip whole process if source is the same as destination */
    if (*ufrom == *uto) {
	ret_val = *value;
	goto L8;
    }
/* change into channel */
    switch ((int)*ufrom) {
	case CHAN:		/* from channel */
	    ret_val = *value;
	    break;
	case FREQ:		/* from frequency */
	    ret_val = stk_[0].expch + (stk_[0].freq - *value) / stk_[0].fwid;
	    break;
	case VEL:		/* from velocity */
	    ret_val = stk_[0].expch + (*value - stk_[0].vlsr) * stk_[0].freq / 
		stk_[0].fwid / C;
	    break;
	default:
	    /* skip whole process if either is temperature (AGST)
	     * skip whole process if tran is being used when it doesn't apply
	     * (0) */
	    ret_val = *value;
	    goto L8;
    }
/* change into destination (note this is skipped if uto is CHAN) */
    switch ((int)*uto) {
	case CHAN:
	    break;
	case FREQ: 	/* to frequency */
	    ret_val = stk_[0].freq - (ret_val - stk_[0].expch) * stk_[0].fwid;
	    break;
	case VEL: 	/* to velocity */
	    ret_val = stk_[0].vlsr + (ret_val - stk_[0].expch) *
	   	 stk_[0].fwid * C / stk_[0].freq;
	    break;
	default:
	    /* skip whole process if either is temperature (AGST)
	     * skip whole process if tran is being used when it doesn't apply
	     * (0) */
	    ret_val = *value;
	    break;
    }
L8:
    return ret_val;
} /* tran_ */

double rtran_(int *ufrom, int *uto)
/* args are htype as defined in main/C.h */
{
	double factor = 1.0;
	register struct of_stk *st0;

	if(*ufrom != *uto) {
		st0 = &stk_[0];
		switch(*ufrom) {
		case VEL:
			factor = st0->freq / (C * st0->fwid);
			break;
		case FREQ:
			factor /= st0->fwid; /* Actually the ratio is negative*/
		}
		switch(*uto) {
		case VEL:
			factor *= C * st0->fwid / st0->freq;
			break;
		case FREQ:
			factor *= st0->fwid; /* Actually the ratio is negative*/
		}
	}
	return(factor);
}
