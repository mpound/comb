#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../graphics/C.h"
# include "../../stacks/C.h"
#include "../../image/C.h"

char *ChkAlloc();

/**************************************************************
 * define an area of an image using the vertices of a polygon *
 **************************************************************/

void da()
{
	register struct of_area *ap;
	register int i, old;
	GCREAD curdata, Pcursor();

# include "da.dc.h"
# include "da.ec.h"

	AreaNumChk(*an);
	if( ! *sbfl) {
	    ap = AllocArea(*an);
	    switch(*sbt) {
	    case 1:
		if(*Xe != *Ye && (*Xe != *Ye + 1 || X[*Xe - 1] != DRAGON) )
			error("x and y coordinates not matched ");
		for(i = 0; i < *Xe; i++) {
			ap->v[i].x = X[i];
			ap->v[i].y = Y[i];
		}
		ap->v[i].x = FDRAGON;
		ap->l = i;
		break;
	    case 2:
		for(i = 0; i < 129; i++) { /* modified for more vertices--
					    * MWP  Mon Aug 2 10:29:15 EDT 1993
				            */
			curdata = Pcursor();
			if( !curdata.isPlot) {
				warn("cursor not on a plot");
				continue;
			}
			ap->v[i].x = curdata.x;
			ap->v[i].y = curdata.y;
			if( !curdata.endFlag) {	/* endFlag has reverse logic */
				ap->v[++i].x = FDRAGON;
				ap->l = i;
				break;
			}
		}
		break;
	    }
	} else {
		ap = areas[*an];
		if(ap == 0)
			error("Area %d not yet defined", *an);
	}
	if( *pf) {
	    printf("da po:[\\\n");
	    for(i = 0; i < ap->l; i++) {
		if(ap->v[i].x == FDRAGON)
			printf("       .DR        .DR\\\n");
		else
			printf("%10.3f %10.3f\\\n", ap->v[i].x, ap->v[i].y);
	    }
	    printf("] an:%d\n",*an);
	}
	if(*plf) {
	    if( PgetType() != SP_SP )
		error("No space-space plot to plot on");
	    Pwindow(1);
            Plinetype((int)*lnt);
	    old = -1;
	    for(i = 0; i <= ap->l; i++) {
		if(ap->v[i].x == FDRAGON) {
		    if(old >= 0)
			Pline(ap->v[old].x, ap->v[old].y);
		    old = -1;
		} else if(old < 0) {
		    Pmove(ap->v[i].x, ap->v[i].y);
		    old = i;
		} else {
		    Pline(ap->v[i].x, ap->v[i].y);
		}
	    }
	    Pupdate();
	}
	if(*paf) {
		AreaArea(*an);
		printf("Area of area %d is %g\n", *an, ap->area);
	}
}
