/*************************************/
/* Routines for handling search file */
/*************************************/

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

# include "S.constant.h"
# include "S.struct.h"

# include "C.h"
# include "../coordsys/C.h"
# include "../main/dragon.h"
# include "../main/C.h"
# include "include.h"
# include "../error/C.h"
# include "../misc/C.h"


typedef struct				/* stack index range */
{
	STKINDX low;				/* low stack index */
	STKINDX high;				/* high stack index */
}
STKRNG;

typedef struct of_STKBUF		/* structure for linked stack */
					/* buffer list */
{
	STKLINK *buf;				/* pointer to a buffer */
	struct of_STKBUF *next;			/* next buffer */
}
STKBUF;

static SFILE searchFile[2] = {0,0};	/* search file pointers */
static STKLINK *stkArray;		/* array of stacks for interpolation */
static char *stkInArray;		/* array of flags for whether there is */
				/* at least one stack in each element of */
				/* stkArray */
static STKBUF arrayBufs;		/* first pointer to a linked buffer list */
static STKBUF *curBuf;			/* current buffer */
static int arraySize;			/* size of stkArray */
static STKLINK *endOfBuffer;		/* end of memory allocated for stkArray */
static STKLINK *nextInBuffer;		/* next unused element of stkArray */
				/* for stkArray */
static int numCols;			/* number of columns in stkArray */
static int numRows;			/* number of rows in stkArray */
static int isLine;			/* 1 if stkArray represents line */
static XYZPOINT *space;		/* array of coordinates indicating space */
				/* being used to fill in stkArray */
static XYPOINT spaceLoc;		/* location of space[0] in projection */
static double xSpcScale;		/* x scale of stkArray in projection */
static double ySpcScale;		/* y scale of stkArray in projection */

/* global variables used by FindNear to communicate with ChkFound */
static struct				/* integer point structure */
{
	int x;					/* horizontal */
	int y;					/* depth */
	int z;					/* vertical */
}
chkPoint;				/* point to check stacks against */
static double maxDis;			/* maximum allowable distance */
static double maxSqrDis;		/* square of maxDis */
static int iMaxDis;			/* integer version of maxDis */
static double curSqrDis;		/* current smallest square of */
					/* distance */
static int curStkNum;			/* current best stack number */

int round();				/* round double to int */
double dissquare();			/* compute square of distance between
					   two XYZPOINTs */
#define bzero(s, n) memset(s, 0, n)

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* search.c */
static int ChkFound P_((register STKINDX *stkInd));
static void InitStkLink P_((int columns, int rows));
static void AddToArray P_((register int stkNum, XYZPOINT loc, register int column, register int row));
static int AddInStk P_((STKINDX *stk));
static int AddToList P_((STKINDX *stk));
static int CmbnSim P_((register STKINDX *stkInd));
#undef P_

/***************************************/
/* CreateSearch - create a search file */
/***************************************/

void CreateSearch()
{
	char fileName[MAXDNLEN + 7];	/* name of search file */
	SFILE Screat();		/* routine for creating search file */

	/* make name */
	sprintf(fileName,"%s/search",dirName[curDir]);

	/* create file if one doesn't exist */
	if(access(fileName,0)) {
	    fprintf(stderr,"Creating search file\n");
		Sclose(Screat(fileName,0666));
	}
}

/***********************************/
/* OpenSearch - open a search file */
/***********************************/

void OpenSearch()
{
	char fileName[MAXDNLEN + 7];		/* name of search file */
	char errStr[MAXDNLEN + 32];		/* error string */
	SFILE Sopen();				/* routine for opening */
						/* search file */

	/* construct name of file */
	sprintf(fileName,"%s/search",dirName[curDir]);

	/* check that file exists */
	sprintf(errStr,"search file for stack directory %s",dirName[curDir]);
	ChkAccess(errStr,fileName,4,0, "");

	/* close old file if open */
	if(searchFile[curDir])
	{
		noint_();
		GarbCollect(searchFile[curDir]);
		Sclose(searchFile[curDir]);
		yesint_();
	}

	/* open file */
	if(access(fileName, W_OK) != 0)
		stkReadOnly[curDir] = 1;
	searchFile[curDir] = Sopen(fileName, stkReadOnly[curDir]);
}

