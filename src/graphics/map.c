# include <stdarg.h>
#include <stdio.h>

# include "C.h"
# include "include.h"
#include "../main/dragon.h"

#define bzero(s, n) memset(s, 0, n)

/* sides of a contour box */
# define LSIDE 0		/* left */
# define BSIDE 1		/* bottom */
# define RSIDE 2		/* right */
# define TSIDE 3		/* top */

# define LBSIZE 5000 		/* size of line buffer */
# define HBSIZE 100		/* size of hatch mark buffer */

/* Map doesn't seem to use the store mechanism, so I have removed it 4/25/89
 * rww */
/* MWP PUT IT BACK IN 3/25/95 --there have been instances where it is needed */
#define STORE 1

typedef struct			/* structure of box containing contour lines */
{
	double x;			/* x location of lower left corner */
	double y;			/* y location of lower left corner */
	double width;			/* width of box */
	double height;			/* height of box */
	double lowerLeft;		/* value of lower left hand corner */
	double lowerRight;		/* value of lower right hand corner */
	double upperLeft;		/* value of upper left hand corner */
	double upperRight;		/* value of upper right hand corner */
}
CBOX;				/* (stands for "contour box") */

typedef struct			/* structure of endpoint of line segment */
{
	int side;			/* side of box that endpoint is on */
					/* (LSIDE, BSIDE, RSIDE, or TSIDE) */
	double distance;		/* distance to nearest endpoint on */
					/* a different contour */
	double x;			/* x location of endpoint */
	double y;			/* y location of endpoint */
	struct of_CLINE *link;		/* line that endpoint is linked to */
}
ENDPOINT;

typedef struct of_CLINE		/* structure of a contour line segment */
{
	int topology;			/* topology of box containing line */
	int level;			/* contour level of line */
	ENDPOINT endPoint[2];		/* endpoints of line segment */
	int rightEnd;			/* number of right endpoint */
	int topEnd;				/* number of top endpoint */
	int plotted;			/* whether this line segment has */
					/* been plotted (1 if it has) */
	struct of_CLINE *newAddr;	/* new address of this line after */
					/* being stored as part of an */
					/* unfinished contour */
}
CLINE;				/* (stands for "contour line") */

typedef struct			/* structure for contour line linking info */
{
	CLINE *top;			/* pointer to line touching top of */
					/* box */
	CLINE *right;			/* pointer to line touching right of */
					/* box */
}
CLINKS;				/* (stands for "contour links") */

typedef struct			/* structure for storing hatch mark */
{
	double x0,y0;			/* end of hatch mark on contour line */
	double x1,y1;			/* end of hatch mark off contour */
}
HATCHMARK;

/* global variables */
static CLINE *lineBuf;		/* pointer to line segment buffer */
static CLINE *curInBuf;		/* current location in buffer */
#if STORE
static CLINE *storeBuf;		/* pointer to buffer for storing lines that */
				/* can't be dumped yet */
static CLINE *curInStore;	/* current location in storeBuf */
#endif /*STORE*/
static int numInBuf;		/* number of lines in buffer */
static CLINKS *linkArray;	/* pointer to array of pointers to */
				/* line segments in lineBuf used for */
				/* linking line segments */
static int lASize;		/* size of linkArray */
static CLINKS *curLinks;	/* linking information in linkArray */
				/* pertaining to the box currently being */
				/* computed */
static int prevRow;		/* amount to add to curLinks to find */
				/* set of links that represent the */
				/* contour box below the one being */
				/* computed */
static CLINKS *lARow1End;	/* end of row 1 in linkArray */
static CLINKS *lARow2End;	/* end of row 2 in linkArray */
static CBOX box;		/* box being plotted */
static int curLevel;		/* contour level being computed */
static double *lVal;		/* contour level values */
				/* (note: this actually points to the */
				/* second location in the allocated memory */
				/* so that lVal[-1] is a real place) */
static double *midLVal;		/* middle of lVal */
static double *lastLVal;	/* end of lVal */
static int *lType;		/* contour level line types */
static int cLevels;		/* number of contour levels */
static ENDPOINT *endPoint;	/* endpoint being computed */
static HATCHMARK *hatchBuf;	/* hatch mark buffer */
static HATCHMARK *lastHatch;	/* last hatch mark in buffer */
static HATCHMARK *maxHatch;	/* maximum value for lastHatch without */
				/* overflowing buffer */
static double hatchLen;		/* length of hatch marks */
static double hatchSpace;	/* spacing between hatch marks */
static double disFromHatch;	/* distance travelled along contour since */
				/* drawing last hatch mark */

/* functions */
char *ChkAlloc(),*SafeAlloc();
void ChkFree(),SafeFree();

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* map.c */
static void MapArray P_((float *array, register int columns, register int rows, double left, double bottom, double right, double top));
static void AllocLevels P_((void));
static void AllocMap P_((int columns));
static void PlotBox P_((void));
static void Saddle P_((int topology));
static void LtoB_RtoT P_((int topology));
static void LtoT_BtoR P_((int topology));
static void LeftPoint P_((void));
static void BottomPoint P_((void));
static void RightPoint P_((void));
static void TopPoint P_((void));
static void FlushLines P_((void));
static void SaveUndone P_((void));
static void SaveContour P_((CLINE **link));
static void GetUndone P_((void));
static void DumpLines P_((void));
static void StartHatch P_((void));
static void CheckHatch P_((CLINE *line, int prevPoint));
static void MakeHatch P_((CLINE *line, double lineLen, double disFromEnd, int prevPoint));
static void DumpHatch P_((void));
#undef P_

