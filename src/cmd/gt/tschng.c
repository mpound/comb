# include "../../parse/C.h"
# include "../../main/C.h"
# include "../../stacks/C.h"

static int svcmdsig = -1;
static float ra,dec,epoch;
static double freq;

void tschng_()
{
	register struct of_stk *stk = &stk_[0];
	if(cmnd_.cmdsig == svcmdsig) {
		if(stk->ra != ra || stk->dec != dec ||
			stk->epoch != epoch || stk->freq != freq)
			main_.test = 0;
		else
			main_.test = 1;
	} else {
		ra = stk->ra;
		dec = stk->dec;
		epoch = stk->epoch;
		freq = stk->freq;
		svcmdsig = cmnd_.cmdsig;
		main_.test = 1;
	}
}
