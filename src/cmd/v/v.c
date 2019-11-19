#include "../../main/C.h"
#include "../../stacks/C.h"

void v()
{
# include "v.dc.h"
# include "v.ec.h"
	if(stkValFlag)
		stkValue = *val;
	else
		printf("%g\n",*val);
}
