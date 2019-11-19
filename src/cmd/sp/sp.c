#include <stdio.h>
#include <math.h>
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../error/C.h"
#include "../../stacks/C.h"
#include "../../image/C.h"
#include "../../coordsys/C.h"
#include "../../graphics/C.h"

#define bzero(s, n) memset(s, 0, n)

char *getenv();
static char spName[64];

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* sp.c */
static char *SpName P_((void));
#undef P_


/*********************
 * sp - scatter plot *
 *********************/

void sp()
{
	int lox,hix;			/* merged limits of the images in */
	int loy,hiy;			/* pixels from the center */
	int lo,hi;			/* temporaries */
	int lox1, lox2;			/* used in indexing x & y image lines */
	double delx, dely;		/* 'Arc per pixel */
	double ymin, ymax;		/* Y extent of the area in 'Arc */
	double xmin, xmax;		/* X extent of the area - not used */
	int xunit, yunit;		/* units of delx and dely */

	/* The following are active while producing the scatter plot values */
	register float *xval, *yval;	/* Pointers to pixel values in the
					 * current line of the x & y image */
	register int line, pix;		/* current line and pixel (pixels
					 * from center */
	int lastPix;			/* Last pixel of current line */
	XLST xList;			/* List of ranges in x ('Arc) which
					 * are within the area */
	float *xp;			/* pointer to x values on the xList */

	/* the following keep statistics as we go through the area */
	double xsum = 0, ysum = 0;	/* The integral of the two images */
	int numpoints = 0;
	double areaPerPixel;

	FILE *spFile;
	/* format for printing the integrals of the two images */
	static char areaFmt[]="Integral%sin %c image is %g [%s]*[%s] over %g SR\n";
	static char xscaled[16];
	static char yscaled[16];

/*
 * Rest is stuff for plotting -- MWP 5/93 			
 */ 
	static float *xpt, *ypt;	/* pointers to values to be plotted */
	struct of_imagehdr *ihp; 	/* pointer to image header          */
	double xmn,ymn,xmx,ymx;		/* min &  max values of x and y     */
	int islog=0;			/* a log plot?  		    */
	float *savex, *savey; 		/* addresses of beginning of xpt    */
					/* and ypt memChunks.    	    */
# include "sp.dc.h"
# include "sp.ec.h"

	strcpy(spName, fn);
	if(*imy >= 0)
		ImageSimilar((int)*imx, (int)*imy);
	else
		ChkImage((int)*imx);
	ChkArea((int)*an);
	spFile = FSafeOpen(spName,spName,"w");

	/* put some (possibly) useful info in the file */
	if(*imy < 0) {
		fprintf(spFile,"# Image data for %s\n",imageHdr[*imx]->fname);
		fprintf(spFile,"# xfact:%g xmin:%g xmax:%g\n",*xfact,*minx,*maxx);
	}
	else {
		fprintf(spFile,"# Scatter plot of %s vs. %s\n",
			imageHdr[*imx]->fname, imageHdr[*imy]->fname);
		fprintf(spFile,
			"# xfact:%g yfact:%g xmin:%g ymin:%g xmax:%g ymax:%g\n", 
			 *xfact,*yfact,*minx,*miny,*maxx,*maxy);
	}
	AreaLimits((int)*an, &xmin, &xmax, &ymin, &ymax);

	/* find the max x range (pixels) which is within both images */
	ImageXGrid((int)*imx,&lox1,&hix,&delx,&xunit);
	if(*imy >=0) {
		ImageXGrid((int)*imy,&lox2,&hi,&delx,&xunit);
		if(lox2 > lox1)
			lox = lox2;
		else
			lox = lox1;
		if(hi < hix)
			hix = hi;
	} else {
		lox = lox1;
	}
	if(*logx) /* flags for log plotting  MWP */
		islog=1; 
	if(*logy)
		islog+=2;

	/* Find the maximum y range (pixels) which is within both images and
	 * the area */
	ImageYGrid((int)*imx,&lo,&hi,&dely,&yunit);
	/* For the area include equality at the low end, but not at the top
	 * so that adjacent areas don't overlap or miss pixels */
	loy = ceil( ymin / dely );
	hiy = ceil( ymax / dely ) - 1;
	if(lo > loy)
		loy = lo;
	if(hi < hiy)
		hiy = hi;
	if(*imy >= 0) {
		ImageYGrid((int)*imy,&lo,&hi,&dely,&yunit);
		if(lo > loy)
			loy = lo;
		if(hi < hiy)
			hiy = hi;
	}
	ihp = imageHdr[*imx];
	xpt = (float *)ChkAlloc((ihp->naxis1*ihp->naxis2+1)*sizeof(float),
		"Sp Xvals");
	ypt = (float *)ChkAlloc((ihp->naxis1*ihp->naxis2+1)*sizeof(float),
		"Sp Yvals");
	savex = &xpt[0];
	savey = &ypt[0];
	xmn = ymn = FDRAGON;
	xmx = ymx = 0.;
	/* now go through the images and do the work */
	for(line = loy; line <= hiy; line++) {
	    xList = XList((int)*an, line * dely);
	    for(xp = xList.x; xp < xList.x + xList.n; ) {

		/* determine the range of pixels between this x pair using the
		 * same half open interval as with y */
		pix = ceil( *xp++ / delx );
		if(pix < lox)
			pix = lox;
		lastPix = ceil( *xp++ / delx ) - 1;
		if(lastPix > hix)
			lastPix = hix;
		if(lastPix < pix)
			continue;
		/* Get the image values and go through this range */
		xval = GetImageLine((int)*imx, -1, line, 10,1) + pix - lox1;
		if(*imy >= 0)
			yval = GetImageLine((int)*imy, -1, line, 10,1) + pix - lox2;
		for( ;pix <= lastPix; pix++) {
		    if(*xval == FDRAGON || *xval < *minx || *xval > *maxx|| (*imy >= 0 &&
			(*yval == FDRAGON || *yval < *miny || *yval > *maxy))) {
			xval++; yval++;
		    } else {
			if(*imy >= 0) {
			    fprintf(spFile, "%8.4g %8.4g %10.4g %10.4g\n",
				pix*delx, line*dely, *xval * *xfact,
				*yval * *yfact);
			    *ypt=*yval * *yfact;
			    *xpt=*xval * *xfact;
			    ysum += *yval++;
			    if(*xpt > xmx)
				xmx = *xpt;
			    /*else if(*xpt < xmn) FAILs is 1st point is min!*/
			    if(*xpt < xmn)
				xmn = *xpt;
			    if(*ypt > ymx)
				ymx = *ypt;
			    if(*ypt < ymn)
				ymn = *ypt;
			} else {
			    fprintf(spFile, "%.4g %.4g %.4g\n",
				pix * delx, line * dely, *xval * *xfact);
			}
			xsum += *xval++;
			numpoints++;
			xpt++;
			ypt++;
		    }
		}
	    }
	}
/*
 * rewind the pointers for plotting and later release
 */
	xpt=savex; 
	ypt=savey;
	FSafeClose(spFile);
	areaPerPixel = delx * cunits[xunit] * dely *cunits[yunit];
	printf("Area/pixel: %g [SR]\n",areaPerPixel); 
	bzero(xscaled,16);
	bzero(yscaled,16);
	if(*xfact ==1.) 
		strcpy(xscaled," ");
	else
		strcpy(xscaled," (scaled) ");
	printf(areaFmt, xscaled, 'x', *xfact*xsum * areaPerPixel,
		imageHdr[*imx]->bunit, "SR", numpoints * areaPerPixel);
	printf("Mean value%sin x image is %g %s (%d pts).\n",xscaled,
		*xfact*xsum/(float)numpoints,imageHdr[*imx]->bunit,numpoints);

	if(*imy >= 0) {
	    if(*yfact ==1.) 
		strcpy(yscaled," ");
	    else
		strcpy(yscaled," (scaled) ");
	    printf(areaFmt, yscaled, 'y',*yfact*ysum * areaPerPixel,
	    	imageHdr[*imy]->bunit, "SR", numpoints * areaPerPixel);
	    printf("Mean value%sin y image is %g %s (%d pts).\n",yscaled,
		*yfact*ysum/(float)numpoints,imageHdr[*imy]->bunit,numpoints);
	
/*
/* now plot it MWP Tue Mar  2 17:00:46 EST 1993 
 */
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
/*
 * Options KEYX and KEYY will make MkGrid() call Rebound() to scale
 * the grid according to the points 
 */
	  if (!*ovl ) { /* not an overlay */

            switch(islog) {
            case 0: /* not a log plot */
                Pbounds(xmn,ymn,xmx,ymx); /*L,B,R,T*/
                break;
            case 1: /* logX-linY plot */
                Plogbounds(xmn,ymn,xmx,ymx,1,0);
	/*	MkOpt(KEYY, NO KEYX,ENDOPT); */
                break;
            case 2: /* linX-logY plot */
                Plogbounds(xmn,ymn,xmx,ymx,0,1);
		/*MkOpt(NO KEYY,KEYX,ENDOPT); */
                break;
            case 3: /* log-log plot */
                Plogbounds(xmn,ymn,xmx,ymx,1,1);
		/*MkOpt(KEYY,KEYX,ENDOPT); */
                break;
	    }	
	    MkGrid(KEYY, KEYX, NO FLAGX0,NO FLAGY0,ENDOPT);
	
	    if(*figw==0) 	/* Figure width not given */
	    	*figw = figw_o =  .025*(xmx-xmn);
	    if(*figh==0) 	/* Figure height not given */
	    	*figh = figh_o = .025*(ymx-ymn);
	  } /* if not overlay */
	else {/* it is an overlay so don't recompute figure sizes. */
	    *figw=figw_o;
	    *figh=figh_o;
	}
	    if(*fill)		/* figure should be filled */
		MkOpt(FILLED,ENDOPT);
	    MkGraph(fXLIST(xpt,numpoints),fYLIST(ypt,numpoints),FIGURE(*mrkn),
		FHEIGHT(*figh),FWIDTH(*figw),ENDOPT); 

	  if (!*ovl ) { /* not an overlay */
            if(!*mlbf)  /* default is no main label */
              	Pplace(0,mlb);
            if(*hlbf) {
                switch(islog) {
                case 0:
                case 2: 
              		sprintf(hlb,"Image %d (%s)",*imx,ihp->bunit);
                        break;
                case 1:
                case 3:
              		sprintf(hlb,"Log(Image %d) (%s)",*imx,ihp->bunit);
                        break;
                } 
	    }
	    Pplace(1,hlb);
            if(*vlbf) {
	    	ihp = imageHdr[*imy];
                switch(islog) {
                case 0:
                case 2: 
              		sprintf(vlb,"Image %d (%s)",*imy,ihp->bunit);
                        break;
                case 1:
                case 3:
              		sprintf(vlb,"Log(Image %d) (%s)",*imy,ihp->bunit);
                        break;
	    	}
	    }
            Pplace(2,vlb);
	  } /* if not overlay */

	    Pshow(bxCol, bxRow);

	} /* if imy > 0 */
	ChkFree((char *)xpt);
	ChkFree((char *)ypt);
# if 0
	if(! *yfl) {
		xList = XList((int)*an, *y);
		for(pix = 0; pix < xList.n; pix++) {
			printf("%g  ", xList.x[pix]);
			if( (pix % 10) == 9 )
				printf("\n");
		}
		printf("\n");
	}
#endif /*0*/
}

static char *SpName()
{
	if(*spName == '\0') {
		sprintf(spName, "%s/scatter", getenv("HOME") );
	}
	return(spName);
}
