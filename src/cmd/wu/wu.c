#include <stdio.h>
# include "../../main/C.h"
#include "../../coordsys/C.h"
# include "../../graphics/C.h"
#include "../../error/C.h"
#include <math.h>
# include <string.h>
#include <ctype.h>

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* wu.c */
static char *WuName P_((void));
static double CurrentYear P_((void));
#undef P_

char *getenv();
static char wuName[64];

void wu()
{
	double llAz,llEl, urAz, urEl;
	double az, el;
	double mat[9];
	double CurrentYear();
	XYZPOINT p1, p2;
	FILE *fp;
	char *savegstr;
	char *srcName;
	char buf[NTTYLEN];
	char tempstr[64];
#include "wuf.dc.h"
#include "wu.dc.h"
#include "wu.ec.h"
	if(*llx > *urx) {
		llAz = *urx;
		urAz = *llx;
	} else {
		llAz = *llx;
		urAz = *urx;
	}
	if(*lly > *ury) {
		llEl = *ury;
		urEl = *lly;
	} else {
		llEl = *lly;
		urEl = *ury;
	}
	if(urAz - llAz < 10 || urEl - llEl < 10)
		error_("Insufficient spatial extent");
	
	strcpy(wuName, fname);
	fp = FSafeOpen("Source coordinate file" ,fname, "r");
#if 0
	if(*yrf) { /* not defaulting to current year */
		fprintf(stderr,"yrf\n");
	}
#endif
	Pstart();
	PsetType(SP_SP);
	Pformat("full square plot");
	Pbounds(*llx,*lly,*urx,*ury);
	MkOpt(CLEAROPT, ENDOPT);
	MkGrid(NO FLAGY0,NO FLAGX0,LTYPE(29),ENDOPT);

	/* generate a rotation matrix between the equatorial system
	 * and the given lattitude at the given local sidereal time. */
	rotmat(mat,"zy",*lst * 15., 90 - *latp);

	savegstr = cmnd_.gstr;
	while( !ReadLine(buf, fp) ) {
		for(srcName = buf; isspace(*srcName); srcName++)
			;
		*(cmnd_.gstr = strchr(srcName, ' ')) = '\0';
		cmnd_.gstr++;
#include "wuf.ec.h"
		SpheretoPoint(*rap * cunits[HOURS],*decp * cunits[DEGREES],&p1);
		precess(*epp,&p1,*year,&p2);
		rotate(&p2, &p1, mat);
		PointtoSphere(&p1, &az, &el);
		az /= cunits[DEGREES];
		az = 180 -az;
		el /= cunits[DEGREES];
		if(az > llAz && az < urAz && el > llEl && el < urEl) {
			Pmove(az, el);
			Pctext(srcName);
		}
	}
	cmnd_.gstr= savegstr;
	FSafeClose(fp);
	/* put on labels */
	if(*mlbf) {
		strcpy(mlb,fname);
	}
	Pplace(0,mlb);

	if(*hlbf)
		strcpy(hlb,"Azimuth");
	Pplace(1,hlb);
	if(*vlbf)
		strcpy(vlb,"Elevation");
	Pplace(2,vlb);
/* put latitude and LST in LLC  -- MWP 3/7/94*/
	sprintf(tempstr,"LST:");
	if(*lst >= 24.)
		*lst -= 24.;
	HmsPrint(tempstr,-1,*lst);
	strcat(tempstr,"  Latitude:");
	HmsPrint(tempstr,0,*latp);
	Pplace(3,tempstr);
	Pshow(bxCol,bxRow);
}
static char *WuName() /* mwp  7/91 */
{
        if(*wuName == '\0') {
                sprintf(wuName, "%s/lib/stars", main_.comb);
        }
        return(wuName);
}

#include <sys/time.h>
#include <time.h>
static double CurrentYear() /* mwp 5/92 */
/* removed references to things HP UX doesnt understand 
 * and use most basic Unix time system calls.  This is accurate
 * to two decimal places in cyear.   -- MWP 4/26/93 
 */
{
	double cyear;
	time_t clock;	/* secs since 1/1/70 (Unix time) */
	time(&clock);  /* this is the same as clock = time((time_t)0) */
	cyear = 1970.0+clock/(86400*365.25);
	return(cyear);
}
