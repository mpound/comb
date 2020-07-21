/********************/
/* Guts of graphics */
/********************/

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <math.h>
# include <ctype.h>

# include "../error/C.h"
# include "../graphics/C.h"
# include "include.h"
#include "../main/C.h"
#include "../main/dragon.h"

#define bzero(s, n) memset(s, 0, n)

#define TWOPI 6.28318530718

PICTURE *cPic = 0;		/* current picture */
PICTURE *oPic = 0;		/* old picture */
short int numForms;		/* number of formats */
char plotting;			/* flag set while plotting to a terminal */
char fDirName[DIRNMLEN] = {0};	/* name of directory from which to get */
				/* formats */
char termType[TERMNMLEN] = {0};	/* name of terminal for which following */
				/* global variables apply */
short int chX[TXTTYPES]; 	/* x size of characters */
short int chY[TXTTYPES]; 	/* y size of characters */
int xRes;			/* x resolution of screen */
int yRes;			/* y resolution of screen */
float xSize;			/* x size of screen (in inches) */
float ySize;			/* y size of screen (in inches) */
short bxCol = 0,bxRow = 0;	/* col, row of current box */
int labelsUC = 0;		/* logical value controls label style */
GRFORMAT formats[MAXFORMS];	/* catalog of formats */

static ovrOn;			/* 1 if overflow marking is on */
static ALLARGS args;		/* Union containing args for next instruction */

extern off_t lseek();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* guts.c */
static void Mcursor P_((int x, int y));
static void Dline P_((int x, int y));
static void InsrtStr P_((char *str));
static int CheckSpace P_((register char *newEnd));
static void mcursor P_((int x, int y));
static void dline P_((int x, int y));
static void dboxfill P_((int x, int y, short int color));
static void ptext P_((int place, char *text));
static void dtext P_((int sideways, char *text));
static void dgrnum P_((int x, int y, double num));
static void dxlabel P_((int x, double num));
static void dylabel P_((int y, double num));
static void setwindow P_((int window));
static void setltype P_((int type));
static void setttype P_((int type));
static void ctext P_((int sideways, char *text));
static void lovrflow P_((int from, int to));
static void bovrflow P_((int from, int to));
static void rovrflow P_((int from, int to));
static void tovrflow P_((int from, int to));
static void GtGen P_((void));
static void GtWindow P_((FILE *file, register SPECWINDOW *window, char *errStr));
static void GtWinLine P_((FILE *file, char *errStr, char *same, unsigned short int *pixel, register int *cx, register int *cy));
static char *FGetLine P_((FILE *file, register char *line));
static int FindForm P_((char *formName));
static void LineCommands P_((register int ox, register int oy, register int nx, register int ny));
static void ovrflow P_((int from, int to, int max, void (*side)()));
static void ScaleXy P_((register PICTURE *picP, double x, double y, register int *tx, register int *ty));
#undef P_


/********************************/
/* Pstart - start a new picture */
/********************************/

void Pstart()
{
	int temp, temp2;
	register PICTURE *picP;

	/* initialize graphics */
	GtGen();

	/* If there is no picture or there are multiple boxes, start a fresh
	 * picture. */
	if(!cPic || xBoxes > 1 || yBoxes > 1)
		Pcreate(&cPic, PICBLOCK);

	/* set up default values for cPic */
	picP = cPic;
	temp = (picP)->fullSize;
	temp2 = picP->numLinks;
	bzero(picP,sizeof(PICTURE));
	(picP)->fullSize = temp;
	(picP)->iBufSize = temp - sizeof(PICTURE) + 1;
	picP->numLinks = temp2;
	(picP)->start = (picP)->iBuf;
	(picP)->right = XDIM;
	(picP)->top = YDIM;
	(picP)->xScale = 1;
	(picP)->yScale = 1;
	(picP)->unitsRatio = 1;
	Pformat("full plot");

	/* set startup values for cPic */
	(picP)->windowNum = 1;
	(picP)->gWindow = (picP)->format->gWindow + 1;
	(picP)->tType = 1;
	(picP)->x = UNKNOWN;
	(picP)->y = UNKNOWN;
	ovrOn = 1;
}

/********************************************************/
/* Pscreen - manipulate a picture that is on the screen */
/********************************************************/

void Pscreen(ixBox,iyBox)
	int ixBox;		/* x box containing picture */
	int iyBox;		/* y box containing picture */
{
	int xBox;		/* x box containing picture */
	int yBox;		/* y box containing picture */

	/* ignore ixBox and iyBox if there is only one box on screen */
	if(xBoxes == 1 && yBoxes == 1)
	{
		xBox = 0;
		yBox = 0;
	}
	else
	{
		xBox = ixBox;
		yBox = iyBox;
	}

	/* make sure this box can be continued */
	if(xBox < 0 || xBox >= xBoxes ||
	   yBox < 0 || yBox >= yBoxes)
		error_("Illegal box to continue plotting");
	if(!boxes[yBox * xBoxes + xBox])
		error_("No plot in box to continue");

	/* set up box as current picture */
	Plink(boxes[yBox * xBoxes + xBox], &cPic);
	ovrOn = 1;
}

/***************************************************/
/* Pfull - report whether a box has a picture in it*/
/***************************************************/

int Pfull(ixBox,iyBox)
	int ixBox;		/* x box containing picture */
	int iyBox;		/* y box containing picture */
{
	return(boxes[iyBox * xBoxes + ixBox] != 0);
}

/*********************************************
 * PsetType(type) - set the picture type in c1*
 **********************************************/

void PsetType(type)
int type;
{
	if( !cPic)
		error("No picture to set the type of");
	(cPic)->htype = type;
}

/********************************************
 * PgetType() - get the picture type from c1*
 ********************************************/

int PgetType()
{
	if( !cPic )
		return(-1);
	return( (cPic)->htype );
}

#if 0 /*RWW disable for now 4/20/92*/
/**********************************************/
/* Psave - make a copy of the current picture */
/**********************************************/

void Psave(cpy)
PICTURE **cpy;		/* copy of picture */
{
	Pcopy(cpy,cPic);
}

/******************************************/
/* Prestore - restore the current picture */
/******************************************/

void Prestore(cpy)
	PICTURE *cpy;		/* copy of picture to be restored */
{
	GtGen();
	if(oPic)
	{
		cPic = oPic;
		oPic = 0;
	}
	Pcopy(&cPic,cpy);
}
/************************************************/
/* Pwrite - write the current picture to a file */
/************************************************/

void Pwrite(fileName)
	char *fileName;
{
	int file;		/* file pointer */
	char errStr[100];	/* error string */

	sprintf(errStr,"graphics picture file, '%s'",fileName);
	file = SafeOpen(errStr,fileName,O_CREAT,0666);
	SafeClose(file);
	file = SafeOpen(errStr,fileName,O_WRONLY);
	write(file,(char *)cPic,(cPic)->fullSize);
	SafeClose(file);
}

/**************************************/
/* Pread - read a picture from a file */
/**************************************/

