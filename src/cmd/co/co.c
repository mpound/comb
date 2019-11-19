#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"

#define bcopy(s, d, n) memcpy(d, s, n)

/* Subroutine */
void co_()
{
    /* Initialized data */

    static int cmdsav = -1;

    int init, nDragon=0;

#include "co.dc.h"
#include "co.ec.h"
    bcopy(use_.muse, use_.nuse, sizeof(use_.nuse));
    if (*locad != 1 || *locsm != 2)  {
        gstak_((int *)locad, (int *)locsm);
    }
    if (cmnd_.cmdsig != cmdsav) {
	cmdsav = cmnd_.cmdsig;
	init = 1;
    } else {
	init = 0;
    }
    if (*addf != 0) {
	init += 8;
    }
    if (*acf != 0) {
	init += 32;
    }
    if (*dcf != 0) {
	init += 4;
    }
    cmbn_(&init);
    stk_[0] = stk_[1];
    if (use_.kusfl != 0) {
	bcopy(stk_[1].kuse, use_.nuse, sizeof(stk_[0].kuse));
    }
    main_.pltd = 0;
} /* co_ */