/***********************************************/
/* CloseSearch - close search file -- mwp 5/92 */
/***********************************************/
void CloseSearch()
{
        char fileName[MAXDNLEN + 7];            /* name of search file */
        char errStr[MAXDNLEN + 32];             /* error string */

        /* construct name of file */
        sprintf(fileName,"%s/search",dirName[curDir]);
 
        /* check that file exists */
        sprintf(errStr,"search file for stack directory %s",dirName[curDir]);
        ChkAccess(errStr,fileName,4,0, "");
 
        /* close file */
        noint_();
        GarbCollect(searchFile[curDir]);
        Sclose(searchFile[curDir]);
        yesint_();
}
/***********************************************/
/* WrSearch - write a stack to the search file */
/***********************************************/

void WrSearch(stk)
	struct of_stk *stk;		/* stack to be written */
{
	STKINDX MkSrchIndx();		/* routine for making stack */
					/* index structure */

	if(stkReadOnly[curDir])
		error("Trying to change read only search file");

	/* write stack index structure to search file */
	noint_();
	Sstore(searchFile[curDir],MkSrchIndx(stk));
	yesint_();
}

/**************************************************/
/* RmSearch - remove a stack from the search file */
/**************************************************/

void RmSearch(stk)
	struct of_stk *stk;		/* stack to be removed */
{
	STKINDX MkSrchIndx();		/* routine for making stack */
					/* index structure */

	/* remove stack index structure from search file */
	noint_();
	Sremove(searchFile[curDir],MkSrchIndx(stk));
	yesint_();
}

/*************************************************************
 * SearchFlush - control auto flush of search file in curdir *
 *************************************************************/

void SearchFlush(flush)
int flush;
{
	if(! notOpen[curDir])
		Sclean(searchFile[curDir],flush);
}

/****************************************************************
 * SearchHdrPrint - print info on the search file header blocks *
 ****************************************************************/

void SearchHdrPrint(dir)
int dir;
{
	ThdrPrint(searchFile[dir]);
}

/************************************************************/
/* MkSrchIndx - make search index structure for a given stack */
/************************************************************/

STKINDX MkSrchIndx(stk)
	register struct of_stk *stk;	/* stack to make index for */
{
	STKINDX temp;			/* temporary stack index structure */
					/* to hold index while being made */
	XYZPOINT loc;			/* location of stack in space */
	XYZPOINT RaDecToXyz();		/* routine for computing loc */

	/* compute location of stack */
	loc = RaDecToXyz(stk->ra + stk->dra,stk->dec + stk->ddec,
		(double)stk->epoch);
	temp.x = round(loc.x) + HALFSPACE;
	temp.y = round(loc.y) + HALFSPACE;
	temp.z = round(loc.z) + HALFSPACE;

	/* compute remainder of index */
	temp.freq = round(stk->freq * FREQSCALE);
	temp.stkNum = stk->numst;

	/* return index */
	return(temp);
}

/**********************************************************
 * OkFreqs - set up low and high freq limits for searches *
 **********************************************************/

void OkFreqs(high,low)
double high,low;
{
	if(high >= low) {
		lowFreq = low;
		highFreq = high;
	} else {
		lowFreq = high;
		highFreq = low;
	}
}

/************************************************************
 * OkStacks - set up high and low stack number for searches *
 ************************************************************/

void OkStacks(high,low)
int high,low;
{
	if(high >= low) {
		lowStkNum = low;
		highStkNum = high;
	} else {
		lowStkNum = high;
		highStkNum = low;
	}
}

/***************************************************************************/
/* FindNear - find the nearest stack to a given point within a given range */
/***************************************************************************/

