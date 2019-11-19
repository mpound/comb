#include <stdio.h>
#include <sys/file.h>
#include <string.h>
#include <math.h>
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../stacks/C.h"
#include "../../image/C.h"
#include "../../error/C.h"
#include "../../graphics/C.h"

static char slName[64] = "";

extern FILE *FSafeOpen();
extern char *getenv();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* sl.c */
static char *SliceName P_((void));
#undef P_

/******************************************************************
 * Make a slice through an image in a file suitable for mongoplot *
 ******************************************************************/

void sl()
{
	double deltax, deltay;
	double x,y,dist;
	float *lp;
	int nx1,nx2,ny1,ny2;
	int ix1,ix2,iy1,iy2;
	int nx,ny;
	int xdir,ydir;
	int xsteps,ysteps;
	int ltype, eqn;
	FILE *slFile;
	FILE *oldFile;
	char oldName[68], tmpName[68];
	char oldLine[132];
	char *cp;
	int dummy;
/*
 * Stuff for plotting.  graphics code lifted from sp() -- MWP 5/94
 */
        static float *xpt, *ypt;        /* pointers to values to be plotted */
        struct of_imagehdr *ihp;        /* pointer to image header          */
        double xmn,ymn,xmx,ymx;         /* min &  max values of x and y     */
        int islog=0;                    /* a log plot?                      */
        float *savex, *savey;           /* addresses of beginning of xpt    */
                                        /* and ypt memChunks.               */
	int numpoints=0; 		/* number points to plot	    */

# include "sl.dc.h"
# include "sl.ec.h"

	*lnt &=255;
	strcpy(slName, fn);
	ChkImage((int)*im);
	ImageXGrid((int)*im, &ix1, &ix2, &deltax, &dummy);
	ImageYGrid((int)*im, &iy1, &iy2, &deltay, &dummy);
	nx1 = round(*x1 / deltax);
	nx2 = round(*x2 / deltax);
	ny1 = round(*y1 / deltay);
	ny2 = round(*y2 / deltay);
	if( (xsteps = nx2 - nx1) >= 0 ) {
		xdir = 1;
	} else {
		xdir = -1;
		xsteps = - xsteps;
	}
	if( (ysteps = ny2 - ny1) >= 0 ) {
		ydir = 1;
	} else {
		ydir = -1;
		ysteps = - ysteps;
	}
	if(nx1 < ix1 || nx1 > ix2 || nx2 < ix1 || nx2 > ix2 || ny1 < iy1 ||
		ny1 > iy2 || ny2 < iy1 || ny2 > iy2)
		error("Slice extends beyond edge of image (%d %d %d %d)", ix1,ix2,iy1,iy2);

/* flags for log plotting */
/* for now don't allow logarithmic x--probably no one will want it */
/*	if(*logx) 
                islog=1;
*/
        if(*logy)
                islog+=2;
/*                      	               */
/* allocate big enough chunks for plot points  */
/*                      	               */
        xpt = (float *)ChkAlloc((xsteps+ysteps+1)*sizeof(float), "Sl Xvals");
        ypt = (float *)ChkAlloc((xsteps+ysteps+1)*sizeof(float), "Sl Yvals");
        savex = &xpt[0];
        savey = &ypt[0];
        xmn = ymn = FDRAGON;
        xmx = ymx = 0.;

	if( ! *mf) {
	    slFile = FSafeOpen(slName,slName,"w");
	    if(xsteps >= ysteps) {
		ltype = 0;
		eqn = xsteps >> 1;
	    } else {
		ltype = 1;
		eqn = ysteps >> 1;
	    }
	    nx = nx1; ny = ny1;
	    lp = GetImageLine((int)*im, -1, ny, 10, ydir);
	    /* Use normal FITS offset coordinates for all offsets */
	    /* Distance will not be useful if deltax != deltay */
/* distance should be redone properly like vc does! -- MWP*/
	    for(;;) {
		if((lp[nx - ix1] != FDRAGON) && 
                   (lp[nx - ix1] >= *miny) && 
		   (lp[nx - ix1] <= *maxy) ) {
			x = (nx - nx1) * deltax;
			y = (ny - ny1) * deltay;
			*xpt = dist = sqrt( x * x + y * y);
			*ypt = lp[nx - ix1]* *fact + *yadd;
                        if(*xpt > xmx)
                             xmx = *xpt;
                        if(*xpt < xmn)
                             xmn = *xpt;
                        if(*ypt > ymx)
                             ymx = *ypt;
                        if(*ypt < ymn)
                             ymn = *ypt;
			fprintf(slFile, "%10.4g %10.4g %8.3g %10.4g\n",
				nx * deltax, ny * deltay, dist,  lp[nx - ix1]
				* *fact + *yadd);
			xpt++;
			ypt++;
			numpoints++;
		}
		if(ltype == 0) {
			if(nx == nx2) {
				FSafeClose(slFile);
				goto plot;
			}
			nx += xdir;
			if( (eqn -= ysteps) < 0) {
				eqn += xsteps;
				ny += ydir;
				lp = GetImageLine((int)*im, -1, ny, 10, ydir);
			}
		} else {
			if(ny == ny2) {
				FSafeClose(slFile);
				goto plot;
			}
			ny += ydir;
			lp = GetImageLine((int)*im, -1, ny, 10, ydir);
			if( (eqn -= xsteps) < 0) {
				eqn += ysteps;
				nx += xdir;
			}
		}
	    }
	} else {	/* merge new data with existing slice file */
	    if((*miny > -1E30)||(*maxy<1E30))
		warn("Can't reset min or max in a merged file.");
	    strcpy(tmpName, slName);
	    strcat(tmpName, ".tmp");
	    slFile = FSafeOpen(tmpName, tmpName, "w");
	    oldFile = FSafeOpen(slName, slName, "r");
	    while( fgets(oldLine,131,oldFile) ) {
		sscanf(oldLine, "%lf %lf %lf", &x, &y, &dist);
		/*fprintf(stderr,"old %f %f %f\n",x,y,dist);*/
		nx = round(x/deltax);
		ny = round(y/deltay);
		if(nx < ix1 || nx > ix2 || ny < iy1 || ny > iy2) {
printf("nx1 = %d, nx2 = %d, ny1 = %d, ny2 = %d, nx = %d, ny = %d\n",nx1,nx2,ny1,ny2,nx,ny);
			continue;
		}
		lp = GetImageLine((int)*im, -1, ny, 10, ydir);
		if( cp = strchr(oldLine, '\n') )
			*cp = '\0';
		if(lp[nx - ix1] != FDRAGON) {
			*xpt = dist;
                        *ypt=lp[nx - ix1]* *fact + *yadd;
                        if(*xpt > xmx)
                             xmx = *xpt;
                        if(*xpt < xmn)
                             xmn = *xpt;
                        if(*ypt > ymx)
                             ymx = *ypt;
                        if(*ypt < ymn)
                             ymn = *ypt;
			fprintf(slFile, "%s %10.4g\n", oldLine, lp[nx - ix1]
				* *fact + *yadd);
		}
		xpt++;
		ypt++;
		numpoints++;
	    }
	    strcpy(oldName, slName);
	    strcat(oldName, ".old");
	    rename(slName, oldName);
	    rename(tmpName, slName);
	    FSafeClose(oldFile);
	}
plot:
/*
 * rewind the pointers for plotting and later release
 */
        xpt=savex;
        ypt=savey;
	if (!*ovl ) {
          Pstart();
          Pformat("full plot");
	  if(*hff==0)
		xmn=*hf;	
	  if(*htf==0)
		xmx=*ht;
	  if(*vff==0)
		ymn=*vf;	
	  if(*vtf==0)
		ymx=*vt;	
	}
	PsetType(XY_PLOT);
        MkOpt(CLEAROPT,ENDOPT);

	if (!*ovl ) {
         MkOpt(FLAGY0,ENDOPT);
/* log X plots cannot be selected but leave the code in
 * in case we ever change.
 */
         switch(islog) {
            case 0: /* not a log plot */
                Pbounds(xmn,ymn,xmx,ymx); /*L,B,R,T*/
        	if(!*fly0)
             		MkOpt(NO FLAGY0,ENDOPT);
                break;
            case 1: /* logX-linY plot */
                Plogbounds(0.1*xmn,ymn,xmx,ymx,1,0);
        	if(!*fly0)
             		MkOpt(NO FLAGY0,ENDOPT);
                break;
            case 2: /* linX-logY plot */
                Plogbounds(xmn,0.1*ymn,xmx,ymx,0,1);
             	MkOpt(NO FLAGY0,ENDOPT);
                break;
            case 3: /* log-log plot */
                Plogbounds(0.1*xmn,0.1*ymn,xmx,ymx,1,1);
             	MkOpt(NO FLAGY0,ENDOPT);
                break;
         }
	 MkGrid(KEYY, KEYX, NO FLAGX0,ENDOPT);
	    if(*figw==0)        /* Figure width not given */
                *figw = figw_o =  .025*(xmx-xmn);
            if(*figh==0)        /* Figure height not given */
                *figh = figh_o = .025*(ymx-ymn);
	}
        else {/* it is an overlay so don't recompute figure sizes. */
            *figw=figw_o;
            *figh=figh_o;
	}
	
        if(!*modef)
           if(*mode == 1) 
                MkOpt(FIGURE(*mrkn),FHEIGHT(*figh),FWIDTH(*figw),ENDOPT);
        if(*fill)
                MkOpt(FILLED,ENDOPT);
	MkGraph(fXLIST(xpt,numpoints),fYLIST(ypt,numpoints), LTYPE(*lnt),ENDOPT);

	if(!*ovl) {
         if(!*mlbf)  /* default is no main label */
                Pplace(0,mlb);
         if(*hlbf) {
                switch(islog) {
                case 0:
                case 2:
			sprintf(hlb,"From (%g,%g) to (%g,%g)",*x1,*y1,*x2,*y2);
                        break;
                case 1:
                case 3:
			sprintf(hlb,"Log From (%g,%g) to (%g,%g)",*x1,*y1,*x2,*y2);
                        break;
                }
         }
         Pplace(1,hlb);
         if(*vlbf) {
                ihp = imageHdr[*im];
                switch(islog) {
                case 0:
                case 1:
                        sprintf(vlb,"Image %d (%s)",*im,ihp->bunit);
                        break;
                case 2:
                case 3:
                        sprintf(vlb,"Log(Image %d) (%s)",*im,ihp->bunit);
                        break;
                }
         }
         Pplace(2,vlb);
	}
        Pshow(bxCol, bxRow);
        ChkFree((char *)xpt);
        ChkFree((char *)ypt);	
}

static char *SliceName()
{
	if(*slName == '\0') {
		sprintf(slName, "%s/slice", getenv("HOME") );
	}
	return(slName);
}
