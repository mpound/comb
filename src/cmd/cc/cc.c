/* cc.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../misc/C.h"
#include "../../scan/C.h"

#define NAMFLG 0

/* Table of constant values */

static int c__1 = 1;
static int c__2 = 2;
static int c__3 = 3;

/* Subroutine */ void cc_()
{

    /* System generated locals */
    double d__1;

    int i;
    float tempstak[MAXCHN];
    float texpch;

#include "cc.dc.h"
#include "cc.ec.h"
    /* Store old value of expch for updating refch later */
    texpch = stk_[0].expch;

    switch ((int)(*iactn + 1)) {
    case 1:
      /* compute new center channel from new frequency */
      stk_[0].expch = tran_(&c__2, &c__1, nwf);
      stk_[0].freq = *nwf;
      break;
    case 2:
      /* compute new center channel from new velocity */
      stk_[0].expch = tran_(&c__3, &c__1, nwv);
      stk_[0].vlsr = *nwv;
      break;
    case 3:
      /* Correct an error in a receiver osc. given an equivalent amount */
      /* to shift the line in velocity.  Save the shift in expch1 in rsdm(1) */
      stk_[0].rsdm[0] -= stk_[0].expch;
      d__1 = stk_[0].vlsr - *oe;
      stk_[0].expch = tran_(&c__3, &c__1, &d__1);
      stk_[0].rsdm[0] += stk_[0].expch;
      break;
    case 4: 
      /* Convert spectrum from USB to LSB or vice versa */
      if(stk_[0].nlist == 1) {
	if((i = stk_[0].nstk[0]) < NAMFLG)
	  i = stk_[0].nstk[2];
	if(i != curscn_.num || i == 0) {
	  error_("Sorry, you can only switch SSB's when using scans fetched using the gt command.");
	  return;
	}
      }
      /* stk_[0].expch=stk_[0].numpt-stk_[0].expch+1; */
      stk_[0].expch -= 2.0*scan_.mmsb*scan_.fbCntr/stk_[0].fwid;
      stk_[0].freq=stk_[0].freq - scan_.mmsb*2.0*scan_.IFCenter;
      /* need to reverse the order of the data in the stack */
      for(i = 0; i < stk_[0].numpt; i++)
	tempstak[i]=stk_[0].stak[i];
      for(i = 0; i < stk_[0].numpt; i++)
	stk_[0].stak[i]=tempstak[(stk_[0].numpt)-i-1];
      for(i = 0; i < stk_[0].nwts; i++)
	tempstak[i]=stk_[0].wght[i];
      for(i = 0; i < stk_[0].nwts; i++)
	stk_[0].wght[i]=tempstak[(stk_[0].nwts)-i-1];
      for(i = 0; i < stk_[0].nwts; i++)
	tempstak[i]=stk_[0].locwt[i];
      for(i = 0; i < stk_[0].nwts; i++) {
	if(i==0)
	  stk_[0].locwt[i]=1;
	else
	  stk_[0].locwt[i]=(stk_[0].numpt)-tempstak[(stk_[0].nwts)-i]+2;
      }
      /*  then set center frequency as requested, if possible */
      scan_.mmsb *= -1;
      stk_[0].expch = tran_(&c__2, &c__1, ssbfr);
      stk_[0].freq = *ssbfr;
      main_.pltd = 0;
      break;
    default:
      error_("Bad action");
    }
    /* for all cases, update refch */
    if (stk_[0].refch != (float)-9999.) {
      stk_[0].refch = stk_[0].refch + stk_[0].expch - texpch;
    }
} /* cc_ */
