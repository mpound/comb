#include "../../main/C.h"
#include "../../stacks/C.h"
#include "../../scan/C.h"
#include "../../scan2/scan2.h"
#include "../../misc/C.h"

#define bzero(s, n) memset(s, 0, n)

/* Table of constant values */

static int c__1 = 1;

/* Subroutine */ void bc_()
{
    /* System generated locals */
    int i__1;

    /* Local variables */
    int i, n[2];

#include "bc.dc.h"
#include "bc.ec.h"

/* Iaction=1 is a request to clear previous bad channels. */
    if (*iact != 0) 
	bzero(&scan_.numbch, 20);

/* Rflg=1 means that no horizontal values were given. */
    if (*rflag != 0) {
	if(*prn) { 		/* MWP 3/24/93 */
		for(i=0;i<scan_.numbch;i++)
			printf("%d ",scan_.nbadch[i]);
		printf("\n");
	}
	return;
    }

/* Check that stack1 is reasonable and contains the scan which was last */
/* read. Indxsn is a function which compensates for the extension */
/* of the scan number list beyond the stack header into an overflow */
/* block. */
    if (stfull_(&c__1) == 0) 
	error_("No data in stack 1");

    switch(sci.fileType) { /* make it work for PDFL and SDD - MWP 3/23/93 */
    case OLDBL:
    default:
      if (scan_.ncur >= 76 && stk_[0].nprc == scan_.kontrl) {
	i__1 = stk_[0].indx;
	for (i = 1; i <= i__1; ++i) {
	    if (stk_[0].nstk[indxsn_(&i) - 1] == scan_.ndat[0]) 
		goto L90;
	}
      }
      error_("Stack 1 too different from scan");
      break;
    case NEWBL: /* scan2 */
/*	error_("Can't set bad channels for Scan2 scan type yet");
	break;
mwp commented out 5/9/96 to test bc on scan2*/
/* The two NRAO formats */
    case PDFL:
    case SDD:  /* 12/15/93 MWP */
        if (stk_[0].nprc == scan_.kontrl) {
  	  i__1 = stk_[0].indx;
  	  for (i = 1; i <= i__1; ++i) {
	    if (stk_[0].nstk[indxsn_(&i) - 1] == sci.nscan) 
		goto L90;
	  }
	}
        error_("Stack 1 too different from scan");
	break;
    } /* switch */

L90:
/*
 *  pltoch converts a range of horiz plot units into an 
 *   ordered pair of chan #.
 */
    pltoch_(h1, h2, n, &n[1]);
    if (n[1] - n[0] > 20) 
	warn_("more than 20 bad channels");

    for (i = 1; i <= 2; ++i) {
	if (scan_.numbch > 7) {
	    error_("Out of bad channel storage");
	}
	++scan_.numbch;
/*
 * For PDFL, scan_.ncur == 0 always, so this will still work.
 * I set scan_.inver in pdfl.c to reflect the sideband -  MWP
 */
	if (scan_.inver > 0) {
	    scan_.nbadch[scan_.numbch - 1] = scan_.ncur + n[i - 1];
	} else {
	    scan_.nbadch[scan_.numbch - 1] = scan_.ncur + 1 + stk_[0].numpt 
		    - n[3 - i - 1];
	}
    }
    if (*prn) { 		/* MWP 3/24/93 */
	for(i=0;i<scan_.numbch;i++)
		printf("%d ",scan_.nbadch[i]);
	printf("\n");
    }
/* now fix stack 1 */
/*
 * elim_(start,end,stack) was originally fortran, so it operates on elements
 * start-1  to end-1 of stk_[stack].
 */
    elim_(n[0], n[1], 0);
} /* bc_ */