int FindNear(point,range)		/* returns stack number */
	XYZPOINT point;			/* point to look around */
	double range;			/* maximum distance to stack found */
					/* (in arcminutes) */
{
	int ChkFound();			/* routine for checking found */
					/* stacks */
	double sin();			/* sine function */
	STKRNG stkRng;			/* range of stack index structures */
					/* that stack must be found within */

	/* compute maxDis and related values */
	SetMaxDis(range);

	/* set point to check against */
	chkPoint.x = round(point.x) + HALFSPACE;
	chkPoint.y = round(point.y) + HALFSPACE;
	chkPoint.z = round(point.z) + HALFSPACE;

	/* compute stack index range in which stack should be found */
	stkRng.low.x = chkPoint.x - iMaxDis;
	stkRng.high.x = chkPoint.x + iMaxDis;
	stkRng.low.y = chkPoint.y - iMaxDis;
	stkRng.high.y = chkPoint.y + iMaxDis;
	stkRng.low.z = chkPoint.z - iMaxDis;
	stkRng.high.z = chkPoint.z + iMaxDis;
	stkRng.low.freq = round(lowFreq * FREQSCALE);
	stkRng.high.freq = round(highFreq * FREQSCALE);
	stkRng.low.stkNum = lowStkNum;
	stkRng.high.stkNum = highStkNum;

	/* set up record keeping variables for ChkFound */
	curSqrDis = maxSqrDis + 1;
	curStkNum = 0;

	/* search */
/*	Ssearch(searchFile[curDir],stkRng,ChkFound); */
	Ssearch(searchFile[curDir],&stkRng.low.x,&stkRng.high.x,ChkFound);

	/* return closest stack */
	return(curStkNum);
}

/*******************************************************************************
 * SetMaxDis - Set up the max distance criterion for FindNear and CombineSimilar
 ******************************************************************************/
void SetMaxDis(range)
double range;			/* max dist in arc min */
{
	maxDis = 2 * sin(range * M_PI / (180 * 60 * 2)) * SPHERERAD;
	maxSqrDis = maxDis * maxDis;
	iMaxDis = (int)(maxDis);
	if(iMaxDis < maxDis)
		iMaxDis++;
}

static int ChkFound(stkInd)		/* returns 0 to continue search */
	register STKINDX *stkInd;	/* stack index passed by Ssearch */
{
	double disSqr;			/* square of distance from this */
					/* stack to chkPoint */
	double dx;			/* difference in x */
	double dy;			/* difference in y */
	double dz;			/* difference in z */

	XYZPOINT loc;
	XYPOINT p;

	/* compute disSqr */
	dx = stkInd->x - chkPoint.x;
	dy = stkInd->y - chkPoint.y;
	dz = stkInd->z - chkPoint.z;
	disSqr = dx * dx + dy * dy + dz * dz;
	if(disSqr > maxSqrDis)
		return(0);	/* continue search */

	/* find loc */
	loc.x = (double)(stkInd->x - HALFSPACE);
	loc.y = (double)(stkInd->y - HALFSPACE);
	loc.z = (double)(stkInd->z - HALFSPACE);
	p = XyzToCsys(loc);

	printf("Stack %d (%.3f,%.3f)\n",stkInd->stkNum,p.x, p.y);

	/* check to see if this stack is the best one found so far */
	if(disSqr < curSqrDis)
	{
		curSqrDis = disSqr;
		curStkNum = stkInd->stkNum;
	}

	/* continue search */
	return(0);
}

/**************************************************************/
/* InitStkLink - initialize array of stacks for interpolation */
/**************************************************************/

static void InitStkLink(columns,rows)
	int columns;		/* number of columns in array */
	int rows;		/* number of rows */
{
	numCols = columns;
	numRows = rows;
	arraySize = rows * columns * sizeof(STKLINK);
	stkArray = (STKLINK *)(SafeAlloc(arraySize,"stkArray"));
	stkInArray = (char *)(SafeAlloc(rows * columns,"stkInArr"));
	arrayBufs.buf = stkArray;
	arrayBufs.next = 0;
	curBuf = &arrayBufs;
	bzero(stkInArray,rows * columns);
	endOfBuffer = (STKLINK *)((char *)(stkArray) + arraySize);
	nextInBuffer = endOfBuffer;
	if(arraySize == sizeof(STKLINK))
		arraySize *= 16;	/* More efficient array for a point */
}

/****************************************************************************/
/* AddToArray - add a stack to the list for a specific element of the array */
/****************************************************************************/