void Pread(fileName)
	char *fileName;		/* name of file to read from */
{
	int file;		/* file descriptor */
	char errStr[100];	/* error string */
	PICTURE header;		/* header of picture */
	static char chkAllocName[] = "Pread";
	register PICTURE *picP;

	GtGen();
	sprintf(errStr,"graphics picture file, '%s'",fileName);
	file = SafeOpen(errStr,fileName,O_RDONLY);
	read(file,(char *)&header,sizeof(PICTURE));
	if(oPic)
	{
		cPic = oPic;
		oPic = 0;
	}
	if(cPic)
	{
	    if((cPic)->fullSize < header.fullSize)
	    {
		ChkFree((char *)cPic);
		cPic = (PICTURE *)(ChkAlloc(header.fullSize,chkAllocName));
	    }
	}
	else
	{
		cPic = (PICTURE *)(ChkAlloc(header.fullSize,chkAllocName));
	}
	picP = cPic;
	lseek(file,0L,0);
	read(file,(char *)picP,header.fullSize);
	SafeClose(file);
	(picP)->format = formats + (picP)->formatNum;
	(picP)->gWindow = (picP)->format->gWindow + (picP)->windowNum;
	ovrOn = 1;
}

/*****************************************/
/* Pcopy - copy one picture into another */
/*****************************************/

void Pcopy(dest,source)
PICTURE **dest;		/* destination picture */
PICTURE *source;	/* source picture */
{
	struct of_block		/* structure for moving whole block */
	{
		char data[PICBLOCK];
	} *src,*dst;
	int blocks;		/* number of blocks that must be copied */
	int block;		/* block being copied */
	int tempSize;		/* temporary variable for maintaining */
				/* the correct fullSize of dest */
	static char chkAllocName[] = "Pcopy";

	/* if no source, just clear dest */
	if(!source)
	{
		if(dest && *dest)
			(*dest)->start = (*dest)->iBuf;
		return;
	}

	/* check that dest has enough room */
	if(dest && *dest) {
		if((*dest)->fullSize < (source)->fullSize) {
			Pfree(*dest);
			*dest = 0;
		}
	}


	/* if dest is empty (either because it was empty to */
	/* begin with or it wasn't big enough to hold source */
	/* and was emptied above), allocate memory for it */
	if(!*dest)
	{
		*dest = (PICTURE *)(ChkAlloc((source)->fullSize,chkAllocName));
		(*dest)->fullSize = (source)->fullSize;
	}

	/* copy picture */
	tempSize = (*dest)->fullSize;
	src = (struct of_block *)(source);
	dst = (struct of_block *)(*dest);
	blocks = (source)->fullSize / PICBLOCK;
	for(block = 0; block < blocks; block++)
		*dst++ = *src++;
	(*dest)->fullSize = tempSize;
	(*dest)->iBufSize = (*dest)->fullSize - sizeof(PICTURE) + 1;
}
#endif /* 0  RWW*/

/******************************************************************/
/* Pformdir - set the directory that formats are to be drawn from */
/******************************************************************/

void Pformdir(name)
	char *name;	/* name of directory */
{
	int len;	/* length of name */

	len = strlen(name);
	strcpy(fDirName,name);
	if(fDirName[len - 1] != '/')
	{
		fDirName[len] = '/';
		fDirName[len + 1] = 0;
	}
}

/***************************************************/
/* Pformat - set the format of the current picture */
/***************************************************/

void Pformat(formName)
	char *formName;		/* format name */
{
	register PICTURE *picP = cPic;
	/* check that the format of the current picture may be set */
	if((picP)->iBufEnd > 0)
		error_("Trying to change format of existing picture");

	/* find format number */
	(picP)->formatNum = FindForm(formName);

	/* update pointers */
	(picP)->format = formats + (picP)->formatNum;
	(picP)->gWindow = (picP)->format->gWindow + (picP)->windowNum;
}

/***********************************************************/
/* Pwindow - set the current window of the current picture */
/***********************************************************/

void Pwindow(windowNum)
	int windowNum;		/* number of window to set to */
				/* (0 = whole screen, 1 = graph window, */
				/* more than 1 = window described in */
				/* format) */
{
	/* make intermediate code command setting window */
	setwindow(windowNum);

	/* update current information in picture */
	(cPic)->windowNum = windowNum;
	(cPic)->gWindow = (cPic)->format->gWindow + windowNum;
}

/***********************************************
 * Pcreate - Allocate memory for a new picture *
 ***********************************************/

void Pcreate(pic, size)
PICTURE **pic;
int size;			/* Total size including header */
{
	Punlink(pic);
	*pic = (PICTURE *)(ChkAlloc(size,"Pcreate"));
	(*pic)->fullSize = size;
	(*pic)->numLinks = 1;
}

/****************************************************************************
 * Plink - link a picture to a pointer after unlinking any previous picture *
 ****************************************************************************/

void Plink(picP, pic)
PICTURE *picP;
PICTURE **pic;
{
	Punlink(pic);
	*pic = picP;
	picP->numLinks++;
}

/***************************************************************************
 * Plunink - unlink a picture from a pointer.  Deallocate memory if needed *
 ***************************************************************************/

void Punlink(pic)
PICTURE **pic;
{
	if(*pic) {
	    if(--(*pic)->numLinks <= 0)
		ChkFree((char *)*pic);
	    *pic = (PICTURE *)0;
	}
}

#if 1 /*RWW*/
/***********************************************************/
/* Pfree - free up the space allocated for a given picture */
/***********************************************************/

void Pfree(picture)
	PICTURE *picture;	/* picture to be deallocated */
{
	/* if no picture, go away */
	if(!picture)
		return;

	/* free picture structure, and pointer to picture structure */
	ChkFree((char *)picture);
}
#endif /*0*/

/******************************************************/
/* Pbounds - set the boundaries of the graphics window */
/******************************************************/

void Pbounds(left,bottom,right,top)
	double left;		/* value of left boundary */
	double bottom;		/* value of bottom boundary */
	double right;		/* value of right boundary */
	double top;		/* value of top boundary */
{
	Plogbounds(left,bottom,right,top,0,0);
}

/********************************************************************
 * Punitsratio - Set the size ratio of units on x axis to those on y.
 ********************************************************************/

void Punitsratio(ratio)
double ratio;
{
	(cPic)->unitsRatio = ratio;
}

/******************************************************************/
/* Plogbounds - set the logarithmic bounds of the graphics window */
/******************************************************************/

void Plogbounds(left,bottom,right,top,xIsLog,yIsLog)
	double left;		/* value of left of window */
	double bottom;		/* value of bottom of window */
	double right;		/* value of right of window */
	double top;		/* value of top of window */
	char xIsLog;		/* 1 if x is logarithmic */
	char yIsLog;		/* 1 if y is logarithmic */
{
	/* make sure boundaries make sense */
	if(left == right)
		error_("Left and right boundaries are equal");
	if(bottom == top)
		error_("Bottom and top boundaries are equal");

	/* make sure it is ok to set boundaries */
	if((cPic)->iBufEnd > 0)
		error_("Trying to change bounds of picture in progress");

	/* set boundaries */
	ReBound(left,bottom,right,top,xIsLog,yIsLog);
}

/********************************************************/
/* ReBound - reset the boundaries of the current picture */
/********************************************************/