/* macro for finding place where a contour intersects a side of a box */
# define Intersect(start,length,val1,val2) \
	((start) + (length) * (lVal[curLevel] - (val1)) / ((val2) - (val1)))

/*******************************************************/
/* MkMap - entry level routine for making contour maps */
/*******************************************************/

/*VARARGS*/
void MkMap(int firstOpt, ...)
{
	va_list opList;		/* list of options */
	char opt;		/* option being processed */
	int storeLins;		/* 1 if lines are to be stored */
	double **outCLevs;	/* where to store levels */
	int **outCLins;		/* where to store line types */
	int *outCNum;		/* where to store number of levels */
	double *levToAdd;	/* list of contour levels to add */
	int *linToAdd;		/* list of contour lines to add */
	int numToAdd;		/* number of contours to add */
	double *levToRem;	/* list of contour levels to remove */
	int numToRem;		/* number of contours to remove */
	double lowLev;		/* lowest contour level */
	double highLev;		/* highest contour level */
	int levDefault;		/* how levels should default: 0 = don't */
				/* default, 1 = default to a scale */
	double *inCLevs;	/* contour levels that were given as option */
	int linType;		/* line type of contour levels */
	int linDefault;		/* how lines should default: 0 = don't */
				/* default, 1 = default to constant, */
				/* 2 = default to scale, 3 = alternate */
	int linType2;		/* second line type for alternating */
	int *inCLins;		/* contour line types given as option */
	float *data;		/* data being mapped */
	int rows;		/* number of rows in data */
	int columns;		/* number of columns in data */
	double left;		/* x location of left of map */
	double bottom;		/* y location of bottom of map */
	double right;		/* x location of right of map */
	double top;		/* y location of top of map */
	float *chkDatum;	/* datum being checked to see if it is an */
				/* extreme value */
	double levStep;		/* step between countour levels */
	register int count,c2;	/* general purpose counters */
	double *newHighLev;	/* pointer to new high level for sorting */
	int *newHighLin;	/* pointer to new high line type */
	double tempLev;		/* temporary level */
	int tempLin;		/* temporary line type */
	double exp();
	double fabs();
	double ChooseStep();	/* this subroutine is in grid.c */
	double Roundd();	/* so is this one */

	/* add list of options to option list */
	va_start(opList, firstOpt);
	AddOpt(firstOpt, opList);
	va_end(opList);

	/* set up defaults */
	numToAdd = 0;
	numToRem = 0;
	cLevels = 10;
	lowLev = 0.;
	highLev = 0.;
	levDefault = 1;
	linType = 255;
	linDefault = 2;
	hatchLen = 0.;
	rows = 0;
	columns = 0;
	levStep = 0.;
	if((cPic)->left < (cPic)->right)
	{
		left = (cPic)->left;
		right = (cPic)->right;
	}
	else
	{
		left = (cPic)->right;
		right = (cPic)->left;
	}
	if((cPic)->xLog)
	{
		left = exp(left);
		right = exp(right);
	}
	if((cPic)->bottom < (cPic)->top)
	{
		bottom = (cPic)->bottom;
		top = (cPic)->top;
	}
	else
	{
		bottom = (cPic)->top;
		top = (cPic)->bottom;
	}
	if((cPic)->yLog)
	{
		bottom = exp(bottom);
		top = exp(top);
	}

	/* process options */
	while(opt = GtOpt()) switch(opt)
	{
		case V_CSTORE:
			storeLins = 1;
			outCLevs = (double **)(optArg[0].l.point.i);
			outCLins = (int **)(optArg[1].l.point.i);
			outCNum = optArg[2].l.point.i;
			break;
		case V_CADD:
			levToAdd = optArg[0].l.point.d;
			linToAdd = optArg[1].l.point.i;
			numToAdd = optArg[2].i;
			break;
		case V_CREM:
			levToRem = optArg[0].l.point.d;
			numToRem = optArg[1].i;
			break;
		case V_CNUM:
			cLevels = optArg[0].i;
			break;
		case V_CLEVRANGE:
			lowLev = optArg[0].d;
			highLev = optArg[1].d;
			levDefault = 1;
			break;
		case V_CLEVSTEP:
			levStep = optArg[0].d;
			levDefault = 1;
			break;
		case V_CLEVS:
			inCLevs = optArg[0].l.point.d;
			levDefault = 0;
			break;
		case V_CLINTYPE:
			linType = optArg[0].i;
			linDefault = 1;
			break;
		case V_CLINSCALE:
			linDefault = 2;
			break;
		case V_CLINCOLOR:
			linDefault = 4;
			break;
		case V_CLINALT:
			linType = optArg[0].i;
			linType2 = optArg[1].i;
			linDefault = 3;
			break;
		case V_CLINS:
			inCLins = optArg[0].l.point.i;
			linDefault = 0;
			break;
		case V_HATCH:
			hatchLen = optArg[0].d;
			hatchSpace = optArg[1].d;
			break;
		case V_ARRAY:
			data = optArg[0].l.point.f;
			rows = optArg[1].i;
			columns = optArg[2].i;
			break;
		case V_XRANGE:
			left = optArg[0].d;
			right = optArg[1].d;
			break;
		case V_YRANGE:
			bottom = optArg[0].d;
			top = optArg[1].d;
			break;
		default:
			error_("Invalid option for MkMap");
	}

	/* check for errors */
	if(left >= right)
		error_("Left bound of map greater than or equal to right");
	if(bottom >= top)
		error_("Bottom of map greater than or equal to top");
	if(rows <= 0 || columns <= 0)
		error_("No data for contour mapping");
	count = cLevels + numToAdd;
	if(count < 1 || count > 25)
		error("%d contour lines not allowed",count);
	if(hatchLen && hatchSpace == 0)
		hatchSpace = hatchLen * 5;

	if(levDefault)
	{
		if(lowLev == highLev)
		{
			chkDatum = data;
			while(*chkDatum++ == FDRAGON)
				if(chkDatum - data == rows * columns)
					error_("No data for map");
			lowLev = chkDatum[-1];
			highLev = lowLev;
			while(chkDatum < data + (rows * columns))
			{
				if(*chkDatum != FDRAGON)
					if(*chkDatum < lowLev)
						lowLev = *chkDatum;
					else if(*chkDatum > highLev)
						highLev = *chkDatum;
				chkDatum++;
			}
		}
		if(lowLev == highLev)
			error_("No range of data for map");
		if(cLevels > 1)
		{
			if(levStep == 0.)
				levStep = ChooseStep(lowLev,highLev,cLevels);
			KeyValues(0,&lowLev,&highLev,levStep,0);
			cLevels = (int)(1.5 + (highLev - lowLev) / levStep);
			if(cLevels > 75) {
			    fprintf(stderr,
			    "Number of contour levels truncated from %d to 75\n"
				,cLevels);
			    cLevels = 75;
			}
			AllocLevels();
			tempLev = lowLev;
			for(count = 0;count < cLevels;) {
				if(tempLev != 0.)
					lVal[count++] = tempLev;
				else
					cLevels--;
				tempLev = Roundd(tempLev + levStep);
			}
		} else {
		AllocLevels();
		*lVal = lowLev;
		}
	} else {
		AllocLevels();
		for(count = 0; count < cLevels; count++)
			lVal[count] = inCLevs[count];
	}

	/* compute contour line types */
	switch(linDefault)
	{
		case 0:
			for(count = 0; count < cLevels; count++)
				lType[count] = inCLins[count];
			break;
		case 1:
			for(count = 0; count < cLevels; count++)
				lType[count] = linType;
			break;
		case 2:
		case 4:
			/* do this after sorting levels */
			break;
		case 3:
			for(count = 0; count < cLevels; count++)
				lType[count] = (count & 1)?linType:linType2;
			break;
	}

	/* remove old contour levels */
	for(count = 0; count < numToRem; count++)
	{
		lVal[cLevels] = levToRem[count];
		c2 = 0;
		while(fabs(lVal[c2] - levToRem[count]) > 1e-20)
			c2++;
		if(c2 <= cLevels)
		{
			cLevels--;
			lVal[c2] = lVal[cLevels];
			lType[c2] = lType[cLevels];
		}
	}

	/* add new contour levels */
	for(count = 0; count < numToAdd; count++)
	{
		lVal[cLevels] = levToAdd[count];
		lType[cLevels++] = linToAdd[count];
	}

	/* warn user if too many contour levels */
	if(cLevels > 75)
		warn("there are %d contour levels\n",cLevels);

	/* sort contour levels */
	for(count = cLevels; count > 1; count--)
	{
		newHighLev = lVal;
		newHighLin = lType;
		for(c2 = 1; c2 < count; c2++)
		{
			if(*newHighLev < lVal[c2])
			{
				newHighLev = lVal + c2;
				newHighLin = lType + c2;
			}
		}
		tempLev = lVal[count - 1];
		tempLin = lType[count - 1];
		lVal[count - 1] = *newHighLev;
		lType[count - 1] = *newHighLin;
		*newHighLev = tempLev;
		*newHighLin = tempLin;
	}
	/* set consecutive line types after sorting */
	if(linDefault == 2) {
		for(c2 = 0;c2 < cLevels && lVal[c2] < 0.;c2++)
			;
		for(count = 0; count < cLevels; count++) {
			if(count >= c2)
				lType[count] = 9 + 20 * ((count - c2)%5);
			else
				lType[count] = 20 * ((c2 - count - 1)%5);
		}
	} else if(linDefault == 4) {
		int levelsppen, penno;
		for(c2 = 0;c2 < cLevels && lVal[c2] < 0.;c2++)
			;
		levelsppen = (int) (cLevels - c2) / 7 ;
		levelsppen = (levelsppen < 1 ? 1 : levelsppen);
		for(count = 0; count < cLevels; count++) {
			if(count >= c2) {
				/* distribute color pens over levels */
				penno = (count - c2) / levelsppen;
				penno = (penno > 6 ? 6 : penno);
				lType[count] = 29 + 10 * penno;
			} else
				/* dotted lines with black pens for neg. contours */
				lType[count] = 10 + ((c2 - count) % 5);
		}
	}

	/* make map */
	MapArray(data,columns,rows,left,bottom,right,top);

	/* free allocated memory or store contour levels */
	if(storeLins)
	{
		for(count = 0; count < cLevels; count++)
			lVal[count - 1] = lVal[count];
		*outCLevs = lVal - 1;
		*outCLins = lType;
		*outCNum = cLevels;
	}
	else
	{
		SafeFree((char *)(lVal - 1));
		SafeFree((char *)lType);
	}
}