static void AddToArray(stkNum,loc,column,row)
	register int stkNum;	/* number of stack to be added */
	XYZPOINT loc;		/* location of stack */
	register int column;	/* column to which to add it */
	register int row;	/* row to which to add it */
{
	register STKLINK *addTo; /* place to add it */

	addTo = stkArray + row * numCols + column;
	if(stkInArray[row * numCols + column])
	{
		while(addTo->next)
			addTo = addTo->next;
		if(nextInBuffer == endOfBuffer)
		{
			nextInBuffer = (STKLINK *)(SafeAlloc(arraySize,
				"stkArray"));
			endOfBuffer = (STKLINK *)((char *)(nextInBuffer)
				      + arraySize);
			curBuf->next = (STKBUF *)(SafeAlloc(sizeof(STKBUF),
				"STKBUF"));
			curBuf = curBuf->next;
			curBuf->buf = nextInBuffer;
			curBuf->next = 0;
			nextInBuffer++;
		}
		addTo->next = nextInBuffer;
		addTo = nextInBuffer;
		nextInBuffer++;
	}
	else
		stkInArray[row * numCols + column] = 1;
	addTo->stkNum = stkNum;
	addTo->loc = loc;
	addTo->next = 0;
}

/********************************/
/* FreeArray - free stack array */
/********************************/

void FreeArray()
{
	register STKBUF *toFree;	/* buffer to be freed */
	register STKBUF *nextBuf;	/* next buffer */

	SafeFree((char *)stkArray);
	SafeFree((char *)stkInArray);
	nextBuf = arrayBufs.next;
	while(nextBuf)
	{
		toFree = nextBuf;
		nextBuf = nextBuf->next;
		SafeFree((char *)toFree->buf);
		SafeFree((char *)toFree);
	}
}

/**********************************************************************/
/* AddInStk - add a stack to all the appropriate elements of stkArray */
/**********************************************************************/

static int AddInStk(stk)
	STKINDX *stk;		/* pointer to stack to be added */
{
	static XYZPOINT loc;	/* location of stack */
	register XYZPOINT *p1,*p2; /* locations being checked for */
				/* nearness to loc */
	static XYPOINT proj;	/* projected coordinates of stack */
	static int row;		/* row of element closest to projection of */
				/* stack */
	static int col;		/* column of element closest to projection */
				/* of stack */
	register int r;		/* row being checked to see if stack should */
				/* be used in its interpolation */
	register int c;		/* column being checked */
	XYPOINT Proj();

	/* find loc */
	loc.x = (double)(stk->x - HALFSPACE);
	loc.y = (double)(stk->y - HALFSPACE);
	loc.z = (double)(stk->z - HALFSPACE);

	/* find element of array closest to projection of stack */
	proj = Proj(loc);
	if(!isLine)
	{
		row = round( (proj.y - spaceLoc.y) * ySpcScale);
		col = round( (proj.x - spaceLoc.x) * xSpcScale);
	}
	else
	{
		if(xSpcScale != DRAGON)
			col = round( (proj.x - spaceLoc.x) * xSpcScale);
		else
			col = round( (proj.y - spaceLoc.y) * ySpcScale);
		row = 0;
	}
	if(row < 0)
		row = 0;
	else if(row >= numRows)
		row = numRows - 1;
	if(col < 0)
		col = 0;
	else if(col >= numCols)
		col = numCols - 1;

	/* add to all appropriate locations */
	p1 = space + row * numCols + col;
	r = row;
	while(r < numRows)
	{
		/* check from center row to higher row numbers */
		if(dissquare(&loc,p1) > iRadSqr)
			break;
		p2 = p1;
		c = col;
		while(c < numCols)
		{
			/* check from center column to higher cols */
			if(dissquare(&loc,p2) > iRadSqr)
				break;
			AddToArray(stk->stkNum,loc,c,r);
			c++;
			p2++;
		}
		p2 = p1 - 1;
		c = col - 1;
		while(c >= 0)
		{
			/* check from center col - 1 to lower cols */
			if(dissquare(&loc,p2) > iRadSqr)
				break;
			AddToArray(stk->stkNum,loc,c,r);
			c--;
			p2--;
		}
		r++;
		p1 += numCols;
	}
	p1 = space + (row - 1) * numCols + col;
	r = row - 1;
	while(r >= 0)
	{
		/* check from cent row - 1 to smaller row numbers */
		if(dissquare(&loc,p1) > iRadSqr)
			break;
		p2 = p1;
		c = col;
		while(c < numCols)
		{
			/* check from center column to higher cols */
			if(dissquare(&loc,p2) > iRadSqr)
				break;
			AddToArray(stk->stkNum,loc,c,r);
			c++;
			p2++;
		}
		p2 = p1 - 1;
		c = col - 1;
		while(c >= 0)
		{
			/* check from center col - 1 to lower cols */
			if(dissquare(&loc,p2) > iRadSqr)
				break;
			AddToArray(stk->stkNum,loc,c,r);
			c--;
			p2--;
		}
		p1 -= numCols;
		r--;
	}

	/* continue search */
	return(0);
}