void ReBound(left,bottom,right,top,xIsLog,yIsLog)
	double left;		/* left boundary */
	double bottom;		/* bottom boundary */
	double right;		/* right boundary */
	double top;		/* top boundary */
	int xIsLog;		/* 1 if x is logarithmic */
	int yIsLog;		/* 1 if y is logarithmic */
{
	double log();
	register PICTURE *picP = cPic;

	/* set boundaries */
	if(xIsLog)
	{
		if(left <= 0 || right <= 0)
			error_("Zero or negative logarithmic scale");
		(picP)->left = log(left);
		(picP)->right = log(right);
	}
	else
	{
		(picP)->left = left;
		(picP)->right = right;
	}
	if(yIsLog)
	{
		if(bottom <= 0 || top <= 0)
			error_("Zero or negative logarithmic scale");
		(picP)->bottom = log(bottom);
		(picP)->top = log(top);
	}
	else
	{
		(picP)->bottom = bottom;
		(picP)->top = top;
	}
	(picP)->xLog = xIsLog;
	(picP)->yLog = yIsLog;

	/* set scale */
	(picP)->xScale = XDIM / ((picP)->right - (picP)->left);
	(picP)->yScale = YDIM / ((picP)->top - (picP)->bottom);
}

/**********************************************/
/* Povrflow - turn overflow marking on or off */
/**********************************************/

/* returns previous value of ovrOn */
int Povrflow(flag)
	int flag;		/* 1 if turn on, 0 if turn off */
{
	int temp;		/* temporary variable for holding */
				/* previous value of ovrOn */

	if(flag != 1 && flag != 0)
		error_("Illegal graphics overflow marking on/off flag");
	temp = ovrOn;
	ovrOn = flag;
	return(temp);
}

/*****************************************************************/
/* Pborder - make a rectangular border around the current window */
/*****************************************************************/

void Pborder()
{
	mcursor(0,0);
	dline(XDIM,0);
	dline(XDIM,YDIM);
	dline(0,YDIM);
	dline(0,0);
}
/*********************************************************************/
/* Pcircborder - make an elliptical border around the current window */
/*********************************************************************/

void Pcircborder()
{
	int nseg, iseg;

	nseg = 3 *  sqrt((double) xRes);	/* number of segments in polygon */
	if (nseg < 128 ) nseg = 768;
	mcursor(XDIM, YDIM/2);
	for ( iseg = 1; iseg <= nseg; iseg++) {
		dline( (int) ((1. + cos(( TWOPI * iseg)/nseg ))/2. * XDIM), 
			(int) ((1. + sin(( TWOPI * iseg)/nseg ))/2. * YDIM));
	}
}

/***************************/
/* Pfigure - make a figure */
/***************************/

void Pfigure(x,y,figure,width,height,filled)
	double x;	/* x location at which to put figure */
	double y;	/* y location at which to put figure */
	char figure;	/* type of figure */
	double width;	/* width of figure */
	double height;	/* height of figure */
	int filled;     /* 1: figure is filled, 0: figure is open */
{
	int tx;		/* true x */
	int ty;		/* true y */
	int htw;	/* half true width */
	int hth;	/* half true height */
	double sc;	/* variable for holding hairy constants */
	double sina;	/* sine of angle for drawing ellipse */
	double tsina;	/* temporary variable for holding new value of */
			/* sina while computing new value of cosa */
	double cosa;	/* cosine of angle for drawing ellipse */
	double sinb;	/* sine of increment angle for drawing ellipse */
	double cosb;	/* cosine of increment angle for drawing ellipse */
	int count;	/* counter for drawing ellipse */
	int xsgn, ysgn;	/* for filling figure */
	int ohtw, ohth; /* original tx + wth, ty + hth */ 
	double aspect;     /* aspect ratio for drawing filled triangles */
	int loop;        /* loops for ellipse */

	/* find true x and y */
	ScaleXy(cPic,x,y,&tx,&ty);

	/* find true width and height */
	ScaleXy(cPic,x + width / 2,y + height / 2,&htw,&hth);
	ohtw=htw;
	ohth=hth;
	htw -= tx;
	hth -= ty;
	xsgn = (cPic->xScale > 0) ? 1 : -1;
	ysgn = (cPic->yScale > 0) ? 1 : -1;
#if 0
printf("%d %d %d %d %g %g %d %d\n",tx,ty,ohth,ohtw,cPic->xScale,cPic->yScale, xRes, yRes);
#endif

/* To fill figures, simply draw a bunch of concentric figures, stopping when
 * one dimension is < 0. 
 */
	/* make figure */
	switch(figure)
	{
		case F_X:
			Mcursor(tx - htw,ty - hth);
			Dline(tx + htw,ty + hth);
			Mcursor(tx + htw,ty - hth);
			Dline(tx - htw,ty + hth);
			break;
		case F_RECTANGLE:
			do { /* we always want at least one pass */
			 Mcursor(tx - htw,ty - hth);
			 Dline(tx + htw,ty - hth);
			 Dline(tx + htw,ty + hth);
			 Dline(tx - htw,ty + hth);
			 Dline(tx - htw,ty - hth);
                /* for filling, take steps which are half a screen pixel 
                   This is empirical and doesnt leave empty spots */
			 htw -= xsgn*((XDIM/xRes)>>1);   
			 hth -= ysgn*((YDIM/yRes)>>1); 
		/* if not filled or not finished filling, loop again */
			} while(filled && (ysgn*hth > 0)&& (xsgn*htw >0) ); 
			break;
		case F_TRIANGLE:
			aspect = ((double) hth)/(double)htw;
			do { /* we always want at least one pass */
			 Mcursor(tx - htw,ty - hth);
			 Dline(tx,ty + hth);
			 Dline(tx + htw,ty - hth);
			 Dline(tx - htw,ty - hth);
			 htw -= xsgn*((XDIM/xRes)>>1);  
		/* keep the aspect ratio the same or whacky things happen! */ 
			 hth = (aspect*(double)htw +0.5); 
		/* if not filled or not finished filling, loop again */
			} while(filled && (ysgn*hth > 0)&& (xsgn*htw >0) ); 
			break;
		case F_CROSS:
			Mcursor(tx - htw,ty);
			Dline(tx + htw,ty);
			Mcursor(tx,ty - hth);
			Dline(tx,ty + hth);
			break;
		case F_ELLIPSE:
			loop = 39;
			sina = 0.;
			cosa = 1.;
			if(!filled) {
			 sinb = 0.156434465; /* angle = 9 degrees */
			 cosb = 0.98768834;
			}
			else { /* if filling, smaller segments are needed
				* to help eliminate empty spots */
			 sinb = 0.1045284633; /* angle = 3 degrees */
			 cosb = 0.9945218954;
			 loop = 59;
			}
			do {
			 Mcursor(tx + htw,ty);
			 for(count = 0; count < loop; count++)
			 {
				tsina = sina * cosb + cosa * sinb;
				cosa = cosa * cosb - sina * sinb;
				sina = tsina;
				Dline(tx + (int)(htw * cosa),
				      ty + (int)(hth * sina));
			 }
			 Dline(tx + htw,ty);
			 htw -= xsgn*((XDIM/xRes)>>1);  
			 hth -= ysgn*((YDIM/yRes)>>1); 
			} while(filled && (ysgn*hth > 0)&& (xsgn*htw >0) ); 
			break;
		case F_DIAMOND:
			do { /* we always want at least one pass */
			 Mcursor(tx - htw,ty);
			 Dline(tx,ty - hth);
			 Dline(tx + htw,ty);
			 Dline(tx,ty + hth);
			 Dline(tx - htw,ty);
			 htw -= xsgn*((XDIM/xRes)>>1);  
			 hth -= ysgn*((YDIM/yRes)>>1); 
		/* if not filled or not finished filling, loop again */
			} while(filled && (ysgn*hth > 0)&& (xsgn*htw >0) ); 
			break;
		case F_DTRIANGLE:
			aspect = ((double) hth)/(double)htw;
			do { /* we always want at least one pass */
			 Mcursor(tx - htw,ty + hth);
			 Dline(tx,ty - hth);
			 Dline(tx + htw,ty + hth);
			 Dline(tx - htw,ty + hth);
		/* for triangles, take half steps (this is empirical) */
			 htw -= xsgn*((XDIM/xRes)>>1);  
		/* keep the aspect ratio the same or whacky things happen! */ 
			 hth = (aspect*(double)htw +0.5); 
		/* if not filled or not finished filling, loop again */
			} while(filled && (ysgn*hth > 0)&& (xsgn*htw >0) ); 
			break;
		case F_BAR:
			Mcursor(tx - htw,ty - hth);
			Dline(tx + htw,ty - hth);
			Mcursor(tx , ty - hth);
			Dline(tx , ty + hth);
			Mcursor(tx - htw,ty + hth);
			Dline(tx + htw,ty + hth);
			break;
		case F_BAR_H: 
			Mcursor(tx - htw,ty - hth);
			Dline(tx - htw,ty + hth);
			Mcursor(tx - htw ,ty);
			Dline(tx + htw ,ty);
			Mcursor(tx + htw,ty - hth);
			Dline(tx + htw,ty + hth);
			break;
		case F_OCTAGON:
			sc = 0.414213562;
			do { /* we always want at least one pass */
			 Mcursor(tx - (int)(htw * sc),ty - hth);
			 Dline(tx + (int)(htw * sc),ty - hth);
			 Dline(tx + htw,ty - (int)(hth * sc));
			 Dline(tx + htw,ty + (int)(hth * sc));
			 Dline(tx + (int)(htw * sc),ty + hth);
			 Dline(tx - (int)(htw * sc),ty + hth);
			 Dline(tx - htw,ty + (int)(hth * sc));
			 Dline(tx - htw,ty - (int)(hth * sc));
			 Dline(tx - (int)(htw * sc),ty - hth);
			 htw -= xsgn*((XDIM/xRes)>>1);  
			 hth -= ysgn*((YDIM/yRes)>>1); 
		/* if not filled or not finished filling, loop again */
			} while(filled && (ysgn*hth > 0)&& (xsgn*htw >0) ); 
			break;
		case F_DOT: /* dot is just an ellipse with 0 size */
			Mcursor(tx,ty); 
                        Dline(tx,ty);
                        break;
		default:
			error_("Illegal figure type");
	}
}

