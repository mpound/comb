/***************************************/
/* Routines for manipulating data file */
/***************************************/

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include "../error/C.h"
# include "C.h"
# include "../coordsys/C.h"
# include "include.h"
# include "../main/C.h"
# include "../main/machinedep.h"
# include "../main/dragon.h"

#if SYSTEM_V
#define bcopy(s, d, n) memmove(d, s, n)
#define bzero(s, n) memset(s, 0, n)
#endif /*SYSTEM_V*/

/* size of stack header */
# define HDSIZ sizeof(stk_[0]) - (sizeof(stk_[0].stak) + sizeof(stk_[0].nstkx))

static struct of_indx			/* index buffer */
{
	unsigned int loc;			/* location of stack in file */
	unsigned short length;			/* length of stack */
						/* if length is 0, then this */
						/* index does not represent */
						/* any space that has been */
						/* allocated */
	char full;				/* 1 if stack is full */
	char unem;				/* 1 if stack recoverable */
}
indxBuf[2][BLKSIZE / sizeof(struct of_indx)];

/* The following variables are true stack numbers.  The indx for stack n will be
at location (n - 1) * sizeof(struct of_indx) in the file.  It will be in
indxBuf[curDir][n - lowInBuf} if it is in the current block ie. if
n >=lowInBuf[curDir] && n <= highInBuf[curDir].
	The buffer for each open file will always appear to be full.
That is highInBuf[curDir] will = lowInBuf [curDir]
+ BLKSIZE/sizeof(struct of_indx).  Any buffer beyond the end of the indx
file will be filled with zeros.  The true end is in lastIndx[curDir] */

static int lowInBuf[2];			/* low stack number in buffer */
static int highInBuf[2];		/* high stack number in buffer */
static int lastIndx[2];			/* last indices in indx files */

static int firstFreeStk[2];		/* Info for NextStk */
static int firstSearch[2] = {0,0};	/* start of search for firstFreeStk */

# define NUMSPDIV ((MAXCHN >> 6) + 2)
static int firstFreeSpace[2][NUMSPDIV];
					/* firstFreeSpace[curDir][n] is a stack
					number at which to to start looking for
					an indx containing an empty block with
					>= (n + 2)<<8 free bytes */
static int dataFile[2] = {-1,-1};	/* data file pointer */
static int indxFile[2] = {-1,-1};	/* index file pointer */

/* Variables for a list of stacks - initially for the unique command.
 * As currently written only one list is provided. */
static char *stkList = 0;		/* Points to the 1 byte/stack list */
static int stkListFirst = 0;		/* number of first stack in list */
static int stkListLast = 0;
static int stkListNext;			/* Next stack to consider in list */

extern off_t lseek();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* data.c */
static int ChkStack(int sn, int swap, int dataFileSize);
static struct of_indx *GtStkIndx P_((int stkNum));
static void GtRoom P_((int length, register struct of_indx *indx));
static void WrIndx P_((register int stkNum, struct of_indx *indx));
static void RdBlkFor P_((int stkNum));
static void WrIndxBuf P_((void));
#undef P_

/**************************************************
 * mrkful_ Fortran entry to mark a core stack full
 *************************************************/

void mrkful_(sn)
int *sn;
{
	register int stkNum = *sn;

	if(stkNum < 1 || stkNum >=FRSTONDISK)
		error_("mrkful: Bad stk num");
	coreFull[ --stkNum] = 1;
}


/***********************************/
/* CreateData - create a data file */
/***********************************/

void CreateData()
{
	char dataName[MAXDNLEN + 5];	/* name of data file */
	char indxName[MAXDNLEN + 6];	/* name of index file */
	int ifd;			/* file descriptor fir index file */
	struct of_indx zindx[3];	/* three zero indices to initialize
					the new indx file */

	/* make names of files */
	sprintf(dataName,"%s/data",dirName[curDir]);
	sprintf(indxName,"%s/index",dirName[curDir]);

	/* If they alerady exist, return */
	if(access(dataName, F_OK) == 0)
		return;

	/* create files */
	close(creat(dataName,0666));
	ifd = creat(indxName,0666);
	bzero((char *)zindx,sizeof(zindx));
	if(write(ifd,zindx,sizeof(zindx)) != sizeof(zindx))
		error("Can't write new index file");
	close(ifd);
}

/**************************************************************/
/* OpenData - open the data file and read in the index buffer */
/**************************************************************/

