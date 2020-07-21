#include "../../main/C.h"
#include "../../stacks/C.h"
#include "../../scan/C.h"
#include "../../misc/C.h"
#include "../../main/machinedep.h"
#include "../../main/dragon.h"
#include <string.h>
#include <fitsio.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include "fitsFileDef.h"
#define SRC 10
#define REF 11
#define UNK 12
#define INTCOUNTPERSEC ((long)1.1e9/4) /* Omnisys intcnt counts in a second */


fitsfile *fptr;         /* FITS file pointer, defined in fitsio.h */
int fitsStatus;   /* CFITSIO fitsStatus value MUST be initialized to zero! */
int use_refstk;
int use_hotstk;

char commentbuf[80], buf[80];
char fitsfilename[300];

#define bcopy(s, d, n) memcpy(d, s, n)

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

void tschng_ P_((void));
static int typcmp_ P_((short *kind));

#undef P_


void gs_()
{

    int n, fnoff;
    char *stk0fn = (char *)stk_[0].nstk;
    int readbad;
    int searching;		/* non zero if searching for a scan type */
    STOScan sv_curSTOscn;	/* Keep original scan or last good scan */

#include "gs.dc.h"

    fnum_o = curSTOscn.filnum;
    *fnum = curSTOscn.filnum;    /* restore old file name before parser */

#include "gs.ec.h"

    if (*fnum < 0 || *fnum > 999999) {
             error("Illegal STO FITS file number= %d", *fnum);
    }


    if (!*refstkf) use_refstk = *refstk;
    else use_refstk = 0;
    if (!*hotstkf) use_hotstk = *hotstk;
    else use_hotstk = 0;


    strcpy(fitsfilename,STOscan_.dirfn);
    sprintf(buf,"%06d", *fnum);
    strcat(fitsfilename,buf);

    fitsStatus = 0;
    if (!fits_open_file(&fptr, fitsfilename, READONLY, &fitsStatus)) { 
             goto is_open; 
    }                                      /* try again add .fits */
    fitsStatus = 0;
    strcat(fitsfilename,".fits");
    if (!fits_open_file(&fptr, fitsfilename, READONLY, &fitsStatus)) {
             goto is_open; 
    }                                      /* try again add .FITS */
    fitsStatus = 0;
    *strrchr(fitsfilename,'.') = '\0';
    strcat(fitsfilename,".FITS");
    if (!fits_open_file(&fptr, fitsfilename, READONLY, &fitsStatus)) {
             goto is_open;
    } else {
             fits_report_error(stderr, fitsStatus); /* print error report */
             error("Failed to open FITS file");
    }
is_open:                      /* opened a FITS file  */

    curSTOscn.filnum = *fnum;
    fnoff = STOscan_.ndfn - 6;    /* index of file name on full path */
                                  /* get first 6 letters of file name */
    bcopy(STOscan_.dirfn + fnoff, curSTOscn.filname, 6); 

    if (*lbknd == 0)  *lbknd = 1;
    
    if (*lbknd < 1 || *lbknd > 8) 
             error("Illegal backend = %d ", *lbknd);
    STOscan_.mbknd = *lbknd;

    curscn_.num = *fnum;

    get3();

#if 0
/* old gt command
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


    scan_.mbknd = *lbknd;
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
	  if(!get3()) {
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

#endif

} /* gs_ */


    char *stk0fn = (char *)stk_[0].nstk;



/* Wed Nov 23 15:23:27 PDT 2011 - version for STO - aas
 * This is the program that does the real work.
 * Basically, all the calls to GI0 and GDI need to be replaced by
 * appropriate FITS file calls.
 */

#include "../../scan2/scan2.h"

#define bzero(s, n) memset(s, 0, n)

static char coordType[][4] =	{"AZEL", "EQU", "GAL", "EQU"};
static char nplcType[] =	{  99,     30,    11,    30 };

int get3()
{
    register struct of_stk *stp = &stk_[0];
    register struct of_stk *ref_stp;
    register struct of_stk *hot_stp;
    struct of_stk trx_stp;
    struct of_stk temp_stp;
    double avg_trx;
    double offset, sbFactor, tAmbient, tAbsorber;
    double tHotLoad;
    double hotSpill, trans, f, skEl, el, dTabs;
    double sig, fbCntr;
    register int arrayNum;
    int i, mmsb, nm1;
    char *tsp;				/* temp string pointer */
    int nDragon=0;
    int iii, i__2, n1, n2;
    float ncnt;
    double navg;

    double ra,dec;
    float fdata[1024];
    char  idata[4096],jdata[4];
    static double dtor;
    int hdutype;
    int colnum;
    int sigref;
    double ts1970,j2sec;
    double restfreq;
    double filwid,filwidsign;
    static double speedlight = 2.99792458e8;
    int typecode;
    long crepeat, cwidth;
    double vlsr;
    double inttime;
    double ldub, bdub;
    double epoch;
    double cdelt1, cdelt2;
    double raoff, decoff;
    double tsys;
    int mirrorin;


    dtor = atan(1.e0) / 45.;   /* degrees to radians conversion */


    bzero((char *)stp, 512);    /* zero out stack header */

    stp->nver = 1;
    stp->ibsln = -1;
    stp->numst = 1;
    stp->nlist = 1;
    stp->indx = 3;

    /* last 3 letters of filename included in scan names within stack */
    strncpy(curscn_.fil, curSTOscn.filname +3, 4);
    putfn_();

    stp->nstk[2] = curSTOscn.filnum;
                                   /* all data in second HDU */
    fits_movabs_hdu(fptr,2,&hdutype,&fitsStatus);

    fits_read_key(fptr, TDOUBLE, "CONELOAD", &tHotLoad , commentbuf, &fitsStatus);

    /* get time of the observation from UDPTIME = seconds since 1970 */

    fits_read_key(fptr, TDOUBLE, "TIME", &ts1970 , commentbuf, &fitsStatus);
    j2sec = 100.0 * ( ts1970/100.0 - (10957*864 + 432)); /* seconds since 2000 */
    scan_.iobssec = ts1970;
    stp->j2second = j2sec;
    main_.pltd = 0;
    stp->factt = 1;

    fits_read_key(fptr, TINT, "MIRRORIN", &mirrorin , commentbuf, &fitsStatus);
    fits_read_key(fptr, TSTRING, "TYPE", buf , commentbuf, &fitsStatus);
    if(i = strlen(buf)) {
      if (STOscan_.mbknd==8) { /* sensing is backwards for the 492 */
	if(!strncmp("SRC",buf,3)) {
	  if (mirrorin) scan_.iobstp = 8;
	  else scan_.iobstp = 31; /* SRC see cmd/pl/pl.c for the full list */
	}
	if(!strncmp("OTF",buf,3)) {
	  if (mirrorin) scan_.iobstp = 8;
	  else scan_.iobstp = 31; /* SRC see cmd/pl/pl.c for the full list */
	}
	if(!strncmp("REF",buf,3)) {
	  if (mirrorin) scan_.iobstp = 9;
	  else scan_.iobstp = 31; /* REF see cmd/pl/pl.c for the full list */
	}
	if(!strncmp("HOT",buf,3)) {
	  if (mirrorin) scan_.iobstp = 8;
	  else scan_.iobstp = 31; /* SRC see cmd/pl/pl.c for the full list */
	}
      } else { /* all of the other receivers are normal */
	if(!strncmp("SRC",buf,3)) {
	  if (mirrorin) scan_.iobstp = 0;
	  else scan_.iobstp = 8; /* SRC see cmd/pl/pl.c for the full list */
	}
	if(!strncmp("OTF",buf,3)) {
	  if (mirrorin) scan_.iobstp = 0;
	  else scan_.iobstp = 8; /* SRC see cmd/pl/pl.c for the full list */
	}
	if(!strncmp("REF",buf,3)) {
	  if (mirrorin) scan_.iobstp = 0;
	  else scan_.iobstp = 9; /* REF see cmd/pl/pl.c for the full list */
	}
	if(!strncmp("HOT",buf,3)) {
	  if (mirrorin) scan_.iobstp = 31;
	  else scan_.iobstp = 0; /* HOT see cmd/pl/pl.c for the full list */
	}
      }
    }

    /* get object name */
    fits_read_key(fptr, TSTRING, "OBJECT", buf , commentbuf, &fitsStatus);
    if(i = strlen(buf)) {
	memcpy(ObjectName, buf, i );
	while(i-- > 0 && isspace(ObjectName[i]))
		ObjectName[i] = 0;
    } else
	ObjectName[0] = 0;

    arrayNum = STOscan_.mbknd - 1;
    if(arrayNum < 0 || arrayNum >= 8)
	error("bad arrayNum = %d", arrayNum);
    fits_get_colnum(fptr, CASEINSEN, "CDELT1", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&filwid,NULL,&fitsStatus);

    filwidsign = 1.0;
    if (filwid < 0.0) {
           filwid *= -1.0;
           filwidsign = -1.0;
    }
    fits_get_colnum(fptr, CASEINSEN, "RESTFREQ", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&restfreq,NULL,&fitsStatus);
    stp->fwid = filwid * 1.0e-6 * restfreq / speedlight;

    fits_get_colnum(fptr, CASEINSEN, "DATA", &colnum, &fitsStatus);
    fits_get_coltype(fptr, colnum, &typecode, &crepeat, &cwidth, &fitsStatus);
    if((stp->numpt = crepeat) > MAXCHN) stp->numpt = MAXCHN;

    stp->freq = restfreq * 1.0e-6;

    fits_read_key(fptr, TDOUBLE, "CRVAL1", &vlsr, commentbuf, &fitsStatus);
    stp->vlsr = vlsr/1000.0;

    fits_get_colnum(fptr, CASEINSEN, "IntTime", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&inttime,NULL,&fitsStatus);
    stp->time = inttime;
    if(stp->time <= .01)
	stp->time = .01;

    fits_read_key(fptr, TDOUBLE, "RA", &ra , commentbuf, &fitsStatus);
    /*    ra /= 15.0;  */              /* convert from degrees to hours */
    stp->ra = ra; 
    fits_read_key(fptr, TDOUBLE, "DEC", &dec , commentbuf, &fitsStatus);
    stp->dec = dec;
    fits_read_key(fptr, TDOUBLE, "EQUINOX", &epoch , commentbuf, &fitsStatus);
    stp->epoch = epoch;
    if(stp->epoch < 1950.) {
         stp->epoch = 2000.0 + j2sec/(3.1556926e7);
    }

    /* Now I should have enough information to calculate az/el */

    /* This is from coordsys ...
    XYZPOINT p1;
    p1 = RaDecToXyz(ra, dec, epoch);
    XyzToCsys(p1);
    
    *** I think I need the code from obs or max3 */

    /* here is the code from src/cmd/wu.c */
    /*
    rotmat(mat,"zy",*lst * 15., 90 - *latp);
    
    SpheretoPoint(*rap * cunits[HOURS],*decp * cunits[DEGREES],&p1);
    precess(*epp,&p1,*year,&p2);
    rotate(&p2, &p1, mat);
    PointtoSphere(&p1, &az, &el);
    az /= cunits[DEGREES];
    az = 180 -az;
    el /= cunits[DEGREES];
    */
    /* end of wu.c code */

    /* Check to see if the following comment from Tony still applies - CLM */
    /* Don't really know what to do with CDELT2 and 3 */
    /* They are in Az-El, not RA-Dec as stated in FITS header */

    fits_get_colnum(fptr, CASEINSEN, "CDELT2", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&cdelt1,NULL,&fitsStatus);
    fits_get_colnum(fptr, CASEINSEN, "CDELT3", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&cdelt2,NULL,&fitsStatus);

    fits_get_colnum(fptr, CASEINSEN, "RAOFF", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&raoff,NULL,&fitsStatus);
    fits_get_colnum(fptr, CASEINSEN, "RAOFF", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&decoff,NULL,&fitsStatus);

    /* convert these offsets to az/el for pointing?, since we don't have
       mapping offsets in the level 0 data */

    stp->odra = raoff * 60.0;
    stp->oddec = decoff * 60.0;
    stp->ddec = stp->oddec / 60.;
    stp->dra = stp->odra * (24. / (360 * 60)) / cos((stp->dec+stp->ddec)/dtor);
    ra += cdelt1/15.0;
    if (ra > 24.) ra -= 24.0;
    if (ra < 0.) ra += 24.0;
    dec += cdelt2;

    /*    printf("\n ra = %g, dec = %g, epoch = %g \n",ra,dec,stp->epoch); */
    
    rdtlb(ra,dec,stp->epoch,&ldub,&bdub);

    /*    printf(" l = %g, b = %g \n",ldub,bdub); */

    stp->l = ldub;
    stp->b = bdub;
    stp->dl = 0;
    stp->db = 0;

    stp->nprc = 1;

    /* We don't know how to calibrate yet, so scan_fmult is set to 1.0 */
    /* we are now approaching a ability to calibrate can this be updated - CLM */
    scan_.fmult = 1.0;

    fits_get_colnum(fptr, CASEINSEN, "TSYS", &colnum, &fitsStatus);
    fits_read_col(fptr,TDOUBLE,colnum,STOscan_.mbknd,1,
                     1,NULL,&tsys,NULL,&fitsStatus);
    if (tsys < 10.) tsys = 10000.0 ;

    /* Do we have a better handle on tsys if we have a hot/ref combo? */

    /* Set the weight.  The weight is the equivalent integration time (sec)
     * above the atmosphere with no spillover with a 1000K receiver */
    stp->nwts = 1;
    stp->locwt[0] = 1;
    stp->wght[0] = 1.0e6 * stp->time / (tsys * tsys);

    stp->expch = (stp->numpt + 1.) / 2. ;
    stp->refch = NOTFS;

    fits_get_colnum(fptr, CASEINSEN, "DATA", &colnum, &fitsStatus);
    fits_read_col(fptr,TFLOAT,colnum,STOscan_.mbknd,1,
                     1024,NULL,fdata,NULL,&fitsStatus);


    nm1 = (filwidsign < 0.0)? 0: stp->numpt - 1;

    for(i = 0; i < stp->numpt; i++) {
      stp->stak[i] = (double) fdata[(nm1)? nm1 - i +1: i + 1]/stp->time;
      /*	  (stp->time*INTCOUNTPERSEC); */
    }

    coreFull[0] = 1;

/* here is where to eliminate bad channels. this code mostly stolen from get1.c
 * except working with floats instead of shorts (scan_.ndat)
 */
   scan_.inver = filwidsign * -1.0;  /* for "bc" */
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
                for (iii = n1; iii <= i__2; ++iii)
                    stp->stak[iii - 1] = navg;
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

      /*    if ( fits_close_file(fptr, &fitsStatus) )
	    fits_report_error(stderr, fitsStatus); */

      fits_close_file(fptr, &fitsStatus);

      /* Now can we calibrate? -CLM */

      if (use_refstk) {
	if ((use_refstk>3) || !coreFull[use_refstk-1]) error("Reference stack %d inaccessible.", use_refstk);
	ref_stp = &stk_[use_refstk-1];
	/* Do (S-R)/R */
	for(i = 0; i < stp->numpt; i++) {
	  stp->stak[i] = (stp->stak[i] - ref_stp->stak[i])/ref_stp->stak[i];
	}
	scan_.iobstp=2;
      }

      if ((use_hotstk) && !(use_refstk)) {
	if ((use_hotstk>3) || !coreFull[use_hotstk-1]) error("Hot stack %d inaccessible.", use_hotstk);
	hot_stp = &stk_[use_hotstk-1];
	/* Note tHotLoad is actually the temp of the hotload when the ref was taken, this should be fixed */

	/* Do (H-R)/T_Hot and assume ref in stk1 */
	avg_trx = 0;
	for(i = 0; i < stp->numpt; i++) {
	  stp->stak[i] = (hot_stp->stak[i] - stp->stak[i] )/tHotLoad;
	  trx_stp.stak[i] = (hot_stp->stak[i]/stp->stak[i]) - tHotLoad;
	  avg_trx += trx_stp.stak[i];
	}
	avg_trx /= stp->numpt;
	printf("T_rx = %f\n",avg_trx);
	scan_.iobstp=10;
      }

      if ((use_hotstk) && (use_refstk)) {
	if ((use_hotstk>3) || !coreFull[use_hotstk-1]) error("Hot stack %d inaccessible.", use_hotstk);
	hot_stp = &stk_[use_hotstk-1];
	if ((use_refstk>3) || !coreFull[use_refstk-1]) error("Reference stack %d inaccessible.", use_refstk);
	ref_stp = &stk_[use_refstk-1];
	if (use_refstk==use_hotstk) error("Reference stack and hot stack cannot be identical.");
	
	/* Note tHotLoad is actually the temp of the hotload when the src was taken, this should be fixed */

	/* Do (H-R)/T_Hot and assume ref in stk1 */
	avg_trx = 0;
	for(i = 0; i < stp->numpt; i++) {
	  temp_stp->stak[i] = (hot_stp->stak[i] - ref_stp->stak[i] )/tHotLoad;
	  trx_stp.stak[i] = (hot_stp->stak[i]/temp_stp->stak[i]) - tHotLoad;
	  avg_trx += trx_stp.stak[i];
	}
	avg_trx /= stp->numpt;
	printf("T_rx = %f\n",avg_trx);
	/* Do (S-R)/R */
	for(i = 0; i < stp->numpt; i++) {
	  stp->stak[i] = (stp->stak[i] - ref_stp->stak[i])/ref_stp->stak[i];
	}
	scan_.iobstp=2;
	/* Use this as our tsys for now */
	stp->wght[0] = 1.0e6 * stp->time / (avg_trx * avg_trx);
      }

    return(1);
}