/******************************************************************
 * dissquare - compute the distance squared between two XYZPOINTs *
 ******************************************************************/
double dissquare(p1,p2)
XYZPOINT *p1,*p2;
{
	register double *a = &p1->x,*b = &p2->x,d,t;

	t = *a++ - *b++;
	d = *a++ - *b++;
	d = t*t + d*d;
	t = *a - *b;
	return(d + t*t);
}

/***************************************************************************/
/* FillInArray - fill in stkArray with all the stacks within a given space */
/***************************************************************************/

void FillInArray(pLowerLeft,pUpperRight,columns,rows,save)
	XYPOINT pLowerLeft;		/* lower left corner of space */
					/* in current projection plane */
	XYPOINT pUpperRight;		/* upper right corner of space */
	int columns;			/* number of columns */
	int rows;			/* number of rows */
	int (*save)();			/* routine to save the stacks found */
{
	register XYZPOINT *chk;		/* point being checked to see if */
					/* it has an extreme value of */
					/* x, y, or z */
	XYZPOINT *endOfSpace;		/* end of data in space */
	STKRNG range;			/* range of stacks to be searched */
	XYZPOINT lowLim;		/* lower bounds of x, y, and z */
	XYZPOINT highLim;		/* upper bounds of x, y, and z */
	double temp;			/* temporary variable */
	STKLINK *curStkList;		/* stack list whose weights are */
					/* being computed */
	char *curStkFlag;		/* whether there is anything in */
					/* curStkList */
	XYZPOINT *curStkLoc;		/* location of curStkList */
	XYZPOINT *UnProjArray();
	XYZPOINT *UnProjLine();
	double dx,dy;			/* temp for x and y length of line */
	double sqrt();

	/* find space */
	if(rows == 0) {			/* A point or a line */
	    rows = 1;
	    space = UnProjLine(pLowerLeft,pUpperRight,columns);
	    if(columns > 1) {		/* A single point needs nothing else */
		isLine = 1;
		dx = pUpperRight.x - pLowerLeft.x;
		dy = pUpperRight.y - pLowerLeft.y;
		if(dx*dx >= dy*dy) {
			xSpcScale = (columns - 1) / dx;
			ySpcScale = DRAGON;
		} else {
			xSpcScale = DRAGON;
			ySpcScale = (columns - 1) / dy;
		}
	    }
	} else {			/* A full 2d array */
		space = UnProjArray(pLowerLeft,pUpperRight,columns,rows);
		xSpcScale = (columns - 1) / (pUpperRight.x - pLowerLeft.x);
		ySpcScale = (rows - 1) / (pUpperRight.y - pLowerLeft.y);
		isLine = 0;
	}
	spaceLoc = pLowerLeft;
	endOfSpace = space + columns * rows;

	/* find limits of space in cartesian coordinates */
	lowLim = highLim = *space;
	for(chk = space + 1; chk < endOfSpace; chk++)
	{
		if(chk->x < lowLim.x)
			lowLim.x = chk->x;
		else if(chk->x > highLim.x)
			highLim.x = chk->x;
		if(chk->y < lowLim.y)
			lowLim.y = chk->y;
		else if(chk->y > highLim.y)
			highLim.y = chk->y;
		if(chk->z < lowLim.z)
			lowLim.z = chk->z;
		else if(chk->z > highLim.z)
			highLim.z = chk->z;
	}

	/* construct range */
	temp = HALFSPACE - iRad;
	range.low.x = round(temp + lowLim.x);
	range.low.y = round(temp + lowLim.y);
	range.low.z = round(temp + lowLim.z);
	range.low.freq = round(lowFreq * FREQSCALE);
	range.low.stkNum = lowStkNum;
	temp = HALFSPACE + iRad;
	range.high.x = round( temp + highLim.x);
	range.high.y = round( temp + highLim.y);
	range.high.z = round( temp + highLim.z);
	range.high.freq = round(highFreq * FREQSCALE);
	range.high.stkNum = highStkNum;

	/* search */
	InitStkLink(columns,rows);
/*	Ssearch(searchFile[curDir],range,save); */
	Ssearch(searchFile[curDir],&range.low.x, &range.high.x,save);


        if(interpType!=NOINTERP) {
	/* fill in weights only if we are interpolating */
	  curStkList = stkArray;
	  curStkFlag = stkInArray;
          for(curStkLoc = space; curStkLoc < endOfSpace; curStkLoc++)
          {
                if(*curStkFlag)
                        FillInWeights(*curStkLoc,curStkList);
                curStkList++;
                curStkFlag++;
          }
        }

	/* deallocate space */
	SafeFree((char *)space);
}

