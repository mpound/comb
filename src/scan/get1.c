/* get1.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include <math.h>

#include "../stacks/C.h"
#include "../main/C.h"
#include "C.h"
# ifndef M_PI
#   define M_PI 3.14159265358979323846
# endif


/* Table of constant values */

static double c_b13 = 10.;
#if 0
static int c__512 = 512;
static int c__256 = 256;
#endif
static int c__1 = 1;

int get1_()
{
    /* System generated locals */
    int i__1, i__2;
    float r__1, r__2;
    double d__1;

    /* Builtin functions */
    double cos(), pow_dd(), exp(), sqrt();

    /* Local variables */
    float tamb;
#define nchn ((short *)&scan_ + 49)
    int nflg;
    float tabs;
    int navg;
    float dtim, trec;
#define nwid ((short *)&scan_ + 43)
    int ncnt;
    float attn, tauo;
/*  extern int move_(); */
    int i;
    float attn2, tauo2, tsky2, f;
/*  extern  int clear_(); */
    float rmage;
    int jsign;
    float sinel, dtsig;
    short itemp[128];
    float oattn, trans;
    int n1, n2;
    float wattn;
    int ii;
    float tc, to, sp, oattn2;
    float tw;
    int np1;
    float to2, sp2, eta;
    int nen, nch1;



/*common curscn - last scan number typed This is linked to parse for old s
can numbers*/
/* clear stack header */
    memset(&stk_[0].numpt, 0, 512);
/*  clear_(&stk_[0].numpt, &c__512); */
/* header version 1, no baseline removed yet */
    stk_[0].nver = 1;
    stk_[0].ibsln = -1;
    stk_[0].numst = 1;
/* put file name and scan number in hdr */
    stk_[0].nlist = 1;
    stk_[0].indx = 3;
    putfn_();
    stk_[0].nstk[2] = scan_.ndat[0];
/* Clear the data plotted flag */
    main_.pltd = 0;
    stk_[0].factt = (float)1.;
    stk_[0].time = scan_.ndat[17] / (float)10.;
    if (stk_[0].time <= (float).01) {
	stk_[0].time = (float).01;
    }
    scan_.iobssec = (scan_.ndat[4] - 7305) * 86400 + scan_.ndat[15] * 
	    86400 / 16384;
/* Not needed here */
/* Set the kuse array to the current nuse array. */
/* 	call move(nuse,kuse1,64) */
/* Set the backend number. */
    if (scan_.ndat[12] >= 20) {
	scan_.nbknd = 1;
    } else if (scan_.mbknd >= 10) {
	i__1 = scan_.ndat[2];
	for (i = 1; i <= i__1; ++i) {
	    if ((scan_.mbknd == 10 && scan_.ndat[i + 42] < 25) || 
		    scan_.mbknd == scan_.ndat[i + 42]) {
		scan_.nbknd = i;
		goto L40;
	    }
	}
	/* If we fell through to here, we didn't find the req. backend */
	return(0);
    } else {
	scan_.nbknd = scan_.mbknd;
    }
L40:
    if (scan_.nbknd < 1 || scan_.nbknd > scan_.ndat[2]) {
	return(0);
    }
    nflg = 1;
    if (scan_.ndat[43] == 100 && scan_.ndat[44] == 100 && scan_.nbknd == 1)
	     {
	if (scan_.ndat[45] == 100 && scan_.ndat[2] == 3) {
	    nflg = 3;
	} else if (scan_.ndat[2] == 2) {
	    nflg = 2;
	}
    }
    scan_.ncur = scan_.nstrt;
/*  get correct backend */
    if (scan_.nbknd == 1) {
	goto L125;
    }
    i__1 = scan_.nbknd - 1;
    for (i = 1; i <= i__1; ++i) {
	scan_.ncur += nchn[i - 1];
/* L120: */
    }
    scan_.ndat[65] = scan_.ndat[scan_.nbknd + 16];
L125:
    stk_[0].fwid = nwid[scan_.nbknd - 1] / (float)100. + scan_.ndat[
	    scan_.nbknd + 4] / (float)1e6;
    nch1 = nchn[scan_.nbknd - 1];
    if (nflg > 1) {
	nch1 = 512;
    }
/*  nch1.eq.0 signifies a virtual backend */
    if (nch1 <= 0) {
	return(0);
    }
    stk_[0].numpt = MIN(nch1,2048);
    if (scan_.ndat[93] == 256) {
	stk_[0].nplc = 11;
    } else {
	stk_[0].nplc = 30;
    }
    if (scan_.ndat[12] >= 20) {
	stk_[0].nplc += 300;
    }
    nen = scan_.ncur + stk_[0].numpt;
    if (nen > scan_.lnfil || scan_.ncur < scan_.nstrt) {
	error_("bad data loc this bknd");
    }
/*  take care of bad channels */
    if (scan_.ndat[12] < 20) {
/* first if the expander, get rid of chan 1 */
	if (stk_[0].fwid < (float).25) {
	    scan_.ndat[256] = scan_.ndat[257];
	}
	i__1 = scan_.numbch;
	for (i = 1; i <= i__1; i += 2) {
	    n1 = scan_.nbadch[i - 1];
	    n2 = scan_.nbadch[i];
	    if (n1 > nen || n2 <= scan_.ncur) {
		goto L140;
	    }
	    if (n1 > scan_.ncur + 1) {
		ncnt = 1;
		navg = scan_.ndat[n1 - 2];
	    } else {
		ncnt = 0;
		navg = 0;
	    }
	    if (n2 < nen) {
		++ncnt;
		navg += scan_.ndat[n2];
	    }
	    if (ncnt > 0) {
		navg /= ncnt;
		i__2 = n2;
		for (ii = n1; ii <= i__2; ++ii) {
		    scan_.ndat[ii - 1] = navg;
/* L135: */
		}
	    }
L140:
	    ;
	}
/* L141: */
/* now also set up the process flag */
	stk_[0].nprc = scan_.kontrl;
    } else if (scan_.ndat[12] == 20) {
	stk_[0].nprc = 2000;
    } else if (scan_.ndat[12] == 21) {
	stk_[0].nprc = 1000;
    } else if (scan_.ndat[12] == 22) {
	stk_[0].nprc = 3000;
    }
    stk_[0].ra = scan_.ndat[54] + scan_.ndat[55] / (float)60. + scan_.ndat[
	    56] / (float)3600.;
/* 	if(ndat(58).lt.0)ndat(59)=-iabs(ndat(59)) */
/* 	if(ndat(59).lt.0)ndat(60)=-iabs(ndat(60)) */
    stk_[0].dec = scan_.ndat[57] + scan_.ndat[58] / (float)60. + scan_.ndat[
	    59] / (float)3600.;
    stk_[0].epoch = scan_.ndat[32] / (float)10.;
    stk_[0].vlsr = scan_.ndat[3] * (float)10. + scan_.ndat[105] / (float)1e3;

/* If data is from before 3/12/82 correct vlsr1 for old scan hdr */
    if (scan_.ndat[4] < 11757) {
	stk_[0].vlsr /= (float)100.;
    }
    stk_[0].freq = scan_.ndat[39] * 1e3 + scan_.ndat[40] + scan_.ndat[41] / 
	    1e3;
    stk_[0].dra = (scan_.ndat[60] + scan_.ndat[106] / (float)1e4) / (float)
	    9e3;
    stk_[0].ddec = (scan_.ndat[61] + scan_.ndat[107] / (float)1e4) / (float)
	    600.;
    stk_[0].oddec = stk_[0].ddec * (float)60.;
    stk_[0].odra = stk_[0].dra * (float)900. * cos((stk_[0].dec + stk_[0].ddec) * 
	    (float)M_PI / (float)180.);
    stk_[0].l = scan_.ndat[95] + scan_.ndat[96] / (float)1e3;
    stk_[0].b = (float) scan_.ndat[97] + (float) scan_.ndat[98] / (float)1e3;
    stk_[0].dl = (float) scan_.ndat[99] / (float)1e3;
    stk_[0].db = (float) scan_.ndat[100] / (float)1e3;
    scan_.azimuth = (float)-1.;

/*  calculate scaling for atmospheric attenuation,image sideband */
/*  suppression,spillover and beam efficiency */
    if (scan_.ndat[22] <= 0) {
	scan_.ndat[22] = 1;
    }
    sinel = scan_.ndat[22] / (float)16384.;
    tabs = (float) (scan_.ndat[28] - scan_.ndat[14]);
    tamb = (float) scan_.ndat[28];
    eta = scan_.ndat[26] / (float)100.;
    sp = scan_.ndat[25] / (float)100.;
    sp2 = (scan_.ndat[25] + scan_.ndat[31]) / (float)100.;
    d__1 = (double) (-scan_.ndat[29] / (float)1e3);
/*RWW    rmage = pow_dd(&c_b13, &d__1); */
    rmage = pow(c_b13, d__1);
/* Use f as a temporary variable */
    f = -scan_.ndat[66] / (float)1e3 / sinel;
/* If HI survey, set zenith tau to .0082 */
    if (stk_[0].freq - (float)1420.4 < .01) {
	f = (float)-.0082 / sinel;
    }
    if (f > (float)-20.) {
	attn = exp(f);
    } else {
	attn = (float)1e-10;
    }
    f = -(scan_.ndat[66] + scan_.ndat[30]) / (float)1e3 / sinel;
    if (f > (float)-20.) {
	attn2 = exp(f);
    } else {
	attn2 = (float)1e-10;
    }
    trans = attn * sp * eta;
    scan_.tsky = tabs * sp * ((float)1. - attn) + tamb * ((float)1. - sp);
    tsky2 = tabs * sp2 * ((float)1. - attn2) + tamb * ((float)1. - sp2);
    if ((float) (scan_.ndat[87] - 1366) < (float)1. && (float) scan_.ndat[29] 
	    == (float)0. && (float) scan_.ndat[39] == (float)115.) {
	goto L150;
    }
    goto L175;
/* Use 2-layer atm model to correct opacities of SIS DSB 12CO J=1-0 data; 
*/
/*formulas for oxygen temp and opacity from Kutner(1978) Ap Lett vol.19 p.
81*/
L150:
    tw = tamb - 10;
/* Computing 3rd power */
    r__1 = tamb, r__2 = r__1;
    tauo = (((float)1. / (r__2 * (r__1 * r__1)) - (float)3.56e-8) * (float)
	    4e7 + (float)1.) * (float).18;
/* Computing 3rd power */
    r__1 = tamb, r__2 = r__1;
    tauo2 = (((float)1. / (r__2 * (r__1 * r__1)) - (float)3.56e-8) * (float)
	    3.7e7 + (float)1.) * (float).08;
/* In general, Tamb in tau's should be f*Tamb+(1.-f)*<Tamb>, */
/* where f=.44 at sea level and <Tamb> is averaged over the day */
    to = tamb * (tauo * (float).02 + (float).9);
    to2 = tamb * (tauo2 * (float).02 + (float).9);
    oattn = exp(-(double)tauo / sinel);
    oattn2 = exp(-(double)tauo2 / sinel);
    wattn = (scan_.tsky * (float)2. - tamb * (float)2. + (tamb - tw) * (sp + 
	    sp2)) / (sp * to * ((float)1. - oattn) + sp2 * to2 * ((float)1. - 
	    oattn2) - tw * (sp + sp2));
    trans = oattn * wattn * sp * eta;
    scan_.tsky = sp * (to * ((float)1. - oattn) * wattn + tw * ((float)1. - 
	    wattn)) + ((float)1. - sp) * tamb;
    tsky2 = sp2 * (to2 * ((float)1. - oattn2) * wattn + tw * ((float)1. - 
	    wattn)) + ((float)1. - sp2) * tamb;
L175:
    trec = (rmage + (float)1.) * scan_.ndat[65];
    if (scan_.ndat[27] < 0) {
	scan_.tsys = (trec + scan_.tsky + rmage * tsky2) / trans;
    } else {
	scan_.tsys = trec / trans;
    }
    if (scan_.ndat[12] > 29) {
	error_("Bad observation type");
    } else if (scan_.ndat[12] >= 20) {
	f = -scan_.ndat[27] / scan_.ndat[64] / trans;
    } else if (scan_.ndat[12] > 10) {
	f = (float)1.;
    } else if (scan_.ndat[12] == 10) {
	f = rmage + (float)1.;
    } else if (scan_.ndat[27] < 0) {
	f = -scan_.ndat[27] / scan_.ndat[64] * (rmage + (float)1.) / trans;
    } else {
	tc = (float) scan_.ndat[27];
	dtsig = tc - scan_.tsky;
	dtim = tc - tsky2;
	f = (dtsig + rmage * dtim) / (scan_.ndat[64] / (float)10. * trans);
    }
/*  wt is the equivalent integration time assuming ssb noise */
/*  figure of 1000k, no atmosphere, no spillover, perfect beam */
/*  and sky temp=chopper temp=300k (tc=600k) */
/*  a single-switching mode is assumed -- thus if wt=60, then */
/*  30 sec was assumed spent looking at the source,30 sec off */
    stk_[0].nwts = 1;
    stk_[0].locwt[0] = 1;
/* Computing 2nd power */
    r__1 = (float)1e3 / scan_.tsys;
    stk_[0].wght[0] = stk_[0].time * (r__1 * r__1);
    if (scan_.ndat[12] < 20) {
	stk_[0].rms = (float)2. / sqrt(stk_[0].fwid * stk_[0].wght[0]);
    } else {
/*make the bad assumption that the full nbe was used in continuum obse
rvations.*/
	stk_[0].rms = (float)2. / sqrt(stk_[0].wght[0] * 64);
    }
/* fixup for b,1,b,4,b,2,3 backend */
    if (nflg == 3) {
	/* This doesn't look right to me, but I don't remember what it does
	 * exactly  - 10/7/94 RWW */
	memcpy(itemp, &scan_.ndat[256], 256);
	memcpy(&scan_.ndat[256], &scan_.ndat[384], 512);
	memcpy(&scan_.ndat[512], itemp, 256);
#if 0
	move_(&scan_.ndat[256], itemp, &c__256);
	move_(&scan_.ndat[384], &scan_.ndat[256], &c__512);
	move_(itemp, &scan_.ndat[512], &c__256);
#endif
    }
/*  now consider fudge factor and exponent of data */
    scan_.fmult = f * (scan_.ndat[24] / (float)100.);
/*RWW   i__1 = scan_.ndat[13];
    f = scan_.fmult * ((float)1. / pow_ri(&c_b20, &i__1)); */
    f = scan_.fmult * ((float)1. / pow(c_b13, (double)scan_.ndat[13]));
    if (scan_.map == 1) {
	error_("can't handle mapping data");
    }
    if (scan_.ndat[12] >= 20) {
	scan_.inver = 1;
	if (scan_.ndat[12] <= 21) {
	    stk_[0].fwid = scan_.ndat[70] / (float)3.6e4;
	}
	if (scan_.ndat[12] == 20) {
	    stk_[0].fwid *= sqrt((float)1. - sinel * sinel);
	}
	stk_[0].expch = nch1 / (float)2. + (float)1.;
	goto L300;
    } else if (scan_.kontrl == 0) {
	scan_.inver = 1;
	stk_[0].expch = (nch1 + 1) / (float)2.;
	goto L300;
/*  if using freq info must decide about inverting data rt to left */
    } else if (scan_.ndat[42] > 2) {
	scan_.inver = -scan_.invrt;
    } else {
	scan_.inver = scan_.invrt;
    }
/*  now velocity will increase with channel # */
/*  test sig and ref */
/* assume center freq corresponds to center of filter bank (numpt1/2.+0.5)
*/
/* fixup for b,1,b,4,b,2,3 and b,1,2,b,3,4 backends */
    if (nflg > 1) {
	scan_.ndat[76] = 0;
	scan_.ndat[88] = 0;
    }
    stk_[0].expch = (nch1 + (float)1.) / (float)2. - (scan_.ndat[1] + 
	    scan_.ndat[10] / (float)1e4 + scan_.inver * (scan_.ndat[
	    scan_.nbknd + 75] + scan_.ndat[scan_.nbknd + 87] / (float)1e4))
	     / (stk_[0].fwid * (float)100.);
    if ((scan_.ndat[42] & 1) == 0) {
	jsign = -1;
    } else {
	jsign = 1;
    }
L300:
    if (scan_.ndat[12] == 1 || scan_.ndat[12] == 3) {
	stk_[0].refch = stk_[0].expch + jsign * (scan_.ndat[62] + scan_.ndat[
		23] / (float)1e4 - scan_.ndat[63] - scan_.ndat[16] / (float)
		1e4) / (stk_[0].fwid * (float)10.);
    } else {
	stk_[0].refch = (float)-9999.;
    }
    np1 = stk_[0].numpt + 1;
    if (stk_[0].numpt >= 2048) {
	error_("Too many chans");
    }
    i__1 = stk_[0].numpt;
    for (i = 1; i <= i__1; ++i) {
	if (scan_.inver > 0) {
	    ii = i;
	} else {
	    ii = np1 - i;
	}
	stk_[0].stak[ii - 1] = f * (scan_.ndat[i + scan_.ncur - 1] + 
		scan_.ndat[48]);
/* L310: */
    }
    mrkful_(&c__1);
    return 1;
} /* get1_ */

#undef nwid
#undef nchn