void OpenData()
{
	char dataName[MAXDNLEN + 5];	/* name of data file */
	char indxName[MAXDNLEN + 6];	/* name of index file */
	char errStr[MAXDNLEN + 30];	/* error string */
	register int * ffsp;

	/* construct names of files */
	sprintf(dataName,"%s/data",dirName[curDir]);
	sprintf(indxName,"%s/index",dirName[curDir]);

	/* close old files if they are open */
	if(dataFile[curDir] != -1)
		close(dataFile[curDir]);
	if(indxFile[curDir] != -1)
		close(indxFile[curDir]);

	/* open new files */
	sprintf(errStr,"the data file for stack directory %s",dirName[curDir]);
	if(access(dataName, R_OK) || access(indxName, R_OK)) {
		*dirName[curDir] = '\0';
		error("Can't open data or index file");
	}
	if(stkReadOnly[curDir] || access(dataName, W_OK)) {
		dataFile[curDir] = ChkOpen(errStr,dataName,O_RDONLY, 0666);
		stkReadOnly[curDir] = 1;
	} else {
		dataFile[curDir] = ChkOpen(errStr,dataName,O_RDWR, 0666);
	}
	sprintf(errStr,
	    "the index file for stack directory %s",dirName[curDir]);
	if(stkReadOnly[curDir] || access(indxName, W_OK)) {
		indxFile[curDir] = ChkOpen(errStr,indxName,O_RDONLY, 0666);
		stkReadOnly[curDir] = 1;
	} else {
		indxFile[curDir] = ChkOpen(errStr,indxName,O_RDWR, 0666);
	}
	lastIndx[curDir] = NumInds();
	for(ffsp = firstFreeSpace[curDir];
		ffsp < &firstFreeSpace[curDir][NUMSPDIV]; ffsp++)
		*ffsp = FRSTONDISK;
	firstFreeStk[curDir] = FRSTONDISK;
	firstSearch[curDir] = FRSTONDISK;
	RdBlkFor(1);
}

/**************************************************************/
/* CloseData - close the data and index files -- mwp 5/92     */
/* bug fix - 4/26/93 mwp removed two offending sprintf's      */
/**************************************************************/

void CloseData()
{
        /* close old files */
        close(dataFile[curDir]);
        close(indxFile[curDir]);
	dataFile[curDir] = -1;
	indxFile[curDir] = -1;
}
/****************************************/
/* NextStk - find the next stack number */
/****************************************/

int NextStk(stkNum,find)
	register int stkNum;		/* stack number at which to start */
					/* looking */
	int find;			/* type of stack to find */
					/* (FULL or EMPTY) */
{
	register int numStks;		/* number of stacks in directory */

	/* check that there is an open directory */
	CDirErr();

	/* find next stack */
	if(stkNum < FRSTONDISK)
		stkNum = FRSTONDISK;
	numStks = lastIndx[curDir];
	switch(find)
	{
		case FULL:
			/* check parameters */
			StkNumErr(stkNum,"Looking for stack after");
			StkDskWrn(stkNum,
			    "Stacks in core ignored when finding next stack",0);
			while(stkNum <= numStks)
				if(ChckData(stkNum++))
					return(stkNum - 1);
			/* indicate no full stacks */
			return(0);
		case EMPTY:
			if(stkNum >= firstSearch[curDir]) {
				if(stkNum != firstSearch[curDir])
					firstSearch[curDir] = stkNum;
				if(stkNum < firstFreeStk[curDir])
					stkNum = firstFreeStk[curDir];
			} else {
				if(stkNum < FRSTONDISK)
					stkNum = FRSTONDISK;
				firstSearch[curDir] = stkNum;
			}
			for(;stkNum <= numStks;stkNum++)
				if(!ChckData(stkNum))
					break;
			/* if no empty stack, stkNum is one beyond the last */
			firstFreeStk[curDir] = stkNum;
			return(stkNum);
		default:
			error_("Illegal call to NextStk");
	}
	/*NOTREACHED*/
}

/****************************************************/
/* StkIsFull - check to see if a given stack exists */
/****************************************************/
/* returns 1 if exists, 0 if not */
int StkIsFull(stkNum)
	int stkNum;			/* stack number to check */
{
	/* check for errors */
	StkNumErr(stkNum,"Checking existance of");

	/* if stack is in core, check there, otherwise check on disk */
	if(stkNum < FRSTONDISK)
		return(coreFull[stkNum - 1]);
	else {
		CDirErr();
		return(ChckData(stkNum));
	}
}

