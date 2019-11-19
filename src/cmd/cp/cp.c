#include <math.h>
# include <string.h>
# include <stdio.h>
# include "../../main/C.h"
# include "../../coordsys/C.h"
# include "../../stacks/C.h"
# include "../../graphics/C.h"
# include "../../graphics/include.h"
# include "../../image/C.h"

#define bcopy(s, d, n) memcpy(d, s, n)

# define AUTOMATIC 0
# define STEPSIZE 1
# define NUMLEVELS 2
# define CONTOURS 3
# define STEPSIZEC 4

void cplot()
{
	static int contourChoice = AUTOMATIC;
	static double cLevels[MAXCLEVELS];
	static int cLTypes[MAXCLEVELS];
	static int numLevels = 12;
	static double stepSize = 0.;
	double *levels;
	int *types;
	/* The communication with the user is in projected coordinates, thus
	 * the following corners are in the projected plane.  The user uses
	 * the same units in the projected plane as for offsets in the current
	 * relative coordinate system
	 */
	double llx,lly,urx,ury;		/* corners with values ordered */
	double temp;
	double p1x, p1y, p2x, p2y;	/* For horiz labels of SP_VEL plots */
	struct AXISNAMES xunit, yunit, zunit;	/* Desc. of axes from FITS */
	static struct AXISNAMES
		raunit = {0.0666666666667,HOURS,SPATIAL,"RA",2,"Ra"},
		decunit =  {1,DEGREES,SPATIAL | LATTYPE,"DEC",3,"Dec"};
	char tempstr[80];
	int ptype, totalRaDec = 0;

	register int i;
	register struct of_imagehdr *ihp;
	int curplane;           /* the current plane being contoured */
	
#include "cp.dc.h"
#include "cp.ec.h"

	ChkImage((int)*imn);
	ihp = imageHdr[*imn];
#if 0
printf("plane = %d, naxis3 = %d\n", curplane, ihp->naxis3);
	if(curplane != 0 && (curplane < 0 || curplane >= ihp->naxis3))
	    error("Bad plane # %d, image has %d planes", curplane, ihp->naxis3);
#endif /*0*/
	if(ihp->naxis1 < 3 || ihp->naxis2 < 3)
		error("Not enough spatial extent to plot");
	if(ihp->cdelt1 == 0) {
		fprintf(stderr,
			"Missing horizontal spatial scale: set to 1 'Arc/pix");
		ihp->cdelt1 = .01666666;
	}
	if(ihp->cdelt2 == 0) {
		fprintf(stderr,
			"Missing vertical spatial scale: set to 1 'Arc/pix");
		ihp->cdelt2 = .01666666;
	}

	xunit = FitsAxisUnits(ihp->ctype1);
	yunit = FitsAxisUnits(ihp->ctype2);
	zunit = FitsAxisUnits(ihp->ctype3);
	if( (xunit.type & 0xf) == SPATIAL && (yunit.type & 0xf) == SPATIAL) {
		ptype = SP_SP;		/* identify plot type */
		/* Check for Ra-Dec absolute plot */
		if(ihp->crval1 == 0 && ihp->crval2 == 0 &&
				strcmp(xunit.fname, "RA") == 0 &&
				strcmp(yunit.fname, "DEC") == 0) {
			xunit = raunit;
			yunit = decunit;
			totalRaDec = 1;
		}
	} else if( (xunit.type & 0xf) == SPATIAL &&
			(yunit.type & 0xf) == VELOCITY)
		ptype = SP_VEL;
	else
		ptype = XY_PLOT;

	if(ptype == SP_VEL && ihp->crota3 != 0) {
	    double dist, spacing, spacing3, crot, srot;
	    double rotX1, rotY1;	/* rotated x and y offsets for p1 */
	    static double min = .00005;

	    spacing = ihp->cdelt1 * cunits[DEGREES] / cunits[xunit.unit] ;
	    spacing3 = ihp->cdelt3 * cunits[DEGREES] / cunits[zunit.unit] ;
	    dist = fabs(spacing) * (ihp->naxis1 - 1);
	    rotX1 = spacing * (1 - ihp->crpix1);
	    rotY1 = spacing3 * (1 - ihp->crpix3);
	    crot = cos(ihp->crota3 * cunits[DEGREES]);
	    srot = sin(ihp->crota3 * cunits[DEGREES]);
	    p1x = rotX1 * crot + rotY1 * srot;
	    if(fabs(p1x) < min) p1x = 0;
	    p1y = rotY1 * crot - rotX1 * srot;
	    if(fabs(p1y) < min) p1y = 0;
	    p2x = p1x - dist * crot;
	    if(fabs(p2x) < min) p2x = 0;
	    p2y = p1y + dist * srot;
	    if(fabs(p2y) < min) p2y = 0;
	    llx = 0;
	    urx = dist;
#if 0
printf("crpix1 = %g, cdelt1 = %g, crpix3 = %g, cdelt3 = %g\n", ihp->crpix1, ihp->cdelt1, ihp->crpix3, ihp->cdelt3);
printf("rotX1 = %g, rotY1 = %g, ctora3 = %g\n", rotX1, rotY1, ihp->crota3);
printf("P1 = (%g, %g) P2 = (%g, %g), dist = %g\n", p1x, p1y, p2x, p2y, dist);
error("Bailing out");
#endif /*0*/
	} else {
	    llx = (1 - ihp->crpix1) * ihp->cdelt1;
	    urx = llx + ihp->cdelt1 * (ihp->naxis1 - 1);
	    if( (xunit.type & OFFSETTYPE) ==0 ) {
		llx += ihp->crval1;
		urx += ihp->crval1;
	    }
	    llx *= xunit.factor;
	    urx *= xunit.factor;
	}
	if(*revx) {
		temp = llx;
		llx = urx;
		urx = temp;
	}

	lly = (1 - ihp->crpix2 ) * ihp->cdelt2;
	ury = lly + ihp->cdelt2 * (ihp->naxis2 - 1);
	if( (yunit.type & OFFSETTYPE) ==0 ) {
		lly += ihp->crval2;
		ury += ihp->crval2;
	}
	lly *= yunit.factor;
	ury *= yunit.factor;
	if(*revy) {
		temp = lly;
		lly = ury;
		ury = temp;
	}

	/* set up contour levels and linetypes */
	if(!*sclf) {
	    switch(*scl) {
	    case 0:
		contourChoice = AUTOMATIC;
		break;
	    case 1:
		contourChoice = STEPSIZE;
		break;
	    case 2:
		contourChoice = STEPSIZEC;
		break;
	    case 3:
		numLevels = *nlv;
		contourChoice = NUMLEVELS;
		break;
	    case 4:
		if(*clve != *clte)
			error_("Number of levels and types don't match");
		numLevels = *clve;
		bcopy(clv, cLevels, numLevels * sizeof(double));
		bcopy(clt, cLTypes, numLevels * sizeof(int));
		contourChoice = CONTOURS;
		break;
	    }
	}
	if((!*lcf) ^ (!*hcf))
		error("Can't specify only one of lc and hc");

	/* loop over the requested planes and plot them; waiting for
         * a carriage return if more than 1 plane requested. 
         * If I were clever, I wouldnt redraw axes, but it's pretty
         * quick anyway.   MWP Mon Jan  9 11:48:32 EST 1995
	 */
        for (curplane = *plane1;curplane<*plane2+1;curplane+=*pstep) {

	/* make sure that the image buffer has the whole array */
   	if(ihp->naxis3>1)
	printf(" Plane %d   CType3 %s  Value %.4f",
		curplane,ihp->ctype3,(curplane-ihp->crpix3+1.0)*ihp->cdelt3+ihp->crval3);
	if(ihp->buflen < ihp->naxis2)
		ReleaseImageBuff((int)*imn);
	(void)GetImageLine((int)*imn, curplane, (ihp->cdelt2 > 0)?
		1 - round(ihp->crpix2): round(ihp->crpix2) - ihp->naxis2,
		ihp->naxis2, 1);

	if(!*ovl) {
		Pstart();
		Pformat((totalRaDec || xunit.unit == yunit.unit)?
			"full square plot": "full plot");
	}

	PsetType(ptype);		/* identify plot type */
	if(*ovl)
		MkOpt(CLEAROPT,ENDOPT);
	else {
		Pbounds(llx,lly,urx,ury);
		if(totalRaDec) {
		    Punitsratio(15.0 * cos(cunits[DEGREES] * (lly + ury) / 2.));
		    MkOpt(CLEAROPT, XLABHMS, YLABHMS, ENDOPT);
		} else {
		    MkOpt(CLEAROPT, ENDOPT);
		}
		if (*circb) 
			MkOpt(CIRCBORDER, ENDOPT);
		MkGrid(NO FLAGY0,NO FLAGX0,ENDOPT);
	}
	MkOpt(CLEVRANGE(ihp->datamin, ihp->datamax), HATCH(*hcl,*hcs),ENDOPT);
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
	if(!*lcf && !*hcf) {
		/* this is to prevent a segmentation fault when hc < lc; in this
		 * case, just switch them - mwp 12/92
		 */
		if(*lc > *hc) {
			temp = *hc;
			*hc = *lc;
			*lc = temp;
		}
		MkOpt(CLEVRANGE(*lc, *hc), ENDOPT);
	}
	MkOpt(CSTORE(&levels,&types,&numLevels), ENDOPT);
	MkMap(ARRAY(ihp->buf, ihp->naxis2, ihp->naxis1), ENDOPT);

	if( (xBoxes | yBoxes) == 1) {
	/* display first, last contour, step, ir, and interp type in place 3 */
                if(*blbf) {
                sprintf(tempstr,"%g, %g, ... %g %s",levels[0],
                        levels[1], levels[numLevels - 1], ihp->param);
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
		if(ihp->title[0])
			Pplace(0, ihp->title);
	} else {
		Pplace(0,mlb);
	}
	if (!*circb) 	/* don't put x and y labels on circular plot */
	{
#if 0
	tempstr[0] = (rcsys.ctp == rcsys.otp && rcsys.x == 0 && rcsys.y == 0 &&
		rcsys.otp != DXDY)? 0 : 'D';
	tempstr[1] = '\0';
		case DLABEL:		/* horiz axis has both x and y change */
			sprintf(hlb,"From (%g,%g) to (%g,%g) in %s%s-%s%s",p1.x,
				p1.y, p2.x, p2.y, tempstr,
				horizCoordNames[csys.type],
				tempstr,vertCoordNames[csys.type]);
			break;
#endif /*0*/
		if(*hlbf) {
		    if(ptype == SP_VEL && ihp->crota3 != 0) {
			sprintf(hlb,"From (%g,%g) to (%g,%g) in D%s-D%s",
				p1x, p1y, p2x, p2y, xunit.lname, zunit.lname);
		    } else {
			sprintf(hlb, "%s%s (%s)",
				(xunit.type & OFFSETTYPE)?"D": "",
				xunit.lname,
				unitNames[xunit.unit]);
		    }
		}
		Pplace(1,hlb);
		if(*vlbf) {
			sprintf(vlb, "%s%s (%s)",
				(yunit.type & OFFSETTYPE)?"D": "",
				yunit.lname,
				unitNames[yunit.unit]);
		}
		Pplace(2,vlb);
	}
	Pshow(bxCol, bxRow);
	if(*plane1!=*plane2)  /* wait for <CR> iff more than one plane to contour */ 
		wait_();
	} /*end for(curplane...) */
	if(ihp->naxis3>1) 
		printf("\n"); 
}