/********************************************************/
/* Stk2Space - fill in a space by space array with data */
/********************************************************/

/* modified to fill a large array using a mosaic of smaller arrays of
 * linked lists of stacks.  Such lists are made by FillInArray and put in
 * an array pointed to by stkArray */
#define ARRAYMAX 900		/* max size of stkArray */
#define AXISMAX 30		/* max size of one axis of a more or less
				   square stkArray */

float *Stk2Space(pLowerLeft,pUpperRight,xRes,yRes,minWt)
	XYPOINT pLowerLeft,pUpperRight;	/* projected corners of array */
	int xRes,yRes;		/* resolution of array */
	double minWt;		/* minimum weight to appear in plot */
{
	double dx,dy;		/* projected pixel width, height */

	float *array;		/* array to be filled in */
	STKLINK *locList;	/* list of stacks for computing this */
				/* location in current box */
	char *locFlag;		/* flag for whether anything is in this */
				/* location in current box */

	float *yStrt,*yEnd;	/* pos in array for rows from this box */
	register float *xEnd;	/* pos in array for columns of current row */
	int xBoxI,yBoxI;	/* indices of ll of current box in array */
	int xBoxRes,yBoxRes;	/* number of pixels in current box */
	XYPOINT boxP1,boxP2;	/* ll and ur of current box */
	register int n;		/* temp number of boxes x or y */

	register float *curEl;	/* element of array currently being */
				/* filled in */

	double LocVal();
	int AddInStk();

	array = (float *)(ChkAlloc(xRes * yRes * sizeof(float),
		"Stk2SpAr"));
	gtArrValues = 0;	/* flag for "Getting Stack Values" message */


	dx = (pUpperRight.x - pLowerLeft.x)/(xRes - 1);
	dy = (pUpperRight.y - pLowerLeft.y)/(yRes - 1);

	/* loop through the boxes */
	for(yBoxI = 0;yBoxI < yRes;yBoxI += yBoxRes) {
	    if(xRes < AXISMAX)
		n = (xRes * (yRes - yBoxI) + ARRAYMAX - 1) / ARRAYMAX;
	    else
		n = (yRes - yBoxI + AXISMAX - 1) / AXISMAX;
	    yBoxRes = (yRes - yBoxI + n - 1) / n;

	    for(xBoxI = 0;xBoxI < xRes;xBoxI += xBoxRes) {
		n = (yBoxRes * (xRes - xBoxI) + ARRAYMAX - 1) / ARRAYMAX;
	        xBoxRes = (xRes - xBoxI + n - 1) / n;
/*
printf("BoxI = (%d,%d) BoxRes = (%d,%d)",xBoxI,yBoxI,xBoxRes,yBoxRes);
*/
		/* compute corners of this box and get stkArray */
		boxP1.x = pLowerLeft.x + dx * xBoxI;
		boxP1.y = pLowerLeft.y + dy * yBoxI;
		boxP2.x = pLowerLeft.x + dx * (xBoxI + xBoxRes - 1);
		boxP2.y = pLowerLeft.y + dy * (yBoxI + yBoxRes - 1);
/*
printf("at (%g,%g) (%g,%g)\n",boxP1,boxP2);
*/
		FillInArray(boxP1,boxP2,xBoxRes,yBoxRes,AddInStk);

		/* Calculate the value for each pixel and put in array */
		locList = stkArray;
		locFlag = stkInArray;
		yStrt = array + xBoxI + yBoxI * xRes;
		yEnd = yStrt + yBoxRes * xRes;
		for(; yStrt < yEnd; yStrt += xRes) {
		    xEnd = yStrt + xBoxRes;
		    for(curEl = yStrt; curEl < xEnd; ) {
			if(*locFlag)
				*curEl++ = LocVal(locList, minWt);
			else
				*curEl++ = FDRAGON;
			locList++;
			locFlag++;
		    }
		}
		FreeArray();
	    }
	}
	gtArrValues = 0;
	return(array);
}

