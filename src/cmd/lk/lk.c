#include <stdio.h>
# include "../../main/C.h"
# include "../../coordsys/C.h"
# include "../../stacks/C.h"
# include "../../stacks/include.h"
# include "../../graphics/C.h"
#include <math.h>
# include <string.h>

/* comminucate with PlotStk */
static double figWidth,figHeight;		/* size of figure to plot */
static XYPOINT ll,ur;
static int chkFlag;
static int fignum;				/* symbol to plot */ 
static int filled;				/* is symbol filled? */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* lk.c */
static int PlotStk P_((register STKINDX *stk));
#undef P_

void lk()
{
	double dx, dy;
	int nx, ny, dir;
	register char *cp;
	char tempstr[4];
	
#include "lk.dc.h"
#include "lk.ec.h"
	if(*llx > *urx) {
		ll.x = *urx;
		ur.x = *llx;
	} else {
		ll.x = *llx;
		ur.x = *urx;
	}
	if(*lly > *ury) {
		ll.y = *ury;
		ur.y = *lly;
	} else {
		ll.y = *lly;
		ur.y = *ury;
	}
	if(ur.x - ll.x < .001 || ur.y - ll.y < .001)
		error_("Insufficient spatial extent");
	SetRcXLimits(ll.x, ur.x);

        /* set directory number */
	/*curDir = FOREGROUND;  mwp 5/92 */
	dir = curDir;
        curDir = (*dt)?1:0;

	CDirErr();
	OkStacks((int)*slh, (int)*sll);
	OkFreqs(*fr1,*fr2);

	fignum=*mrkn;
	if (!*ovl ) {
	 Pstart();
	 PsetType(SP_SP);
	 Pformat("full square plot");
	 Pbounds(*llx,*lly,*urx,*ury);
	 if(csys.type == RADEC && (csys.xun & SCALED) == 0) {
		Punitsratio(15.0 * cos(csys.y + cunits[csys.yun] *
			(*lly + *ury) / 2.));
		MkOpt(CLEAROPT, XLABHMS, YLABHMS, ENDOPT);
	 } else
		MkOpt(CLEAROPT, ENDOPT);
	 MkGrid(NO FLAGY0,NO FLAGX0,LTYPE(29),ENDOPT);
	}

	if(*psn) {
		figWidth = 0;
		figHeight = 0;
	} else { if(*figh == 0) 
		    figHeight = (ur.y - ll.y)*.025;
	         else	 
		    figHeight = *figh;
	         if(*figw == 0) 
		    figWidth = (ur.x - ll.x)*.025;
	         else
		    figWidth = *figw;
	}
	filled = (*fill)? 1 : 0;

	/* Set up nx, ny, and interpolation radius so that FillInArray will
	 * find an adequate, but not too large range in x, y, and z */
	dx = (ur.x - ll.x) * cunits[csys.xun & 0xf];
	dy = (ur.y - ll.y) * cunits[csys.yun];
	/* pick somewhat arbitrary divisions of the space */
	nx = 2 + 5 * sqrt(dx);
	ny = 2 + 5 * sqrt(dy);
#if 0
	/* coordinate range in an individual cell */
	dx /= nx;
	dy /= ny;
	/* Interpolation radius  = asin(1 - cos(span / 2)) ~ (dx*dx + dy*dy)/8
	 * Here we approximate the trig and give some extra margin */
	interpRad = (dx*dx + dy*dy) * .14 / cunits[ARCMINUTES];
	if(interpRad < 1.0)
		interpRad = 1.0;
/* fprintf(stderr,"nx = %d, ny = %d, ir = %g\n",nx, ny, interpRad); */
#endif
	SetStkInterp(NOINTERP,0.25,0.0,0.0);
	chkFlag = *chk;
	Plinetype((int)*lnt);
	FillInArray(ll,ur,nx,ny,PlotStk);
	FreeArray();

	if (!*ovl) {
	 /* put on labels */
	 if(*mlbf) {
		if(cp = strrchr(dirName[curDir],'/'))
			cp ++;
		else
			cp = dirName[curDir];
		strcpy(mlb,cp);
		strcat(mlb," Stack Locations");
	 }
	 Pplace(0,mlb);
	 /* Differential or absolute coords? */
	 tempstr[0] = (rcsys.ctp == rcsys.otp && rcsys.x == 0 && rcsys.y == 0 &&
		rcsys.otp != DXDY)? 0 : 'D';
	 tempstr[1] = '\0';
	 if(*hlbf)
		sprintf(hlb,"%s%s(%s)",tempstr,horizCoordNames[csys.type],
			unitNames[7 & csys.xun]);
	 Pplace(1,hlb);
	 if(*vlbf)
		sprintf(vlb,"%s%s(%s)",tempstr,vertCoordNames[csys.type],
			unitNames[csys.yun]);
	 Pplace(2,vlb);
	}
	printf("Displaying map\n");
	Pshow(bxCol,bxRow);
        /* set directory number back to what it was -- mwp 5/92*/
        curDir = dir;
}

static int PlotStk(stk)
register STKINDX *stk;
{
	XYZPOINT loc;
	XYPOINT rc;
	STKINDX tempIndx;		/* temporary stack index structure */
					/* to hold index while being checked */

	loc.x = stk->x - HALFSPACE;
	loc.y = stk->y - HALFSPACE;
	loc.z = stk->z - HALFSPACE;
	rc = Proj(loc);
/*	if(ll.x > 0 && rc.x < 0)
		MakeXPos(&rc);
*/

	if(figWidth) {
		if(rc.x >= ll.x && rc.x <= ur.x && rc.y >= ll.y &&
				rc.y <= ur.y) {
			Pfigure(rc.x, rc.y,fignum,figWidth,figHeight,filled);
			/*Pfigure(rc.x, rc.y,F_CROSS,figWidth,figHeight);*/
		}
		if(chkFlag) {
		    getStack(stk->stkNum, 1);
		    if( !coreFull[0] )
			fprintf(stderr,
			"Stack %d in search file at (%g, %g) is empty\n"
			, stk->stkNum, rc.x, rc.y);
		    tempIndx = MkSrchIndx(&stk_[0]);
		    if(abs(tempIndx.x - stk->x) +
			abs(tempIndx.y - stk->y) +
			abs(tempIndx.z - stk->z) +
			abs(tempIndx.freq - stk->freq) > 0)
			fprintf(stderr, "Search returns (%d, %d, %d, %d, %d)\nStack gives    (%d, %d, %d, %d, %d)\n",
			tempIndx.x, tempIndx.y, tempIndx.z,
			tempIndx.freq, tempIndx.stkNum);
		}
	} else {
/*		Pgrnum(rc,(double)stk->stkNum);	*/
		char numStr[16];
		if(rc.x > ll.x && rc.x < ur.x && rc.y > ll.y && rc.y < ur.y) {
			Pmove(rc.x, rc.y);
			sprintf(numStr,"%d",stk->stkNum);
			Pctext(numStr);
		}
	}

	return(0);
}