/* FORTRAN callable version */

int stfull_(n)
int *n;
{
	return(StkIsFull(*n ));
}

/*****************************************************/
/* NumInds - return number of indexes = last stack # */
/*****************************************************/

int NumInds()
{
	/* number of indexes is determined by size of index file */
	return(lseek(indxFile[curDir],0l,2) / sizeof(struct of_indx));
}

/****************************************************/
/* ChckData - check whether data exists for a stack */
/****************************************************/

/* returns 1 if data exists, else 0 */
int ChckData(stkNum)
	int stkNum;			/* stack number being checked */
{
	/* return flag for whether stack is full */
	return(GtStkIndx(stkNum)->full);
}

/**********************************************************/
/* EmData - set flags so that a stack is considered empty */
/**********************************************************/

void EmData(stkNum)
	int stkNum;			/* number of stack to be emptied */
{
	struct of_indx *indx;
	int sizenum;

	/* set flags */
	indx = GtStkIndx(stkNum);
	indx->full = 0;

	/* write out new index structure */
	WrIndxBuf();

	/* update first free stack if needed */
	sizenum = indx->length >> 8;
	if(stkNum < firstFreeSpace[curDir][sizenum - 2])
		firstFreeSpace[curDir][sizenum - 2] = stkNum;
	if(stkNum < firstFreeStk[curDir])
		firstFreeStk[curDir] = stkNum;
}
/***************************************/
/* UnEmData - recover data if possible */
/***************************************/

void UnEmData(stkNum)
	int stkNum;			/* stack to be recovered */
{
	register struct of_indx *indx;	/* index of stack */

	/* check that data is recoverable */
	indx = GtStkIndx(stkNum);
	if(!indx->unem)
	{
		error("Sorry - too late to restore stack %d", stkNum);
	}

	/* recover data */
	indx->full = 1;
	WrIndxBuf();
}

/*********************************************/
/* WrData - write the data for a given stack */
/*********************************************/

void WrData(stkNum,stk)
	int stkNum;			/* stack number */
	register struct of_stk *stk;	/* pointer to data */
{
	int length;			/* total length of stack */
	int len;			/* length of stack hdr + data */
	struct of_indx indx;		/* index of stack */

	if(stkReadOnly[curDir])
		error("Trying to write read only stack");

	/* compute the length of the data */
	length = len = HDSIZ + stk->numpt * sizeof(float);
	if(stk->indx > 32)
		length += (stk->indx - 32) * sizeof(short);

	/* if data is too big for old space, find new */
	indx = *GtStkIndx(stkNum);
	if(indx.length < length)
		GtRoom(length,&indx);

	/* set index flags */
	indx.full = 1;
	indx.unem = 1;

	/* write out data */
	lseek(dataFile[curDir],indx.loc,0);
	noint_();
	stk->numst = stkNum;
	if(write(dataFile[curDir],stk,len) != len)
		error("Error writing data");
	if(stk->indx > 32)
		if(write(dataFile[curDir],stk->nstkx,length - len) !=
				length - len)
			error("Error writing data");
	WrIndx(stkNum,&indx);
	yesint_();
}

/**********************************************************
 * gstak FORTRAN callable routine to fill stacks 1 and 2 **
 *********************************************************/
void gstak_(s1,s2)
register int *s1,*s2;
{
	register int t;
	if(*s2 == 1) {
		if(*s1 == 2) {
			exchq(&stk_[0],&stk_[1],sizeof(struct of_stk)/8);
			t = coreFull[0];
			coreFull[0] = coreFull[1];
			coreFull[1] = t;
		} else {
			getStack(1,2);
			getStack(*s1,1);
		}
	} else {
		getStack(*s1,1);
		getStack(*s2,2);
	}
}
/***********************************************************
 * getStack - get a stack whether it is on disk or in core *
 **********************************************************/

void getStack(int src,int dest)
{
	int nDragon;
	dest--;
	if(src < FRSTONDISK) {
		src--;
		stk_[dest] = stk_[src];
		coreFull[dest] = coreFull[src];
	} else {
		CDirErr();
		coreFull[dest] = RdData(src,&stk_[dest]);
	}
	if(main_.drgmes!=0) {
	if( (nDragon=ChkDrag(dest)) > 0)
	  switch(main_.drgmes) {
	  case 1:
		/* no message */
		break;
	  case 2:
		printf(" Found and eliminated %d BLANK%s in (the memory copy of) stack %d\n",nDragon,(nDragon==1)?"":"s",(src < FRSTONDISK)?src+1:src);
		printf(" Use 'op msg:' to eliminate this message.\n");
		break;
	 }
	}
}

