# include "../../stacks/C.h"
# include "../../coordsys/C.h"
# include "../../stacks/include.h"
# include "../../main/C.h"
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/types.h>
# include <math.h>
# define STKUSELEN sizeof(stk_[0].kuse)
# define USELEN sizeof(use_.nuse)

#define bcopy(s, d, n) memcpy(d, s, n)
#define bzero(s, n) memset(s, 0, n)

extern void rdmac();
void CloseDir(), OpenDir();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* rtst.c */
static void FixnWrite P_((register struct of_stk *stk, register int needswrite));
#undef P_

void rt()
{
# include "rt.dc.h"
	register  int sn;
	register int n;
	XYPOINT p1;
	char *errStr;

# include "rt.ec.h"

	/* set directory number */
	curDir = (*dt)?1:0;

	/* open directory if not open already */
	if(!*ff) {
		OpenDir(sf,0,0,0);
		errStr=TestByteOrder(1);
		if(strcmp(errStr,"OK")!=0)
			warn(errStr);
	}

	/* check for errors */
	if(*dest < 1 || *dest >= FRSTONDISK)
		error_("Illegal destination");
	if(*sn2 < *sn1)
		error_("First stack above last");

	if(*c1fl && *c2fl && *ptfl && *frfl && *skfl && !*msfl) {
	    /* retrieve stacks */
	    for(sn = *sn1;sn <= *sn2;sn++)
	    {
		sn1_o = sn2_o = sn;

		if(StkIsFull(sn)) {
			if(*tflg)
				main_.test = 1;
			getStack(sn, (int)*dest);
			if(*dest == 1)
				main_.pltd = 0;
			else
				break;
		} else {
			if(*tflg) {
				main_.test = 0;
			} else {
				error("No data in stack %d",sn);
			}
		}
	
		/* retrieve use array */
		if(*dest == 1) {
		    if(*su) {
			if(stk_[0].numpt > 512)
			    warn_("Stacks only have 512 chans of use array stored");
			bcopy(stk_[0].kuse,use_.nuse,STKUSELEN);
		    } else
			bcopy(use_.muse,use_.nuse,(31 + stk_[0].numpt) >> 3 &
				~3);
		}
		if(sn != *sn2)
		{
			/* plot */
			main_.pltd = 0;
			combex_("pl");
	
			/* wait */
			wait_();
		}
	    }
	} else {
		XYZPOINT center;

		CDirErr();
		if(!*frfl)
			OkFreqs(*fr1,*fr2);
		if(!*skfl)
			OkStacks((int)*stk1,(int)*stk2);
		if(*msfl) {
			if((n = *msn - 1) > 2 || n < 0)
				error_("Can only match core stacks");
			if(!coreFull[n])
				error_("Can't match an empty stack");
			center = RaDecToXyz(stk_[n].ra + stk_[n].dra,
				stk_[n].dec + stk_[n].ddec,
				(double)stk_[n].epoch);
			p1 = XyzToCsys(center); 
		} else {
			p1.x = *c1;
			p1.y = *c2;
			center = CsysToXyz(p1);
		}
		if(!*cmdfl) {
			SetStkInterp(DUMBINTERP, *ptol, 0.0, 0.0);
			if( !ExecuteAtPoint(p1.x, p1.y, cmd)) 
				error("No stacks near %g, %g\n", p1.x, p1.y);
			return;
		}
		sn = FindNear(center, *ptol);
		if(sn && StkIsFull(sn)) {
			if(*tflg)
				main_.test = 1;
			getStack(sn, (int)*dest);
			if(*dest == 1)
				main_.pltd = 0;
			else
				return;
		} else {
			if(*tflg) {
				main_.test = 0;
			} else {
				error("No stack found at %f, %f",*c1,*c2);
			}
		}

		/* retrieve use array */
		if(*dest == 1) {
		    if(*su) {
			if(stk_[0].numpt > 512)
			    warn_("Stacks only have 512 chans of use array stored");
			bcopy(stk_[0].kuse,use_.nuse,STKUSELEN);
		    } else
			bcopy(use_.muse,use_.nuse,(31 + stk_[0].numpt) >> 3 &
				~3);
		}
	}
#include "rt.uc.h"
}

