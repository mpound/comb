# include "../../stacks/C.h"
# include "../../coordsys/C.h"
# include "../../stacks/include.h"
# include "../../main/dragon.h"
# include "../../main/C.h"
# include "../../image/C.h"
# include "../../misc/C.h"
# include <stdio.h>
# include "../../error/C.h"
# include <sys/file.h>
# include <sys/types.h>
#if 0
# include <sys/dir.h>
#endif
# include <math.h>

#define bzero(s, n) memset(s, 0, n)

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
 
 /* in.c */
double integ_ P_((double *h1, double *h2, float *tmax, float *cenvel, float *vpeak, float *fwhm));
  
#undef P_

void vm()
{
	static int interpType = DUMBINTERP;

# include "vm.dc.h"
	struct of_stk tstk;		/* for accumulating the avg spectrum */
	double ymin, ymax;		/* Y extent of the area in 'Arc */
	double xmin, xmax;		/* X extent of the area */
	double xspacing, yspacing;	/* pixel spacing in rel coordinates */
	double x, y;			/* current x and y in rcsys units */
	double val;			/* temp integ value of current pix */
	double gravSum = 0, tSum = 0;	/* Sums for the harmonic mean radius */
	double hmRadius;		/* calculated harmonic mean radius */
	double t = 0, st = 0;		/* Used to calculate the moments of */

	double stv = 0, stvv = 0;	/*  the average spectrum */
	double v, w;
	double rmsVel;			/* Calculated velocity */
	double ch1, ch2;		/* chans corresponding to vel range */
	double pixArea;			/* Area of one pixel (Steradians) */
	int nbeg, nend, i;		/* integer chans */
	int numArray = 0;		/* number of actual spectra in array */
	int numEmpty = 0;		/* number of array points w/o data */
	int numDragon = 0;		/* # points w/ data, but bad integral */
	int lox, hix, loy, hiy;		/* Extent of area in pixels */
	float *array;			/* Points to array of int intensity */
	float *curLine;			/* points to current array line*/
	register float *curVal;		/* points to current array element*/
	register float *prevVal;	/* prev array element */
	float tmax, cenvel, vpeak;	/* places for integ to store things
					 * that aren't needed here */
	int arraySize;			/* # of bytes in the array */
	int xsize, ysize;		/* # of lines, pix / line in array */
	register int pline, ppix;	/* previous line and pixel */
	int line;			/* current line and pixel (pixels */
	register int pix;		/* from center) */
	double  delx, dely;		/* x and y distance cur to prev pixel */
	double distance;		/* total distance cur to prev pixel */
	int lastPix;			/* Last pixel of current line segment */
	XLST xList;			/* List of ranges in x ('Arc) which
					 * are within the area */
	float *xp;			/* pointer to x values on the xList */
	static int init;		/* init value for cmbn. */
	static int cmdsigsv;		/* Save command line number to control
					 * printing of header */
	static int chan = CHAN, vel = VEL;	/* For calling FORTRAN */
	double curChan;

	/* variable to accomodate parameter fwhm in integ_() - mh 6/25/96 */
	float fw;

# include "vm.ec.h"

	/* set directory number */
	curDir = FOREGROUND;
	CDirErr();
	OkStacks((int)*slh,(int)*sll);
	OkFreqs(*fr1,*fr2);
	if(!*itf)
		interpType = *it;
        if (interpType == GAUSSINTERP) *efd = *fwhm;
        SetStkInterp(interpType, *ir, *efd, *gs);
/*	SetStkInterp(interpType, *ir); -- mwp 5/92 */
	tstk.numpt = 0;			/* no data yet */

	if(fabs(*lv - *hv) < 0.1)
		error("Velocity range too small");

	/* If first execution for this cmd line, set up init value for cmbn.
	 * Ignore posn diff, simple add, don't cmbn scan number lists.
	 * Start by resetting. */
	if(cmnd_.cmdsig != cmdsigsv)
		init = 29;
	
	/* Allocate and clear an array to hold the integrated temp of each
	 * spectrum so that we can accumulate the gravatational potential
	 * energy within the area */

/* changed from & 3 to & 0xf because I added arcsecs as 4th unit - MWP 1/11/95 */
	if((csys.xun & 0xf) != csys.yun)
		warn("X and Y units differ");
	xspacing = *sp * cunits[ARCMINUTES] / cunits[csys.xun & 0xf];
	yspacing = *sp * cunits[ARCMINUTES] / cunits[csys.yun];

	ChkArea((int)*an);
	AreaLimits((int)*an, &xmin, &xmax, &ymin, &ymax);
	/* For the area include equality at the low end, but not at the top
	 * so that adjacent areas don't overlap or miss pixels */
	loy = ceil( ymin / yspacing );
	hiy = ceil( ymax / yspacing ) - 1;
	lox = ceil( xmin / xspacing );
	hix = ceil( xmax / xspacing ) - 1;
	xsize = hix - lox + 1;
	ysize = hiy - loy + 1;
	arraySize = xsize * ysize * sizeof(float);
	array = (float *)SafeAlloc(arraySize, "Integrated Intensities");
	bzero(array, arraySize);
	SetRcXLimits(xmin, xmax);

	for(line = loy,curLine = array; line <= hiy; line++,curLine += xsize) {
	    y = line * yspacing;
	    if(*vfl)
		printf("Y = %g\n", y);
	    xList = XList((int)*an, line * xspacing);
	    for(xp = xList.x; xp < xList.x + xList.n; ) {

		/* determine the range of pixels between this x pair using the
		 * same half open interval as with y */
		pix = ceil( *xp++ / xspacing);
		curVal = curLine + pix - lox;
		lastPix = ceil( *xp++ / xspacing) - 1;
		for( ;pix <= lastPix; pix++, curVal++) {
		    x = pix * xspacing;
		    InterpAtPoint( x, y, *fwid, 0);
		    if(coreFull[0]) {
			/* This is an inefficient, but easy way to get the sum
			 * spectrum.  It should be recoded to eliminate the
			 * moves */
			if(tstk.numpt > 0) {
			    stk_[1] = tstk;
			    cmbn_(&init);
			    init = 28;
			}
			tstk = stk_[1];

			/* Integrate this position over the vel range */
			if( (val = integ_(lv, hv, &tmax, &cenvel, &vpeak, &fw))
				== DRAGON) {
			    numDragon++;
			    continue;
			} else {
			    *curVal = val;
			    numArray ++;
			    tSum += val;
			}

			/* Do the gravitational energy sum of this element with
			 * the previous ones */
			prevVal = array;
			for(pline = loy; pline <= line; pline++) {
			    dely = line - pline;
			    for(ppix = lox; ppix <= hix; ppix++) {
				delx = pix - ppix;
				distance = sqrt(delx * delx + dely * dely);
				if(distance == 0)/* Have we reached cur pix? */
					break;
				gravSum += *curVal * *prevVal++ / distance;
			    }
			}
		    } else
			numEmpty++;
		}
	    }
	}
	if(numArray <= 0)
	    error("No data found");
	else if(*vfl)
	    printf("Data at %d points were used\n",numArray);
	if(numEmpty | numDragon)
	    fprintf(stderr,
			"%d points had no data, %d points had bad integral\n",
		numEmpty, numDragon);
	for(i = 0; i < tstk.numpt; i++)
	    tstk.stak[i] /= numArray;

	hmRadius = *sp * tSum * tSum  / (gravSum * 2);

	/* the big job is done now. */
	SafeFree((char *)array);

	/* put the sum spectrum in stacks 1 and 2 */
	stk_[0] = tstk ;
	coreFull[0] = 1;
	stk_[1] = tstk ;
	coreFull[1] = 1;

	/* pltchr translates the velocity limits to channels and orders them */
	pltchr_(lv, hv, &ch1, &ch2);
	if(ch1 > ch2)
	    error("No overlap between vel limits and avg spectrum");
	if( !cifdif(ch1, .501, 1e-5) || !cifdif(ch2, (double)stk_[0].numpt +
		.499, 1e-3) )
	    fprintf(stderr, "Vel limits extend beyond range of avg spectrum");
	nbeg = round(ch1);
	nend = round(ch2);

	/* Now compute the moments of the average spectrum.  Channels nbeg
	 * and nend are probably fractional chans, w is the fractional weight
	 * of each chan */
	w = 0.5 + nbeg - ch1;
	for(i = nbeg; i <= nend;) {
	    curChan = i;
	    v = tran_(&chan, &vel, &curChan);
	    st += t = w * stk_[0].stak[i - 1];
	    stv += t * v;
	    stvv += t * v * v;
	    if(++i == nend)
		w = 0.5 + ch2 - nend;
	    else
		w = 1;
	}
	rmsVel = sqrt((stvv - stv * stv / st) / st);
/*	printf("RmsVel = %g, Harmonic mean radius ('Arc) = %g\n", rmsVel,
		hmRadius);
	printf("Mass estimate is %g *R100 / F\n",63.76 * hmRadius * rmsVel *
		rmsVel); */
	if(cmnd_.cmdsig != cmdsigsv) {
		printf("Vir Mass     T*DV*DA      Avg Line     Area         Harm mean    Vel Disp\n");
		printf(
		    "%-13s%-13sK-km/s       Steradians   radius 'Arc  Km/s\n",
		    (cifdif(*dist, 100., .1))? "Scaled": "*R100 / F",
		    (cifdif(*scale, 1.0, 1e-4))? "Scaled": "K-km/s-Sr");
		cmdsigsv = cmnd_.cmdsig;
	}
	pixArea = *sp * *sp * cunits[ARCMINUTES] * cunits[ARCMINUTES];
	printf("%-13g%-13g%-13g%-13g%-13g%-13g\n",
		0.6376 * *dist * hmRadius * rmsVel * rmsVel,
		tSum * pixArea * *scale, tSum / numArray,
		numArray * pixArea, hmRadius, rmsVel);
}