/***************************************************************
 * ChkDrag - check to see if a stack has any channels = DRAGON *
 *           returns number of DRAGON(s) present         --mwp *
 ***************************************************************/
int ChkDrag(int stkNum)
{
	int i, val, j;

	val=0; /* default return value is clean */

	if(stkNum>=FRSTONDISK)
	  error("Improper call to ChkDrag: Can't remove BLANKs from disk stacks");

	for(i=0;i<stk_[stkNum].numpt;i++) {
		if(stk_[stkNum].stak[i]==FDRAGON) {
			j=i;
			i=ClrDrag(stkNum,i); 
	/*
	 * i has now been stepped up past any adjacent DRAGONs in this
	 * stack. It's value is now the channel number of the most
	 * recently encountered DRAGON 
   	 */
			val+=((i-j)+1);
		}
	}
	return(val);
}
/***************************************************************
 * ClrDrag()  - Clear DRAGON(s) from a stack by calling elim() *
 *              starting from the given channel.         --mwp *
 ***************************************************************/
int ClrDrag(int stkNum,int startch)
{
	int endch;
	
/*
 * If  the channel value is DRAGON, eliminate it, using "el."  
 * But first check for any adjacent DRAGON(s).
 * Note that adjacent DRAGONs are taken care of when the first one 
 * is encountered
 */

	for(endch=startch;endch<stk_[stkNum].numpt;endch++) {
		if (stk_[stkNum].stak[endch]!=DRAGON)
                  break;
	}
/*
 * elim_(start,end,stack) was originally fortran, so it operates on elements
 * start-1  to end-1 of stk_[stack].
 */
	startch++;
	elim_(startch,endch,stkNum);
		 
	return(endch-1); /* return last channel that had a dragon */
}

/********************************************/
/* RdData - read the data for a given stack */
/********************************************/
/* returns 1 if stack exists and was read */
int RdData(stkNum,stk)
	int stkNum;			/* number of stack to be read */
	register struct of_stk *stk;	/* location in which to read it */
{
	register struct of_indx *indx;	/* index structure of stack */

	/* get stack index */
	indx = GtStkIndx(stkNum);

	/* read in the data */
	if(indx->full) {
		lseek(dataFile[curDir],indx->loc,0);
		read(dataFile[curDir],stk,HDSIZ);
		if(stk->numpt > MAXCHN || stk->numpt < 1) {
			printf("Ridiculous numpt = %d in stk %d\n",stk->numpt,
				stkNum);
			stk->numpt = 512;
		}
		read(dataFile[curDir],stk->stak,stk->numpt * sizeof(float));
		if(stk->indx > 32) {
			if(stk->indx > 159) {
				printf("Bad scanlist index in stack %d\n",stkNum);
				stk->indx = 159;
			}
			read(dataFile[curDir],stk->nstkx,
				(stk->indx - 32) * sizeof(short));
		}
		return(1);
	} else {
		return(0);
	}
}

/**********************************************************
 * RdVaxData - read the data for a stack written on a VAX *
 **********************************************************/
/* returns 1 if stack exists and was read */
int RdVaxData(stkNum,stk)
	int stkNum;			/* number of stack to be read */
	register struct of_stk *stk;	/* location in which to read it */
{
	register struct of_indx *indx;	/* index structure of stack */
	register short *sp, *endp;	/* for the scan number list */

	/* get stack index */
	indx = GtStkIndx(stkNum);

	/* read in the data */
	if(indx->full) {
		/* fix up the index */
		vaxlong(&indx->loc, 1);
		vaxshort(&indx->length, 1);

		lseek(dataFile[curDir],indx->loc,0);
		read(dataFile[curDir],stk,indx->length);

		/* fix up the stack */
		vaxshort(&stk->numpt,22);
		vaxlong((int *)stk->kuse, 16);
		vaxshort(&stk->nlist, 2);
		vaxlong(&stk->numst, 1);
		vaxfloat(&stk->fwid,50);
		vaxdouble(&stk->freq, 1);
		if(stk->numpt > MAXCHN || stk->numpt < 1) {
			printf("Ridiculous numpt = %d in stk %d\n",stk->numpt,
				stkNum);
			stk->numpt = 512;
		}
		vaxfloat(stk->stak, (int)stk->numpt);
		if(stk->indx > 32) {
			if(stk->indx > 159) {
			    printf("Bad scanlist index in stack %d\n",stkNum);
				stk->indx = 159;
			}
			bcopy((char *)&stk->stak[stk->numpt],
				(char *)stk->nstkx,
				(stk->indx - 32) * sizeof(short));
		}

		sp = stk->nstk;
		endp = (stk->indx < 32)? &stk->nstk[stk->indx]:
			&stk->nstkx[stk->indx - 32];
		while(sp < endp) {
			vaxshort(sp, 1);
			/* if scan #, skip next two chars */
			if(*sp < 0 && ++sp == &stk->nstk[32])
				sp = stk->nstkx;
			if(++sp == &stk->nstk[32])
				sp = stk->nstkx;
		}

		return(1);
	} else {
		indx->loc = 0;
		indx->length = 0;
		indx->unem = 0;
		WrIndx(stkNum, indx);
		return(0);
	}
}

