/* ca.f -- translated by f2c (version of 16 May 1991  13:06:06).
*/

#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"
#include "../../parse/C.h"

void ca_()
{
#include "ca.dc.h"
#include "caf.dc.h"

    /* Local variables */
    register int chan;
    int from;
    int to;
    int off2, off3;
    char *savegstr;

#include "ca.ec.h"
    /* Request for tree info from caf? */
    if (strcmp(fn, "?!") == 0 || strcmp(fn, "??") == 0) {
	cmnd_.gstr = fn;
#include "caf.ec.h"
    }
    if (*ns < 1 || *ns > 3) {
	error_("Bad number of stacks");
    }
    if (!coreFull[0]) {
	error_("Stack 1 is empty");
    }
/* fill used stacks, decide offsets, and bounds. */
    from = 1;
    to = stk_[0].numpt;
    switch ((int)*ns) {
	case 3:
	    if (!coreFull[2]) {
		warn_("No data in stack 3");
		stk_[2] = stk_[0];
		coreFull[2] = 1;
	    }
	    off3 = stk_[2].expch - stk_[0].expch;
	    if(from < 1 - off3)
		from = 1-off3;
	    if(to > stk_[2].numpt - off3)
		to = stk_[2].numpt - off3;
	case 2:
	    if (!coreFull[1]) {
		warn_("No data in stack 2");
		stk_[1] = stk_[0];
		coreFull[1] = 1;
	    }
	    off2 = stk_[1].expch - stk_[0].expch;
	    if(from < 1 - off2)
		from = 1-off2;
	    if(to > stk_[1].numpt - off2)
		to = stk_[1].numpt - off2;
    }
    if (from >= to) {
	error_("No overlapping channels");
    }
/*check that the use array is set for at least some channels in the overlap.*/
    while(!ibit(use_.nuse, from)) {
	++from;
	if (from >= to) {
	    error_("Use array not set within range");
	}
    }
/* step through data and perform calculation */
    savegstr = cmnd_.gstr;
    for (chan = from; chan <= to; ++chan) {
	if (ibit(use_.nuse, chan)) {
	    switch ((int)*ns) {
		case 3:
		    gg_[3] = stk_[2].stak[chan + off3 - 1];
		case 2:
		    gg_[2] = stk_[1].stak[chan + off2 - 1];
		case 1:
		    gg_[1] = stk_[0].stak[chan - 1];
	    }

	    /* Cary out the function on global variables */
	    cmnd_.gstr = fn;
#include    "caf.ec.h"

	    switch ((int)*ns) {
		case 3:
		    stk_[2].stak[chan + off3 - 1] = gg_[3];
		case 2:
		    stk_[1].stak[chan + off2 - 1] = gg_[2];
		case 1:
		    stk_[0].stak[chan - 1] = gg_[1];
	    }
	}
    }
    /* reset gstr for the next command if any */
    cmnd_.gstr = savegstr;
    /* set plot flag */
    main_.pltd = 0;
} /* ca_ */