/*******************************************/
/* Mcursor - low level move cursor command */
/*******************************************/

static void Mcursor(x,y)
	int x;		/* x location to move to */
	int y;		/* y location to move to */
{
	if(x >= 0 && x <= XDIM && y >= 0 && y <= YDIM)
		mcursor(x,y);
	(cPic)->x = x;
	(cPic)->y = y;
}

/***************************************/
/* Dline - low level draw line command */
/***************************************/

static void Dline(x,y)
	int x;		/* x location to draw to */
	int y;		/* y location to draw to */
{
	LineCommands((cPic)->x,(cPic)->y,x,y);
	(cPic)->x = x;
	(cPic)->y = y;
}

/***********************************************/
/* Pmove - move the cursor to a given location */
/***********************************************/

void Pmove(x,y)
	double x;		/* x location to move to */
	double y;		/* y location to move to */
{
	int tx;		/* true x */
	int ty;		/* true y */

	/* find point to move to in window */
	ScaleXy(cPic,x,y,&tx,&ty);

	/* make command to move */
	Mcursor(tx,ty);
}

/***********************/
/* Pline - plot a line */
/***********************/

void Pline(x,y)
	double x;		/* x value to plot to */
	double y;		/* y value to plot to */
{
	int tx;		/* true x */
	int ty;		/* true y */

	/* find point to plot to in window */
	ScaleXy(cPic,x,y,&tx,&ty);

	/* make commands to draw line */
	if((cPic)->x == UNKNOWN || (cPic)->y == UNKNOWN)
		error_("Trying to draw a line from an unknown position");
	Dline(tx,ty);
}

/********************************/
/* Pboxfill - plot a filled box */
/********************************/

void Pboxfill(x,y,color)
	double x;		/* x value to plot to */
	double y;		/* y value to plot to */
	short int color;         /* gray scale color to plot */
{
	int tx;		/* true x */
	int ty;		/* true y */

	/* find point to plot to in window */
	ScaleXy(cPic,x,y,&tx,&ty);

	/* make commands to draw box */
	if((cPic)->x == UNKNOWN || (cPic)->y == UNKNOWN)
		error_("Trying to draw a line from an unknown position");
	dboxfill(tx,ty,color);

	(cPic)->x = tx;
	(cPic)->y = ty;
}

/******************************/
/* Ptext - draw graphics text */
/******************************/

void Ptext(text)
	char *text;		/* text to be drawn */
{
	/* make intermediate code command */
	dtext(0,text);

	/* indicate that we don't know where that left us */
	(cPic)->x = UNKNOWN;
	(cPic)->y = UNKNOWN;
}

/*******************************/
/* Pstext - draw text sideways */
/*******************************/

void Pstext(text)
	char *text;		/* text to be drawn */
{
	/* make intermediate code command */
	dtext(1,text);

	/* indicate that we don't know where that left us */
	(cPic)->x = UNKNOWN;
	(cPic)->y = UNKNOWN;
}

/*******************************************/
/* Pplace - put some text in a given place */
/*******************************************/

void Pplace(place,text)
	int place;		/* number of place to put text in */
	register char *text;	/* text to put in it */
{
	char tmpstr[80];
	register char *op;

	/* check that place is legal */
	if(place < 0 || place >= (int)(cPic)->format->numPlaces)
	{
		error("No place %d in current format",place);
	}

	/* make intermediate code command */
	if(labelsUC) {
		op = tmpstr;
		while(*text)
			if(islower(*text))
				*op++ = toupper(*text++);
			else
				*op++ = *text++;
		*op = '\0';
		text = tmpstr;
	}
	ptext(place,text);

	/* indicate that we don't know where that left us */
	(cPic)->x = UNKNOWN;
	(cPic)->y = UNKNOWN;
}

/************************/
/* Pctext - center text */
/************************/

void Pctext(text)
	char *text;		/* text to be centered around current */
				/* point */
{
	register PICTURE *picP = cPic;
	/* check that current point makes sense */
	if((picP)->x < 0 || (picP)->x > XDIM ||
	   (picP)->y < 0 || (picP)->y > YDIM)
		error_("Bad point to center text around");

	/* make intermediate code command */
	ctext(0,text);

	/* indicate that we don't know where that left us */
	(picP)->x = UNKNOWN;
	(picP)->y = UNKNOWN;
}

/**********************************/
/* Pcstext - center text sideways */
/**********************************/