/*************************************************************************
 * CvtBEndianToLEndian - Reverse the byte order of a stack and its index *
 *************************************************************************/
/* returns 1 if stack exists and was read */
int CvtBEndianToLEndian(stkNum,stk)
	int stkNum;			/* number of stack to be read */
	register struct of_stk *stk;	/* location in which to read it */
{
	register struct of_indx *indx;	/* index structure of stack */
	register short *sp, *endp;	/* for the scan number list */

	/* get stack index */
	indx = GtStkIndx(stkNum);

	/* read in the data */
	if(indx->full) {
		/* fix up the index */
		declong(&indx->loc, 1);
		decshort(&indx->length, 1);

		lseek(dataFile[curDir],indx->loc,0);
		read(dataFile[curDir],stk,indx->length);

		/* fix up the stack */
		decshort(&stk->numpt,22);
		declong((int *)stk->kuse, 16);
		decshort(&stk->nlist, 2);
		declong(&stk->numst, 1);
		decfloat(&stk->fwid,50);
		decdouble(&stk->freq, 1);
		if(stk->numpt > MAXCHN || stk->numpt < 1) {
			printf("Ridiculous numpt = %d in stk %d\n",stk->numpt,
				stkNum);
			stk->numpt = 512;
		}
		decfloat(stk->stak, (int)stk->numpt);
		if(stk->indx > 32) {
			if(stk->indx > 159) {
			    printf("Bad scanlist index in stack %d\n",stkNum);
				stk->indx = 159;
			}
			bcopy((char *)&stk->stak[stk->numpt],
				(char *)stk->nstkx,
				(stk->indx - 32) * sizeof(short));
		}

		sp = stk->nstk;
		endp = (stk->indx < 32)? &stk->nstk[stk->indx]:
			&stk->nstkx[stk->indx - 32];
		while(sp < endp) {
			decshort(sp, 1);
			/* if scan #, skip next two chars */
			if(*sp < 0 && ++sp == &stk->nstk[32])
				sp = stk->nstkx;
			if(++sp == &stk->nstk[32])
				sp = stk->nstkx;
		}

		return(1);
	} else {
		indx->loc = 0;
		indx->length = 0;
		indx->unem = 0;
		WrIndx(stkNum, indx);
		return(0);
	}
}

/* TestByteOrder - Check recently opened stacks data and index files
 * to determine whether the most likely byte order is different from that
 * expected.  Return a string with detail information about what was found.
 * "OK" is returned if no errors were found.
 */
char errStr[1024];
char *TestByteOrder(int expectCorrect) {
	int sn, dataFileSize;
	int unswErrors, swappedErrors, cnt;
	char ans[32];

	unswErrors = 0;
	swappedErrors = 0;
	dataFileSize = (int)lseek(dataFile[curDir],0l,2);
	for(sn = FRSTONDISK, cnt = 0; cnt < 10; sn++) {
	    sn = NextStk(sn,FULL);	/* find next full stack */
	    if(sn >= lastIndx[curDir] || sn < FRSTONDISK)
		break;
	    cnt++;
	    unswErrors += ChkStack(sn, 0, dataFileSize);
	    swappedErrors += ChkStack(sn, 1, dataFileSize);
	}
	if(expectCorrect && unswErrors) {
	    if(swappedErrors==0) 
	    sprintf(errStr,
	       "\007\nThese stacks are probably in the wrong byte order "
		"and need 'up swap:' run on them.\n"
		"I made %d tests on the first %d stacks of %s\n"
		"and found %d errors which might be caused by the\n" 
		"stacks having the wrong byte order for your computer.\n"
		"If the byte order were reversed by running \n"
		"'up \"%s\" swap:' before trying 'ns',\n"
		"there would be %d errors in the same tests",
		3 * cnt, cnt, dirName[curDir],swappedErrors,
		dirName[curDir],unswErrors);
	} else if(!expectCorrect && swappedErrors) {
	    if(unswErrors==0) 
	    sprintf(errStr,
	        "\007\nThese stacks are probably already in the correct byte order\n"
		"and do not need to be swapped.\n"
		"I made %d tests on swapped-byte versions of the first"
		" %d stacks of\n %s\n"
		"and found %d errors which might be caused by the stacks\n"
		"already being in the correct byte order for your computer.\n"
		"There would be %d errors in the same tests "
		"if the byte order\nwere left as it is",
		3 * cnt, cnt, dirName[curDir], swappedErrors, unswErrors);
	} else {
	    sprintf(errStr,"OK");
	}
	return (errStr);
}

