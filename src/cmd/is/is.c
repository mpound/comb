# include "../../stacks/C.h"
# include "../../coordsys/C.h"
# include "../../stacks/include.h"
# include "../../main/C.h"
# include <stdio.h>
#if 0
# include <sys/file.h>
# include <sys/types.h>
# include <sys/dir.h>
# include <math.h>
#endif
# define USELEN sizeof(use_.nuse)

void is()
{
# include "is.dc.h"
	static int interpType = DUMBINTERP;

# include "is.ec.h"

	/* set directory number */
	curDir = FOREGROUND;
	CDirErr();
	OkStacks((int)*slh, (int)*sll);
	OkFreqs(*fr1,*fr2);
	if(!*itf)
		interpType = *it;
	if (interpType == GAUSSINTERP) *efd = *fwhm;
	SetStkInterp(interpType, *ir, *efd, *gs);
	SetRcXLimits(*x, *x);

	InterpAtPoint(*x,*y, *fwid, 1);

	if(coreFull[0]) {
		if(*tflg)
			main_.test = 1;
		main_.pltd = 0;
	} else {
		if(*tflg) {
			main_.test = 0;
		} else {
			error_("No stack found");
		}
	}
	
	/* retrieve use array */
	if(*su)
/*		cmove(stk_[0].kuse,use_.nuse,USELEN); */
		memcpy(use_.nuse,stk_[0].kuse,USELEN);
	else
/*		cmove(use_.muse,use_.nuse,USELEN); */
		memcpy(use_.nuse,use_.muse,USELEN);
}