void Pcstext(text)
	char *text;		/* text to be centered */
{
	register PICTURE *picP = cPic;
	/* check that current point makes sense */
	if((picP)->x < 0 || (picP)->x > XDIM ||
	   (picP)->y < 0 || (picP)->y > YDIM)
		error_("Bad point to center text around");

	/* make intermediate code command */
	ctext(1,text);

	/* indicate that we don't know where that left us */
	(picP)->x = UNKNOWN;
	(picP)->y = UNKNOWN;
}

/*********************************************************************
 * Plabelfmt - Set up the format of numerical labels on x and y axes *
 *********************************************************************/

void Plabelfmt(htype, hfmt, vtype, vfmt)
int htype, hfmt;
int vtype, vfmt;
{
	
	args.lf.htype = htype;
	args.lf.hfmt = hfmt;
	args.lf.vtype = vtype;
	args.lf.vfmt = vfmt;
	PushInstr(LABELFMT, sizeof(LF_ARGS));
}

/**************************************************************/
/* Pxlabel - put a label below the x axis of the graph window */
/**************************************************************/

void Pxlabel(x)
	double x;		/* x location to label */
{
	int tx;		/* true x */
	int ty;		/* dummy true y */
	register PICTURE *picP = cPic;

	/* check that current window is graph window */
	if((picP)->windowNum != 1)
		error_("Trying to label x axis of other than graph window");

	/* find true x */
	ScaleXy(picP,x,0.,&tx,&ty);

	/* make intermediate code */
	dxlabel(tx,(float)(x));

	/* indicate that we don't know where that left us */
	(picP)->x = UNKNOWN;
	(picP)->y = UNKNOWN;
}

/***********************************************************************/
/* Pylabel - put a label to the left of the y axis of the graph window */
/***********************************************************************/

void Pylabel(y)
	double y;		/* y location to label */
{
	int tx;		/* dummy true x */
	int ty;		/* true y */
	register PICTURE *picP = cPic;

	/* check that current window is graph window */
	if((picP)->windowNum != 1)
		error_("Trying to label y axis of other than graph window");

	/* find true y */
	ScaleXy(picP,0.,y,&tx,&ty);

	/* make intermediate code */
	dylabel(ty,(float)(y));

	/* indicate that we don't know where that left us */
	(picP)->x = UNKNOWN;
	(picP)->y = UNKNOWN;
}

/***********************************/
/* Pgrnum - plot a graphics number */
/***********************************/

void Pgrnum(x,y,num)
	double x;		/* x location at which to plot it */
	double y;		/* y location at which to plot it */
	double num;		/* number to be plotted */
{
	int tx;		/* true x */
	int ty;		/* true y */

	/* compute true x and y */
	ScaleXy(cPic,x,y,&tx,&ty);

	/* make intermediate code */
	if(tx >= 0 && tx <= XDIM && ty >= 0 && ty <= XDIM)
		dgrnum(tx,ty,(float)(num));

	/* indicate that we don't know where that left us */
	(cPic)->x = UNKNOWN;
	(cPic)->y = UNKNOWN;
}

/*****************************/
/* Ptexttype - set text type */
/*****************************/

void Ptexttype(type)
	int type;		/* type to set to */
{
	/* check that type is legal */
	if(type < 0 || type >= TXTTYPES)
		error_("Illegal text type");

	/* make intermediate code */
	setttype(type);
}

/*****************************/
/* Plinetype - set line type */
/*****************************/

void Plinetype(type)
	int type;		/* type to set to */
{
	/* check that type is legal */
	if(type < 0 || type >= LINTYPES)
		error_("Illegal line type");

	/* make intermediate code */
	setltype(type);
}

/********************************************************************/
/* Routines for adding instructions to the intermediate code buffer */
/********************************************************************/

/* routine for placing a string in the intermediate code buffer */

static void InsrtStr(str)
	char *str;		/* string to be put in */
{
	char *newEnd;		/* new end of buffer after inserting string */
	int len;		/* length of string */
	register PICTURE *picP = cPic;

	len = strlen(str) + 1;		/* (1 added for null at end) */
	newEnd = (picP)->iBuf + ((picP)->iBufEnd + len);
	CheckSpace(newEnd);
	strcpy((picP)->iBuf + (picP)->iBufEnd,str);
	(picP)->iBufEnd += len;
}

/* routine for making sure enough space is available in buffer */

/* returns 1 if memory reallocated */
static int CheckSpace(newEnd)
	register char *newEnd;	/* proposed end of buffer */
{
	register PICTURE *picP;
	int startLoc;		/* starting location */

	picP = cPic;
	if(picP->iBufSize < (newEnd - picP->iBuf))
	{
		startLoc = picP->start - picP->iBuf;
		cPic = (PICTURE *)(ChkRealloc((char *)picP,picP->fullSize +
			PICBLOCK, "PICTURE"));
		(cPic)->fullSize += PICBLOCK;
		(cPic)->iBufSize += PICBLOCK;
		if(cPic != picP)
		{
			(cPic)->start = (cPic)->iBuf + startLoc;
			return(1);
		}
	}
	return(0);
}

/* macro for finding place to put an instruction */
/* type = type of arguments */
/* ipoint = variable to become pointer to instruction */
/* apoint = variable to become pointer to arguments */

# define GTIPLACE(type,ipoint,apoint) \
{\
	ipoint = (cPic)->iBuf + (cPic)->iBufEnd;\
	apoint = (type *)(ipoint + 1);\
	if(CheckSpace((char *)apoint + 1))\
	{\
		ipoint = (cPic)->iBuf + (cPic)->iBufEnd;\
		apoint = (type *)(ipoint + 1);\
	}\
	(cPic)->iBufEnd = (char *)(apoint + 1) - (cPic)->iBuf;\
}\

/* Subroutine to store an instruction and its arguments in the intermediate
 * code buffer */ 
PushInstr(instr, arglen)
int instr;
int arglen;
{
	register  PICTURE *picP = cPic;	/* current picture */
	register char *bufp;		/* Next location in int code buff */
	register char *endp;		/* Position beyond this instr */
	register char *ap;		/* Arguments for instruction */

	bufp = picP->iBuf + picP->iBufEnd;
	if(CheckSpace(bufp + sizeof(args))) {	/* enough space for any arg */
		picP = cPic;
		bufp = picP->iBuf + picP->iBufEnd;
	}
	ap = (char *)&args;
	*bufp++ = instr;
	for(endp = bufp + arglen; bufp < endp; )
		*bufp++ = *ap++;
		
	picP->iBufEnd = endp - picP->iBuf;
}

/* individual routines for instructions */

/* Mat declared x and y as unsigned short int */
static void mcursor(x,y)
	int x;
	int y;
{
	args.mc.x = (unsigned short int)x;
	args.mc.y = (unsigned short int)y;
	PushInstr(MCURSOR, sizeof(MC_ARGS));
}

/* Mat declared x and y as unsigned short int */
static void dline(x,y)
	int x;
	int y;
{
	args.dl.x = (unsigned short int)x;
	args.dl.y = (unsigned short int)y;
	PushInstr(DLINE, sizeof(DL_ARGS));
}

/* Mat declared x and y as unsigned short int */
static void dboxfill(x,y,color)
	int x;
	int y;
	short int color;
{
	args.bf.x = (unsigned short int)x;
	args.bf.y = (unsigned short int)y;
	args.bf.color = (short int) color;
	PushInstr(DBOXFILL, sizeof(BF_ARGS));
}

