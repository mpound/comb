#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"
#include "../../main/machinedep.h"
# ifndef M_PI
#   define M_PI 3.14159265358979323846
# endif


#if SYSTEM_V
#define bcopy(s, d, n) memmove(d, s, n)
#define bzero(s, n) memset(s, 0, n)
#endif /*SYSTEM_V*/

#if __STDC__
static void catsn_(int *init);
#else
static void catsn_();
#endif

/* Table of constant values */

static int c__1 = 1;
static int c__2 = 2;
static int c__1025 = 1025;
static float c_b21 = (float).003;
static float tenth = (float).1;
static int c__3 = 3;

void cmbn_(init)
int *init;
{
    /* System generated locals */
    int i__1, i__2, i__3;
    float r__1, r__2;

    /* Local variables */
    int nend;
    int izer;
    short nwts;
    int ioff1, ioff2, i;
    extern int irond_();
    extern double shift_();
    int mmove, i1, i2;
    float s1, s2;
    float totwt;
    int ii;
    extern double wt_();
    short itmplc[NWEIGHTS];
    extern int stfull_();
    float tempwt[NWEIGHTS], wt1, wt2;


/*Do a weighted average of stack 1 and stack 2, leaving the result in stac
k*/
/* 2. Init controls tests for stack differences: */
/*  Init=1 - difference storage is reset and any differences result in a 
*/
/* 	call to warn. */
/* Init=0 - same except that there is no initial reset (continuation call)
.*/
/*  Init=2 - no attention is paid to differences. */
/*  If 4 is ored with one of the above, c don't pay attention to position 
*/
/* 	differences. */
/*  If 8 is ored with one of the above, do a simple add rather than a */
/* 	weighted agerage. */
/*  If 16 is ored in, don't combine scan number lists */
/* If 32 is ored in, align the center channels independent of freq or velo
city*/

/* If stack #1 is empty, abort */
    if (stfull_(&c__1) == 0) {
	error_("Empty stack 1 sub cmbn");
    }
/* If stack #2 is empty just move stack 1 in. */
    if (stfull_(&c__2) == 0) {
	stk_[1] = stk_[0];
/*Call shift here so that its error storage will be initialized. */
	(void)shift_(init, &i);
/* same for catsn */
	catsn_(&c__1025);
	mrkful_(&c__2);
	stk_[1].numst = 2;
	return;
    }
/*check for stack differences and get the amount to shift stack1 to the right.*/
    r__1 = shift_(init, &i);
    mmove = irond_(&r__1);
    if ((*init & 32) != 0) {
	mmove = stk_[1].expch - stk_[0].expch;
    }
    if (mmove >= 0) {
/*In this case leave stack 2 in place and shift stack 1 to the */
/* right, including as many chans as possible. */
	ioff1 = -mmove;
	ioff2 = 0;
    } else {
/* here we move stack 2 to the right as much as possible to avoid losing */
/* low chans in stack 1. */
/* Computing MAX */
	ioff2 = ((i__2 = stk_[1].numpt - MAXCHN) > mmove)? i__2: mmove;
	ioff1 = ioff2 - mmove;
    }
/* Computing MIN */
/* Computing MAX */
    i__2 = stk_[0].numpt - ioff1, i__3 = stk_[1].numpt - ioff2;
    nend = (i__2 > i__3)? i__2: i__3;
    if(nend > MAXCHN)
	nend = MAXCHN;
    if (nend + ioff1 <= 0 || ioff1 >= MAXCHN) {
	error_("No overlap of stacks");
    }
/* If stack 2 is to be shifted left, */
/* shift the data in stack 2 to avoid overwriting it. It would be */
/* awkward to run the loop backward because of pkwts. */
    if (ioff2 < 0) {
	i__1 = stk_[1].numpt;
	if(i__1 - ioff2 >= MAXCHN)	/* RWW added during translation to .c */
		i__1 = MAXCHN + ioff2;
	bcopy(stk_[1].stak, &stk_[1].stak[1 - ioff2 - 1], i__1 << 2);
/* adjust the sig and ref chans. */
	stk_[1].expch -= ioff2;
	if (stk_[1].refch != (float)-9999.) {
	    stk_[1].refch -= ioff2;
	}
    }
    nwts = 0;
    izer = 0;
    bzero(tempwt, sizeof(tempwt));
    bzero(itmplc, sizeof(itmplc));
    i__1 = nend;
    for (i = 1; i <= i__1; ++i) {
	i1 = i + ioff1;
	i2 = i + ioff2;
	if (i1 < 1 || i1 > stk_[0].numpt) {
	    wt1 = (float)0.;
	    s1 = (float)0.;
	} else {
	    s1 = stk_[0].stak[i1 - 1];
	    if (DABS(s1) > MAXVALUE) {
		s1 = MAXVALUE;
	    }
/* 			if(s1.eq.dragon) then */
/* 				wt1=0. */
/* 			else */
	    wt1 = wt_(&i1, &c__1);
/* 			endif */
	}
	if (i2 < 1 || i2 > stk_[1].numpt) {
	    wt2 = (float)0.;
	    s2 = (float)0.;
	} else {
	    s2 = stk_[1].stak[i - 1];
	    if (DABS(s2) > MAXVALUE) {
		s2 = MAXVALUE;
	    }
/* 			if(s2.eq.dragon) then */
/* 				wt2=0. */
/* 			else */
	    wt2 = wt_(&i2, &c__2);
/* 			endif */
	}
	totwt = wt1 + wt2;
	if (totwt == (float)0.) {
	    ++izer;
/* 	Don't propagate dragon in stacks for now. */
/* 			stak2(i)=dragon */
	    stk_[1].stak[i - 1] = (float)0.;
	} else {
	    if ((8 & *init) == 0) {
		stk_[1].stak[i - 1] = (wt1 * s1 + wt2 * s2) / totwt;
	    } else {
		if (wt2 == (float)0.) {
		    stk_[1].stak[i - 1] = (float)0.;
		}
		if (wt1 != (float)0.) {
		    stk_[1].stak[i - 1] += s1;
		}
	    }
	}
	pkwts_(&totwt, &i, tempwt, itmplc, &nwts);
/* L100: */
    }
    stk_[1].time += stk_[0].time;
    stk_[1].numpt = nend;
    if (izer != 0) {
	printf("%d wts zero\n", izer);
    }
    bcopy(tempwt, stk_[1].wght, sizeof(tempwt));
    bcopy(itmplc, stk_[1].locwt, sizeof(itmplc));
    stk_[1].nwts = nwts;
/*  set up use array */
    if (use_.kusfl != 0) {
	for (ii = 1; ii <= 32; ++ii) {
	    stk_[1].kuse[ii - 1] = stk_[0].kuse[ii - 1] & stk_[1].kuse[ii - 1];
/* L111: */
	}
    }
/* Calculate the new rms2 */
    if (stk_[0].rms != (float)0. && stk_[1].rms != (float)0.) {
/* Computing 2nd power */
	r__1 = stk_[0].rms;
/* Computing 2nd power */
	r__2 = stk_[1].rms;
	stk_[1].rms = 1 / sqrt(1 / (r__1 * r__1) + 1 / (r__2 * r__2));
    }
/* Concatenate the two scan number lists. */
    if ((*init & 16) == 0) {
	catsn_(init);
    }
} /* cmbn_ */


