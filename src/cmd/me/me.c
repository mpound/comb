# include <stdio.h>
# include <string.h>
#include <math.h>
# include "../../main/C.h"
# include "../../error/C.h"
# include "../../parse/C.h"
# include "../../graphics/C.h"
#include "../../coordsys/C.h"
#include "../../stacks/C.h"
#include "../../stacks/include.h"
#include "../../scan/C.h"
#include "../../scan2/scan2.h"

#define bcopy(s, d, n) memcpy(d, s, n)

#define FS 1
#define OBSFS 1
#define PSw 2
#define OBSPS 2
#define BS 4
#define OBSBS 4
#define BP 8
#define OBSBP 5
#define SK 16
#define OBSSK 12
#define SA 32
#define OBSSA 20
#define SE 64
#define OBSSE 21
char typeTrans[] = {FS,PSw,0,BS,BP,0,0,0,0,0,0,SK,0,0,0,0,0,0,0,SA,SE};

static char dfltName[] = "meLastScan";
static char jbknd[] = {100,25,10,0};
static double hours = 24.;
static double degrees = 360.;
static double minutes = 60.;

void ReadMapsFile();
int FindMap();
void RecordLast();
void ReleaseMaps();

void me()
{
#include "me.dc.h"
	double xSource, x1, x2, ySource, y1, y2, f;
	int currentScan;
	int search_all_bes = 0;
	int max_be;
	register int scanCount;
	register char *fileName = &scan_.datfn[scan_.ndfn - 3];
	int scansInMap = 0;		/* Count scans in each map */
	int obsType;
	int svbknd;
	static int step = 1;		/* for fortran compat call */
	char ts[24];

	/* Preset scan numbers which parse can't do.  GtStkDflt requires
	 * an open stacks directory. */
	curDir = FOREGROUND;	/* always use the foreground stacks */
	if(!notOpen[FOREGROUND] && GtStkDflt(dfltName, "%s", ts, 0)) {
	    sscanf(ts, "%3s%hd", fileName, &curscn_.num);
	    *sn1n = *sn2n = curscn_.num;
	    bcopy(fileName, sn1f, 3);
	    bcopy(fileName, sn2f, 3);
	    bcopy(fileName, curscn_.fil, 3);
	} else {
	    *sn1f = 0;
	}

#include "me.ec.h"

	if(!*sssfl) {
	    /* NOTE that the parse routine puts the scan num in curscn_ */
	    strncpy(fileName, sssf,3);
	    fileName[3] = '\0';
	    curscn_.num = *sssn - 1;
	    goto setnum;
	}
	if(*ctpfl)
	    error("Must give position [range]");
	if(*befl)
	    error("Must specify backend");
	if(!*snf) {		/* user gave scan number range */
		strncpy(fileName, sn1f,3);
		fileName[3] = '\0';
		curscn_.num = *sn1n - 1;
	}
	if(!*sn1f)
		error("No scan number in command or default file");
	/* Set up a few preliminaries for the map description */
	obsType = 0;
	if(*fs) obsType |= FS;
	if(*ps) obsType |= PSw;
	if(*bs) obsType |= BS;
	if(*bp) obsType |= BP;
	if(*sk) obsType |= SK;
	if(*se) obsType |= SE;
	if(*sa) obsType |= SA;
	if(obsType == 0) obsType = FS | PSw;
	if(*ctp == RADEC) {
	    if(!*rdecf) {
		y1 = fmod(*cdec + *rdec1 / minutes, degrees); 
		y2 = fmod(*cdec + *rdec2 / minutes, degrees); 
	    }
	} else {
	    if(!*rlf) {
		x1 = fmod(*cl + *rl1, degrees);
		x2 = fmod(*cl + *rl2, degrees);
	    }
	    if(!*rbf) {
		y1 = fmod(*cb + *rb1, degrees);
		y2 = fmod(*cb + *rb2, degrees);
	    }
	}

/* mbknd will be set to 1,2,or3 for specific backends or 100,25,10 for */
/* bbe,nbe,or exp. */

	if (*lbetyp > 0) {
	    scan_.mbknd = jbknd[*lbetyp - 1];
	} else {
	    scan_.mbknd = *lbknd;
	}
	if (scan_.mbknd == 0) {
	  scan_.mbknd = 1;
	  search_all_bes = 1;
	}
	svbknd = scan_.mbknd;
	opend_();
	scnlmt_(sn1n, sn2n, sn1f, sn2f, step, (int)*snf);
	for(scanCount = 0; (currentScan = nxtscn_()); ) {
	  curscn_.num = currentScan;
	  if(read_(&curscn_.num))
	    continue;
	  if(search_all_bes) {
	    max_be = sci.cNumArrays;
	  } else {
	    max_be = svbknd;
	  }
	  for(scan_.mbknd = svbknd ; scan_.mbknd <= max_be ; scan_.mbknd++) {
	    if(scan_.iobstp <= 0 || scan_.iobstp > 21 ||
			(typeTrans[scan_.iobstp - 1] & obsType) == 0 ||
	    		!get_() ||
			stk_[0].wght[0] < *fwght ||
			stk_[0].freq < *ffreq1 || stk_[0].freq > *ffreq2)
		continue;

	    if(*ctp == RADEC) {
		if(!*rdecf || !*rraf) {
		    ySource = fmod(stk_[0].dec + stk_[0].ddec, degrees);
		}
		if(*rdecf) {
		    if(cifdif(*cdec, stk_[0].dec, (1./3600.)))
			continue;
		} else {
		    if(y1 < y2) {
			if(ySource < y1 || ySource > y2)
			    continue;
		    } else {
			if(ySource < y1 && ySource > y2)
			    continue;
		    }
		}
		if(*rraf) {
		    if(cifdif(*cra, stk_[0].ra, (1./36000.)))
			continue;
		} else {
		    f = 900 * cos(ySource * cunits[DEGREES]);
		    xSource = fmod(stk_[0].ra + stk_[0].dra, hours);
		    x1 = fmod(*cra + *rra1 / f, hours);
		    x2 = fmod(*cra + *rra2 / f, hours);
		    if(x1 < x2) {
			if(xSource < x1 || xSource > x2)
			    continue;
		    } else {
			if(xSource < x1 && xSource > x2)
			    continue;
		    }
		}
	    } else {
		if(*rlf) {
		    if(cifdif(*cl, stk_[0].l, (1./3600.)))
			continue;
		} else {
		    xSource = fmod(stk_[0].l + stk_[0].dl, degrees);
		    if(x1 < x2) {
			if(xSource < x1 || xSource > x2)
			    continue;
		    } else {
			if(xSource < x1 && xSource > x2)
			    continue;
		    }
		}
		if(*rbf) {
		    if(cifdif(*cb, stk_[0].b, (1./3600.)))
			continue;
		} else {
		    ySource = fmod(stk_[0].b + stk_[0].db, degrees);
		    if(y1 < y2) {
			if(ySource < y1 || ySource > y2)
			    continue;
		    } else {
			if(ySource < y1 && ySource > y2)
			    continue;
		    }
		}
	    }

	    if(!scansInMap && *smac) {
		combex_(smac);
		get_();
	    }
	    if(scansInMap++ == 0 && !notOpen[FOREGROUND])
		gg_[1] = NextStk(10,EMPTY);
	    if(*rmac)
	        combex_(rmac);

	    if(++scanCount >= *pl) {
		scanCount = 0;
		combex_("pl");
	    }
	  }
	}
	if(scansInMap) {
	    printf("%d scans found\n", scansInMap);
	    if(*emac)
		combex_(emac);
	}
setnum:
	if(*updat || !*sssfl)
	    if(!notOpen[FOREGROUND])
		WrStkDflt(dfltName, "%3s%d", fileName, curscn_.num);
	    else
		error("No stacks directory open for update");
#include "me.uc.h"
}
