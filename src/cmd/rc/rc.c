# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include "../../main/C.h"
# include "../../main/machinedep.h"
# include "../../stacks/C.h"
# include "../../coordsys/C.h"
# include "../../stacks/include.h"
# include "../../error/C.h"

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* rc.c */
static void prsyshead P_((void));
static void printsys P_((RelCoordSys system));
static int openlocal P_((int flag));
static int openglobal P_((int flag));
static int getsys P_((int filid, char *name, RelCoordSys *system));
static void prsysfile P_((int filid));
static void writesys P_((int whichfil, RelCoordSys system));
static void replacesys P_((int filid, RelCoordSys system));
static void removesys P_((int filid, RelCoordSys system));
#if BYTEREVERSED
static void ChgEndianSys P_((RelCoordSys *system)); 
#endif /* BYTEREVERSED */
#undef P_


extern char *getenv();
extern off_t lseek();

void rc()
{
	static XYZPOINT tp;
	static XYPOINT tp2 = {1,0};
	static int tpflag = 0;
	XYPOINT tpoint;

	char tempstr[64];
	char *fmt;
# include "rc.dc.h"
# include "rc.ec.h"

	switch(*act) {
	/* read coordinate system from local or global file */
	case 1:
		if(getsys(openlocal(O_RDONLY),rcsnm,&rcsys))
		{
			if(getsys(openglobal(O_RDONLY),rcsnm,&rcsys))
			{
				error("Can't find %s",rcsnm);
			}
		}
		break;
	/* remove from local file */
	case 6:
		removesys(openlocal(O_RDWR),rcsys);
		return;

	/* remove from global file */
	case 7:
		removesys(openglobal(O_RDWR),rcsys);
		return;

	/* print local coordinate systems */
	case 9:
		printf("Local coordinate systems:\n");
		prsysfile(openlocal(O_RDONLY));
		return;

	/* print global coordinate systems */
	case 10:
		printf("Global coordinate systems:\n");
		prsysfile(openglobal(O_RDONLY));
		return;
	/* convert total coordinates to relative and print */
	case 11:
		switch(rcsys.ctp) {
		case RADEC:
			tp = RaDecToXyz(*t1,*t2,(double)rcsys.cepoch);
			fmt = "%8.1f %8.1f";
			break;
		case LB:
			tp = LbToXyz(*t1,*t2);
			fmt = "%10.3f %10.3f";
			break;
		default:
			error("Unknown center type");
		}
		tp2 = XyzToCsys(tp);
		printf(fmt,tp2.x, tp2.y);
		return;
	/* convert relative to total coords and print them */
	case 12:
		tpoint.x = *t1;
		tpoint.y = *t2;
		tp2 = CsysToAbs(tpoint);
		switch(rcsys.ctp) {
		case RADEC:
			tempstr[0] = '\0';
			tp2.x /= cunits[HOURS];
			HmsPrint(tempstr, 11, (tp2.x < 0)? tp2.x + 24.: tp2.x);
			(void)strcat(tempstr," ");
			HmsPrint(tempstr, 0, tp2.y / cunits[DEGREES]);
			puts(tempstr);
			return;
		case LB:
			printf("%g %g",tp2.x / cunits[DEGREES],tp2.y /
				cunits[DEGREES]);
			return;
		}
	}

	if(!*ctypf) { 
	    switch(*ctyp) {
	    case 0:  	/* Completely set up rcsys from stack 1
			* even though it may be overwritten later */
		if(! coreFull[0])
			error_("No data in stack 1");
		rcsys.rot = 0.;
		if((stk_[0].nplc & 1) == 0) {	/* ra-dec center coordinates */
			rcsys.x = stk_[0].ra;
			rcsys.y = stk_[0].dec;
			rcsys.oepoch = rcsys.cepoch = stk_[0].epoch;
			rcsys.ctp = RADEC;
		} else {			/* l-b center coordinates */
			rcsys.x = stk_[0].l;
			rcsys.y = stk_[0].b;
			rcsys.oepoch = rcsys.cepoch = EPOCH;
			rcsys.ctp = LB;
		}
		switch((stk_[0].nplc/10) &7) {
		case 0:				/* dra-ddec offsets */
			rcsys.otp = RADEC;
			rcsys.xun = HOURS;
			rcsys.yun = DEGREES;
			break;
		case 1:				/* dl-db offsets */
			rcsys.otp = LB;
			rcsys.xun = DEGREES;
			rcsys.yun = DEGREES;
			break;
		case 2:				/* dx-dy offsets */
			rcsys.otp = DXDY;
			rcsys.xun = ARCMINUTES;
			rcsys.yun = ARCMINUTES;
			break;
		case 3:				/* odra-oddec offsets */
			rcsys.otp = RADEC;
			rcsys.xun = ARCMINUTES | SCALED;
			rcsys.yun = ARCMINUTES;
			break;
		case 4:                         /* arcsec offsets     */ 
			rcsys.otp = RADEC;
			rcsys.xun = ARCSECONDS | SCALED;
			rcsys.yun = ARCSECONDS;
			break;
		default:
			error_("Unknown offset type in stack 1");
		}
	    		break;
	    case 1: 	/* ra-dec center given */
			rcsys.x = *cc1;
			rcsys.y = *cc2;
			rcsys.ctp = RADEC;
			rcsys.oepoch = rcsys.cepoch = *cep;
			break;
	    case 2: 	/* l-b center given */
			rcsys.x = *cc1;
			rcsys.y = *cc2;
			rcsys.ctp = LB;
			break;
	    } /* end switch (*ctyp) */
	} /* end if (!*ctypf) */
	 if(*ofl==0) { 
/* && !*ctypf) { /* skip if fs is given -- this didnt work mwp 5/92*/
		  switch(*otyp) {
		  case 0:				/* dra-ddec offsets */
			rcsys.otp = RADEC;
			rcsys.xun = HOURS;
			rcsys.yun = DEGREES;
			break;
		  case 1:				/* dl-db offsets */
			rcsys.otp = LB;
			rcsys.xun = DEGREES;
			rcsys.yun = DEGREES;
			break;
		  case 2:				/* dx-dy offsets */
			if(!*rotef && *rote == 2) {
				tpoint.x = *ep1;
				tpoint.y = *ep2;
				rcsys.rot = RotFromPoint(rcsys, tpoint);
			}
			rcsys.otp = DXDY;
			if(!*uufl || rcsys.xun != DEGREES || rcsys.yun !=
				DEGREES)
				rcsys.xun = rcsys.yun =
					(*uutyp)? DEGREES: ARCMINUTES;
			break;
		  case 3:				/* odra-oddec offsets */
			rcsys.otp = RADEC;
			rcsys.xun = ARCMINUTES | SCALED;
			rcsys.yun = ARCMINUTES;
			break;
		  case 4:                         /* arcsec offsets     */ 
			if(rcsys.ctp == RADEC) {
			   rcsys.otp = RADEC;
			   rcsys.xun = ARCSECONDS | SCALED;
			}
			else { /*this ignores possibility of DXDY -- mwp 5/24/95*/
			   rcsys.otp = LB;
			   rcsys.xun = ARCSECONDS;
			}
			rcsys.yun = ARCSECONDS;
		  }
	 } /* if (*ofl && *ctyp) */

	/* Set up csys */
	MkCsys(rcsys);

	/* set and/or display test point */
	if(!*t1f) {
		tpflag++;
		tp2.x = *t1;
		tp = CsysToXyz(tp2);
	}
	if(!*t2f) {
		tpflag++;
		tp2.y = *t2;
		tp = CsysToXyz(tp2);
	}
	if(tpflag) {
		tp2 = XyzToCsys(tp);
		printf("Test Point %.6f %.6f\n",tp2.x, tp2.y);
	}

	/* select action */
	switch(*act)
	{

		/* store current system locally (after changes) */
		case 2:
			writesys(0,rcsys);
			break;

		/* store current system globally (after changes) */
		case 3:
			writesys(1,rcsys);
			break;

		/* replace current system locally (after changes) */
		case 4:
			replacesys(openlocal(O_RDWR),rcsys);
			break;

		/* replace current system globally (after changes) */
		case 5:
			replacesys(openglobal(O_RDWR),rcsys);
			break;

			break;

		/* make current system the default for dir 1 */
		case 13:
			WrStkDflt("RCSys","%s",rcsys.name);
			break;
	}

	/* print current coordinate system */
	if(*act < 9)
	{
		printf("Current relative coordinate system:\n");
		prsyshead();
		printsys(rcsys);
	}
#include "rc.uc.h"
}