double shift_(kntrl, idif)
int *kntrl, *idif;
{
    /* System generated locals */
    float ret_val, r__1, r__2, r__3, r__4, r__5;
    double d__1;

    /* Local variables */
    float fmove;
    static int nwrsv;
    int nwr;


/* Shift checks that stack1 and stack2 are similar enough observations */
/*to be combined and returns the amt which stack1 should be shifted right. */
/* Kntrl controls the action if a difference is found. Legal values are:*/
/* 	0 If new differences are found print messages and call warn. */
/* 	1 Reset difference storage, then same as 0. */
/* 	2 return the bit coded difference in idif, don't call warn. */
/* 	4 ored with the above causes position differences to be ignored. */

    if ((3 & *kntrl) != 0) {
	nwrsv = 0;
    }
    nwr = nwrsv;
    fmove = (float)0.;
    if (stk_[0].numpt > MAXCHN || stk_[1].numpt > MAXCHN) {
	error_("Stack too big");
    }
    if (stk_[0].nprc != stk_[1].nprc) {
	nwr |= 1;
    }
    r__1 = stk_[0].dec + stk_[0].ddec;
    r__2 = stk_[1].dec + stk_[1].ddec;
    r__3 = stk_[0].ra + stk_[0].dra;
    r__4 = stk_[1].ra + stk_[1].dra;
    r__5 = (float)2e-4 / cos(stk_[0].dec * (float)M_PI / (float)180.);

    if ((4 & *kntrl) == 0 && (ifdif_(&r__1, &r__2, &c_b21) || ifdif_(&r__3, &
	    r__4, &r__5))) {
	nwr |= 4;
    }
    if (stk_[1].nprc == 0) {
	goto L5;
    }
    r__1 = stk_[0].fwid * (float).1 / stk_[0].numpt;
    if (ifdif_(&stk_[0].fwid, &stk_[1].fwid, &r__1)) {
	nwr |= 8;
	fmove = (float)0.;
    } else {
	fmove = (stk_[1].freq - stk_[0].freq) / stk_[0].fwid;
	if (DABS(fmove) > tenth) {
	    nwr |= 2;
	}
    }
    r__1 = stk_[0].expch - stk_[0].refch;
    r__2 = stk_[1].expch - stk_[1].refch;
    if (stk_[0].refch != (float)-9999. && stk_[1].refch != (float)-9999. && 
	    ifdif_(&r__1, &r__2, &tenth)) {
	nwr |= 16;
    }
    r__1 = stk_[0].fwid * rtran_(&c__2, &c__3) * tenth;
    if (ifdif_(&stk_[0].vlsr, &stk_[1].vlsr, &r__1)) {
	nwr |= 32;
    }
L5:
    if (stk_[0].nprc >= 1000 && stk_[0].fwid != stk_[1].fwid) {
	nwr |= 64;
    }
    d__1 = (double) stk_[1].vlsr;
    ret_val = fmove + stk_[1].expch - tran_(&c__3, &c__1, &d__1);
    if ((3 & *kntrl) == 2) {
	*idif = nwr;
    } else if (nwr != nwrsv) {
	nwr ^= nwrsv;
	if ((nwr & 1) != 0) {
	    printf("Process types differ\n");
	}
	if ((nwr & 2) != 0) {
	    printf("Frequencies differ\n");
	}
	if ((nwr & 4) != 0) {
	    printf("Positions differ\n");
	}
	if ((nwr & 8) != 0) {
	    printf("Filter widths differ - no freq shift done\n");
	}
	if ((nwr & 16) != 0) {
	    printf("Amt of freq shift differs\n");
	}
	if ((nwr & 32) != 0) {
	    printf("Lsr velocities differ\n");
	}
	if ((nwr & 64) != 0) {
	    printf("Scan parameters differ\n");
	}
	nwrsv |= nwr;
	warn_("");
    }
    return ret_val;
} /* shift_ */

