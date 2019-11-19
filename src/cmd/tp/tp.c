#include "../../stacks/C.h"
#include "../../coordsys/C.h"
#include "../../stacks/include.h"
#include "../../main/C.h"
#include <math.h>

void tp()
{
	double sum = 0,totwt = 0,wt,rms;
	register int i;
	extern int ibit_();

#include "tp.dc.h"
#include "tp.ec.h"
	if(!coreFull[0])
		error("No data in stack 1");
	if(!coreFull[2])
		error("No cal in stack 3");
	if(stk_[0].nprc != stk_[2].nprc || stk_[0].numpt != stk_[2].numpt)
		error("Stacks 1 and 3 not compatible");

	for(i = 0;i < stk_[0].numpt;i++) {
		if(ibit(use_.nuse,i) && (wt = stk_[2].stak[i]) > 0.) {
			wt = 1./(wt * wt);
			totwt += wt;
			sum += stk_[0].stak[i] * wt;
		}
	}
	if(totwt > 0) {
		sum /= totwt;
		stk_[0].area = sum;
		rms = sqrt(2./(totwt * stk_[0].fwid * 1000000 * stk_[0].time));
		printf("avg = %g +- %g K\n",sum,rms);
	} else {
		error("No chans to avg");
	}
}