/***************************************************/
/* prsyshead - print heading for coordinate system */
/***************************************************/

static void prsyshead()
{
	printf("    SYSTEM NAME     |");
	printf("        CENTER COORDINATES        |");
	printf("     OFFSET TYPE\n");
}

/*************************************************/
/* printsys - print a relative coordinate system */
/*************************************************/

static void printsys(system)
	RelCoordSys system;
{
	static char punit[] =	/* compact names of degrees,radians,hours */
		{'d','r','h','\'','\"'};	/* and arcminutes, secs */
		/*{'d','r','h','\'',};	/* and arcminutes */
	char t1[12],t2[12];	/* temporary for printing hms format */
	static char *ofmt[] = {
		" %cdra(%c) ddec(%c)",
		" %cdl(%c) db(%c)",
		" %cdx(%c) dy(%c) pa %6.2f",
		" %cdra(%c) ddec(%c)",
		" Bad Offset type"
	};
	register int i;

	/* print the coordinate system name */
	printf("%-19s |",system.name);

	/* print center coordinates */
	if(system.ctp ==LB) {
		printf("    lb      %7.3lf      %7.3lf  |",system.x,system.y);
	} else {
		*t1 = '\0';
		*t2 = '\0';
		HmsPrint(t1,11,system.x);
		HmsPrint(t2,0,system.y);
		printf(" rd(%4.1f) %s, %s |",system.cepoch,t1,t2);
	}

	/* print offset coordinate type */
	if((i = system.otp & 7) != system.otp)
		i = 3;
	printf(ofmt[i],(system.xun & SCALED)?'o':' ',punit[system.xun & 7],
		punit[system.yun],system.rot);

	/* go on to next line */
	printf("\n");
}