/******************************************/
/* MapArray - make a map of a given array */
/******************************************/

static void MapArray(array,columns,rows,left,bottom,right,top)
	float *array;		/* array to be mapped */
	register int columns;	/* columns in array */
	register int rows;	/* rows in array */
	double left;		/* x location of left of map */
	double bottom;		/* y location of bottom of map */
	double right;		/* x location of right of map */
	double top;		/* y location of top of map */
{
	register float *upperLeft; /* pointer to value of upper left corner */
	register float *lowerLeft; /* pointer to value of lower left corner */
	int row;		/* row currently being computed */
	int col;		/* column currently being computed */

	/* allocate space for map */
	AllocMap(columns - 1);

	/* compute constant values in box */
	box.width = (right - left) / (columns - 1);
	box.height = (top - bottom) / (rows - 1);

	/* make map */
	lowerLeft = array;
	upperLeft = array + columns;
	box.y = bottom;
	for(row = 0; row < rows - 1; row++)
	{
		box.x = left;
		for(col = 0; col < columns - 1; col++)
		{
			if((box.lowerLeft = *lowerLeft) != DRAGON &&
			   (box.lowerRight = lowerLeft[1]) != DRAGON &&
			   (box.upperLeft = *upperLeft) != DRAGON &&
			   (box.upperRight = upperLeft[1]) != DRAGON)
				PlotBox();
			curLinks += cLevels;
			if(curLinks == lARow1End)
			{
				prevRow *= -1;
				curLinks += cLevels;
			}
			else if(curLinks == lARow2End)
			{
				prevRow *= -1;
				curLinks = linkArray + cLevels;
			}
			bzero(curLinks,cLevels * sizeof(CLINKS));
			lowerLeft++;
			upperLeft++;
			box.x += box.width;
		}
		lowerLeft++;
		upperLeft++;
		box.y += box.height;
	}
	DumpLines();

	/* free buffers */
	SafeFree((char *)lineBuf);
#if STORE
	SafeFree((char *)storeBuf);
#endif /*STORE*/
	SafeFree((char *)hatchBuf);
	SafeFree((char *)linkArray);
}