static void ptext(place,text)
	int place;
	char *text;
{
	args.pt.place = (unsigned char)place;
	PushInstr(PTEXT, sizeof(PT_ARGS));
	InsrtStr(text);
}

static void dtext(sideways,text)
	char sideways;
	char *text;
{
	args.dt.sideways = sideways;
	PushInstr(DTEXT, sizeof(DT_ARGS));
	InsrtStr(text);
}

/* Mat declared x and y as unsigned short int */
static void dgrnum(x,y,num)
	int x;
	int y;
	float num;
{
	args.dg.x = (unsigned short int)x;
	args.dg.y = (unsigned short int)y;
	args.dg.num = num;
	PushInstr(DGRNUM, sizeof(DG_ARGS));
}

static void dxlabel(x,num)
	int x;
	float num;
{
	args.dx.x = (unsigned short int)x;
	args.dx.num = num;
	PushInstr(DXLABEL, sizeof(DX_ARGS));
}

static void dylabel(y,num)
	int y;
	float num;
{
	args.dy.y = (unsigned short int)y;
	args.dy.num = num;
	PushInstr(DYLABEL, sizeof(DY_ARGS));
}

static void setwindow(window)
	int window;
{
	args.sw.window = (unsigned char)window;
	PushInstr(SETWINDOW, sizeof(SW_ARGS));
}

static void setltype(type)
	int type;
{
	args.sl.type = (unsigned char)type;
	PushInstr(SETLTYPE, sizeof(SL_ARGS));
}

static void setttype(type)
	int type;
{
	args.st.type = (unsigned char)type;
	PushInstr(SETTTYPE, sizeof(ST_ARGS));
}

static void ctext(sideways,text)
	char sideways;
	char *text;
{
	args.ct.sideways = sideways;
	PushInstr(CTEXT, sizeof(CT_ARGS));
	InsrtStr(text);
}

static void lovrflow(from,to)
	int from;
	int to;
{
	args.lo.from = (unsigned short int)from;
	args.lo.to = (unsigned short int)to;
	PushInstr(LOVRFLOW, sizeof(LO_ARGS));
}

static void bovrflow(from,to)
	int from;
	int to;
{
	args.bo.from = (unsigned short int)from;
	args.bo.to = (unsigned short int)to;
	PushInstr(BOVRFLOW, sizeof(BO_ARGS));
}

static void rovrflow(from,to)
	int from;
	int to;
{
	args.ro.from = (unsigned short int)from;
	args.ro.to = (unsigned short int)to;
	PushInstr(ROVRFLOW, sizeof(RO_ARGS));
}

static void tovrflow(from,to)
	int from;
	int to;
{
	args.to.from = (unsigned short int)from;
	args.to.to = (unsigned short int)to;
	PushInstr(TOVRFLOW, sizeof(TO_ARGS));
}

/*************************************************/
/* GtGen - get general information about formats */
/*************************************************/

static void GtGen()
{
	static done = 0;	/* flag for whether the general information */
				/* has already been gotten */
	char fileName[DIRNMLEN + 12]; /* name of general information file */
	FILE *file;		/* pointer for general information file */
	FILE *FSafeOpen();
	int window;		/* counter for reading in windows */
	int form;		/* counter for format being read in */
	char line[80];		/* line of general information file */
	char xDef[4];		/* definition of whether x is scaled */
	char yDef[4];		/* definition of whether y is scaled */
	char sDef[4];		/* definition of whether window is square */
	char *sLine;		/* start of information in line */
	static char *errStr = "Bad general graphics format file"; /* error */

	/* check that this hasn't been done yet */
	if(done)
		return;

	/* open file */
	if(!*fDirName) {
		sprintf(fDirName,"%s/src/graphics/",main_.comb);
	}
	sprintf(fileName,"%sform.general",fDirName);
	file = FSafeOpen("general graphics format file",fileName,"r");

	/* read in formats */
	form = 0;

	/* read format name */
	while(sLine = FGetLine(file,line))
	{
		strcpy(formats[form].name,sLine);

		/* set general information about whole screen and */
		/* graphics window */
		formats[form].gWindow[0].xIsScaled = 0;
		formats[form].gWindow[0].yIsScaled = 0;
		formats[form].gWindow[0].square = 0;
		formats[form].gWindow[1].xIsScaled = 1;
		formats[form].gWindow[1].yIsScaled = 1;

		/* read in whether graphics window is square */
		if(!(sLine = FGetLine(file,line)))
			error_(errStr);
		sscanf(sLine,"%s",sDef);
		if(strcmp(sDef,"yes"))
			formats[form].gWindow[1].square = 0;
		else
			formats[form].gWindow[1].square = 1;

		/* read number of windows */
		if(!(sLine = FGetLine(file,line)))
			error_(errStr);
		sscanf(sLine,"%hd",&formats[form].numWindows);
		formats[form].numWindows += 2;

		/* read general information about other windows */
		for(window = 2; window < (int)formats[form].numWindows;
			window++) {
			if(!(sLine = FGetLine(file,line)))
				error_(errStr);
			sscanf(sLine,"%s %s %s",xDef,yDef,sDef);
			if(strcmp(xDef,"yes"))
				formats[form].gWindow[window].xIsScaled = 0;
			else
				formats[form].gWindow[window].xIsScaled = 1;
			if(strcmp(yDef,"yes"))
				formats[form].gWindow[window].yIsScaled = 0;
			else
				formats[form].gWindow[window].yIsScaled = 1;
			if(strcmp(sDef,"yes"))
				formats[form].gWindow[window].square = 0;
			else
				formats[form].gWindow[window].square = 1;
		}

		/* read in number of places */
		if(!(sLine = FGetLine(file,line)))
			error_(errStr);
		sscanf(sLine,"%hd",&formats[form].numPlaces);

		/* go on to next format */
		form++;
	}

	FSafeClose(file);
	numForms = form;
	done = 1;
}

/***************************************************/
/* GtSpec - get specific information about formats */
/***************************************************/