static int ChkStack(int sn, int swap, int dataFileSize) {
	int errCnt;
	struct of_indx indx;
	struct of_stk *sp;
	char buf[256];

	indx = *GtStkIndx(sn);
/*	fprintf(stderr,"filesize: %d: indx.loc %d indx.length %d \n",
		dataFileSize,indx.loc,indx.length);
*/
	if(swap) {
	    declong(&indx.loc, 1);
	    decshort(&indx.length, 1);
	}
	if(indx.loc + indx.length > dataFileSize ||
		indx.loc > indx.loc + indx.length || indx.length < 512) {
	    return(3);
	}
	errCnt = 0;
	sp = (struct of_stk *)buf;
	lseek(dataFile[curDir],indx.loc,0);
	if(read(dataFile[curDir], buf, sizeof(buf)) != sizeof(buf))
	    error("Error reading stack %d\n", sn);
	if(swap) {
	    decshort(&sp->nver, 1);
	    decshort(&sp->indx, 1);
	    declong(&sp->numst, 1);
	}
	if((unsigned short)sp->nver != 1) errCnt++;
	if((unsigned short)sp->indx > 169) errCnt++;
	if(sp->numst != sn) errCnt++;
/* fprintf(stderr,"got nver %d indx %d numst %d",sp->nver, sp->indx, sp->numst);
*/
	return(errCnt);
}

/*********************************************************/
/* GtStkIndx - get the index structure for a given stack */
/*********************************************************/

static struct of_indx *GtStkIndx(stkNum)
	int stkNum;			/* number of stack whose index is */
					/* to be gotten */
{
	/* make sure correct block is in buffer */
	if(stkNum < lowInBuf[curDir] || stkNum > highInBuf[curDir])
		RdBlkFor(stkNum);

	return(&indxBuf[curDir][stkNum - lowInBuf[curDir]]);
}

/****************************************************************/
/* GtRoom - find space in data file for a given number of bytes */
/****************************************************************/

static void GtRoom(length,indx)
	int length;			/* number of bytes */
	register struct of_indx *indx;	/* index which is to point to space */
{
	struct of_indx *ip;		/* pointer to indx during search */
	struct of_indx tempIndx;	/* temporary index structure */
	register int stkNum;		/* stack number of index being */
					/* checked */
	register int *ffsp;		/* Pointer to first free space number
					for this length */
	int sizenum;			/* length >> 8 */
	int freefound;			/* flag for when ffs has been found */

	/* If there might be empty indx's holding space this size,look for one*/
	ffsp = &firstFreeSpace[curDir][(sizenum = length >> 8) - 2];
	if((stkNum = *ffsp) <= lastIndx[curDir] ) {
	    freefound = 0;
	    if(stkNum < lowInBuf[curDir] || stkNum > highInBuf[curDir])
		RdBlkFor(stkNum = *ffsp);
	    for(ip = &indxBuf[curDir][stkNum - lowInBuf[curDir]];
		stkNum <= lastIndx[curDir];stkNum++,ip++) {
		if(stkNum > highInBuf[curDir]) {
		    RdBlkFor(stkNum);
		    ip = indxBuf[curDir];
		}
		if(!ip->full) {
		    if(!freefound && ip->length >> 8 == sizenum) {
			*ffsp = stkNum;
			freefound++;
		    }
		    if(ip->length >= length) {
			if(ip->length - length < 512) {
			    tempIndx = *indx;
			    *indx = *ip;
			    *ip = tempIndx;
			    if(!freefound)
				*ffsp = stkNum + 1;
			} else if(ip->length > length << 1) {
			    indx->loc = ip->loc;
			    indx->length = length;
			    ip->loc += length;
			    ip->length -= length;
			    if(!freefound)
				*ffsp = stkNum;
			} else {
			    continue;
			}
			ip->full = ip->unem = 0;
			WrIndxBuf();
			return;
		    }
		}
	    }
	/* If we get here, we have searched the whole file */
	    if(!freefound)
	/* There will be no more free space of this size till a stack is emptied */
		*ffsp = MAXSTKNUM;
	}
	/* We didn't find any suitable free space, so save the old space
	in a new indx at the end of the indx file and create new space 
	that is needed.  Update the appropriate first free space entry if needed */
	if(indx->length > 0) {
		stkNum = lastIndx[curDir];
		ffsp = &firstFreeSpace[curDir][(sizenum = indx->length >> 8) - 2];
		if(*ffsp > stkNum)
			*ffsp = stkNum;
		indx->full = indx->unem = 0;
		WrIndx(lastIndx[curDir] + 1,indx);
	}
	indx->loc = (int)lseek(dataFile[curDir],0l,2);
	indx->length = length;
}