/****************************************************************************
 * AllocLevels Allocate memory to store contour levels and their line types *
 ****************************************************************************/

static void AllocLevels()
{
	lVal = (double *)(SafeAlloc((cLevels + 2) * sizeof(double),"cLineVal"));
	lVal++;
	lType = (int *)(SafeAlloc((cLevels) * sizeof(int),"cLineTyp"));
}

/****************************************************************/
/* AllocMap - allocate memory for buffers needed for making map */
/****************************************************************/

static void AllocMap(columns)
	int columns;		/* number of columns of contour boxes */
{

	/* allocate line buffer */
	lineBuf = (CLINE *)(SafeAlloc(LBSIZE * sizeof(CLINE),"lineBuf"));

	/* set up global info about line buffer */
	curInBuf = lineBuf;
	numInBuf = 0;

#if STORE
	/* allocate storage buffer */
	storeBuf = (CLINE *)(SafeAlloc(LBSIZE * sizeof(CLINE),"storeBuf"));
#endif /*STORE*/

	/* compute size of linkArray */
	lASize = (columns * 2		/* two rows of boxes */
		  + 3)			/* extra boxes between rows */
		 * cLevels		/* each box has levels links */
		 * sizeof(CLINKS);

	/* allocate linkArray */
	linkArray = (CLINKS *)(SafeAlloc(lASize,"LinkArr"));
	bzero(linkArray,lASize);

	/* set up global info about linkArray */
	curLinks = linkArray + cLevels;
	prevRow = (columns + 1) * cLevels;
	lARow1End = linkArray + (columns + 1) * cLevels;
	lARow2End = linkArray + (columns * 2 + 2) * cLevels;

	/* allocate hatch mark buffer */
	hatchBuf = (HATCHMARK *)(SafeAlloc(HBSIZE * sizeof(HATCHMARK)
		,"hatchBuf"));

	/* set up global info about hatch mark buffer */
	maxHatch = hatchBuf + (HBSIZE - 1);
	lastHatch = hatchBuf;

	/* set up global info about contour levels */
	midLVal = lVal + cLevels / 2;
	lastLVal = lVal + (cLevels - 1);
}

/**************************************************/
/* PlotBox - draw all contours inside a given box */
/**************************************************/