/* NOTE: this is not static because it is called by the terminal drivers */
void GtSpec(fileName)
	char *fileName;		/* name of file to get info from */
{
	char trueName[DIRNMLEN + 20]; /* true name of file */
	FILE *file;		/* file to get info from */
	FILE *FSafeOpen();
	int window;		/* counter for reading in windows */
	int place;		/* counter for reading in places */
	int formNum;		/* number of format being read in */
	char sDef[4];		/* definition of whether place is sideways */
	char cDef[4];		/* definition of whether place is centered */
	char xDef[4];		/* definition of whether x labels are */
				/* sideways */
	char yDef[4];		/* definition of whether y labels are */
				/* sideways */
	char errStr[DIRNMLEN + 30];	/* error string */
	char line[80];		/* line of file */
	char *sLine;		/* start of information on line */

	/* clear old definitions */
	for(formNum = 0; formNum < numForms; formNum++)
		formats[formNum].defined = 0;

	/* open file */
	sprintf(trueName,"%s%s",fDirName,fileName);
	sprintf(errStr,"graphics format file '%s'",trueName);
	file = FSafeOpen(errStr,trueName,"r");
	sprintf(errStr,"Bad graphics format file '%s'",trueName);

	/* read in formats */
	while(sLine = FGetLine(file,line))
	{

		/* find out which format is being read in */
		formNum = FindForm(sLine);

		/* set up information about whole screen */
		formats[formNum].sWindow[0].left = 0;
		formats[formNum].sWindow[0].bottom = 0;
		formats[formNum].sWindow[0].right = xRes;
		formats[formNum].sWindow[0].top = yRes;
		formats[formNum].sWindow[0].xpels = xRes;
		formats[formNum].sWindow[0].ypels = yRes;
		formats[formNum].sWindow[0].leftSame = 0;
		formats[formNum].sWindow[0].bottomSame = 0;
		formats[formNum].sWindow[0].rightSame = 0;
		formats[formNum].sWindow[0].topSame = 0;

		/* get information about graph window */
		GtWindow(file,formats[formNum].sWindow + 1,errStr);
		if(formats[formNum].sWindow[1].leftSame ||
		   formats[formNum].sWindow[1].bottomSame ||
		   formats[formNum].sWindow[1].rightSame ||
		   formats[formNum].sWindow[1].topSame)
			error_(errStr);

		/* get additional information */
		if(!(sLine = FGetLine(file,line)))
			error_(errStr);
		sscanf(sLine,"%hd %hd",
		    &formats[formNum].xOvflow,
		    &formats[formNum].yOvflow);
		if(!(sLine = FGetLine(file,line)))
			error_(errStr);
		sscanf(sLine,"%hd %hd",
		    &formats[formNum].xLab,
		    &formats[formNum].yLab);
		if(!(sLine = FGetLine(file,line)))
			error_(errStr);
		sscanf(sLine,"%s %s",xDef,yDef);
		if(strcmp(xDef,"yes"))
			formats[formNum].xIsSideways = 0;
		else
			formats[formNum].xIsSideways = 1;
		if(strcmp(yDef,"yes"))
			formats[formNum].yIsSideways = 0;
		else
			formats[formNum].yIsSideways = 1;

		/* read in windows */
		for(window = 2; window < (int)formats[formNum].numWindows;
				window++)
			GtWindow(file,formats[formNum].sWindow + window,
				 errStr);

		/* read in places */
		for(place = 0; place < (int)formats[formNum].numPlaces; place++)
		{
			if(!(sLine = FGetLine(file,line)))
				error_(errStr);
			sscanf(sLine,"%hd %hd %hd %hd %hd %hd %s %s",
			    &formats[formNum].sPlace[place].x,
			    &formats[formNum].sPlace[place].y,
			    &formats[formNum].sPlace[place].type,
			    &formats[formNum].sPlace[place].width,
			    &formats[formNum].sPlace[place].height,
			    &formats[formNum].sPlace[place].indent,
			    sDef,
			    cDef);
			if(strcmp(sDef,"yes"))
				formats[formNum].sPlace[place].isSideways = 0;
			else
				formats[formNum].sPlace[place].isSideways = 1;
			if(strcmp(cDef,"yes"))
				formats[formNum].sPlace[place].isCentered = 0;
			else
				formats[formNum].sPlace[place].isCentered = 1;
		}

		/* indicate that format is defined */
		formats[formNum].defined = 1;
	}
	FSafeClose(file);
}

/**********************************************************/
/* GtWindow - get the specific information about a window */
/**********************************************************/

static void GtWindow(file,window,errStr)
	FILE *file;		/* file to get info from */
	register SPECWINDOW *window;	/* window to be gotten */
	char *errStr;		/* error string */
{
	GtWinLine(file, errStr, &window->leftSame, &window->left,
		window->clx, window->cly);
	GtWinLine(file, errStr, &window->bottomSame, &window->bottom,
		window->cbx, window->cby);
	GtWinLine(file, errStr, &window->rightSame, &window->right,
		window->crx, window->cry);
	GtWinLine(file, errStr, &window->topSame, &window->top,
		window->ctx, window->cty);
}

static void GtWinLine(file,errStr,same,pixel,cx,cy)
	FILE *file;		/* file to get info from */
	char *errStr;		/* error string */
	char *same;
	unsigned short int *pixel;
	register int *cx;
	register int *cy;
{
	char line[80];		/* line buffer */
	register char *sLine;	/* start of info in line */

	if(!(sLine = FGetLine(file,line)))
		error_(errStr);
	if(!strcmp(sLine,"graph window"))
	{
		*same = 1;
		*pixel = 0;
		*cx++ = 0;
		*cx++ = 0;
		*cx++ = 0;
		*cy++ = 0;
		*cy++ = 0;
		*cy++ = 0;
	}
	else
	{
		*same = 0;
		sscanf(sLine,"%hd %d %d %d %d %d %d",pixel,
			cx,cy,cx + 1,cy + 1,cx + 2,cy + 2);
	}
}

/***************************************/
/* FGetLine - get a line from a stream */
/***************************************/

/* returns pointer to first meaningful */
static char *FGetLine(file,line)
				/* character of line (0 if EOF) */
	FILE *file;		/* file to read from */
	register char *line;	/* buffer to read into */
{
	register int c, count = 0;

	while((c = getc(file)) != EOF) {
		switch(c) {
		case ' ':
		case '\t':
			if(count)
				line[count++] = c;
			break;
		case '\n':
			if(count && line[0] != '#') {
				line[count] = '\0';
				return(line);
			} else {
				count = 0;
			}
			break;
		default:
			line[count++] = c;
		}
	}
	return((char *)0);
}

#if 0
/* Matt's version */
static char *FGetLine(file,line)
				/* character of line (0 if EOF) */
	FILE *file;		/* file to read from */
	char *line;		/* buffer to read into */
{
	char *sLine;		/* start of meaningful stuff in line */
	do
	{

		/* read in line */
		while((*line = getc(file)) == '\n')
			;
		line[1] = 0;
		if(fscanf(file,"%[^\n]",line + 1) == EOF)
			return(0);
	
		/* find start of meaningful information */
		for(sLine = line; *sLine == ' ' || *sLine == '	'; sLine++)
			;

	/* get another line if this is blank or a comment */
	}
	while(!*sLine || *sLine == '#');

	/* return pointer to start of meaningful stuff */
	return(sLine);
}
#endif /*0*/

/***************************************/
/* FindForm - find a particular format */
/***************************************/

/* returns number of format in formats[] */
static int FindForm(formName)
	char *formName;		/* name of format to find */
{
	int chk;		/* format being checked */
	char errStr[FORMNMLEN + 29]; /* error string */

	/* find format */
	for(chk = 0; chk < numForms; chk++)
		if(!strcmp(formats[chk].name,formName))
			return(chk);

	/* if not found, error out */
	sprintf(errStr,"Graphics format '%s' not found",formName);
	error_(errStr);
	/*NOTREACHED*/
}

/*******************************************************************/
/* LineCommands - make sequence of commands to draw a clipped line */
/*******************************************************************/

