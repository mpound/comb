#include <stdio.h>
#include <math.h>
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../coordsys/C.h"
#include "../../stacks/C.h"
#include "../../stacks/include.h"
#include "../../error/C.h"
#include "../../graphics/C.h"
#include "../../image/C.h"
#include "../../misc/C.h"
#define MAXBIN 10000

#define bzero(s, n) memset(s, 0, n)

char *getenv();
static char thName[64];

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* th.c */
static char *THName P_((void));
#undef P_

/***********************************************************
 * th - temperature histogram  :  written by M. Pound 6/91 *
 *							   *
 * graphics plot added 3/93    :MWP	  	           *
 * graphics debugging and full functionality 5/93 :MWP     *
 ***********************************************************/

void th()
{
	FILE *thFile;
	int neg[MAXBIN], pos[MAXBIN], totalPix;
	int i, j, index, MaxPosInd, MaxNegInd, nEmpty, npt;
	double lowChan, highChan; 
	int lastStk;
        int lox, hix, loy, hiy;         /* Extent of area in pixels */
        int xsize, ysize;               /* # of lines, pix / line in array */
	int cmdsigsv;            	/* Save command line number to control
                                         * printing of header */
        struct of_stk tstk;             /* for accumulating the avg spectrum */
        double ymin, ymax;              /* Y extent of the area in 'Arc */
        double xmin, xmax;              /* X extent of the area */
        double xspacing, yspacing;      /* pixel spacing in rel coordinates */
        double x, y;                    /* current x and y in rcsys units */
	double xmn,xmx,ymn,ymx;		/* max, min of graph */
	int islog = 0;			/* a log plot ?      */
	static float *xpt,*ypt;
	extern void pltchr_(/* &h1, &h2, &r1, &r2 */);
	char llclb[64];			/* Lower Left Corner LaBel */
	float *savex, *savey;
	
# include "th.dc.h"
# include "th.ec.h"

	nEmpty = totalPix = MaxPosInd = MaxNegInd = 0;

	curDir = FOREGROUND;
	CDirErr();
	lastStk =  ((*slh < NumInds()) ? *slh : NumInds());
        OkStacks(lastStk,*sll);

/* FUTURE WORK: allow use of area number 
 * (must add it to syntax tree	)
 * I think I can make it so that cmbn_() is used on stacks
 * at same position. by stealing code from vm.
 *
 *       if((csys.xun & 0xf) != csys.yun)
 *               warn("X and Y units differ");
 *
 * sp is in the tree
 *
 *       xspacing = *sp * cunits[ARCMINUTES] / cunits[csys.xun & 0xf];
 *       yspacing = *sp * cunits[ARCMINUTES] / cunits[csys.yun];
*/
 
/*        ChkArea((int)*an); */
/*        AreaLimits((int)*an, &xmin, &xmax, &ymin, &ymax); */
        /* For the area include equality at the low end, but not at the top
         * so that adjacent areas don't overlap or miss pixels */
/*
 *       loy = ceil( ymin / yspacing );
 *       hiy = ceil( ymax / yspacing ) - 1;
 *       lox = ceil( xmin / xspacing );
 *       hix = ceil( xmax / xspacing ) - 1;
 *       xsize = hix - lox + 1;
 *       ysize = hiy - loy + 1;
*/

        if(*logx)
                islog=1;
        if(*logy)
                islog+=2;
 
	strcpy(thName, fn);
	thFile = FSafeOpen(thName,thName,"w");

	for(i=0;i<MAXBIN;i++) 
		pos[i] = neg[i] = 0;

	for (i=lowStkNum;i<highStkNum+1;i++) {
	/* get i-th stack and put in stack 1 */
		StkNumErr(i,NULL);
		getStack(i,1);
		if(!coreFull[0]) {
			printf("Stack %d empty. Getting next stack.\n",i);
			continue;
		}
		/* set channel limits */
		switch(*svl) {
			case 0: 	/* use all velocities */
				lowChan=1.;
				highChan = (double)stk_[0].numpt;
				sprintf(llclb,"tbin:%g a: xfact:%g yfact:%g",
*tbin,*xfact,*yfact);
				break; 
			case 1: /* pltchr translates the velocity 
				 * limits to channels and orders them 
				 */
        			pltchr_(lv, hv, &lowChan, &highChan);
				sprintf(llclb,"tbin:%g vl:%g,%g xfact:%g yfact:%g",
*tbin,*lv,*hv, *xfact,*yfact);
				break;
		}
		for (j=round(lowChan); j < round(highChan)+1;j++ ) {
		    if(j < 1) { 
			j = 1;
			warn_("Velocity limits extend below stack.\n");
		    }
		    if(j > stk_[0].numpt) {
			warn_("Velocity limits extend above stack.\n");
			break;
		    }
/* don't include points with values less than minx or more than maxx (unscaled) */
		    if(stk_[0].stak[j] >= *minx && stk_[0].stak[j] <= *maxx){ 
		     if((index = (int)(stk_[0].stak[j]*(*xfact)/(*tbin)))>MAXBIN)
			error_("Too many bins");
/* put temp zero in with positives Temps */
		     if(index >= 0) {
		        MaxPosInd = ((MaxPosInd > index) ? MaxPosInd : index);
			++(pos[index]);
		     } else {
			++(neg[-index]);
		        MaxNegInd = ((MaxNegInd > -index) ? MaxNegInd : -index);
		     }
		    }
		}
	}

/* output negs first, zero, then pos -- so histogram in mongo
 * connects nicely! 
 */
	npt = MaxPosInd+MaxNegInd+1;
	xpt = (float*) ChkAlloc(npt*sizeof(float),"X hist pt");
	ypt = (float*) ChkAlloc(npt*sizeof(float),"Y hist pt");
	bzero(xpt,npt*sizeof(float));
	bzero(ypt,npt*sizeof(float));
	savex=&xpt[0];
	savey=&ypt[0];
	ymn = -1.;
	ymx = 0.;
        for (j=0,i = MaxNegInd; i > 0; i--,j++) {
		xpt[j] = -(*tbin*(float)i)*(*xfact);
		ypt[j] = (float)neg[i]*(*yfact);
                fprintf(thFile,"%f %f\n",xpt[j],ypt[j]);
		if(ypt[j] > ymx)
			ymx = ypt[j];
                totalPix += neg[i];
                if(neg[i] == 0)
                        nEmpty++;
        }
/* neg[0] should always be zero */
	if(neg[0] != 0)
		fprintf(stderr,"BUG: neg[0] != 0\n");
	for (i = 0;i < MaxPosInd+1; i++) {
		xpt[j] = *tbin*(float)i*(*xfact);
		ypt[j] = (float)pos[i]*(*yfact);
		if(ypt[j] > ymx)
			ymx = ypt[j];
		fprintf(thFile,"%f %f\n",xpt[j],ypt[j]);
		j++;
		totalPix += pos[i];
		if(pos[i] == 0) 
			nEmpty++;
	}
	printf("\nMax. Temp.(K)    Min. Temp.(K)   Total Pixels    \
Num. Empty Bins\n");
	printf(" %6.3f\t\t\t%6.3f\t\t%d\t\t%d\
\n",MaxPosInd*(*tbin), -MaxNegInd*(*tbin),totalPix,nEmpty);
        FSafeClose(thFile);
	xmn = -(*tbin*(double)MaxNegInd);
	xmx = *tbin*(double)MaxPosInd;

/*
 * now plot it -- MWP Thu Mar  4 15:52:17 EST 1993 
 * Stolen from sp.c graphics.
 */
	Pstart();
	Pformat("full plot");
        MkOpt(CLEAROPT,ENDOPT);
/*
 * Options KEYX and KEYY will make MkGrid() call Rebound() to scale
 * the grid according to the points
 */
            switch(islog) {
            case 0: /* not a log plot */
                Pbounds(xmn,ymn,xmx,ymx); /*L,B,R,T*/
                break;
            case 1: /* logX-linY plot */
		ymn=1;
                Plogbounds(xmn,ymn,xmx,ymx,1,0);
                break;
            case 2: /* linX-logY plot */
		ymn=1;
                Plogbounds(xmn,ymn,xmx,ymx,0,1);
                break;
            case 3: /* log-log plot */
		ymn=1;
                Plogbounds(xmn,ymn,xmx,ymx,1,1);
                break;
            }
	MkOpt(KEYY,NO KEYX,ENDOPT);
        MkGrid(NO FLAGX0,FLAGY0,ENDOPT);
        MkGraph(fYLIST(savey,npt-1),XSTEP(*tbin),HIST,ENDOPT);
        if(!*mlbf)  /* default is no main label */
                Pplace(0,mlb);
        if(*hlbf)
		switch(islog) {
		case 0:
		case 2:
                	sprintf(hlb,"Temperature (K)");
			break;
		case 1:
		case 3:
                	sprintf(hlb,"Log(Temperature) (K)");
			break;
		}
        Pplace(1,hlb);
        if(*vlbf) {
		switch(islog) {
		case 0:
		case 1:
                	sprintf(vlb,"N");
			break;
		case 2:
		case 3:
                	sprintf(vlb,"Log(N)");
			break;
		}
	}
        Pplace(2,vlb);
        Pplace(3,llclb);
	Pshow(bxCol,bxRow);
	xpt=savex;
	ypt=savey;
	ChkFree((char *)xpt);
	ChkFree((char *)ypt);
}

static char *THName()
{
        if(*thName == '\0') {
                sprintf(thName, "%s/th.out", getenv("HOME") );
        }
        return(thName);
}