float *Stk1Space(point1,point2,low,numCh,spaceRes,filtWid)
	XYPOINT point1;		/* one end of line in projected plane */
	XYPOINT point2;		/* other end of line */
	double low;		/* low velocity in plot units */
	int numCh;		/* number of channels in velocity */
	int spaceRes;		/* number of boxes in space */
	double filtWid;		/* the filter width to be used */
{
	float *array;		/* array to be filled in */
	STKLINK *locList;	/* list of stacks for computing each */
				/* location */
	STKLINK *lastLoc;	/* last location */
	char *locFlag;		/* flag for whether anything is in this */
				/* location */
	float *curEl;		/* element of array currently being filled */
				/* in */
	float *curCol;		/* first element in column being filled in */
	float *lastEl;		/* last element in array */
	float *channel;		/* channel currently being filled in */
	float *lastChan;	/* last channel to be filled in */
	static int one = 1,three = 3;
	double tran_();
	int round();
	int AddInStk();

	FillInArray(point1,point2,spaceRes,0,AddInStk);
	array = (float *)(ChkAlloc(spaceRes * numCh * sizeof(float),
		"Stk1SpAr"));
	curCol = array;
	lastEl = array + numCh * spaceRes;
	locFlag = stkInArray;
	lastLoc = stkArray + spaceRes;
	for(locList = stkArray; locList < lastLoc; locList++)
	{
		curEl = curCol;
		if(*locFlag)
		{
			if(AvgStacks(locList,filtWid, 0))
			{
				channel = stk_[0].stak - 1 +
					  round(tran_(&three,&one,&low));
				lastChan = channel + numCh;
				while(channel < stk_[0].stak && curEl < lastEl)
				{
					*curEl = FDRAGON;
					curEl += spaceRes;
					channel++;
				}
				if(lastChan > stk_[0].stak + stk_[0].numpt)
					lastChan = stk_[0].stak + stk_[0].numpt;
				while(channel < lastChan)
				{
					*curEl = *channel++;
					curEl += spaceRes;
				}
				while(curEl < lastEl)
				{
					*curEl = FDRAGON;
					curEl += spaceRes;
				}
			}
			else
				while(curEl < lastEl)
				{
					*curEl = FDRAGON;
					curEl += spaceRes;
				}
		}
		else
			while(curEl < lastEl)
			{
				*curEl = FDRAGON;
				curEl += spaceRes;
			}
		curCol++;
		locFlag++;
	}
	FreeArray();
	return(array);
}

static int AddToList(stk)
	STKINDX *stk;
{
	XYZPOINT loc;			/* location of stack */

	/* find loc */
	loc.x = (double)(stk->x - HALFSPACE);
	loc.y = (double)(stk->y - HALFSPACE);
	loc.z = (double)(stk->z - HALFSPACE);
	if(dissquare(&loc,space) <= iRadSqr)
		AddToArray(stk->stkNum, loc, 0, 0);
	/* continue the search */
	return(0);
}

/*********************************************************
 * InterpAtPoint - Make an interpolated stack at a point *
 *********************************************************/

void InterpAtPoint(x, y, filtWid, catScanLst)
	double x,y;		/* coordinates of point */
	double filtWid;		/* the filter width to be used, 0.0 to take the
				first thing that comes along */
	int catScanLst;
{

	XYPOINT point;		/* position of point */
	point.x = x;
	point.y = y;
	FillInArray(point, point, 1, 0, AddToList);
	if(*stkInArray) {
		AvgStacks(stkArray, filtWid, catScanLst);
		SetStackPosition(&stk_[0], point);
		stk_[1] = stk_[0];
	} else
		coreFull[0] = 0;
	FreeArray();
}

/********************************************************************
 * ExecuteAtPoint - Run combex_ for each stack within interp radius *
 ********************************************************************/

int ExecuteAtPoint(x, y, string)
	double x,y;		/* coordinates of point */
	char *string;		/* string to be executed for each stack */
{
	XYPOINT point;		/* position of point */
	register STKLINK *list;	/* list of stacks to average */

	point.x = x;
	point.y = y;
	FillInArray(point, point, 1, 0, AddToList);
	if(*stkInArray) {
		for(list = stkArray; list; list = list->next) {
		    getStack(list->stkNum,1);
		    if(!coreFull[0])
			SearchFileError(list->stkNum);
		    combex_(string);
		}
	}
	FreeArray();
	return(*stkInArray);
}

