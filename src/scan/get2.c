/* Thu Jul 15 12:16:26 EDT 1999 - this version reconciles the
 * different dTabs for south pole & bell labs - mwp 
 */
#include <math.h>
#include <ctype.h>
#include "../main/C.h"
#include "C.h"
#include "../stacks/C.h"
#include "../coordsys/C.h"
#include "../scan2/scan2.h"

#define bzero(s, n) memset(s, 0, n)

static char coordType[][4] =	{"AZEL", "EQU", "GAL", "EQU"};
static char nplcType[] =	{  99,     30,    11,    30 };

int get2()
{
    register struct of_stk *stp = &stk_[0];
    double offset, sbFactor, tAmbient, tAbsorber;
    double hotSpill, trans, f, skEl, el, dTabs;
    double sig, fbCntr;
    register int arrayNum;
    int i, mmsb, nm1;
    char *tsp;				/* temp string pointer */
    int nDragon=0;
    int ii, i__2, n1, n2;
    float ncnt;
    double navg;


    bzero((char *)stp, 512);
    stp->nver = 1;
    stp->ibsln = -1;
    stp->numst = 1;
    stp->nlist = 1;
    stp->indx = 3;
    putfn_();
    stp->nstk[2] = GI0(ScanNumN);
    scan_.iobssec = GI0(J2SecondN) + (10957 * 86400 + 43200);
    stp->j2second=GI0(J2SecondN);
    main_.pltd = 0;
    stp->factt = 1;
    if((tsp = GS(ObjectN))) {
	memcpy(ObjectName, tsp, i = sizeof(ObjectName));
	while(i-- > 0 && isspace(ObjectName[i]))
		ObjectName[i] = 0;
    } else
	ObjectName[0] = 0;

    /* Get the requested backend.  If mbknd < 10, it is the backend number,
     * otherwise it is 100 for 1Mhz, 25 for .25MHz, and 10 for the expander.
     * Mbknd and nbknd are 1 based backend numbers */
    if(scan_.iobstp > 20)
	arrayNum = 0;
    else if(scan_.mbknd >= 10)
	for(arrayNum = 0; arrayNum < sci.cNumArrays; arrayNum++) {
	    SetScan2Array(arrayNum);
	    stp->fwid = GDI(CDelt1N, arrayNum);
	    if((scan_.mbknd == 10 && stp->fwid < .25) || (int)(stp->fwid *
		100 + 0.5) == scan_.mbknd) {
		goto gotbackend;
	    }
	}
    else
	arrayNum = scan_.mbknd - 1;
    if(arrayNum < 0 || arrayNum >= sci.cNumArrays)
	return(0);
    SetScan2Array(arrayNum);
    stp->fwid = GDI(CDelt1N, arrayNum);
gotbackend:

    scan_.nbknd = arrayNum + 1;
    if((stp->numpt = GII(NAxis1N, arrayNum)) > MAXCHN)
	stp->numpt = MAXCHN;
    stp->freq = GDI(LineFreqN, arrayNum);
    stp->vlsr = GDI(VSourceN, arrayNum);
    scan_.VAnt = GDI(VAntN, arrayNum);

    stp->time = GDI(IntTimeN, arrayNum);
    if(stp->time <= .01)
	stp->time = .01;
    stp->ra = GDI(CentRaN, arrayNum);
    stp->dec = GDI(CentDecN, arrayNum);
    stp->epoch = GD0(EpochN);
    stp->l = GDI(CentLN, arrayNum);
    stp->b = GDI(CentBN, arrayNum);
    stp->odra = GDI(RaMapN, arrayNum);
    stp->oddec = GDI(DecMapN, arrayNum);
    stp->ddec = stp->oddec / 60.;
    stp->dra = stp->odra * (24. / (360 * 60)) / cos((stp->dec + stp->ddec) *
	cunits[DEGREES]);
    stp->dl = GDI(LMapN, arrayNum);
    stp->db = GDI(BMapN, arrayNum);
    scan_.azimuth = GD0(AzimuthN);
    scan_.elevation = GD0(ElevatioN);
    tsp = GS(PosTypeN);
    for(i = 0; i < sizeof(coordType); i++) {
	if(strcmp(tsp, coordType[i]) == 0) {
	   stp->nplc = nplcType[i];
	   break;
	}
    }

    if(scan_.iobstp < 20)
	stp->nprc = 1;
    else  {
	stp->nplc += 300;
	switch(scan_.iobstp) {
	case 20:
	    stp->nprc = 2000;
	    break;
	case 21:
	    stp->nprc = 1000;
	    break;
	case 22:
	    stp->nprc = 3000;
	    break;
	case 30:
	    stp->nprc = 100;
	    break;
	}
    }
    if(scan_.iobstp < 30) {
	/* Convert from Ta with the receiver's sideband ratio to SSB Ta* */
	if(HEXISTS(SbRatioN))
	    sbFactor= 1 + 1 / GDI(SbRatioN, arrayNum);
	else
	    sbFactor= 1;
	if(HEXISTS(TAmbientN))
	    tAmbient = GD0(TAmbientN);
	else
	    tAmbient = 273;
	scan_.TAmbient=tAmbient;
	/* hotSpill is  (1 - eta_l) for eta_l defined as in 
	 * Ulich & Haas 1976, ApJS, 30, 247.  eta_l accounts for
	 * rearward spillover, aperture blockage, and ohmic losses. 
         * In the case of the 7-meter, blockage is zero, and spillover is
    	 * minimal.
         */
	if(HEXISTS(HotSpillN))
	    hotSpill = GDI(HotSpillN, arrayNum);
	else
	    hotSpill = 0;

        /* check for DTAbs, the difference between
         * the absorbing temperature of the atmosphere
         * and ambient temperature.  For the south pole, default 
         * dTabs is -24K, otherwise set it to 30 K
         */
        if(HEXISTS(DTAbsN))
          dTabs = GDI(DTAbsN, arrayNum);
        else 
          dTabs = GDI(LatitudeN, arrayNum) < -80? -24 : 30;
/*printf("dTabs=%g\n",dTabs);*/

	scan_.DTAbs = dTabs;
        tAbsorber = tAmbient - dTabs;

        /* at the elevation that the sky was measured */
	trans = 1 - (GDI(TSkyN, arrayNum)- hotSpill * tAmbient) /
	     ((1 - hotSpill) * tAbsorber);
	/* Avoid ridiculous conditions causing range errors in log() */
	if(trans < .01)
	    trans = .01;
	else {
	    /* At the elevation angle of this observation */
	    skEl = GD0(TSkyElevN);
	    el = GDI(ElevatioN, arrayNum);
	    if(el > 2 && skEl > 2)
		trans = exp(log(trans) * sin(skEl * cunits[DEGREES]) /
			sin(el * cunits[DEGREES]));
	}
	scan_.tsky = (1 - trans) * tAbsorber * (1 - hotSpill) + hotSpill *
		tAmbient;
	if(HEXISTS(SysTempN))
	    scan_.tsys = GDI(SysTempN, arrayNum);
	else if(HEXISTS(RecTempN))
	    scan_.tsys = (GDI(RecTempN, arrayNum) + scan_.tsky) * sbFactor /
		trans;
	else
	    scan_.tsys = 1000;
	if(scan_.iobstp >= 20)	/* continuum scan */
	     f = 1 / trans;
	else if(scan_.iobstp > 10)	/* sky or gains */
	     f = 1.0;
	else if(scan_.iobstp == 10)	/* cal */
	     f = sbFactor;
	else
	     f = sbFactor / ((1 - hotSpill) * trans);
    } else {
	f = 1.0;
	/* take care of a few things that we don't know */
	scan_.tsky = GDI(TSkyN, arrayNum);
	if((scan_.tsys = GDI(RecTempN, arrayNum)) <= 1)
	    scan_.tsys = 1000000;
    }
    scan_.fmult = f;
    /* Set the weight.  The weight is the equivalent integration time (sec)
     * above the atmosphere with no spillover with a 1000K receiver */
    stp->nwts = 1;
    stp->locwt[0] = 1;
    stp->wght[0] = 1e6 * GD0(IntTimeN) / (scan_.tsys * scan_.tsys);
    if(scan_.iobstp < 20)
	stp->rms = 2 / sqrt(stp->fwid * stp->wght[0]);
    else
	stp->rms = 2 / sqrt(64. * stp->wght[0]);	/* use bw of NBE */
    fbCntr = GDI(CRVal1N, arrayNum);
    scan_.fbCntr = fbCntr;
    /* Should we do frequency processing? */
    if((tsp = GS(CType1N)) && strcmp(tsp, "Fr") != 0) {
	stp->expch = (stp->numpt + 1.) / 2. + fbCntr / stp->fwid;
	stp->refch = NOTFS;
	nm1 = 0;
    } else {	/* Do Freq processing */
	mmsb = ((i = *GS(MMSBN)) == 'U' || i == 'u')? 1: -1;
	sig = GDI(SigIFN, arrayNum);
	stp->expch = (stp->numpt + 1.) / 2. + ((mmsb * fbCntr) + sig) / stp->fwid;
	if(scan_.iobstp == 1)
	    stp->refch = stp->expch + (GDI(RefIFN, arrayNum) - sig) /
		stp->fwid;
	else
	    stp->refch = NOTFS;
	/* Must the order of the data be reversed for increasing velocity? 
	 * Use nm1 as a flag for reversal (if non zero) */
	nm1 = (mmsb < 0)? 0: stp->numpt - 1;
	if (HEXISTS(IFCenterN))
	  scan_.IFCenter=GDI(IFCenterN,arrayNum);
	else
	  scan_.IFCenter=0;
	scan_.mmsb=mmsb;
    }

    f *= GDI(BScaleN, arrayNum);
    offset = (HEXISTS(BZeroN))? GD0(BZeroN):0;

    for(i = 0; i < stp->numpt; i++) {
	stp->stak[i] = offset + GDI(DataN, (nm1)? nm1 - i: i) * f;
    }
    coreFull[0] = 1;

/* here is where to eliminate bad channels. this code mostly stolen from get1.c
 * except working with floats instead of shorts (scan_.ndat)
 */
   scan_.inver = mmsb;  /* for "bc" */
   scan_.ncur = 0;      /* for "bc" */

        for (i = 1; i <= scan_.numbch; i += 2) {
            n1 = scan_.nbadch[i - 1];
            n2 = scan_.nbadch[i];
            if (!(n1 > stp->numpt || n2 <= 0)) {
             if (n1 > 1) {
                ncnt = 1.0;
                navg = stp->stak[n1 - 2];
             } else {
                ncnt = 0.0;
                navg = 0.0;
             }
             if (n2 < stp->numpt) {
                ncnt+=1.0;
                navg += stp->stak[n2];
             }
             if (ncnt > 0) {
                navg /= ncnt;
                i__2 = n2;
                for (ii = n1; ii <= i__2; ++ii)
                    stp->stak[ii - 1] = navg;
             }
           } /* if !(n1...) */
        } /* for i */


/* check for DRAGONs -- MWP 5/7/96 */
 
      if(main_.drgmes !=0 ) {
       if( (nDragon=ChkDrag(0)) > 0) {
           switch(main_.drgmes) {
           case 1:
                /* no message */
                break;
           case 2:
                printf(" Found and eliminated %d BLANK%s in scan %d\n",
                         nDragon,(nDragon==1)?"":"s",stp->nstk[2]);
                printf(" Use 'op msg:' to alter or eliminate this message.\n");
                break;
           }
       }
      }

    return(1);
}
