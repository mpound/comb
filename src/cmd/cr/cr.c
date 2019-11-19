# include "../../main/C.h"
# include "../../parse/C.h"
# include "../../graphics/C.h"
GCREAD Pcursor();

void cr()
{
	GCREAD var;
	int cnt;
	char cret;
# include "cr.dc.h"
# include "cr.ec.h"
	if(*reads > 10)
		error_("Too many cursor reads");
	for(cnt = 0;cnt < *reads;cnt++)
	{
		var = Pcursor();
		if(!var.isPlot)
			error_("No plot to read cursor on");
		xc[cnt] = var.x;
		yc[cnt] = var.y;
		crsr[cnt] = var.endFlag;
		if (*cb) {
			bxCol = var.xBox;
			bxRow = var.yBox;
			if( Pfull(bxCol, bxRow) )
				Pscreen(bxCol, bxRow);
		}
		if(!*pccf)
			switch(*pcc) {
			case 0:
		 		printf(" (%.3f,%.3f)\n",xc[cnt],yc[cnt]);
				break;
			case 1:
		 		printf(" x=%.3f\n",xc[cnt]);
				break;
			case 2:
		 		printf(" y=%.3f\n",yc[cnt]);
				break;
			}
	}
}