static void PlotBox()
{
	register double *curLVal; /* pointer to value of contour level */
				/* being plotted */
	double maxVal;	/* highest value of a corner of box */
	double minVal;	/* lowest value of a corner of box */
	double step;	/* a detectable change in value */

	/* find highest and lowest values of the corners of box */
	minVal = maxVal = box.lowerLeft;
	if(box.lowerRight < minVal)
		minVal = box.lowerRight;
	else if(box.lowerRight > maxVal)
		maxVal = box.lowerRight;
	if(box.upperLeft < minVal)
		minVal = box.upperLeft;
	else if(box.upperLeft > maxVal)
		maxVal = box.upperLeft;
	if(box.upperRight < minVal)
		minVal = box.upperRight;
	else if(box.upperRight > maxVal)
		maxVal = box.upperRight;
	if(maxVal < *lVal)
		return;
	if(minVal > *lastLVal)
		return;
	if(maxVal > *lastLVal)
		maxVal = *lastLVal;
	step = (maxVal - minVal) / 100.;

	/* find first contour level that passes through box */
	lVal[cLevels] = maxVal + step;
	lVal[-1] = minVal - step;
	curLVal = midLVal;
	if(*curLVal < minVal)
		do
			curLVal++;
		while(*curLVal < minVal);
	else
	{
		while(*curLVal > minVal)
			curLVal--;
		if(*curLVal <= minVal)
			curLVal++;
	}
	curLevel = curLVal - lVal;

	/* draw contour lines for all contours that pass through box */
	while(*curLVal <= maxVal)
	{
		curInBuf->level = curLevel;
		curInBuf->plotted = 0;
		endPoint = curInBuf->endPoint;
		if(box.upperRight < *curLVal)
			if(box.upperLeft < *curLVal)
				if(box.lowerRight < *curLVal)
					if(box.lowerLeft < *curLVal)
					{
						curInBuf--;
						numInBuf--;
					}
					else
					{
						curInBuf->topology = 1;
						LeftPoint();
						endPoint++;
						BottomPoint();
					}
				else
					if(box.lowerLeft < *curLVal)
					{
						curInBuf->topology = 2;
						BottomPoint();
						endPoint++;
						RightPoint();
					}
					else
					{
						curInBuf->topology = 3;
						LeftPoint();
						endPoint++;
						RightPoint();
					}
			else
				if(box.lowerRight < *curLVal)
					if(box.lowerLeft < *curLVal)
					{
						curInBuf->topology = 4;
						LeftPoint();
						endPoint++;
						TopPoint();
					}
					else
					{
						curInBuf->topology = 5;
						BottomPoint();
						endPoint++;
						TopPoint();
					}
				else
					if(box.lowerLeft < *curLVal)
						Saddle(6);
					else
					{
						curInBuf->topology = 7;
						RightPoint();
						endPoint++;
						TopPoint();
					}
		else
			if(box.upperLeft < *curLVal)
				if(box.lowerRight < *curLVal)
					if(box.lowerLeft < *curLVal)
					{
						curInBuf->topology = 8;
						RightPoint();
						endPoint++;
						TopPoint();
					}
					else
						Saddle(9);
				else
					if(box.lowerLeft < *curLVal)
					{
						curInBuf->topology = 10;
						BottomPoint();
						endPoint++;
						TopPoint();
					}
					else
					{
						curInBuf->topology = 11;
						LeftPoint();
						endPoint++;
						TopPoint();
					}
			else
				if(box.lowerRight < *curLVal)
					if(box.lowerLeft < *curLVal)
					{
						curInBuf->topology = 12;
						LeftPoint();
						endPoint++;
						RightPoint();
					}
					else
					{
						curInBuf->topology = 13;
						BottomPoint();
						endPoint++;
						RightPoint();
					}
				else
					if(box.lowerLeft < *curLVal)
					{
						curInBuf->topology = 14;
						LeftPoint();
						endPoint++;
						BottomPoint();
					}
					else
					{
						curInBuf--;
						numInBuf--;
					}
		curInBuf++;
		if(numInBuf++ == LBSIZE - 1)
			FlushLines();
		curLevel++;
		curLVal++;
	}
}

/***************************************/
/* Routines for handling saddle points */
/***************************************/

static void Saddle(topology)
	int topology;		/* topology of box */
{
	static double total;	/* total value of all four corners */

	total = box.lowerLeft + box.lowerRight +
		box.upperLeft + box.upperRight;
	if(total > lVal[curLevel] * 4)
		if(topology & 1)
			LtoT_BtoR(topology);
		else
			LtoB_RtoT(topology);
	else
		if(topology & 1)
			LtoB_RtoT(topology);
		else
			LtoT_BtoR(topology);
}

static void LtoB_RtoT(topology)
	int topology;		/* topology of box */
{
	curInBuf->topology = topology ^ 8;
	LeftPoint();
	endPoint++;
	BottomPoint();
	curInBuf++;
	if(numInBuf++ == LBSIZE - 1)
		FlushLines();
	curInBuf->level = curLevel;
	curInBuf->plotted = 0;
	endPoint = curInBuf->endPoint;
	curInBuf->topology = topology ^ 1;
	RightPoint();
	endPoint++;
	TopPoint();
}

