#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"
#include "../../main/machinedep.h"
 
#if SYSTEM_V
#define bcopy(s, d, n) memmove(d, s, n)
#endif /*SYSTEM_V*/

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* us.c */
static void stuse_ P_((int *ivalue, int *nb, int *ne));
#undef P_

void us_()
{
    /* Local variables */
    int i, idiff;
    int nb, ne;
    int ivalue;
    int nb2, ne2;
#include "us.dc.h"

#include "us.ec.h"
/* Convert iactn to int*4 for passing to the stuse subroutine. */
    ivalue = *iactn;
/* If retrieve was requeted, do it first. */
    if (*rtflg != 0) {
	bcopy(use_.muse, use_.nuse, sizeof(use_.nuse));
    }
/* Compliment is the next operation. */
    if (*coflg != 0) {
	for (i = 0; i <= sizeof(use_.nuse); ++i) {
	    use_.nuse[i] ^= -1;
	}
    }
/* hflg will = 0 if horiz values were given. If they weren't given, either */
/* don't call use at all or use the full range of stack1. */
    if (*hflg != 0) {
/* If any other action was requested, don't call use. */
	if (*rtflg + *coflg + *stflg != 0) {
	    goto L100;
	}
/*As a special convenience, the command us with no arguments sets the 
use array*/
/* rather than clearing it as would be logical from the input tree. */

/*This test allows the user to clear the use array if specificaly requ
ested.*/
	if (*actfl == 0) {
	    ivalue = 1;
	}
/* Finally the default channel numbers. */
	nb = 1;
	ne = stk_[0].numpt;
    } else {
/* make sure that plot units are defined */
	if (coreFull[0] == 0) {
	    error_("No data in stack 1");
	}
/* convert from plot units to chans. */
	pltoch_(h1, h2, &nb, &ne);
    }
    stuse_(&ivalue, &nb, &ne);
/*If requested and freq sw data repeat the action shifted from sigch to re
fch.*/
    if (*rfflg != 0 && stk_[0].refch != (float)-9999.) {
	idiff = stk_[0].refch - stk_[0].expch;
	nb2 = nb + idiff;
	ne2 = ne + idiff;
	if (idiff > 0 && nb2 < ne || idiff < 0 && ne2 > nb) {
	    error_("Use for sig & ref overlap");
	}
	stuse_(&ivalue, &nb2, &ne2);
    }
/* Finally save the result if requested. */
L100:
    if (*stflg != 0) {
	bcopy(use_.nuse, use_.muse, sizeof(use_.nuse));
    }
} /* us_ */

static void stuse_(ivalue, nb, ne)
int *ivalue, *nb, *ne;
{
    int n1, n2;

/* Local variables */
/*If ivalue is 0 or 1 the use array is cleared or set between chan nb and ne.*/
/* First limit the values to real channels. */
    if (*ne <= 0 || *nb > stk_[0].numpt || *ne < *nb) {
	return;
    }
    n1 = MAX(*nb,1);
    n2 = MIN(*ne,(int) stk_[0].numpt);
    filbts_(use_.nuse, ivalue, &n1, &n2);
} /* stuse_ */