void SearchFileError(int stkNum) {
	fprintf(stderr, "Please run 'up' on %s!!!\n", dirName[curDir]);
	error("Search file references stack %d which is empty", stkNum);
}

/***********************************************************************
 * CombineSimilar - Combine all of the stacks close to a given stack.
 * Use SetMaxDis(range) to set up the ptol criterion, lowStkNum and
 * highStkNum (OkStacks) to limit the range of stacks.  Require that the
 * bandwidths be the same and that the scan numbers be different.
 ***********************************************************************/

static int simInit;
static int simVerbose;
static short simPrint;
static short simDropped;
static short simCombined;

int CombineSimilar(verbose)
int verbose;
{
	STKRNG stkRng;
	XYZPOINT loc;
	static int savesig = -1;

	simPrint = 1;
	simVerbose = verbose;
	simDropped = 0;
	simCombined = 0;
	loc = RaDecToXyz(stk_[1].ra + stk_[1].dra,stk_[1].dec +
		stk_[1].ddec, (double)stk_[1].epoch),

	/* set point to check against */
	chkPoint.x = round(loc.x) + HALFSPACE;
	chkPoint.y = round(loc.y) + HALFSPACE;
	chkPoint.z = round(loc.z) + HALFSPACE;

	/* compute stack index range in which similar stacks should be found */
	stkRng.low.x = chkPoint.x - iMaxDis;
	stkRng.high.x = chkPoint.x + iMaxDis;
	stkRng.low.y = chkPoint.y - iMaxDis;
	stkRng.high.y = chkPoint.y + iMaxDis;
	stkRng.low.z = chkPoint.z - iMaxDis;
	stkRng.high.z = chkPoint.z + iMaxDis;
	stkRng.low.freq = stk_[1].freq * FREQSCALE;
	stkRng.high.freq = stk_[1].freq * FREQSCALE + .999;
	stkRng.low.stkNum = lowStkNum;
	stkRng.high.stkNum = highStkNum;

	if(savesig != cmnd_.cmdsig) {
		simInit = 5;
		savesig = cmnd_.cmdsig;
	}
	/* Search for similar stacks */
/*	Ssearch(searchFile[curDir],stkRng,CmbnSim); */
	Ssearch(searchFile[curDir],&stkRng.low.x, &stkRng.high.x,CmbnSim);
	return( (simDropped << 16) | simCombined);
}

static int CmbnSim(stkInd)
register STKINDX *stkInd;
{
	double dx, dy, dz;
	int listoverlap;

	/* if this is the same stack as in 2, ignore it but continue search */
	if(stkInd->stkNum == stk_[1].numst)
		return(0);

	/* compute disSqr */
	dx = stkInd->x - chkPoint.x;
	dy = stkInd->y - chkPoint.y;
	dz = stkInd->z - chkPoint.z;
	if(dx * dx + dy * dy + dz * dz > maxSqrDis)
		return(0);	/* continue search */

	if(simVerbose > 2) {
		if(simPrint) {
			putc('\n',stderr);
			simPrint = 0;
		}
		fprintf(stderr,"\tFound stack %d ",stkInd->stkNum);
	}
	getStack(stkInd->stkNum, 1);
	if(coreFull[0] && stk_[0].fwid == stk_[1].fwid) {
		if( !(listoverlap = ScanListSame()) ) {
			if(simVerbose > 2) {
				fprintf(stderr,"doing cmbn");
				simPrint = 0;
			}
			cmbn_(&simInit);
			simCombined++;
			simInit = 4;
		} else {
			if(simVerbose > 2) {
				if(listoverlap < 0)
					fprintf(stderr,
						"All scans already in stk %d",
						stk_[1].numst);
				else
					fprintf(stderr,
						"%d Scans already in %d",
						listoverlap,stk_[1].numst);
				fprintf(stderr," - not used\n");
			}
			simDropped++;
		}
		RmStkOnList(stkInd->stkNum);
	} else if(coreFull[0])
		if(simVerbose > 2)
			fprintf(stderr,"Filter width different - not used\n");
	else
		if(simVerbose > 2)
			fprintf(stderr,"No data\n");
	return(0);
}