/*****************************************************/
/* openlocal - open the local coordinate system file */
/*****************************************************/

static int openlocal(flag)
	int flag;		/* flag for open */
{
	char lfil[100];		/* local file name */

	/* construct file name */
	(void)sprintf(lfil,"%s/.LCOORDS",getenv("HOME"));

	/* create file if it doesn't exist */
	if(access(lfil,0) == -1)
	{
		warn_("No local coordinates file -- will create one\n");
		noint_();
		(void)close(creat(lfil,0666));
		yesint_();
	}

	/* open file */
	return(SafeOpen(lfil,lfil,flag,0));
}

/***************************************************/
/* openglobal - open global coordinate system file */
/***************************************************/

static int openglobal(flag)
	int flag;		/* flag for open */
{
	char gfil[100];		/* global file name */

	/* get file name */
	(void)sprintf(gfil,"%s/lib/.GCOORDS",main_.comb);

	/* create file if it doesn't exist */
	if(access(gfil,0) == -1)
	{
		warn_("No global coordinates file -- will create one\n");
		noint_();
		(void)close(creat(gfil,0666));
		yesint_();
	}

	/* open file */
	return(SafeOpen(gfil,gfil,flag,0));
}

/*************************************************/
/* getsys - read a coordinate system from a file */
/*************************************************/

static int getsys(filid,name,system)
	int filid;		/* file identifier of file to read from */
	char *name;		/* name of system to read */
	RelCoordSys *system;	/* system to read */
{
	RelCoordSys tsys;	/* temporary coordinate system */

	/* if filid is bad, return */
	if(filid == -1)
		return(-1);

	/* find coordinate system */
	while(read(filid,(char *)&tsys,sizeof(RelCoordSys)))
	{
		if(!strcmp(name,tsys.name))
		{
			*system = tsys;
#if BYTEREVERSED
			ChgEndianSys(system);
#endif /* BYTEREVERSED */
			SafeClose(filid);
			return(0);
		}
	}

	/* if coordinate system not found, return error */
	SafeClose(filid);
	return(-1);
}

/**********************************************/
/* prsysfile - print out everything in a file */
/**********************************************/

static void prsysfile(filid)
	int filid;		/* file identifier */
{
	RelCoordSys tsys;	/* temporary coordinate system */

	/* check that filid is ok */
	if(filid == -1)
		error_("Can't read coordinate system file");

	/* print out */
	prsyshead();
	while(read(filid,(char *)&tsys,sizeof(RelCoordSys))) {
#if BYTEREVERSED
		ChgEndianSys(&tsys);
#endif /* BYTEREVERSED */
		printsys(tsys);
	}

	/* close file */
	SafeClose(filid);
}

/****************************************************/
/* writesys - write a coordinate system into a file */
/****************************************************/