/* Subroutine */
static void catsn_(init)
int *init;
{
    /* Initialized data */

    static unsigned short dumnam[2] = { 32853,19278 };

    /* System generated locals */
    int i__1, i__2;

    /* Local variables */
    short ntemp;
    int i1, i2, istrt;
    static int nwrsv;
    extern int indxsn_();
    short ifn[2];

/* catsn concatinates the scan number list in stacks 1&2 taking account */

/* of imbeded file names. The result is in stack2 (and extension). */
/* If and(init,1024).ne.0, return after initializing nwrsv if needed */
/* dumnam contains the file name "UNK" coded as in a stack hdr. */
    if ((1 & *init) != 0) {
	nwrsv = 0;
    }
    if ((1024 & *init) != 0) {
	return;
    }
/* is there any room in stack 2? */
    if (stk_[1].indx >= 160 && nwrsv == 1) {
	return;
    }
/* Find the file name at the end of the list in stack2. Default to UNK. */

    ifn[0] = dumnam[0];
    ifn[1] = dumnam[1];
    i__1 = stk_[1].indx;
    for (i2 = 1; i2 <= i__1; ++i2) {
	if (stk_[1].nstk[indxsn_(&i2) - 1] < -32000) {
	    ifn[0] = stk_[1].nstk[indxsn_(&i2) - 1];
	    i__2 = i2 + 1;
	    ifn[1] = stk_[1].nstk[indxsn_(&i__2) - 1];
/* Transfer the complete file name here. The next pass of the do l
oop will */
/* do no harm. */
	}
/* L100: */
    }
/*Now move the list from stack1 in. If the first file name in 1 equals the
 last*/
/* name in 2, don't transfer the file name. */
    if (ifn[0] == stk_[0].nstk[0] && ifn[1] == stk_[0].nstk[1]) {
	istrt = 3;
    } else {
	istrt = 1;
    }
    i__1 = stk_[0].indx;
    for (i1 = istrt; i1 <= i__1; ++i1) {
	if (stk_[1].indx >= 160) {
	    if (nwrsv != 1) {
		printf("Scan number overflow\n");
		nwrsv = 1;
	    }
	    return;
	}
	++stk_[1].indx;
/* 		if(indx2.eq.33) call clear(nstkx2,256) */
	ntemp = stk_[0].nstk[indxsn_(&i1) - 1];
	i2 = stk_[1].indx;
	stk_[1].nstk[indxsn_(&i2) - 1] = ntemp;
	if (ntemp < -32000) {
	    --stk_[1].nlist;
	} else {
	    ++stk_[1].nlist;
	}
    }
} /* catsn_ */