static void LtoT_BtoR(topology)
	int topology;		/* topology of box */
{
	curInBuf->topology = topology ^ 2;
	LeftPoint();
	endPoint++;
	TopPoint();
	curInBuf++;
	if(numInBuf++ == LBSIZE - 1)
		FlushLines();
	curInBuf->level = curLevel;
	curInBuf->plotted = 0;
	endPoint = curInBuf->endPoint;
	curInBuf->topology = topology ^ 4;
	BottomPoint();
	endPoint++;
	RightPoint();
}

/****************************************************/
/* Routines for putting endpoints on sides of a box */
/****************************************************/

static void LeftPoint()
{
	register CLINE *link;		/* pointer to linked line */

	endPoint->side = LSIDE;
	endPoint->distance = 0;
	endPoint->x = box.x;
	if(link = curLinks[-cLevels + curLevel].right)
	{
		endPoint->y = link->endPoint[link->rightEnd].y;
		link->endPoint[link->rightEnd].link = curInBuf;
	}
	else
		endPoint->y =
		   Intersect(box.y,box.height,
		   	     box.lowerLeft,box.upperLeft);
	endPoint->link = link;
}

static void BottomPoint()
{
	register CLINE *link;		/* pointer to linked line */

	endPoint->side = BSIDE;
	endPoint->distance = 0;
	if(link = curLinks[prevRow + curLevel].top)
	{
		endPoint->x = link->endPoint[link->topEnd].x;
		link->endPoint[link->topEnd].link = curInBuf;
	}
	else
		endPoint->x =
		   Intersect(box.x,box.width,
			     box.lowerLeft,box.lowerRight);
	endPoint->y = box.y;
	endPoint->link = link;
}

static void RightPoint()
{
	endPoint->side = RSIDE;
	endPoint->distance = 0;
	endPoint->x = box.x + box.width;
	endPoint->y =
	   Intersect(box.y,box.height,box.lowerRight,box.upperRight);
	endPoint->link = 0;
	curLinks[curLevel].right = curInBuf;
	curInBuf->rightEnd = endPoint - curInBuf->endPoint;
}

static void TopPoint()
{
	endPoint->side = TSIDE;
	endPoint->distance = 0;
	endPoint->x =
	   Intersect(box.x,box.width,box.upperLeft,box.upperRight);
	endPoint->y = box.y + box.height;
	endPoint->link = 0;
	curLinks[curLevel].top = curInBuf;
	curInBuf->topEnd = endPoint - curInBuf->endPoint;
}

/**************************************/
/* FlushLines - flush the line buffer */
/**************************************/

static void FlushLines()
{
	if(hatchLen)
	{
		SaveUndone();
		DumpLines();
#if STORE
		GetUndone();
#endif /*STORE*/
	}
	else
	{
		DumpLines();
		bzero(linkArray,lASize);
	}
}

/*********************************************/
/* SaveUndone - save all unfinished contours */
/*********************************************/

static void SaveUndone()
{
	CLINKS *tempLinks;		/* temporary pointer to links that */
					/* connect with unfinished contours */

#if STORE
	/* start storage */
	curInStore = storeBuf;
#endif /*STORE*/

	/* store all contours that are linked to the top of the row */
	/* of boxes being worked on */
	if(curLinks > lARow1End)
		tempLinks = lARow1End + cLevels;
	else
		tempLinks = linkArray + cLevels;
	while(tempLinks != curLinks)
	{
		if(tempLinks->top)
			SaveContour(&(tempLinks->top));
		tempLinks++;
	}

	/* store all contours linked to top and right of box being worked on */
	while(tempLinks != curLinks + cLevels)
	{
		if(tempLinks->top)
			SaveContour(&(tempLinks->top));
		if(tempLinks->right)
			SaveContour(&(tempLinks->right));
		tempLinks++;
	}

	/* store contours linked to top of previous line where new */
	/* boxes haven't yet been computed */
	tempLinks += prevRow;
	while(tempLinks != lARow1End && tempLinks != lARow2End)
	{
		if(tempLinks->top)
			SaveContour(&(tempLinks->top));
		tempLinks++;
	}

#if STORE
	/* check for error */
	if(curInStore - storeBuf >= numInBuf)
		error_("Map too complicated -- try fewer contour levels");
#endif /*STORE*/
}

/********************************************/
/* SaveContour - save an incomplete contour */
/********************************************/

static void SaveContour(link)
	CLINE **link;		/* pointer to link to first line of contour */
				/* (the saved line will be in a different */
				/* place, so its link must be updated */
				/* hence the added indirection */
{
#if STORE
	CLINE *line;		/* line of contour being stored */
	CLINE *prevLine;	/* previously stored line */
	CLINE *newAddr;		/* new address of line being stored */
	CLINE *firstInStore;	/* first line of contour in storeBuf */
#endif /*STORE*/
/* printf("SaveContour entered\n"); */

	/* if this line is "plotted," it means it is already stored */
	/* just change link */
	if((*link)->plotted)
		*link = (*link)->newAddr;
	else
	{
/*		error("Need to save an unplotted contour"); */
#if STORE
		/* otherwise, store contour labelling it plotted */
		firstInStore = curInStore;
		newAddr = lineBuf + (curInStore - storeBuf);
		line = *link;
		prevLine = 0;
		while(line)
		{
			*curInStore++ = *line;
			line->newAddr = newAddr++;
			line->plotted = 1;
			if(line->endPoint->link == prevLine)
			{
				prevLine = line;
				line = line->endPoint[1].link;
			}
			else
			{
				prevLine = line;
				line = line->endPoint->link;
			}
		}

		/* prepare linkage information */
		*link = (*link)->newAddr;
		line = firstInStore;
		while(line < curInStore)
		{
			if(line->endPoint->link)
				line->endPoint->link =
					line->endPoint->link->newAddr;
			if(line->endPoint[1].link)
				line->endPoint[1].link =
					line->endPoint[1].link->newAddr;
			line++;
		}
#endif /*STORE*/
	}
}

