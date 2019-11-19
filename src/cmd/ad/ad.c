#include "../../main/C.h"
#include "../../stacks/C.h"
#include "../../scan/C.h"
#include "../../misc/C.h"
#include "../../main/machinedep.h"

#define bcopy(s, d, n) memcpy(d, s, n)

/* Table of constant values */

static int c__2 = 2;

void ad_()
{
    /* Initialized data */

    static int cmdsav = -1;
    static short jbknd[3] = { 100,25,10 };


    /* System generated locals */
    int i__1;

#include "ad.dc.h"

    /* Local variables */
    int isav, init;
    int nscan;
    int itest, kount;
    extern int nxtscn_();

/*  ncare=0 means add will add all scans regardless of baseline */
/*      or clouds or pointing problems */
/*  ncare=1,2,3 means that add is increasingly severe about using */
/*      dubious scans */
/*  ncare=1 is default in cmi */

/* Set up the scan number range to the old values.  Parse can't do this */
    *sn1n = *sn2n = curscn_.num;
    bcopy(curscn_.fil, sn1f, 3);
    bcopy(curscn_.fil, sn2f, 3);

#include "ad.ec.h"

    i__1 = scan_.ndfn - 3;
    bcopy(sn1f, scan_.datfn + i__1, 3);
    subscanReq = *ssn;
/* set up limits, always declaring that limits were set */
    scnlmt_(sn1n, sn2n, sn1f, sn2f, (int)*istep, 0);
/* set up backend */
    if (*lbetyp > 0) {
	scan_.mbknd = jbknd[*lbetyp - 1];
    } else {
	scan_.mbknd = *lbknd;
    }
/* open data file and initialize a few things */
    opend_();
    bcopy(use_.muse, use_.nuse, sizeof(use_.nuse));
    if (cmnd_.cmdsig != cmdsav) {
	cmdsav = cmnd_.cmdsig;
	init = 1;
    } else {
	init = 0;
    }
    kount = 0;
    nscan = *sn1n;
L100:
    curscn_.num = nscan;
    if(read_(&curscn_.num)) {
	if(*sn1n == *sn2n)
	    error("Scan %d is empty", curscn_.num);
	++kount;
	goto L150;
    }
    if (nscan == *sn1n) {
	isav = scan_.iobstp;
    } else if (scan_.iobstp != isav) {
	++kount;
	goto L150;
    }
/* this is the place to check icare stuff */
    if (scan_.ndat[33] + *ncare > 2) {
	++kount;
	goto L150;
    }
    if(!get_())
	error("Bad backend");
/* do we need to check for matching header? */
    if (*mhflg != 0 && stk_[1].nlist != 0 && stk_[0].nlist != 0) {
	shift_(&c__2, &itest);
	if (itest != 0) {
	    ++kount;
	    goto L150;
	}
    }
    cmbn_(&init);
    init = 0;
L150:
    nscan = nxtscn_();
    if (nscan > 0) {
	goto L100;
    }
    if (kount != 0) {
	printf("%d scans not added\n", kount);
    }
    stk_[0] = stk_[1];
    main_.pltd = 0;
} /* ad_ */

