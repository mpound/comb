#include "../../main/C.h"
#include "../../stacks/C.h"
#include "../../scan/C.h"
#include "../../scan2/scan2.h"
#include "../../misc/C.h"
#include "../../main/machinedep.h"
#include "../../main/dragon.h"
#include <stdio.h>

#define bcopy(s, d, n) memcpy(d, s, n)

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* tschng.c */
void tschng_ P_((void));

#undef P_

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* gt.c */
static int typcmp_ P_((short *kind));
#undef P_

void gt_()
{
    /* Initialized data */

    static short jbknd[3] = { 100,25,10 };

    /* System generated locals */
    int i__1;

    int n;
    char *stk0fn = (char *)stk_[0].nstk;
    int readbad;
    int searching;		/* non zero if searching for a scan type */
    Scan sv_curscn;		/* Keep original scan or last good scan */

#include "gt.dc.h"

/* Set up the scan number range to the old values.  Parse can't do this */
    *sn1n = *sn2n = curscn_.num;
    bcopy(curscn_.fil, sn1f, 3);
    bcopy(curscn_.fil, sn2f, 3);

#include "gt.ec.h"
    i__1 = scan_.ndfn - 3;
    subscanReq = *ssn;
    bcopy(sn1f, scan_.datfn + i__1, 3);
    if (*fss != 0) {
	n = 1;
	if (stk_[0].nstk[0] < 0) {
	    n = 3;
#if BYTEREVERSED
	    scan_.datfn[i__1] = stk0fn[0];
	    scan_.datfn[i__1 + 1] = stk0fn[2];
	    scan_.datfn[i__1 + 2] = stk0fn[3];
#else
    	    bcopy( stk0fn + 1, scan_.datfn + i__1, 3);
#endif /*BYTEREVERSED*/
	}
	*sn1n = stk_[0].nstk[n - 1];
    }
    if(*pfl) {
	printf("Current scan %3s%d\n", sn1f, *sn1n);
	if(*fss == 0 && *snf)

		return;
    }
    scnlmt_(sn1n, sn2n, sn1f, sn2f,(int)*istep, (int)*snf);

/* mbknd will be set to 1,2,or3 for specific backends or 100,25,10 for */
/* bbe,nbe,or exp. */

    if (*lbetyp > 0) {
	scan_.mbknd = jbknd[*lbetyp - 1];
    } else {
	scan_.mbknd = *lbknd;
    }
    opend_();
/* this must come after opend() because opend() calls LastScan
 * which determines the first and last scan numbers and puts them
 * in sci.FirstScan and sci.LastScan
 */
    if(*first) {
	*sn1n = sci.firstScan;
	*sn2n = sci.firstScan;
        scnlmt_(sn1n, sn2n, sn1f, sn2f,(int)*istep, (int)*snf);
    }
    if(*last) {
      /* For some reason opend seems insufficient, let's try an explicit 
         call to LastScan to pull in the actual LastScan number
         - CLM 20 Jun 2001 */
        LastScan();
	*sn1n = sci.lastScan;
	*sn2n = sci.lastScan;
        scnlmt_(sn1n, sn2n, sn1f, sn2f,(int)*istep, (int)*snf);
    }
    if(!*typef && (sci.fileType != 1 && sci.fileType !=3 ) )
	error("Can only use 'type:' with Bell Labs scan2 (BLSDF) files");

/* If snf .ne.0 no new scan numbers were given. If lmttyp.ne.0 a limiting type
 * (eg "dt:") was given or if *typef == 0 we are searching for an ascii value
 *  in ObsType. If both are true, search forward until the required
 * scan type is found. Start off by incrementing the scan number.  */

    searching = *lmttyp || !*typef;
    if (*snf != 0 && searching) {
	*sn1n = nxtscn_();
	if (*sn1n <= 0) {
	    if( *tflg) {
		main_.test = 0;
		return;
	    } else {
		error_("End of file");
	    }
	}
    }
    bcopy(use_.muse, use_.nuse, sizeof(use_.nuse));
    sv_curscn = curscn_;
    for(curscn_.num = *sn1n; curscn_.num > 0; curscn_.num = nxtscn_()) {
	/* If this scan is empty and we are not searching for a specific type,
	 * fail */
	if((readbad = read_(&curscn_.num)) && !searching) {
	    if( *tflg) {
		main_.test = 0;
		return;
	    } else {
		error("Scan %d is empty", curscn_.num);
	    }
	}

	/*fprintf(stderr,"requested: %d\n",(GI0(SubScanN) == subscanReq));*/
	/*fprintf(stderr,"dragon: %d\n",(GI0(SubScanN) == iDRAGON));*/
	/*fprintf(stderr,"value: %d\n",GI0(SubScanN));*/

/* First, check for requested subscan number.
 * Previously, reaching the end of an SDD/PDFL file meant the requested
 * subscan was not present and error() was called from ReadSDD/PDFL.
 * Now, since gt() can search on type:, reaching the end of an SDD/PDFL 
 * may mean that the ObsType was not found, so we have to return from 
 * ReadSDD/PDFL and do the test here.  BUT, blsdf files don't have subscans, 
 * so for them iDRAGON is returned from GI0(SubScanN). Thus, the proper subscan 
 * test is: 
 *
 * "If the subscan doesn't match the requested subscan AND it's not a dragon, 
 *  then bail out"  
 *
 * We don't want to add this to the *typef test below because comb will then
 * loop over all scans looking for a matching subscan, but ss: means
 * get the subscan for the given *sn range not all scans.
 *        -- MWP Thu Apr 25 14:57:37 EDT 1996
 */

      if(readbad == 0) {
        if(( (GI0(SubScanN) != subscanReq) && (GI0(SubScanN) != iDRAGON) )) {
	   if( *tflg) {
                main_.test = 0;
                return;
            } else 
	        error("Requested subscan %d not found",subscanReq);
	}
      }

	/* If the scan has data and either we are not searching for a specific
	 * type or it matches the search type, process it. */

	if(readbad == 0 && ((*typef)? typcmp_(lmttyp):
			      !strcmp(type, GS(ObsTypeN)) )) {
	  if(!get_()) {
	    /* This keeps it simple.  Other options would be to skip over
	     * the scan without the requested backend or if t: was used
	     *  just return .test = 0. */
	    /* It turns out to be more helpful to allow one to test for
	       the existence of the other backend rather than killing a 
	       macro entirely, hence I have added to code to check for the
	       test flag - CLM 20 Jun 2001 */
	    if (*tflg) {
	      main_.test = 0;
	      return;
	    } else
	      error("Bad backend");
	  }
	  if (*tsfl != 0) {
	    tschng_();		/* This sets .test depending on match */
	  }
	  if(GS(ObjectN)!=0)
	    strcpy(stk_[0].label,GS(ObjectN)); 
	  /* Copy the object name to the
	   * stack label. If not present,
	   * GS() returns 0.  */
	  if (*snf == 0 && ! islast_()) {
	    combex_("pl fhr:");
	    sv_curscn = curscn_;	/* Update fall back scan number */
	    wait_();
	  } else {
	    main_.pltd = 0;
	    if( *tflg)
	      main_.test = 1;
	    return;
	  }
	}
    }
    curscn_ = sv_curscn;	/* Failure, so restore last good scan number */
    if( *tflg)
      main_.test = 0;
    else
      error("Failed to find requested scan");
} /* gt_ */


static int typcmp_(kind)
short *kind;
{
    switch ((int)*kind) {
	case 0:
	    return (1);
	case 1:
	    return (scan_.iobstp < 10 || (scan_.iobstp > 29 &&
	   	 scan_.iobstp < 40));
	case 2:
	    return (scan_.iobstp == 1);
	case 3:
	    return (scan_.iobstp == 2);
	case 4:
	    return (scan_.iobstp == 4);
	case 5:
	    return (scan_.iobstp == 5);
	case 6:
	    return (scan_.iobstp == 8);  /* MWP 3/23/93 - PDFL Total Power On */
	case 7:
	    return (scan_.iobstp == 9);  /* MWP 3/23/93 - PDFL Total Power Off */
	case 8:
	    return (scan_.iobstp == 10);
	case 9:
	    return (scan_.iobstp == 11);
	case 10:
	    return (scan_.iobstp == 12);
	case 11:
	    return (scan_.iobstp == 13);
	case 12:
	    return (scan_.iobstp >= 20 && scan_.iobstp <= 29);
	case 13:
	    return (scan_.iobstp == 22);
	default:
	    error_("Internal error in typcmp");
	}
}