/************************************/
/* WrIndx - write out a stack index */
/************************************/

static void WrIndx(stkNum,indx)
	register int stkNum;		/* stack whose index this is */
	struct of_indx *indx;		/* index to be written out */
{
	/* If the new stack is beyond the present end of the indx file, make sure
	that the intervening space is filled with zeros */
	while(stkNum > lastIndx[curDir]) {
		if(++lastIndx[curDir] > highInBuf[curDir] ||
			lastIndx[curDir] < lowInBuf[curDir])
			RdBlkFor(lastIndx[curDir]);
		if(stkNum > highInBuf[curDir]) {
			lastIndx[curDir] = highInBuf[curDir];
			WrIndxBuf();
		} else {
			lastIndx[curDir] = stkNum;
		}
	}
	if(stkNum < lowInBuf[curDir] ||stkNum > highInBuf[curDir])
		RdBlkFor(stkNum);
	indxBuf[curDir][stkNum - lowInBuf[curDir]] = *indx;
	WrIndxBuf();
}

/****************************************************************************/
/* RdBlkFor - read the block of indexes which contains a given stack number */
/****************************************************************************/

static void RdBlkFor(stkNum)
	int stkNum;			/* stack number */
{
	int blkNum;			/* number of block */
	int blkLoc;			/* location of block */
	register int len;		/* length of current block in bytes */

	/* find block to be read */
	blkNum = ((stkNum - 1) * sizeof(struct of_indx)) / BLKSIZE;
	blkLoc = blkNum * BLKSIZE;

	lowInBuf[curDir] = 1 + blkLoc / sizeof(struct of_indx);
	highInBuf[curDir] = lowInBuf[curDir] +
		BLKSIZE/sizeof(struct of_indx) - 1;
	if(lowInBuf[curDir] > lastIndx[curDir]) {	/* beyond end of file */
		bzero(indxBuf[curDir],BLKSIZE);
	} else {
		/* read block */
		lseek(indxFile[curDir],blkLoc,0);
		len = read(indxFile[curDir],indxBuf[curDir],BLKSIZE);
		if(len < BLKSIZE)
			bzero(((char *)indxBuf[curDir]) + len,BLKSIZE - len);
	}
}

/************************************************************************
 * WrIndxBuf - write the current indx buffer to disc to record a change *
 ************************************************************************/

static void WrIndxBuf()
{
	int blkLoc;			/* location of block */
	register int len;		/* length of current block in bytes */
	
	len = highInBuf[curDir];
	if(len > lastIndx[curDir])
		len = lastIndx[curDir];
	len = (1 + len - lowInBuf[curDir]) * sizeof(struct of_indx);
	blkLoc = (lowInBuf[curDir] - 1) * sizeof(struct of_indx);
	lseek(indxFile[curDir],blkLoc,0);
	if(write(indxFile[curDir],indxBuf[curDir],len) != len)
		error_("Trouble writing stack indx file");
}

/****************************************************************************
 * The following routines manage a list of stacks where you want to process
 * each stack only once, but not necessarily in order.  One byte per stack
 * is allocated for the list and pointed to by stkList.  MkStkList sets each
 * byte to 0 or 1 depending on the existance of the corresponding stack.
 * stkListFirst, stkListLast, and stkListNext are used to mannage the list
 * and a pass through it.  Only one list is provided, but it may be used
 * with either directory or confused between the two.
 ****************************************************************************/