#if STORE
/*************************************************************/
/* GetUndone - retrieve unfinished contours that were stored */
/*************************************************************/

static void GetUndone()
{
	CLINE *stored;		/* pointer to stored line being retrieved */

	for(stored = storeBuf; stored < curInStore; stored++)
	{
		*curInBuf++ = *stored;
		numInBuf++;
	}
}
#endif /*STORE*/

/**************************/
/* DumpLines - plot lines */
/**************************/

static void DumpLines()
{
	CLINE *nextContour;	/* pointer to first line in next contour */
				/* to be plotted */
	CLINE *curLine;		/* line currently being plotted */
	CLINE *prevLine;	/* line just plotted */
	int curLType;		/* current line type */
	float area;		/* signed area inside contours for */
				/* deciding whether to draw hatchmarks */
	double tempHatchLen;	/* temporary hatch length -- zero if */
				/* hatch marks should not be drawn */
	CLINE *tLine;		/* temporary pointer to lines for computing */
				/* signed area inside contour */
	CLINE *pLine;		/* previous value of tLine */
	ENDPOINT e1,e2;		/* kluge jobs */

	/* current line type is meaningless */
	curLType = -1;

	/* loop through each contour */
	nextContour = lineBuf;
	while(nextContour < curInBuf)
	{
		/* only plot if this is a new contour */
		if(!nextContour->plotted)
		{
			/* set line type for contour */
			curLine = nextContour;
			if(lType[curLine->level] != curLType)
			{
				curLType = lType[curLine->level];
				Plinetype(curLType);
			}
	
			/* find first line of contour */
			prevLine = curLine;
			do
			{
				if(curLine->endPoint->link != prevLine)
				{
					prevLine = curLine;
					curLine = curLine->endPoint->link;
				}
				else
				{
					prevLine = curLine;
					curLine = curLine->endPoint[1].link;
				}
			}
			while(curLine && curLine != nextContour);
			if(!curLine)
			{
				curLine = prevLine;
				prevLine = 0;
			}

			/* find out if hatchmarks should be drawn */
			if(hatchLen)
			{
				tLine = curLine;
				pLine = prevLine;
				area = 0;
				do
				{
					if(tLine->endPoint->link != pLine)
					{
						area += tLine->endPoint->x *
						        tLine->endPoint[1].y -
						        tLine->endPoint[1].x *
							tLine->endPoint->y;
						pLine = tLine;
						tLine = tLine->endPoint->link;
					}
					else
					{
						area += tLine->endPoint[1].x *
							tLine->endPoint->y -
							tLine->endPoint->x *
							tLine->endPoint[1].y;
						pLine = tLine;
						tLine = tLine->endPoint[1].link;
					}
				}
				while(tLine && tLine != curLine);
				if(!tLine)
				{
					if(!pLine->endPoint->link)
						e1 = *(pLine->endPoint);
					else
						e1 = pLine->endPoint[1];
					if(!curLine->endPoint->link)
						e2 = *(curLine->endPoint);
					else
						e2 = curLine->endPoint[1];
					area += e2.x * e1.y - e1.x * e2.y;
				}
			}
	
			/* move to first point on contour */
			if(curLine->endPoint->link != prevLine)
				Pmove(curLine->endPoint[1].x,
				      curLine->endPoint[1].y);
			else
				Pmove(curLine->endPoint->x,
				      curLine->endPoint->y);
	
			/* start hatchmarks */
			if(hatchLen)
				StartHatch();
	
			/* draw remainder of contour */
			while(curLine)
			{
				if(curLine->plotted)
					break;
				curLine->plotted = 1;
				if(curLine->endPoint->link == prevLine)
				{
					Pline(curLine->endPoint[1].x,
					      curLine->endPoint[1].y);
					if(hatchLen)
						CheckHatch(curLine,0);
					prevLine = curLine;
					curLine = curLine->endPoint[1].link;
				}
				else
				{
					Pline(curLine->endPoint->x,
					      curLine->endPoint->y);
					if(hatchLen)
						CheckHatch(curLine,1);
					prevLine = curLine;
					curLine = curLine->endPoint->link;
				}
			}
	
			/* plot hatch marks */
			if(hatchLen)
				DumpHatch();
		}

		/* go on to next contour */
		nextContour++;
	}

	/* clear buffers */
	curInBuf = lineBuf;
	numInBuf = 0;
}

/********************************************/
/* StartHatch - start recording hatch marks */
/********************************************/