static void LineCommands(ox,oy,nx,ny)
	register int ox,oy;	/* coordinates to draw from (old x & y) */
	register int nx,ny;	/* coordinates to draw to (new x & y) */
{
	double slope;		/* slope of line */
	double yint;		/* y intercept of line */
	int y_at_x0;		/* value of y at x = 0 on line */
	int x_at_y0;		/* value of x at y = 0 on line */
	int y_at_xMax;		/* value of y at x = XDIM on line */
	int x_at_yMax;		/* value of x at y = YDIM on line */

	/* if clipping isn't needed, just make line */
	if(ox >= 0 && ox <= XDIM &&
	   oy >= 0 && oy <= YDIM &&
	   nx >= 0 && nx <= XDIM &&
	   ny >= 0 && ny <= YDIM)
	{
		dline(nx,ny);
		return;
	}

	/* check for lines that are completely out of bounds off one side */
	if(ox < 0 && nx < 0)
	{
		ovrflow(oy,ny,YDIM,lovrflow);
		return;
	}
	if(oy < 0 && ny < 0)
	{
		ovrflow(ox,nx,XDIM,bovrflow);
		return;
	}
	if(ox > XDIM && nx > XDIM)
	{
		ovrflow(oy,ny,YDIM,rovrflow);
		return;
	}
	if(oy > YDIM && ny > YDIM)
	{
		ovrflow(ox,nx,XDIM,tovrflow);
		return;
	}

	/* clip horizontal and vertical lines */
	if(oy == ny)
	{
		if(ox < 0)
		{
			lovrflow(oy,oy);
			mcursor(0,oy);
		}
		else if(ox > XDIM)
		{
			rovrflow(oy,oy);
			mcursor(XDIM,oy);
		}
		if(nx < 0)
		{
			lovrflow(ny,ny);
			dline(0,ny);
		}
		else if(nx > XDIM)
		{
			rovrflow(ny,ny);
			dline(XDIM,ny);
		}
		else
			dline(nx,ny);
		return;
	}
	if(ox == nx)
	{
		if(oy < 0)
		{
			bovrflow(ox,ox);
			mcursor(ox,0);
		}
		else if(oy > YDIM)
		{
			tovrflow(ox,ox);
			mcursor(ox,YDIM);
		}
		if(ny < 0)
		{
			bovrflow(nx,nx);
			dline(nx,0);
		}
		else if(ny > YDIM)
		{
			tovrflow(nx,nx);
			dline(nx,YDIM);
		}
		else
			dline(nx,ny);
		return;
	}

	/* ok, this is gonna be a rough one, compute the line */
	/* and find the places at which it intersects the edges of the */
	/* window */
	slope = (double)(ny - oy) / (nx - ox);
	yint = ny - slope * nx;
	y_at_x0 = (int)(yint);
	x_at_y0 = (int)(-yint / slope);
	y_at_xMax = (int)(XDIM * slope + yint);
	x_at_yMax = (int)((YDIM - yint) / slope);

	/* make overflows */
	if(ox < 0)
		ovrflow(oy,y_at_x0,YDIM,lovrflow);
	else if(nx < 0)
		ovrflow(y_at_x0,ny,YDIM,lovrflow);
	if(oy < 0)
		ovrflow(ox,x_at_y0,XDIM,bovrflow);
	else if(ny < 0)
		ovrflow(x_at_y0,nx,XDIM,bovrflow);
	if(ox > XDIM)
		ovrflow(oy,y_at_xMax,YDIM,rovrflow);
	else if(nx > XDIM)
		ovrflow(y_at_xMax,ny,YDIM,rovrflow);
	if(oy > YDIM)
		ovrflow(ox,x_at_yMax,XDIM,tovrflow);
	else if(ny > YDIM)
		ovrflow(x_at_yMax,nx,XDIM,tovrflow);

	/* if the old point (ox,oy) is out of bounds, go through */
	/* all the various machinations needed to bring it in bounds */
	if(ox < 0)
		if(y_at_x0 >= 0 && y_at_x0 <= YDIM)
			mcursor(0,y_at_x0);
	if(oy < 0)
		if(x_at_y0 >= 0 && x_at_y0 <= XDIM)
			mcursor(x_at_y0,0);
	if(ox > XDIM)
		if(y_at_xMax >= 0 && y_at_xMax <= YDIM)
			mcursor(XDIM,y_at_xMax);
	if(oy > YDIM)
		if(x_at_yMax >= 0 && x_at_yMax <= XDIM)
			mcursor(x_at_yMax,XDIM);

	/* draw line from wherever the above code left us to */
	/* the new point (nx,ny) */
	if(nx < 0)
		if(y_at_x0 >= 0 && y_at_x0 <= YDIM)
		{
			dline(0,y_at_x0);
			return;
		}
	if(ny < 0)
		if(x_at_y0 >= 0 && x_at_y0 <= XDIM)
		{
			dline(x_at_y0,0);
			return;
		}
	if(nx > XDIM)
		if(y_at_xMax >= 0 && y_at_xMax <= YDIM)
		{
			dline(XDIM,y_at_xMax);
			return;
		}
	if(ny > YDIM)
		if(x_at_yMax >= 0 && x_at_yMax <= XDIM)
		{
			dline(x_at_yMax,YDIM);
			return;
		}
	if(nx >= 0 && nx <= XDIM && ny >= 0 && ny <= YDIM)
		dline(nx,ny);
}

/*******************************************************************/
/* ovrflow - call one of the overflow routines with good arguments */
/*******************************************************************/

static void ovrflow(from,to,max,side)
	int from;		/* nominal value to use as from */
				/* (may be out of bounds) */
	int to;			/* nominal value to use as to */
				/* (may be out of bounds and/or out of */
				/* order with from) */
	int max;		/* maximum value in bounds */
	void (*side)();		/* overflow routine to call */
{
	/* check that overflow marking is on */
	if(!ovrOn || (cPic)->windowNum != 1)
		return;

	/* fix from and to if they are out of bounds or out of order and */
	/* call side */
	if(from < 0)
	{
		if(to > max)
			side(0,max);
		else if(to > 0)
			side(0,to);
	}
	else if(from > max)
	{
		if(to < 0)
			side(0,max);
		else if(to < max)
			side(to,max);
	}
	else
		if(to < 0)
			side(0,from);
		else if(to > max)
			side(from,max);
		else if(to > from)
			side(from,to);
		else if(from > to)
			side(to,from);
}

/************************************/
/* ScaleXy - get scaled coordinates */
/************************************/

static void ScaleXy(picP,x,y,tx,ty)
	register PICTURE *picP;	/* picture in which to scale them */
	double x;		/* unscaled x */
	double y;		/* unscaled y */
	register int *tx;	/* scaled x */
	register int *ty;	/* scaled y */
{
	double log();

	/* compute x */
	if(picP->gWindow->xIsScaled)
	{
		if(picP->xLog)
		{
			if(x <= 0)
				*tx = -32767;
			else
				*tx = (int)((log(x) - picP->left) *
				      picP->xScale + 1e-8);
		}
		else
			*tx = (int)((x - picP->left) * picP->xScale + 1e-8);
	}
	else
		*tx = (int)(x + 1e-8);

	/* scale y */
	if(picP->gWindow->yIsScaled)
	{
		if(picP->yLog)
		{
			if(y <= 0)
				*ty = -32767;
			else
				*ty = (int)((log(y) - picP->bottom) *
				     picP->yScale + 1e-8);
		}
		else
			*ty = (int)((y - picP->bottom) * picP->yScale + 1e-8);
	}
	else
		*ty = (int)(y + 1e-8);
}
/********************/