void MkStkList(first, last)
register int first, last;
{
	register char *cp, *endp;
	struct of_indx *GetStkIndx();
	char *SafeAlloc();

	if(first < FRSTONDISK)
		first = FRSTONDISK;
	if(last > lastIndx[curDir])
		last = lastIndx[curDir];
	stkList = SafeAlloc(last - first + 1, "Stack list");
	stkListFirst = first;
	stkListLast = last;
	stkListNext = first;
	/* initialize the list */
	for(cp = stkList,endp = cp + last - first; cp <= endp;)
		*cp++ = GtStkIndx(first++)->full;
}

int NextStkOnList()
{
	if(!stkList)
		error_("Internal error: Stack list not setup");
	if(stkListNext > stkListLast)
		return(0);
	while(stkList[stkListNext++ - stkListFirst] == 0)
		if(stkListNext > stkListLast)
			return(0);
	return(stkListNext - 1);
}

void RmStkOnList(stack)
register int stack;
{
	if(stack <= stkListLast && stack >= stkListFirst)
		stkList[stack - stkListFirst] = 0;
}

void FreeStkList()
{
	SafeFree(stkList);
	stkList = (char *)0;
	stkListLast = 0;
}

/*****************************************************************************/
/* IsSameData - returns 1 if two stack structures have same ra, dec, freq,   */
/*              filters and scan numbers                                     */
/*****************************************************************************/

int IsSameData(stk1,stk2)
	register struct of_stk *stk1, *stk2;	/* location of data */
{
	register int i; 

	if (stk1->dra != stk2->dra) return(0);
	if (stk1->ddec != stk2->ddec) return(0);
	if (stk1->nstk[0] != stk2->nstk[0]) return(0);
	if (stk1->ra  != stk2->ra  ||
	    stk1->dec != stk2->dec ||
	    stk1->l   != stk2->l   ||
	    stk1->b   != stk2->b   ||
	    stk1->dl  != stk2->dl  ||
	    stk1->db  != stk2->db  ||
	    stk1->dx  != stk2->dx  ||
	    stk1->dy  != stk2->dy  ||
	    stk1->odra != stk2->odra ||
	    stk1->oddec != stk2->oddec ||
	    stk1->epoch != stk2->epoch ||
	    stk1->fwid != stk2->fwid ||
	    stk1->freq != stk2->freq ||
	    stk1->nlist != stk2->nlist ) return(0);
	for ( i = 1; i < MAXNSTK_; i++ )
		if (stk1->nstk[i] != stk2->nstk[i]) return(0);
	for ( i = 0; i < MAXNSTKX; i++ )
		if (stk1->nstkx[i] != stk2->nstk[i]) return(0);
	return(1);
}

/*
 * Set the position of a stack (given a pointer to it) to the given XYPOINT
 * (relative coordinate).
 * Use the center of the current relative coordinate system as the center
 * coordinates of the stack unless the point is more than 10 degrees away.
 */
void SetStackPosition(stp, p)
struct of_stk *stp;
XYPOINT p;
{
	XYZPOINT here, cntr;
	XYPOINT p1;
	double ra, dec, l, b;

	p1.x = p1.y = 0;
	cntr = CsysToXyz(p1);
	here = CsysToXyz(p);
	XyzToRaDec(&here, csys.epoch, &ra, &dec);
	XyzToLb(&here, &l, &b);

	if(dissquare(&here, &cntr) > (M_PI / 18. * SPHERERAD * M_PI / 18. *
		SPHERERAD)) {
		stp->ra = ra;
		stp->dec = dec;
		stp->l = l;
		stp->b = b;
		stp->dra = stp->ddec = stp->odra = stp->oddec = stp->dl =
			 stp->db = 0;
	} else {
		XyzToRaDec(&cntr, csys.epoch, &p.x, &p.y);
		stp->ra = p.x;
		stp->dec = p.y;
		stp->dra = ra - p.x;
		stp->ddec = dec - p.y;
		stp->odra = 900 * (ra - p.x) * cos(dec * cunits[DEGREES]);
		stp->oddec = 60 * (dec - p.y);
		XyzToLb(&cntr, &p.x, &p.y);
		stp->l = p.x;
		stp->b = p.y;
		stp->dl = l - p.x;
		stp->db = b - p.y;
	}
}