static void StartHatch()
{
	disFromHatch = 0.;
}

/********************************************************************/
/* CheckHatch - make all hatch marks that should be on a given line */
/********************************************************************/

static void CheckHatch(line,prevPoint)
	CLINE *line;		/* line to make hatch marks on */
	int prevPoint;		/* number of end point that has already */
				/* been plotted (0 or 1) */
{
	static double lineLen;	/* length of line */
	static double newDis;	/* distance from last hatch mark after this */
				/* line has been drawn */
	static double disFromEnd;/* distance of each hatch mark from the end */
				/* of the line */
	double sqrt();

	/* compute length of line */
	lineLen = sqrt((line->endPoint->x - line->endPoint[1].x) *
		       (line->endPoint->x - line->endPoint[1].x) +
		       (line->endPoint->y - line->endPoint[1].y) *
		       (line->endPoint->y - line->endPoint[1].y));

	/* make hatch marks */
	newDis = disFromHatch + lineLen;
	disFromEnd = 0;
	while(newDis > hatchSpace)
	{
		disFromEnd = disFromEnd + hatchSpace;
		newDis = newDis - hatchSpace;
		MakeHatch(line,lineLen,disFromEnd - disFromHatch,prevPoint);
	}
	disFromHatch = newDis;
}

/*********************************/
/* MakeHatch - make a hatch mark */
/*********************************/

static void MakeHatch(line,lineLen,disFromEnd,prevPoint)
	CLINE *line;		/* line on which to make hatch mark */
	double lineLen;		/* length of line */
	double disFromEnd;	/* distance from endpoint of line at which */
				/* to make hatch mark */
	int prevPoint;		/* number of endpoint that disFromEnd refers */
				/* to (i.e. the last endpoint plotted) */
{
	static double slope;	/* slope of hatch mark */
	static double xOff;	/* x offset of endpoint of hatch mark not */
				/* on contour */
	static double yOff;	/* y offset */
	static double x0,y0,x1,y1; /* endpoints of line */
	double sqrt();

	/* get endpoints of line */
	x0 = line->endPoint[prevPoint].x;
	y0 = line->endPoint[prevPoint].y;
	x1 = line->endPoint[1 - prevPoint].x;
	y1 = line->endPoint[1 - prevPoint].y;

	/* compute endpoint of hatch mark on contour */
	lastHatch->x0 = x0 + disFromEnd * (x1 - x0) / lineLen;
	lastHatch->y0 = y0 + disFromEnd * (y1 - y0) / lineLen;

	/* compute offsets for other end of hatch mark */
	if(x1 == x0)
	{
		xOff = hatchLen;
		yOff = 0;
	}
	else if(y1 == y0)
	{
		xOff = 0;
		yOff = hatchLen;
		slope = 1;		/* fix to make things work if */
					/* topology is 3 or 12 */
	}
	else
	{
		slope = -(x1 - x0) / (y1 - y0);
		xOff = hatchLen / sqrt(slope * slope + 1.);
		yOff = slope * xOff;
	}

	/* compute endpoint of hatch mark not on contour */
	switch(line->topology)
	{
		case 1:
		case 4:
		case 5:
		case 7:
		case 13:
			lastHatch->x1 = lastHatch->x0 + xOff;
			lastHatch->y1 = lastHatch->y0 + yOff;
			break;
		case 2:
		case 8:
		case 10:
		case 11:
		case 14:
			lastHatch->x1 = lastHatch->x0 - xOff;
			lastHatch->y1 = lastHatch->y0 - yOff;
			break;
		case 3:
			if(slope > 0)
			{
				lastHatch->x1 = lastHatch->x0 + xOff;
				lastHatch->y1 = lastHatch->y0 + yOff;
			}
			else
			{
				lastHatch->x1 = lastHatch->x0 - xOff;
				lastHatch->y1 = lastHatch->y0 - yOff;
			}
			break;
		case 12:
			if(slope < 0)
			{
				lastHatch->x1 = lastHatch->x0 + xOff;
				lastHatch->y1 = lastHatch->y0 + yOff;
			}
			else
			{
				lastHatch->x1 = lastHatch->x0 - xOff;
				lastHatch->y1 = lastHatch->y0 - yOff;
			}
			break;
		default: error_("Illegal box topology for hatch mark");
	}

	/* make sure there is enough room in hatch mark buffer for */
	/* hatch marks to come */
	if(lastHatch++ == maxHatch)
	{
		DumpHatch();
		Pmove(x1,y1);
	}
}

/********************************/
/* DumpHatch - plot hatch marks */
/********************************/

static void DumpHatch()
{
	while(lastHatch-- > hatchBuf + 1)
	{
		Pmove(lastHatch->x0,lastHatch->y0);
		Pline(lastHatch->x1,lastHatch->y1);
		lastHatch--;
		Pmove(lastHatch->x1,lastHatch->y1);
		Pline(lastHatch->x0,lastHatch->y0);
	}
	if(lastHatch == hatchBuf)
	{
		Pmove(lastHatch->x0,lastHatch->y0);
		Pline(lastHatch->x1,lastHatch->y1);
	}
	else
		lastHatch = hatchBuf;
}