void st()
{
	char *errStr;
# include "st.dc.h"
# include "st.ec.h"

	/* set directory number */
	curDir = (*dt)?1:0;

	/* open directory if not open already */
	if(!*ff) {
		OpenDir(sf,1,0,0);
		errStr=TestByteOrder(1);
		if(strcmp(errStr,"OK")!=0)
			warn(errStr);
	}

	/* set automatic stack # */
	if( *autof ) {
		*sn = NextStk((int)*frfre,EMPTY);
		printf("Stored in stack %d\n",*sn);
	/* or set default stack number */
	} else if(*nf) {
		*sn = stk_[0].numst;
	}

	/* check for errors */
	StkNumErr((int)*sn,"");
	StkNumErr((int)*src,"");
	if(*src >= FRSTONDISK)
		error_("Can only store from core stacks");
	if(!coreFull[*src - 1])
		error("Stack %d is empty",*src);

	/* record nuse */
	bcopy(use_.nuse,stk_[0].kuse,STKUSELEN);

	/* store stack */
	if(*sn >= FRSTONDISK) {
		if(stkReadOnly[curDir])
			error("Can't store in read only stacks file");
		if(StkIsFull((int)*sn)) {
			struct of_stk tstk;
			if(!*dc)
				warn("%sground directory stack %d contains data", curDir==0?"Fore":"Back",*sn);
			RdData((int)*sn,&tstk);
			noint_();
			SearchFlush(0);
			RmSearch(&tstk);
		} else {
			noint_();
			SearchFlush(0);
		}
		ClrStkValue((int)*sn);
		WrData((int)*sn,&stk_[*src - 1]);
		WrSearch(&stk_[*src - 1]);
		yesint_();
	} else {
		getStack((int)*src,(int)*sn);
		if(stk_[*sn - 1].numst < FRSTONDISK)
			stk_[*sn - 1].numst = *sn;
	}
#include "st.uc.h"
}

void ns()
{
	int saveDir;
	char *errStr;
# include "ns.dc.h"
# include "ns.ec.h"
	if(*prn) {
		saveDir = curDir;
		if(! notOpen[0]) {
			curDir = FOREGROUND;
			printf("Foreground - %s, last stk = %d, next = %d\n",
				dirName[0],NumInds(),NextStk(10,EMPTY));
		}
		if(! notOpen[1]) {
			curDir = BACKGROUND;
			printf("Background - %s, last stk = %d, next = %d\n",
				dirName[1],NumInds(),NextStk(10,EMPTY));
		}
		curDir = saveDir ;
	} else if(*psh) {
		if( !notOpen[FOREGROUND] ){
			fprintf(stderr,
				"Header info from foreground search file\n");
			SearchHdrPrint(FOREGROUND);
		}
		if( !notOpen[BACKGROUND] ){
			fprintf(stderr,
				"Header info from background search file\n");
			SearchHdrPrint(BACKGROUND);
		}
	} else if(*clf) { /* mwp 5/92 */
		curDir = (*dt)?1:0;
		CDirErr();
		noint_();
		CloseDir();
		yesint_();
	} else {
		if(!*fs)
			error_("No directory");
		curDir = (*dt)?1:0;
		OpenDir(fs,1,*qf,*tf);
		errStr=TestByteOrder(1);
		if(strcmp(errStr,"OK")!=0)
			warn(errStr);
	}
#include "ns.uc.h"
}

void em()
{
	struct of_stk stk;
	register int sn;
	static char es[] = "No data in stack %d";
# include "em.dc.h"
# include "em.ec.h"
	curDir = (*dt)?1:0;
	for(sn = *istak1;sn <= *istak2;sn++) {
	    StkNumErr(sn,"");
	    if(*unem) {
		if(sn < FRSTONDISK) {
			if(stk_[sn - 1].nlist)
				coreFull[sn - 1] = 1;
			else
				error(es,sn);
		} else {
			CDirErr();
			noint_();
			UnEmData(sn);
			RdData(sn,&stk);
			WrSearch(&stk);
			yesint_();
		}
	    } else {
		if(sn < FRSTONDISK) {
			bzero(&stk_[sn - 1],sizeof(struct of_stk));
			coreFull[sn - 1] = 0;
		} else {
		    CDirErr();
		    if(ChckData(sn)) {
			RdData(sn,&stk);
			noint_();
			RmSearch(&stk);
			EmData(sn);
			yesint_();
		    }
		}
	    }
	}
#include "em.uc.h"
}

