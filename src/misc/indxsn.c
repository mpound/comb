#include "../stacks/C.h"

int indxsn_(int *ip)
{
	register int i;

/* Indxsn returns the proper index into the nstk array to take care of the
 * scan numbers that are stored in the extension array - nstkx.
 */
    if ((i = *ip) > MAXNSTK_) {
	i += (stk_[0].nstkx - stk_[0].nstk) - MAXNSTK_;
    }
    return(i);
}