static void writesys(whichfil,system)
	int whichfil;		/* flag for which file to write to */
				/* 0 = local */
				/* 1 = global */
	RelCoordSys system;	/* coordinate system to be written */
{
	int filid;		/* file identifier */
	RelCoordSys tsys;	/* temporary coordinate system */

	/* see if coordinate system already exists */
	if(whichfil)
		filid = openglobal(O_RDONLY);
	else
		filid = openlocal(O_RDONLY);
	if(!getsys(filid,system.name,&tsys))
	{
		printf("%s already exists:\n",system.name);
		prsyshead();
#if BYTEREVERSED
		ChgEndianSys(&tsys);
#endif /* BYTEREVERSED */
		printsys(tsys);
		if(whichfil)
			error_("Try using 'rg' instead of 'sg'");
		else
			error_("Try using 'rl' instead of 'sl'");
	}

	/* write out coordinate system */
#if BYTEREVERSED
	ChgEndianSys(&system);
#endif /* BYTEREVERSED */
	if(whichfil)
		filid = openglobal(O_WRONLY | O_APPEND);
	else
		filid = openlocal(O_WRONLY | O_APPEND);
	if(filid == -1)
		error_("Can't store in file");
	if(write(filid,(char *)&system,sizeof(RelCoordSys)) !=
		sizeof(RelCoordSys))
		error("Error writing %s", system.name);
	SafeClose(filid);
}

/******************************************************/
/* replacesys - replace a coordinate system in a file */
/******************************************************/

static void replacesys(filid,system)
	int filid;		/* file identifier of file to change */
	RelCoordSys system;	/* system to change */
{
	RelCoordSys tsys;	/* temporary coordinate system */
	int curpos;		/* current position in file */

	/* if filid is bad, error */
	if(filid == -1)
		error_("Can't replace coordinate system\n");

	/* find coordinate system */
	while(read(filid,(char *)&tsys,sizeof(RelCoordSys)))
	{
		if(!strcmp(system.name,tsys.name))
		{
#if BYTEREVERSED
			ChgEndianSys(&system);
#endif /* BYTEREVERSED */
			/* replace system */
			curpos = lseek(filid,0L,1);
			(void)lseek(filid,(off_t)curpos - sizeof(RelCoordSys),0);
			if(write(filid,(char *)&system,sizeof(RelCoordSys)) !=
				sizeof(RelCoordSys))
				error("Error replacing %s", system.name);
			SafeClose(filid);
			return;
		}
	}

	/* if coordinate system not found, error */
	SafeClose(filid);
	error_("No such coordinate system to replace - try 'sl' or 'sg'");
}

/******************************************************/
/* removesys - remove a coordinate system from a file */
/******************************************************/

static void removesys(filid,system)
	int filid;		/* file identifier of file to change */
	RelCoordSys system;	/* system to remove */
{
	RelCoordSys tsys;	/* temporary coordinate system */
	int curpos;		/* current position in file */
	int epos;		/* ending position of file */

	/* if filid is bad, error */
	if(filid == -1)
		error_("Can't remove coordinate system\n");

	/* find end of file */
	epos = lseek(filid,0L,2);
	(void)lseek(filid,0L,0);

	/* find coordinate system */
	while(read(filid,(char *)&tsys,sizeof(RelCoordSys)))
	{
		if(!strcmp(system.name,tsys.name))
		{

			/* remove system */
			curpos = lseek(filid,0L,1);
			(void)lseek(filid,(off_t)epos - sizeof(RelCoordSys),0);
			if(read(filid,(char *)&tsys,sizeof(RelCoordSys)) !=
				sizeof(RelCoordSys))
				error("Error reading to remove %s",
					system.name);
			(void)lseek(filid,(off_t)curpos - sizeof(RelCoordSys),0);
			if(write(filid,(char *)&tsys,sizeof(RelCoordSys)) !=
				sizeof(RelCoordSys))
				error("Error writing to remove %s",
					system.name);
			(void)ftruncate(filid,epos - sizeof(RelCoordSys));
			SafeClose(filid);
			return;
		}
	}

	/* if coordinate system not found, error */
	SafeClose(filid);
	error_("No such coordinate system to remove");
}

#if BYTEREVERSED
/*
 * ChgEndisnSys Change the byte order of a RelCoordSys structure
 */

# include "../../misc/C.h"

static void ChgEndianSys(system)
	RelCoordSys *system;	/* system to change byte order of */
{
	decdouble(&system->x, 2);	/* convert x and y */
	decfloat(&system->cepoch, 2);	/* convert cepoch and oepoch */
	decdouble(&system->rot, 1);
}
#endif /* BYTEREVERSED */

