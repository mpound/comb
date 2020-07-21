# include <string.h>
# include <math.h>
# include <stdio.h>
# include "../../main/C.h"
# include "../../main/dragon.h"
# include "../../coordsys/C.h"
# include "../../stacks/C.h"
# include "../../stacks/include.h"
# include "../../graphics/C.h"
# include "../../graphics/include.h"
# include "../../image/C.h"
# include "../../error/C.h"
# include "../../misc/C.h"

# define AUTOMATIC 0
# define STEPSIZE 1
# define NUMLEVELS 2
# define CONTOURS 3
# define STEPSIZEC 4
# define XLABEL 0
# define YLABEL 1
# define DLABEL 2

extern int Already_warned; /* Warned about differing filterwidths? */
void vc()
{
	static int contourChoice = AUTOMATIC;
	static double cLevels[MAXCLEVELS];
	static int cLTypes[MAXCLEVELS];
	static int numLevels = 10;
	static double stepSize = 0.;
	static XYPOINT p1 = {0,0},p2 = {0,0};
	static int interpType = DUMBINTERP;
	static char stszN[] = "vcStepSize";
	static char nlvlN[] = "vcNumLevels";
	static char nlvlF[] = "%d";
	static char lvlN[] = "vcCntrLevels";
	static char lvlF[] = "%s";
	static char ltpN[] = "vcLineTypes";
	float *array,*Stk1Space();
	double atof();
	double *levels;
	int *types;
	XYPOINT rp1,rp2;		/* end point coords in radians */
	double delta;		/* for the space axis in ft */
	int revAxis;			/* tell ft if space axis reversed */
	int numChan;
	int numPoints;			/* number of spacial points */
	int c1,c2;
	double dist,delx,dely,h1,h2;
	double spacing, spacing3;	/* Axis 1 & 3 point spacing (radians) */
	double loVel, hiVel;		/* velocities adjusted to even chans */
	int horizLabel;
	register int i;
	register char *cp;
	register struct of_imagehdr *ihp;
	char tempstr[512];
	
	/* concession to FORTRAN */
	static int one = 1,three = 3;

#include "vc.dc.h"
#include "vc.ec.h"
	Already_warned=0;
	fprintf(stderr, "p1 = (%g,%g)  p2 = (%g,%g)\n",p1.x, p1.y,p2.x,p2.y);

	/* get the number of points from the spacing in the projected plane */
	rp1.x = p1.x * cunits[csys.xun & 0xf];
	rp2.x = p2.x * cunits[csys.xun & 0xf];
	rp1.y = p1.y * cunits[csys.yun];
	rp2.y = p2.y * cunits[csys.yun];
	delx = rp2.x - rp1.x;
	dely = rp2.y - rp1.y;
	dist = sqrt(delx * delx + dely * dely);
	if(dist < M_PI/(180 * 60))	/* 1'Arc */
		error_("No spatial extent");
	if(*sp < .017)
		error("Spacing of points is bad");
	numPoints = 1.5 + dist / (*sp * cunits[ARCMINUTES]);
	if(numPoints < 2)
		error_("Not enough Spatial points");
	if(numPoints > 1024)
		warn("The number of spatial points is %d", numPoints);
	/* Get the final spacing since numPoints is an integer */
	spacing = dist / (numPoints - 1);
	fprintf(stderr,"dist = %f, spacing = %f, numPoints = %d\n", dist /
		cunits[ARCMINUTES], spacing / cunits[ARCMINUTES], numPoints);

	/* Is the path along a coordinate axis? */
	if( !cifdif( rp1.y, rp2.y, spacing / 2.0)) {
		/* y1 ~ y2, so label plot with x values */
		h1 = p1.x;
		h2 = p2.x;
		horizLabel = XLABEL;
	} else if( !cifdif(rp1.x, rp2.x, spacing / 2.0)) {
		/* x1 ~ x2, so label plot with y values */
		h1 = p1.y;
		h2 = p2.y;
		horizLabel = YLABEL;
	} else {
		h1 = 0;
		h2 = dist / cunits[csys.yun];
		horizLabel = DLABEL;
	}
	/* SetRcXLimits doesn't need ordered values */
	SetRcXLimits(p1.x, p2.x);

	curDir = FOREGROUND;
	CDirErr();

	/* set up contour levels and linetypes */
	if(!*sclf) {
	    switch(*scl) {
	    case 0:
		contourChoice = AUTOMATIC;
		break;
	    case 1:
		WrStkDflt(stszN,"%lg",stepSize);
		contourChoice = STEPSIZE;
		break;
	    case 2:
		WrStkDflt(stszN,"%lg",stepSize);
		contourChoice = STEPSIZEC;
		break;
	    case 3:
		if( !GtStkDflt(stszN,"%lf",(char *)&stepSize,0) &&
				stepSize <= 0)
			error_("No old stepsize");
		contourChoice = STEPSIZE;
		fprintf(stderr, "old stepsize = %lf\n",stepSize);
		break;
	    case 4:
		numLevels = *nlv;
		contourChoice = NUMLEVELS;
		break;
	    case 5:
		if(GtStkDflt(nlvlN,nlvlF,(char *)&numLevels,0)) {
			if(numLevels <= 0)
				error_("Less than 1 contour level");
			if(numLevels > MAXCLEVELS)
				error_("Too many contour levels");
			GtStkDflt(lvlN,lvlF,tempstr,2);
			cp = tempstr;
			for(i = 0;i < numLevels && cp >= tempstr;i++) {
				cLevels[i] = atof(cp);
				cp = strchr(cp,',') + 1;
			}
			GtStkDflt(ltpN,lvlF,tempstr,2);
			cp = tempstr;
			for(i = 0;i < numLevels && cp >= tempstr;i++) {
				cLTypes[i] = atoi(cp);
				cp = strchr(cp,',') + 1;
			}
		} else {
			error_("No old contours");
		}
		contourChoice = CONTOURS;
		break;
	    case 6:
		if(*clve != *clte)
			error_("Number of levels and types don't match");
		numLevels = *clve;
		WrStkDflt(nlvlN,nlvlF,numLevels);
		cp = tempstr;
		for(i = 0;i < numLevels;i++) {
			sprintf(cp,"%lg,",cLevels[i] = clv[i]);
			cp = strchr(cp,',') + 1;
		}
		*(cp - 1) = 0;
		WrStkDflt(lvlN,lvlF,tempstr);
		cp = tempstr;
		for(i = 0;i < numLevels;i++) {
			sprintf(cp,"%d,",cLTypes[i] = clt[i]);
			cp = strchr(cp,',') + 1;
		}
		*(cp - 1) = 0;
		WrStkDflt(ltpN,lvlF,tempstr);
			
		contourChoice = CONTOURS;
		break;
	    }
	}
	if((!*lcf) ^ (!*hcf))
		error("Can't specify only one of lc and hc");

	fprintf(stderr, "Computing array for map\n");
	if(! *itf)
		interpType = *it;
	OkStacks((int)*slh,(int)*sll);
	OkFreqs(*fr1,*fr2);
	/*SetStkInterp(interpType, *ir); -- mwp 5/92 */
        if (interpType == GAUSSINTERP) *efd = *fwhm;
        SetStkInterp(interpType, *ir, *efd, *gs);
/* need to get a stack */
	if(!coreFull[0])
		error_("Need an example stack in stack 1");
	c1 = round(tran_(&three,&one,lv));
	delta = c1;
	loVel = tran_(&one,&three,&delta);
	c2 = round(tran_(&three,&one,hv));
	delta = c2;
	hiVel = tran_(&one, &three, &delta);
	numChan = c2 - c1 + 1;
	if(numChan < 0)
		numChan = -numChan;
	if(numChan < 2)
		error_("Not enough chans for a vel-space map\n");
	if( (horizLabel == 0 && p1.x > p2.x) ||
	    (horizLabel == 1 && p1.y > p2.y) ) {
		array = Stk1Space(p2,p1,loVel,numChan,numPoints,
			(double)stk_[0].fwid);
		revAxis = 1;
	} else {
		array = Stk1Space(p1,p2,loVel,numChan,numPoints,
			(double)stk_[0].fwid);
		revAxis = 0;
	}
	if(*dbg)
	{
		register float *fp = array;     /* pointer to data in array */

		printf("p1 = (%g,%g)  p2 = (%g,%g)  %d channels\n",p1,p2,numChan);
		printf("\nActual data:\n");
		for(c1 = 0; c1 < numChan; c1++)
		{
			for(c2 = 0; c2 < numPoints; ) {
				if(*fp == FDRAGON)
					printf("       .DR ");
				else
					printf("%10.4g ", *fp);
				if( (++c2 % 5) == 0)
					putchar('\n');
				fp++;
			}
			if(c2 % 5)
				putchar('\n');
			putchar('\n');        /* double space between points */
		}
	}

	/* Put the result in image 1 */
	StartStdImage(1);
	ihp = imageHdr[1];
	ihp->naxis = 3;
	ihp->naxis1 = numPoints;
	ihp->naxis3 = 1;
	switch(horizLabel) {
	case XLABEL:	/* path alligned along longitude type axis */
		strcpy(ihp->ctype1, FitsAxisLabel(0));
		strcpy(ihp->ctype3, FitsAxisLabel(1));
		ihp->crval1 = csys.x / cunits[DEGREES];
		ihp->crval3 = csys.y / cunits[DEGREES];
		spacing = delx / (numPoints - 1);
		spacing3 = fabs(spacing);
		ihp->crpix1 = - rp1.x / spacing + 1.0;
		ihp->crpix3 = 1 - (rp1.y + rp2.y) / (2 * spacing3);
		break;
	case YLABEL:	/* path alligned along lattitude type axis */
		strcpy(ihp->ctype1, FitsAxisLabel(1));
		strcpy(ihp->ctype3, FitsAxisLabel(0));
		ihp->crval1 = csys.y / cunits[DEGREES];
		ihp->crval3 = csys.x / cunits[DEGREES];
		spacing = dely / (numPoints - 1);
		spacing3 = - fabs(spacing);
		ihp->crpix1 = - rp1.y / spacing + 1.0;
		ihp->crpix3 = 1 - (rp1.x + rp2.x) / (2 * spacing3);
		break;
	case DLABEL:
		/* In this case we compute the reference pixel from p1
		 * in a coordinate system at the center, but rotated
		 * so that its pos x axis is parallel to the line from
		 * p1 to p2.  Cos of the rot angle is - delx / dist,
		 * sin is dely / dist.
		 */
		strcpy(ihp->ctype1, FitsAxisLabel(0));
		strcpy(ihp->ctype3, FitsAxisLabel(1));
		ihp->crval1 = csys.x / cunits[DEGREES];
		ihp->crval3 = csys.y / cunits[DEGREES];
		spacing3 = spacing;
		spacing = - spacing;
		ihp->crpix1 = 1 + (rp1.x * delx  + rp1.y * dely) /
			(dist * spacing);
		ihp->crpix3 = 1 - (rp1.x * dely - rp1.y * delx) /
			(dist * spacing3);
		ihp->crota3 = atan2(dely, -delx) / cunits[DEGREES];
		break;
	}
	ihp->cdelt1 = spacing / cunits[DEGREES];
	ihp->cdelt3 = spacing3 / cunits[DEGREES];;

	ihp->naxis2 = numChan;
	strcpy(ihp->ctype2, "FELO-LSR");
	ihp->crval2 = loVel * 1000.;
	ihp->cdelt2 = 1000. * (hiVel - loVel)/(numChan - 1);
	ihp->crpix2 = 1.;

	if(csys.type == RADEC)
		ihp->Fequinox = rcsys.cepoch;
	strcpy(ihp->object, obj);
	ihp->buf = array;
	ihp->buflen = numChan;
	ihp->bufline = 0;
	strcpy(ihp->bunit,zun);
	sprintf(ihp->param,"ir:%g %s: %s:", *ir, interpNames[interpType],
		projNames[cproj.type]);
	SetDataScale(1);

	fprintf(stderr, "Constructing map\n");
	Pstart();
	PsetType(SP_VEL);	/* identify this picture as a space-vel plot */
	Pformat("full plot");
	Pbounds(h1,loVel,h2,hiVel);
	/* MkGrid(CLEAROPT,NO FLAGY0,NO FLAGX0,ENDOPT); */
	switch(contourChoice) {
	case AUTOMATIC:
		MkOpt(CLINSCALE,ENDOPT);
		break;
	case STEPSIZEC:
		MkOpt(CLINCOLOR,ENDOPT);
	case STEPSIZE:
		MkOpt(CLEVSTEP(stepSize),ENDOPT);
		break;
	case NUMLEVELS:
		MkOpt(CNUM(numLevels),ENDOPT);
		break;
	case CONTOURS:
		MkOpt(CNUM(numLevels), CLEVS(cLevels), CLINS(cLTypes), ENDOPT);
		break;
	}
	if(!*lcf && !*hcf)
		MkOpt(CLEVRANGE(*lc, *hc), ENDOPT);
	MkOpt(CSTORE(&levels,&types,&numLevels), ENDOPT);
	MkMap(ARRAY(array,numChan,numPoints), ENDOPT);
	MkGrid(CLEAROPT,NO FLAGY0,NO FLAGX0,ENDOPT);

	if( (xBoxes | yBoxes) == 1) {
		/* display first, last contour, step, ir, and interp type
		in place 3 */
               if(*blbf) {
                   sprintf(tempstr,"%g, %g, ... %g  ir:%g %s: %s:",levels[0],
                        levels[1], levels[numLevels - 1],*ir,
                        interpNames[interpType],projNames[cproj.type]);
                   Pplace(3,tempstr);
                }
                else
                   Pplace(3,blb);
	} else {
		/* print the contour levels that were used */
		for(i = 0;i < numLevels;) {
			fprintf(stderr, "%g,%d  ",levels[i],types[i]);
			if((++i % 10) == 0)
				putc('\n',stderr);
		}
		if(i %10)
			putc('\n',stderr);
	}
	SafeFree((char *)levels);
	SafeFree((char *)types);

	/* make labels */
	if(*mlbf) {
		if(cp = strrchr(dirName[curDir],'/'))
			cp++;
		else
			cp = dirName[curDir];
		sprintf(mlb,"%s",cp);
	}
	strcpy(ihp->title, mlb);
	Pplace(0,mlb);
	/* Differential or absolute coords? */
	tempstr[0] = (rcsys.ctp == rcsys.otp && rcsys.x == 0 && rcsys.y == 0 &&
		rcsys.otp != DXDY)? 0 : 'D';
	tempstr[1] = '\0';
	if(*hlbf) {
		switch(horizLabel) {
		case XLABEL:		/* horiz axis has x change only */
			sprintf(hlb,"%s%s(%s) at %s%s(%s) = %g",
				tempstr,horizCoordNames[csys.type],
				unitNames[7 & csys.xun],
				tempstr,vertCoordNames[csys.type],
				unitNames[csys.yun],
				p1.y);
			break;
		case YLABEL:		/* horiz axis has y change only */
			sprintf(hlb,"%s%s(%s) at %s%s(%s) = %g",
				tempstr,vertCoordNames[csys.type],
				unitNames[csys.yun],
				tempstr,horizCoordNames[csys.type],
				unitNames[7 & csys.xun],
				p1.x);
			break;
		case DLABEL:		/* horiz axis has both x and y change */
			sprintf(hlb,"From (%g,%g) to (%g,%g) in %s%s-%s%s",p1.x,
				p1.y, p2.x, p2.y, tempstr,
				horizCoordNames[csys.type],
				tempstr,vertCoordNames[csys.type]);
			break;
		}
	}
	Pplace(1,hlb);
	if(*vlbf)
		Pplace(2,"VELOCITY(KM/S)");
	else
		Pplace(2,vlb);
	fprintf(stderr, "Displaying map\n");
	Pshow(bxCol, bxRow);
}
