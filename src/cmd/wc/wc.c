# include "../../main/C.h"
# include "../../main/dragon.h"
# include "../../coordsys/C.h"
# include "../../stacks/C.h"
# include "../../stacks/include.h"
# include "../../misc/C.h"
#if 0
# include "../../graphics/C.h"
# include "../../graphics/include.h"
#endif /*0*/
# include "../../image/C.h"
# include <string.h>
# include <stdio.h>
# include <math.h>

extern int Already_warned; /* warned about differing fwids? see AvgStacks()*/
void wc()
{
	static int interpType = DUMBINTERP;
	static float velCntr,delVel;
	static int zValue;
	float *array;
	double atof();
	/* The communication with the user is in projected coordinates, thus
	 * the following corners are in the projected plane.  The user uses
	 * the same units in the projected plane as for offsets in the current
	 * relative coordinate system
	 */
	XYPOINT ll, ur;			/* corners with values ordered */
	XYPOINT lp, rp;			/* left and right points for each
					 * vel-space map */

	double delta, dY;
	double loVel, hiVel;		/* velocities adjusted to even chans */
	double fw;
	int c1,c2;
	int numChan;
	int line, plane;
	char *cp;
	register struct of_imagehdr *ihp;
	char ts[128];
	int i;

	/* concession to FORTRAN */
	static int one = 1,three = 3;

	
#include "wc.dc.h"
#include "wc.ec.h"
	Already_warned=0;
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
	lp.x = ur.x;
	rp.x = ll.x;

	curDir = FOREGROUND;
	CDirErr();

	if(! *itf)
		interpType = *it;


	OkStacks((int)*slh,(int)*sll);
	OkFreqs(*fr1,*fr2);
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
	printf("ch1 = %d, loVel = %g, ch2 = %d, hiVel = %g, numChans = %d\n",
		c1, loVel, c2, hiVel, numChan);
	if(numChan < 2)
		error_("Not enough chans for a cube\n");

	/* Set up image 1 for the result */
	StartStdImage(1);
	ihp = imageHdr[1];
	ihp->bitpix = -32;	/*Write the result as floats */
	ihp->naxis = 3;
	ihp->naxis1 = *rh;
        if(csys.type == RADEC)
                ihp->Fequinox = rcsys.cepoch;
	strcpy(ihp->ctype1, FitsAxisLabel(0));
	ihp->crval1 = csys.x / cunits[DEGREES];
	/* put on labels */
	if(*mlbf) {
		if(cp = strrchr(dirName[curDir],'/'))
			cp ++;
		else
			cp = dirName[curDir];
		sprintf(mlb,"%s Data Cube",cp);
	}
	strcpy(ihp->title, mlb);

	delta = (ll.x -  ur.x)/(*rh - 1);
	ihp->cdelt1 = delta * cunits[csys.xun & 0xf] / cunits[DEGREES];
	ihp->crpix1 = - ur.x/delta + 1.0;

	ihp->naxis2 = *rv;
	strcpy(ihp->ctype2, FitsAxisLabel(1));
	ihp->crval2 = csys.y / cunits[DEGREES];
	dY = (ur.y - ll.y)/(*rv - 1);
	ihp->crpix2 = - ll.y / dY + 1.0;
	ihp->cdelt2 = dY * cunits[csys.yun] / cunits[DEGREES];

	ihp->naxis3 = numChan;
	strcpy(ihp->ctype3, "FELO-LSR");
	ihp->crval3 = loVel * 1000.;
	ihp->cdelt3 = 1000. * (hiVel - loVel)/(numChan - 1);
	ihp->crpix3 = 1.;

	strcpy(ihp->bunit,"K");
	sprintf(ihp->param,"ir:%g %s: %s:", *ir, interpNames[interpType],
		projNames[cproj.type]);
        if(*obj)
		strcpy(ihp->object, obj);
	if(*tel)
		strcpy(ihp->telescop, tel);
	ihp->Frestfreq = stk_[0].freq * 1e6;
	printf("restfreq %g\n",ihp->Frestfreq );
	OpenFitsOut(ff);
        WriteFitsHdr(1);
	fprintf(stderr, "Computing array for cube\n");

	fw = stk_[0].fwid;
	rp.y = lp.y = ll.y;
	for(line = 0; line < ihp->naxis2; line++) {
	    array = Stk1Space(lp,rp,loVel,numChan,ihp->naxis1, fw);
	    for(plane = 0; plane < numChan; plane++)
		WriteFitsDataLine(1, array + plane * ihp->naxis1, line, plane);
	    ChkFree(array);
	    rp.y = (lp.y += dY);
	}

	PadFitsFile();
	CloseFitsOut();
	sprintf(ts, "af \"%s\" im:1", ff);
	combex_(ts);
}
