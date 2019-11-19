#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"

#define bcopy(s, d, n) memcpy(d, s, n)
#define bzero(s, n) memset(s, 0, n)

/* Table of constant values */

static int c__1 = 1;

void fo_()
{
    /* System generated locals */
    int i__1, i__2;

    /* Local variables */
    int nfld;
    short nwts;
    int ichan;
    float refwt;
    float sigwt;
    float totwt;
    int iabfld;
    int nrefof, nsigof;
    short itmplc[NWEIGHTS];
    float tempwt[NWEIGHTS];

#include "fo.dc.h"

/* call parse to advance the ntty pointer. */
#include "fo.ec.h"

    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }
    if ((stk_[0].nprc & 4) != 0) {
	error_("Stack already folded");
    }
    if (stk_[0].refch == (float)-9999.) {
	error_("NO FOLD - not freq switched");
    }
    nfld = round(stk_[0].expch - stk_[0].refch);
    iabfld = ABS(nfld);
    if (stk_[0].numpt - iabfld < 5) {
	error_("NO FOLD - too few chans left");
    }
    if (nfld > 0) {
	nsigof = iabfld;
	nrefof = 0;
	stk_[0].expch -= nsigof;
	stk_[0].refch -= nsigof;
    } else {
	nsigof = 0;
	nrefof = iabfld;
    }
    bzero(tempwt, sizeof(tempwt));
    bzero(itmplc, sizeof(itmplc));
    nwts = 0;

    stk_[0].numpt -= iabfld;
    i__1 = stk_[0].numpt;
    for (ichan = 1; ichan <= i__1; ++ichan) {
	i__2 = ichan + nsigof;
	sigwt = wt_(&i__2, &c__1);
	i__2 = ichan + nrefof;
	refwt = wt_(&i__2, &c__1);
	totwt = sigwt + refwt;
	stk_[0].stak[ichan - 1] = (stk_[0].stak[ichan + nsigof - 1] * sigwt - 
		stk_[0].stak[ichan + nrefof - 1] * refwt) / totwt;
	pkwts_(&totwt, &ichan, tempwt, itmplc, &nwts);
/* L100: */
    }
    bcopy(tempwt, stk_[0].wght, sizeof(tempwt));
    bcopy(itmplc, stk_[0].locwt, sizeof(itmplc));
    stk_[0].nwts = nwts;
    bzero(&stk_[0].stak[stk_[0].numpt], iabfld << 2);
    stk_[0].rms *= (float).707106781;
    stk_[0].nprc |= 4;
    main_.pltd = 0;
} /* fo_ */

