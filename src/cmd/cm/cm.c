# include "../../main/C.h"
# include "../../main/dragon.h"
# include "../../coordsys/C.h"
# include "../../stacks/C.h"
# include "../../stacks/include.h"
# include "../../graphics/C.h"
# include "../../graphics/include.h"
# include "../../image/C.h"
# include <string.h>
# include <stdio.h>
# include <math.h>

# define AUTOMATIC 0
# define STEPSIZE 1
# define NUMLEVELS 2
# define CONTOURS 3
# define STEPSIZEC 4

void cm()
{
	static int contourChoice = AUTOMATIC;
	static double cLevels[MAXCLEVELS];
	static int cLTypes[MAXCLEVELS];
	static int numLevels = 12;
	static double stepSize = 0.;
	static int interpType = DUMBINTERP;
	static char stszN[] = "cmStepSize";
	static char nlvlN[] = "cmNumLevels";
	static char nlvlF[] = "%d";
	static char lvlN[] = "cmCntrLevels";
	static char lvlF[] = "%s";
	static char ltpN[] = "cmLineTypes";
	static float velCntr,delVel;
	static int zValue;
	float *array,*Stk2Space();
	double atof();
	double *levels;
	int *types;
	/* The communication with the user is in projected coordinates, thus
	 * the following corners are in the projected plane.  The user uses
	 * the same units in the projected plane as for offsets in the current
	 * relative coordinate system
	 */
	XYPOINT ll, ur;			/* corners with values ordered */

	double delta;
	int c1,c2;
	register int i;
	register char *cp;
	register struct of_imagehdr *ihp;
	char tempstr[512];
	
#include "cm.dc.h"
#include "cm.ec.h"
	if(*rh < 2 || *rv < 2)
		error_("Not enough Spatial points");
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

	if(*hcl && !*hcs)
		error_("Can't space hatch marks by zero");

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
				error_("Les than 1 contour level");
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

	if(! *itf)
		interpType = *it;

	OkStacks((int)*slh,(int)*sll);
	OkFreqs(*fr1,*fr2);
	if (interpType == GAUSSINTERP) *efd = *fwhm;
	SetStkInterp(interpType, *ir, *efd, *gs);



	/* Don't allow cmbn first option yet */
	if( ! *zvfl) {
		zValue = *zv;
		if(zValue == 1) {	/* vel limits given */
			sprintf(mcr_o,"T*dV %.4f,%.4f",*lv,*hv);
			velCntr = (*lv + *hv)/2.;
			delVel = *hv - *lv;
		}
	}
	if(!strncmp(mcr_o,"DFLT",4))
		error_("You need to give me a velocity range or a macro name");
	StkValMacro(mcr_o,0);

	switch(main_.htype) {
	case CHAN:
		warn_("Horizontal plot units are CHANNELS");
		break;
	case FREQ:
		warn_("Horizontal plot units are FREQUENCY");
		break;
	}

	fprintf(stderr, "Computing array for map\n");
	array = Stk2Space(ll, ur, (int)*rh, (int)*rv, *mwp);
	if(*dbg)
	{
		register float *fp = array;	/* pointer to data in array */

		printf("\nActual data from (%g,%g) to (%g,%g):\n\n",ll.x,ll.y,
			ur.x,ur.y);
		for(c1 = 0; c1 < *rv; c1++)
		{
			for(c2 = 0; c2 < *rh;) {
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
			putchar('\n');		/* double space between lines */
		}
	}

	/* Put the result in image 1 */
	StartStdImage(1);
	ihp = imageHdr[1];
	ihp->naxis1 = *rh;
	strcpy(ihp->ctype1, FitsAxisLabel(0));
	ihp->crval1 = csys.x / cunits[DEGREES];
	delta = (ll.x -  ur.x)/(*rh - 1);
	ihp->cdelt1 = delta * cunits[csys.xun & 0xf] / cunits[DEGREES];
	ihp->crpix1 = - ur.x/delta + 1.0;

	ihp->naxis2 = *rv;
	strcpy(ihp->ctype2, FitsAxisLabel(1));
	ihp->crval2 = csys.y / cunits[DEGREES];
	delta = (ur.y - ll.y)/(*rv - 1);
	ihp->crpix2 = - ll.y / delta + 1.0;
	ihp->cdelt2 = delta * cunits[csys.yun] / cunits[DEGREES];
	if( zValue == 1) {
		ihp->naxis = 3;
		ihp->naxis3 = 1;
		strcpy(ihp->ctype3, "FELO-LSR");
		ihp->crval3 = velCntr * 1000.;
		ihp->cdelt3 = delVel * 1000.;
		ihp->crpix3 = 1.0;
	} else {
		ihp->naxis = 2;
	}
	if(csys.type == RADEC) 
		ihp->Fequinox = rcsys.cepoch;
	strcpy(ihp->object, obj);
	ihp->buf = array;
	ihp->buflen = *rv;
	ihp->bufline = 0;
	strcpy(ihp->bunit,zun);
	sprintf(ihp->param,"ir:%g %s: %s:", *ir, interpNames[interpType],
		projNames[cproj.type]);
	SetDataScale(1);

	fprintf(stderr, "Constructing map\n");
	Pstart();
	PsetType(SP_SP);	/* identify this plot as a space-space map */
	Pformat("full square plot");
	Pbounds(*llx,*lly,*urx,*ury);
	if(csys.type == RADEC && (csys.xun & SCALED) == 0) {
		Punitsratio(15.0 * cos(csys.y + cunits[csys.yun] *
			(*lly + *ury) / 2.));
		MkOpt(CLEAROPT, XLABHMS, YLABHMS, ENDOPT);
	} else
		MkOpt(CLEAROPT, ENDOPT);
	/*	MkGrid(NO FLAGY0,NO FLAGX0,LTYPE(29),ENDOPT); */
	MkOpt(HATCH(*hcl,*hcs),ENDOPT);
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
	MkMap(ARRAY(array,*rv,*rh), ENDOPT);
	MkGrid(NO FLAGY0,NO FLAGX0,LTYPE(29),ENDOPT);

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
		for(i = 0;i < numLevels;) {
			fprintf(stderr, "%g,%d  ",levels[i],types[i]);
			if((++i % 10) == 0)
				putc('\n',stderr);
		}
		if(i % 10)
			putc('\n',stderr);
	}
	SafeFree(( char *)levels);
	SafeFree((char *)types);

	/* put on labels */
	if(*mlbf) {
		if(cp = strrchr(dirName[curDir],'/'))
			cp ++;
		else
			cp = dirName[curDir];
		sprintf(mlb,"%s  %s",cp,mcr_o);
	}
	strcpy(ihp->title, mlb);
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
	fprintf(stderr, "Displaying map\n");
	Pshow(bxCol, bxRow);
}
