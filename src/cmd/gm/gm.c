# include "../../main/C.h"
# include "../../graphics/C.h"

void gm()
{
	register int ptype;
#include "gm.dc.h"

#include "gm.ec.h"

	ptype = PgetType();
	if(*act > 3 && (ptype < 0 || ptype > XY_PLOT) )
		error("No plot to draw on");
	if(ptype > 0)
		Plinetype((int)*lnt);
	switch(*act) {
	case 0:
	        Pterminal(dtermstr);
		break;
	case 1:
		if( *cc < 0 || *cc >= *nc || *cr < 0 || *cr >= *nr)
			error("No Box %d, %d",bxCol,bxRow);
		if( Pfull(bxCol, bxRow) )
			Pscreen(bxCol, bxRow);
		break;
	case 2:
		Pboxes((int)*nc, (int)*nr);
		if( *cc < 0 || *cc >= *nc || *cr < 0 || *cr >= *nr)
			bxCol = bxRow = 0;
		break;
	case 4:
		Pwindow(1);
		Pmove(*hpos,*vpos);
		Ptext(ti);
		Pupdate();
	case 5:
		Pwindow(1);
		Pmove(*hpos,*vpos);
		break;
	case 6:
		Pwindow(1);
		Pline(*hpos,*vpos);
		Pupdate();
		break;
	case 7: /* MWP 7/92 -- draw one of the available shapes */
		Pwindow(1);
		Pfigure(*hpos,*vpos,*mrkn,*figw,*figh,*fill);
		if(*fill)
		 Pshow((int)bxCol, (int)bxRow);
		else
		 Pupdate(); /* caused problems sometimes with filled symbols */
		break;
	case 8:
		Pshow((int)bxCol, (int)bxRow);
		break;
	}
}