static int stkcount;
void up()
{
# include "up.dc.h"
	int cstk,laststk, exists;
	register int needsWrite;
	char name[MAXDNLEN];
	char dirPath[MAXDNLEN];
	char *errStr;
	struct of_stk stk;
# include "up.ec.h"

	/* default path may be found in main_.stkpth */
	if(*drc != '/' && *drc != '.' && *main_.stkpth != 0) {
		sprintf(dirPath,"%s/%s",main_.stkpth,drc);
		strcpy(drc,dirPath);
	}

	curDir = (*dt)?1:0;
	stkcount = 0;

	if((access(drc, X_OK || W_OK)) < 0) {
		error("Can't open %s",drc);
	}

	notOpen[curDir]++;
	OpenDir(drc,0,0,0);
	if ( *tbo ) { /* test the byte order only and return */
		printf("%s.\n",TestByteOrder(0));
		return;
	}

	if ( *swap ) {
                errStr=TestByteOrder(0);
	} else {
                errStr=TestByteOrder(1);
	}
        if(strcmp(errStr,"OK")!=0)
                warn(errStr);

	/* remove the old search file or quit if it is readonly */
	sprintf(name,"%s/search",drc);
	if(access(name, W_OK) == 0) {
		/*unlink(name);*/ /* NO! this causes the file to disappear if
				   * up on a normal search file */
	} else { 
		if(access(name,F_OK) == 0) 
		    error("Trying to 'up' a read-only search file");
	}

	if(stkReadOnly[curDir]) {
	error("Quit comb, make data and index writable (664), and run up again");
	}
	laststk = NumInds();
	printf("Last stack number in file is %d\n",laststk);
	SearchFlush(0);
	for(cstk = FRSTONDISK;cstk <= laststk;cstk++)
	{
		if(*fvax) {
			needsWrite = 7;
			exists = RdVaxData(cstk, &stk);
                } else if(*swap) {
                        needsWrite = 7;
                        exists = CvtBEndianToLEndian(cstk, &stk);
		} else {
			needsWrite = 0;
			exists = RdData(cstk, &stk);
		}
		if(exists) {
			/* semi-kludge fix-up of bad staks - aas 26 Sept 87 */
			if( *fix && (stk.numpt<250 || stk.numpt>256) ) { 
				printf("Bad stack %d zeroed out\n",stk.numst);
				noint_();
				/* RmSearch(&stk);  */
				EmData(cstk);
				yesint_();
				continue;
				/*XXXX*/ 
			}
			if(!stk.numst) {
				needsWrite++;
				stk.numst = stk.onstk;
			}
			if(stk.numst != cstk) {
				printf("Stack # %d is in %d\n",stk.numst,cstk);
				stk.numst = cstk;
				needsWrite++;
			}
			if(*fhp) {
				stk.odra = stk.dra*cos((stk.dec + stk.ddec)
					*M_PI/180.)*900.0;
/* This calculation is not very accurate, so round to nearest 0.2 arcmin */
				stk.odra = 0.2 * round(stk.odra *5.0);
				needsWrite++;
			}
			FixnWrite(&stk,needsWrite);
		}
	}
	SearchFlush(1);
	printf("\n%d Stacks in file\n",stkcount);
#include "up.uc.h"
}

static void FixnWrite(stk,needswrite)
register struct of_stk *stk;
register int needswrite;
{
	if((stkcount++ %10) == 9)
	{
		printf("%6d",stk->numst);
		if(stkcount % 100 == 0)
			printf("\n");
	}
	if(stk->rms <= 0.) {
	    needswrite++;
	    if(stk->nprc < 1000) {
		if(stk->wght[0] <= 0.)
			stk->wght[0] = 1000.;
		stk->rms = 2.0/sqrt(stk->fwid * stk->wght[0]);
	    } else {
/* make the assumption that the full nbe was used in continuum observations.*/
		stk->rms = 2.0/sqrt(64 * stk->wght[0]);
	    }
	}

	if(stk->epoch <= 100) {
		needswrite++;
		stk->epoch = 1950.0;
	}
	noint_();
	if(needswrite)
		WrData(stk->numst,stk);
	WrSearch(stk);
	yesint_();
}

void OpenDir(fn,create,quiet,test) /* Assume that curDir is set to fgnd or bkgnd */
char *fn;			/* Pointer to new stacks directory name */
int create;	  /* Allow creation of a new directory? */
int quiet, test;  /* 04/04/1996 -mh. ask when creating? set test ? */
{
	char ts[MAXDNLEN + 11],rccmd[MAXDNLEN + 20];
	char *cp,*getenv();

	/* default path may be found in main_.stkpth */
	if(*fn != '/' && *fn != '.' && *main_.stkpth != 0) {
		sprintf(ts,"%s/%s",main_.stkpth,fn);
		cp = ts;
	} else {
		cp = fn;
	}

	if(!notOpen[curDir])		/* Some directory is open */
		if(!strcmp(dirName[curDir],cp))
			return;
	strcpy(dirName[curDir],cp);

	notOpen[curDir] = 1;
	stkReadOnly[curDir] = 0;
	if(access(dirName[curDir],0)) {	/* Does it exist? */
	    if (test) {
		main_.test=0;
	    } else {
		if(create) {
		    if (quiet) {
			printf("Creating stacks Directory %s\n",dirName[curDir]);
		    } else {
			warn("Creating stacks Directory %s",dirName[curDir]);
		    }
		    sprintf(ts,"/bin/mkdir %s",dirName[curDir]);
		    noint_();
		    eshell_(ts);
		    CreateData();
		    yesint_();
		} else {
		    error("No stacks directory %s",fn);
		}
	    }
	} else { /* does exist */
	    if (test) {
		main_.test=1;
	    }
	}
	if ( !test || main_.test ) { 
	    /* if we are not testing, the dir must be open.
	       if we are, open only when .test is true */
	    OpenData();
	    CreateDefault();
	    OpenDefault();
	    CreateSearch();
	    OpenSearch();
	    InitValues();
	    notOpen[curDir] = 0;
	    rdmac();
	    if(curDir == 0) {
		/* Get the default coordinate system if one exists */
		if(GtStkDflt("RCSys","%s",ts,0)) {
		    sprintf(rccmd,"rc \"%s\" read:",ts);
			combex_(rccmd);
		}
	    }
	}
}
void CloseDir()
{
	CloseData(); /* close data and index */
	CloseSearch();
	CloseDefault();

/* don't need to close macros file because they are never opened, just
 * read into a buffer via rdmac() */

	notOpen[curDir] = 1;
	/* MWP - I think this causes problems 
        curDir = (curDir)?0:1; *//* set to the other directory */
}
