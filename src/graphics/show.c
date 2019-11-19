# include <stdio.h>
# include <string.h>

# include "C.h"
# include "include.h"
# include "../error/C.h"

# define GASPECT 1.4		/* aspect ratio of graphic number characters */
				/* (y / x) */
# define GNUMSPACE 1.4		/* graphics number character spacing */
# define MAXOVER 500		/* maximum number of overflow lines */

# define min(a,b) (((a) < (b))?(a):(b))
# define max(a,b) (((a) > (b))?(a):(b))

void (*d_start)();		/* driver routine for starting graphics */
void (*d_move)();		/* driver routine for moving cursor */
void (*d_line)();		/* driver routine for drawing line */
void (*d_text)();		/* driver routine for drawing text */
void (*d_ctext)();		/* driver routine for drawing centered text */
void (*d_setlin)();		/* driver routine for setting line type */
void (*d_settxt)();		/* driver routine for setting text type */
void (*d_cleanup)();		/* driver routine for cleaning up terminal */
				/* when done plotting */
void (*d_clear)();		/* driver routine for clearing part of the */
				/* screen */
void (*d_cursor)();		/* driver routine for reading cursor */

/* external variables */
int noShow = 0;
char hasClear;
char hasCursor;
int xBoxes = 1;
int yBoxes = 1;
char defTerm[TERMNMLEN] = {0};
char clearAll = 1;
static PICTURE *oneBox = 0;	/* dummy picture */
PICTURE **boxes = &oneBox;	/* array of pictures in boxes on screen */
int ppState;			/* state of show-penplotter interaction in
				making multiple passes through the code */
int ppCarousel;			/* Type of pens mounted on the pen plotter:
				communication from hc to the pp driver */
float xSquish = 1.0, ySquish = 1.0;	/* Factors for adjusting the size of a plot */

/* global variables */
int
# include "d.routines"
	dummy;
static struct		/* list of drivers */
{
	char *name;		/* name of driver */
	int (*driver)();	/* driver initialization routine */
}
*cDrive,		/* driver being looked at */
drivers[] =		/* drivers */
{
# include "d.names"
	"end",0
};
static int minLabX;		/* minimum x value in Y Labels */
static int minLabY;		/* minimum y value in X Labels */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* show.c */
static void ShowAll P_((void));
static void ShowInBox P_((register PICTURE *picP, int xBox, int yBox));
static void Show P_((register PICTURE *picP, int xRes, int yRes, double xSize, double ySize, GRFORMAT form));
static int InLine P_((int x0, int y0, int x1, int y1, int x2, int y2));
static void ClipSeg P_((int xfrom, int yfrom, int xto, int yto, int left, int bottom, int right, int top));
static void SegSeq P_((int x, int y, int xSize, int ySize, int left, int bottom, int right, int top, char *segStr));
static void GrNumChar P_((int x, int y, int xSize, int ySize, int left, int bottom, int right, int top, int grChar));
static void DGrNum P_((int x, int y, int xSize, int ySize, int left, int bottom, int right, int top, double number));
static void PlaceText P_((register GRFORMAT *form, int placeNum, register char *text, int textType));
static int InitDevice P_((char *device));
static int GetDevice P_((char *device));
#undef P_


/************************************/
/* Pterminal - set default terminal */
/************************************/

void Pterminal(terminal)
	char *terminal;		/* name of default terminal */
{
	strcpy(defTerm,terminal);
}

/**********************************************/
/* Pboxes - set the number of boxes on screen */
/**********************************************/

void Pboxes(xIn,yIn)
	int xIn;	/* number of boxes in x */
	int yIn;	/* number of boxes in y */
{
	register PICTURE **pBox;	/* plot box being cleared */
	int nBoxes;			/* Number of Box es */

	clearAll = 1;
	nBoxes = xBoxes * yBoxes;
	for(pBox = boxes; pBox < boxes + nBoxes; pBox++)
		Punlink(pBox);
	if(boxes != &oneBox)
		ChkFree((char *)boxes);
	if((nBoxes = (xBoxes = xIn) * (yBoxes = yIn)) > 1)
		boxes = (PICTURE **)(ChkAlloc(nBoxes * sizeof(PICTURE *),
		"Pboxes"));
	else
		boxes = &oneBox;
	/* clear the new pointers */
	for(pBox = boxes; pBox < boxes + nBoxes; pBox++)
		*pBox = (PICTURE *)0;
}

/************************************/
/* Pshow - show the current picture */
/************************************/

void Pshow(ixBox,iyBox)
	int ixBox;		/* x box to show it in */
	int iyBox;		/* y box to show it in */
{
	int xBox;		/* x box to show in */
	int yBox;		/* y box to show in */
	register PICTURE *picP;	/* copy of current picture */
	PICTURE **pBox;		/* Current picture box */
	int boxEmpty;		/* flag for empty box */
	int boxLeft;		/* left of box */
	int boxBottom;		/* bottom of box */
	int boxRight;		/* right of box */
	int boxTop;		/* top of box */

	/* ignore ixBox and iyBox if there's only one box on screen */
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

	/* make sure xBox and yBox are ok */
	if(xBox < 0 || xBox >= xBoxes ||
	   yBox < 0 || yBox >= yBoxes)
		error_("Illegal box for plotting in");

	/* Link the picture into the box buffer, releasing any previous pict */
	boxEmpty = !*(pBox = &boxes[yBox * xBoxes + xBox]);
	Plink(cPic, pBox);

	picP = cPic;
	(picP)->start = (picP)->iBuf;
	(picP)->lastx = 0;
	(picP)->lasty = 0;
	(picP)->lastwindow = 1;
	(picP)->lastltype = LINTYPES - 1;
	(picP)->lastttype = 1;

	/* initialize driver */
	if(GetDevice(defTerm))
		return;
	d_start();

	/* show the picture */
	if((boxEmpty || hasClear) && !clearAll)
	{
		if(!boxEmpty)
		{
			boxLeft = xBox * (xRes + 1) / xBoxes;
			boxBottom = yBox * (yRes + 1) / yBoxes;
			boxRight = boxLeft + (xRes + 1) / xBoxes - 1;
			boxTop = boxBottom + (yRes + 1) / yBoxes - 1;
			d_clear(boxLeft,boxBottom,boxRight,boxTop);
		}
		ShowInBox(picP,xBox,yBox);
		(picP)->start = (picP)->iBuf + (picP)->iBufEnd;
		d_cleanup();
	}
	else
	{
		d_clear(0,0,xRes,yRes);
		ShowAll();
		clearAll = 0;
	}
}

/********************************************************************/
/* Psend - send the stuff currently on the screen to a given device */
/********************************************************************/

void Psend(device)
	char *device;		/* device to send it to */
{
	char tempDev[TERMNMLEN]; /* temporary device name in case */
				/* user changes it */
	int saveNoShow;

	if(!cPic)
		error("no picture to send");

	/* make sure screen is up to date */
	Pupdate();

	/* initialize driver for given device */
	saveNoShow = noShow;
	noShow = 0;
	strcpy(tempDev,device);
	if(GetDevice(tempDev))
		return;
	d_start();

	/* send stuff on screen */
	d_clear(0,0,xRes,yRes);
	ShowAll();
	noShow = saveNoShow;
}

/*******************************************/
/* Pupdate - update all pictures on screen */
/*******************************************/

void Pupdate()
{
	int xBox;		/* x box being updated */
	int yBox;		/* y box being updated */
	PICTURE **pic;		/* picture being updated */

	/* initialize terminal drivers */
	if(GetDevice(defTerm))
		return;
	d_start();

	/* update boxes */
	pic = boxes;
	for(yBox = 0; yBox < yBoxes; yBox++)
		for(xBox = 0; xBox < xBoxes; xBox++)
		{
			if(*pic)
				if((*pic)->start < (*pic)->iBuf +
				   (*pic)->iBufEnd)
				{
					ShowInBox(*pic,xBox,yBox);
					(*pic)->start = (*pic)->iBuf +
						(*pic)->iBufEnd;
				}
			pic++;
		}
	d_cleanup();
}

/***************************************/
/* ShowAll - show all graphs on screen */
/***************************************/

static void ShowAll()
{
	int xBox;		/* xBox being plotted */
	int yBox;		/* yBox being plotted */
	PICTURE **pic;		/* picture being plotted */

	pic = boxes;
	for(yBox = 0; yBox < yBoxes; yBox++)
		for(xBox = 0; xBox < xBoxes; xBox++)
		{
			if(*pic)
			{
				(*pic)->start = (*pic)->iBuf;
				(*pic)->lastx = 0;
				(*pic)->lasty = 0;
				(*pic)->lastwindow = 1;
				(*pic)->lastltype = LINTYPES - 1;
				(*pic)->lastttype = 1;
				ShowInBox(*pic,xBox,yBox);
				(*pic)->start = (*pic)->iBuf +
					(*pic)->iBufEnd;
			}
			pic++;
		}
	d_cleanup();
}

/*********************************************/
/* ShowInBox - show a picture in a given box */
/*********************************************/

static void ShowInBox(picP,xBox,yBox)
	register PICTURE *picP;	/* picture to be shown */
	int xBox;		/* horizontal box number to show in */
	int yBox;		/* vertical box number to show in */
{
	GRFORMAT form;		/* modified format of picture */
	int mxr;		/* modified x resolution */
	int myr;		/* modified y resolution */
	float mxs;		/* modified x size */
	float mys;		/* modified y size */
	register int window;	/* counter for preparing windows */
	register int chType;	/* counter for stepping through */
				/* character (text) types */
	double aspect;		/* aspect ratio of a window */
	double cntr, off;	/* temps for computing window edges */
	double fabs();
	int txpels;		/* temporary variable for number of */
				/* pixels in x */
	int typels;		/* temporary variable for number of */
				/* pixels in y */
	int xchng;		/* amount to change x to make units square */
	int ychng;		/* amount to change y to make units square */
	register int place;	/* counter for preparing places */
	int plow;		/* lower boundary of a place */
	int phigh;		/* upper boundary of a place */
	char errStr[100];	/* error string */

	/* make sure format is defined */
	if(!(picP)->format->defined)
	{
		sprintf(errStr,"No format %s for device %s\n",
			(picP)->format->name,termType);
		error_(errStr);
	}

	/* begin computing modified values */
	mxr = (xRes + 1) / xBoxes;
	myr = (yRes + 1) / yBoxes;
	mxs = xSize / (xBoxes * xSquish);
	mys = ySize / (yBoxes * ySquish);
	form = *((picP)->format);

	/* scale overflow information */
	form.xOvflow /= (unsigned)xBoxes;
	form.yOvflow /= (unsigned)yBoxes;

	/* scale labeling information */
	form.xLab /= (unsigned)yBoxes;
	form.yLab /= (unsigned)xBoxes;

	/* prepare windows */
	for(window = 0; window < (int)form.numWindows; window++)
	{
		cntr = xRes / 2;
		off = xBox * mxr;
		form.sWindow[window].left = ((form.sWindow[window].left -
			cntr) * xSquish + cntr) / (unsigned)xBoxes + off;
		form.sWindow[window].right = ((form.sWindow[window].right -
			cntr) * xSquish + cntr) / (unsigned)xBoxes + off;
		cntr = yRes / 2;
		off = yBox * myr;
		form.sWindow[window].bottom = ((form.sWindow[window].bottom -
			cntr) * ySquish + cntr) / (unsigned)yBoxes + off;
		form.sWindow[window].top = ((form.sWindow[window].top -
			cntr) * ySquish + cntr) / (unsigned)yBoxes + off;

		for(chType = 0; chType < TXTTYPES; chType++)
		{
			form.sWindow[window].left += (chX[chType] *
				form.sWindow[window].clx[chType]) / 2 +
				(chY[chType] *
				form.sWindow[window].cly[chType]) / 2;
			form.sWindow[window].bottom += (chX[chType] *
				form.sWindow[window].cbx[chType]) / 2 +
				(chY[chType] *
				form.sWindow[window].cby[chType]) / 2;
			form.sWindow[window].right -= (chX[chType] *
				form.sWindow[window].crx[chType]) / 2 +
				(chY[chType] *
				form.sWindow[window].cry[chType]) / 2;
			form.sWindow[window].top -= (chX[chType] *
				form.sWindow[window].ctx[chType]) / 2 +
				(chY[chType] *
				form.sWindow[window].cty[chType]) / 2;
		}
		if(form.sWindow[window].leftSame)
			form.sWindow[window].left = form.sWindow[1].left;
		if(form.sWindow[window].bottomSame)
			form.sWindow[window].bottom = form.sWindow[1].bottom;
		if(form.sWindow[window].rightSame)
			form.sWindow[window].right = form.sWindow[1].right;
		if(form.sWindow[window].topSame)
			form.sWindow[window].top = form.sWindow[1].top;
		form.sWindow[window].xpels = form.sWindow[window].right -
			form.sWindow[window].left;
		form.sWindow[window].ypels = form.sWindow[window].top -
			form.sWindow[window].bottom;
		if(form.gWindow[window].square)
		{
			if((picP)->xLog != (picP)->yLog)
				error_("Log-linear plot in square window");
			aspect = fabs(((picP)->top - (picP)->bottom) /
				 ((picP)->unitsRatio * ((picP)->right -
				 (picP)->left)));
		/*
			aspect = fabs((picP)->top - (picP)->bottom) /
				 fabs((picP)->right - (picP)->left);
		*/
			if(aspect * (int)form.sWindow[window].xpels * mxs /
			    mxr <= (int)form.sWindow[window].ypels * mys / myr)
			{
				typels = form.sWindow[window].ypels;
				form.sWindow[window].ypels = aspect *
				    form.sWindow[window].xpels * (mxs / mxr) *
				    (myr / mys) + 0.5;
				ychng = (typels -
				    (int)form.sWindow[window].ypels) / 2;
				form.sWindow[window].bottom += ychng;
				form.sWindow[window].top -= ychng;
			}
			else
			{
				txpels = form.sWindow[window].xpels;
				form.sWindow[window].xpels = 
				    ((int)form.sWindow[window].ypels / aspect) *
				    (mys / myr) * (mxr / mxs) + 0.5;
				xchng = (txpels -
				    (int)form.sWindow[window].xpels) / 2;
				form.sWindow[window].left += xchng;
				form.sWindow[window].right -= xchng;
			}
		}
	}

	/* record information for reading cursor */
	(picP)->leftPel = form.sWindow[1].left;
	(picP)->bottomPel = form.sWindow[1].bottom;
	(picP)->xPelScale = ((picP)->right - (picP)->left) /
			     (int)form.sWindow[1].xpels;
	(picP)->yPelScale = ((picP)->top - (picP)->bottom) /
			     (int)form.sWindow[1].ypels;

	/* prepare places beyond 2 */
	for(place = 3; place < (int)form.numPlaces; place++)
	{
		form.sPlace[place].x = form.sPlace[place].x / (unsigned)xBoxes;
		form.sPlace[place].y = form.sPlace[place].y / (unsigned)yBoxes;
		if(form.sPlace[place].isSideways)
		{
			plow = form.sPlace[place].x -
			       chY[form.sPlace[place].type] / 2;
			phigh = form.sPlace[place].x +
				chY[form.sPlace[place].type] *
				form.sPlace[place].height -
				chY[form.sPlace[place].type] / 2;
			if(plow < 0)
				form.sPlace[place].x -= plow;
			if(phigh > mxr)
				form.sPlace[place].x -= phigh - mxr;
		}
		else
		{
			plow = form.sPlace[place].y -
			       chY[form.sPlace[place].type] *
			       form.sPlace[place].height +
			       chY[form.sPlace[place].type] / 2;
			phigh = form.sPlace[place].y +
				chY[form.sPlace[place].type] / 2;
			if(plow < 0)
				form.sPlace[place].y -= plow;
			if(phigh > myr)
				form.sPlace[place].y -= phigh - myr;
		}
		form.sPlace[place].x += xBox * mxr;
		form.sPlace[place].y += yBox * myr;
	}

	/* show picture */
	ppState = FRESHSTART;
	d_setlin(19);
	do {
		ppState = NEWPASS;
		Show(picP,mxr,myr,mxs,mys,form);

	} while(ppState == MOREPENS);
}

/************************************************************************/
/* Show - show a picture with a given format and device characteristics */
/************************************************************************/

static void Show(picP,xRes,yRes,xSize,ySize,form)
	register PICTURE *picP;	/* picture to be shown */
	int xRes;		/* x resolution of screen */
	int yRes;		/* y resolution of screen */
	float xSize;		/* x size of screen (in inches) */
	float ySize;		/* y size of screen (in inches) */
	GRFORMAT form;		/* format to show picture with */
{
	int grNumX;		/* x size of graphics number */
	int grNumY;		/* y size of graphics number */
	double grNumWidth;	/* width of graphics number (in inches) */
	double grNumHeight;	/* height of graphics number (in inches) */
	int t;			/* temporary */
	register SPECWINDOW *sWindow;	/* window being plotted in */
	double xScale;		/* x scaling factor in current window */
	double yScale;		/* y scaling factor in current window */
	int left;		/* left side of current window */
	int bottom;		/* bottom of current window */
	int right;		/* right side of graph window */
	int top;		/* top of graph window */
	register char *instr;	/* instruction being executed */
	char *endBuf;		/* end of instructions in buffer */
	char numStr[30];	/* string used for printing numbers */
	ALLARGS args;		/* arguments of instruction */
	struct
	{
		unsigned short int from;
		unsigned short int to;
	}
	lovr[MAXOVER],
	bovr[MAXOVER],
	rovr[MAXOVER],
	tovr[MAXOVER],		/* overflow buffers */
	*ovr,			/* pointer for looking through overflow */
	*eOvr;			/* last overflow structure in a buffer */
				/* buffers */
	int lovrs,bovrs,rovrs,tovrs; /* number of overflows in each buffer */
	int ox;			/* x origin of a line */
	int oy;			/* y origin of a line */
	int fx;			/* first x point on line */
	int fy;			/* first y point on line */
	int nx;			/* next x location to plot when making line */
	int ny;			/* next y location to plot when making line */
	char sameSlope;		/* flag for whether next line has same slope */
				/* as previous (used when doing DLINE) */
	
	LF_ARGS labelFmt;	/* Save info on label formats */

	labelFmt.htype = GFMT;
	labelFmt.vtype = GFMT;

	/* compute shape of graphics numbers */
	grNumWidth = (double)(xSize) / (GNUMSPACE * 100.);
	grNumX = (int)(grNumWidth * (double)(xRes) / (double)(xSize));
	if(grNumX < 3)
	{
		grNumX = 3;
		grNumWidth = 3. * (double)(xSize) / (double)(xRes);
	}
	grNumHeight = GASPECT * grNumWidth;
	grNumY = (int)(grNumHeight * (double)(yRes) / (double)(ySize));
	if(grNumY < 5)
	{
		grNumY = 5;
		grNumHeight = 5. * (double)(ySize) / (double)(yRes);
		grNumWidth = grNumHeight / GASPECT;
		grNumX = (int)(grNumWidth * (double)(xRes) / (double)(xSize));
	}

	/* set up default values */
	d_settxt( (picP)->lastttype);
	d_setlin( (picP)->lastltype);
	sWindow = form.sWindow + (picP)->lastwindow;
	xScale = (double)(sWindow->xpels) / XDIM;
	yScale = (double)(sWindow->ypels) / YDIM;
	left = sWindow->left;
	bottom = sWindow->bottom;
	minLabX = form.sWindow[1].left;
	minLabY = form.sWindow[1].bottom;
	lovrs = 0;
	bovrs = 0;
	rovrs = 0;
	tovrs = 0;
	d_move((picP)->lastx,(picP)->lasty);

	/* loop through intermediate code buffer */
	endBuf = (picP)->iBuf + (picP)->iBufEnd;
	instr = (picP)->start;
	while(instr < endBuf)
	{

		/* move enough bytes from the int code for any arguments */ 
/*		cmove(instr + 1, &args, sizeof(args)); */
		memcpy(&args, instr + 1, sizeof(args));

		/* select instruction */
		switch(*instr++)
		{

			case MCURSOR:
				(picP)->lastx = (int)(xScale * args.mc.x +
					left);
				(picP)->lasty = (int)(yScale * args.mc.y +
					bottom);
				d_move((picP)->lastx,(picP)->lasty);
				instr += sizeof(MC_ARGS);
				break;
			case DLINE:
				ox = (picP)->lastx;
				oy = (picP)->lasty;
				fx = (int)(xScale * args.dl.x + left);
				fy = (int)(yScale * args.dl.y + bottom);
				nx = fx;
				ny = fy;
				do
				{
					(picP)->lastx = nx;
					(picP)->lasty = ny;
					instr += sizeof(DL_ARGS);
					if(instr == endBuf)
					{
						instr++;
						break;
					}
					if(*instr++ == DLINE)
					{
						/* get next args */
/*						cmove(instr, &args, sizeof(DL_ARGS)); */
						memcpy(&args, instr, sizeof(DL_ARGS));
						nx = (int)(xScale * args.dl.x
						     + left);
						ny = (int)(yScale * args.dl.y
						     + bottom);
						if(fx == ox && fy == oy)
						{
							fx = nx;
							fy = ny;
						}
						sameSlope = InLine(ox,oy,
								   fx,fy,
								   nx,ny);
					}
					else
						sameSlope = 0;
				}
				while(sameSlope);
				instr--;
				d_line((picP)->lastx,(picP)->lasty);
				break;
			case PTEXT:
				instr += sizeof(PT_ARGS);
				PlaceText(&form,(int)args.pt.place,instr,
					(int)(picP)->lastttype);
				instr += strlen(instr) + 1;
				break;
			case DTEXT:
				instr += sizeof(DT_ARGS);
				d_text(instr,args.dt.sideways);
				instr += strlen(instr) + 1;
				break;
			case DGRNUM:
				DGrNum((int)(xScale * args.dg.x + left),
				       (int)(yScale * args.dg.y + bottom),
				       grNumX,grNumY,
				       left,bottom,
				       (int)sWindow->right,(int)sWindow->top,
				       args.dg.num);
				instr += sizeof(DG_ARGS);
				break;
			case DXLABEL:
				switch(labelFmt.htype) {
				case GFMT:
					sprintf(numStr,"%g",args.dx.num);
					break;
				case FFMT:
					LabelPrint(numStr, labelFmt.hfmt,
						args.dx.num);
					break;
				case HMSFMT:
					numStr[0] = 0;
					HmsPrint(numStr, labelFmt.hfmt,
						args.dx.num);
					break;
				}
				if(form.xIsSideways)
				{
					t = bottom - form.xLab -
					   strlen(numStr) * chX[(picP)->lastttype];
					d_move((int)(xScale * args.dx.x +
					   left),t);
					d_text(numStr,1);
				}
				else
				{
					t = bottom - form.xLab -
					   chY[(picP)->lastttype] / 2;
					d_move((int)(xScale * args.dx.x +
					   left),t);
					d_ctext(numStr,0);
					t -= chY[(picP)->lastttype] / 2;
				}
				if(t < minLabY)
					minLabY = t;
				instr += sizeof(DX_ARGS);
				break;
			case DYLABEL:
				switch(labelFmt.vtype) {
				case GFMT:
					sprintf(numStr,"%g",args.dy.num);
					break;
				case FFMT:
					LabelPrint(numStr, labelFmt.vfmt,
						args.dy.num);
					break;
				case HMSFMT:
					numStr[0] = 0;
					HmsPrint(numStr, labelFmt.vfmt,
						args.dy.num);
					break;
				}
				if(form.yIsSideways)
				{
					t = left - form.yLab -
					   chY[(picP)->lastttype] / 2;
					d_move(t, (int)(yScale * args.dy.y +
					   bottom));
					d_ctext(numStr,1);
					t -= chY[(picP)->lastttype] / 2;
				}
				else
				{
					t = left - form.yLab -
					   strlen(numStr) * chX[(picP)->lastttype];
					d_move(t, (int)(yScale * args.dy.y +
					   bottom));
					d_text(numStr,0);
				}
				if(t < minLabX)
					minLabX = t;
				instr += sizeof(DY_ARGS);
				break;
			case SETWINDOW:
				(picP)->lastwindow = args.sw.window;
				sWindow = form.sWindow + args.sw.window;
				xScale = (double)(sWindow->xpels) / XDIM;
				yScale = (double)(sWindow->ypels) / YDIM;
				left = sWindow->left;
				bottom = sWindow->bottom;
				instr += sizeof(SW_ARGS);
				break;
			case SETLTYPE:
				(picP)->lastltype = args.sl.type;
				d_setlin(args.sl.type);
				instr += sizeof(SL_ARGS);
				break;
			case SETTTYPE:
				(picP)->lastttype = args.st.type;
				d_settxt(args.st.type);
				instr += sizeof(ST_ARGS);
				break;
			case CTEXT:
				instr += sizeof(CT_ARGS);
				d_ctext(instr,args.ct.sideways);
				instr += strlen(instr) + 1;
				break;
			case LOVRFLOW:
				lovr[lovrs].from = args.lo.from;
				lovr[lovrs].to = args.lo.to;
				ovr = lovr;
				while(args.lo.from < ovr->from ||
				      args.lo.to > ovr->to)
					if(args.lo.from < ovr->from &&
					   args.lo.to >= ovr->from)
					{
						ovr->from = args.lo.from;
						if(args.lo.to > ovr->to)
							ovr->to =
							    args.lo.to;
					}
					else
					if(args.lo.from <= ovr->to &&
					   args.lo.to > ovr->to)
					{
						ovr->to = args.lo.to;
						if(args.lo.from < ovr->from)
							ovr->from =
							    args.lo.from;
					}
					else
						ovr++;
				if(ovr == lovr + lovrs)
					if(lovrs == MAXOVER)
						error_(
						    "Too many left overflows");
					else
						lovrs++;
				instr += sizeof(LO_ARGS);
				break;
			case BOVRFLOW:
				bovr[bovrs].from = args.bo.from;
				bovr[bovrs].to = args.bo.to;
				ovr = bovr;
				while(args.bo.from < ovr->from ||
				      args.bo.to > ovr->to)
					if(args.bo.from < ovr->from &&
					   args.bo.to >= ovr->from)
					{
						ovr->from = args.bo.from;
						if(args.bo.to > ovr->to)
							ovr->to =
							    args.bo.to;
					}
					else
					if(args.bo.from <= ovr->to &&
					   args.bo.to > ovr->to)
					{
						ovr->to = args.bo.to;
						if(args.bo.from < ovr->from)
							ovr->from =
							    args.bo.from;
					}
					else
						ovr++;
				if(ovr == bovr + bovrs)
					if(bovrs == MAXOVER)
						error_(
						"Too many bottom overflows");
					else
						bovrs++;
				instr += sizeof(BO_ARGS);
				break;
			case ROVRFLOW:
				rovr[rovrs].from = args.ro.from;
				rovr[rovrs].to = args.ro.to;
				ovr = rovr;
				while(args.ro.from < ovr->from ||
				      args.ro.to > ovr->to)
					if(args.ro.from < ovr->from &&
					   args.ro.to >= ovr->from)
					{
						ovr->from = args.ro.from;
						if(args.ro.to > ovr->to)
							ovr->to =
							    args.ro.to;
					}
					else
					if(args.ro.from <= ovr->to &&
					   args.ro.to > ovr->to)
					{
						ovr->to = args.ro.to;
						if(args.ro.from < ovr->from)
							ovr->from =
							    args.ro.from;
					}
					else
						ovr++;
				if(ovr == rovr + rovrs)
					if(rovrs == MAXOVER)
						error_(
						  "Too many right overflows");
					else
						rovrs++;
				instr += sizeof(RO_ARGS);
				break;
			case TOVRFLOW:
				tovr[tovrs].from = args.to.from;
				tovr[tovrs].to = args.to.to;
				ovr = tovr;
				while(args.to.from < ovr->from ||
				      args.to.to > ovr->to)
					if(args.to.from < ovr->from &&
					   args.to.to >= ovr->from)
					{
						ovr->from = args.to.from;
						if(args.to.to > ovr->to)
							ovr->to =
							    args.to.to;
					}
					else
					if(args.to.from <= ovr->to &&
					   args.to.to > ovr->to)
					{
						ovr->to = args.to.to;
						if(args.to.from < ovr->from)
							ovr->from =
							    args.to.from;
					}
					else
						ovr++;
				if(ovr == tovr + tovrs)
					if(tovrs == MAXOVER)
						error_(
						    "Too many top overflows");
					else
						tovrs++;
				instr += sizeof(TO_ARGS);
				break;
			case LABELFMT:
				labelFmt = args.lf;
				instr += sizeof(LF_ARGS);
				break;
			default:
				error_("Illegal command in graphics");
				return;
		}
	}

	/* flush overflow buffers */
	d_setlin(GRID0lt);
	sWindow = form.sWindow + 1;
	xScale = (double)(sWindow->xpels) / XDIM;
	yScale = (double)(sWindow->ypels) / YDIM;
	left = sWindow->left;
	bottom = sWindow->bottom;
	right = sWindow->right;
	top = sWindow->top;
	eOvr = lovr + lovrs;
	for(ovr = lovr; ovr < eOvr; ovr++)
	{
		d_move(left - form.xOvflow,
		       (int)(yScale * ovr->from + bottom));
		d_line(left - form.xOvflow,
		       (int)(yScale * ovr->to + bottom));
	}
	eOvr = bovr + bovrs;
	for(ovr = bovr; ovr < eOvr; ovr++)
	{
		d_move((int)(xScale * ovr->from + left),
		       bottom - form.yOvflow);
		d_line((int)(xScale * ovr->to + left),
		       bottom - form.yOvflow);
	}
	eOvr = rovr + rovrs;
	for(ovr = rovr; ovr < eOvr; ovr++)
	{
		d_move(right + form.xOvflow,
		       (int)(yScale * ovr->from + bottom));
		d_line(right + form.xOvflow,
		       (int)(yScale * ovr->to + bottom));
	}
	eOvr = tovr + tovrs;
	for(ovr = tovr; ovr < eOvr; ovr++)
	{
		d_move((int)(xScale * ovr->from + left),
		       top + form.yOvflow);
		d_line((int)(xScale * ovr->to + left),
		       top + form.yOvflow);
	}
}

/*********************************************************************/
/* InLine - check that three points are on the same line in sequence */
/*********************************************************************/

/* returns 1 if on same line, else 0 */
static int InLine(x0,y0,x1,y1,x2,y2)
	int x0,y0; /* first point */
	int x1,y1; /* second point */
	int x2,y2; /* third point */
{
	double m;	/* slope of line from x0,y0 to x1,y1 */
	double b;	/* y intercept of line from x0,y0 to x1,y1 */
	int dx;		/* change in x from x0,y0 to x1,y1 */

	/* check special case of x1,y1 == x2,y2 */
	if(x1 == x2 && y1 == y2)
		return(1);

	/* check special case of vertical line */
	if(!(dx = x1 - x0))
		if(x2 == x0)
			return((y2 > y1) == (y1 > y0));
		else
			return(0);

	/* find equation of line from x0,y0 to x1,y1 */
	m = (double)(y1 - y0) / dx;
	b = y1 - m * x1;

	/* check that x2,y2 lies on line */
	if(y2 == (int)(m * x2 + b))
		return((x2 > x1) == (x1 > x0));
	else
		return(0);
}

/****************************************************************/
/* ClipSeg - draw a clipped vertical or horizontal line segment */
/****************************************************************/

static void ClipSeg(xfrom,yfrom,xto,yto,left,bottom,right,top)
	int xfrom;		/* x coordinate of start of line */
	int yfrom;		/* y coordinate of start of line */
	int xto;		/* x coordinate of end of line */
	int yto;		/* y coordinate of end of line */
	int left;		/* left side of window */
	int bottom;	/* bottom of window */
	int right;		/* right of window */
	int top;		/* top of window */
{
	/* draw vertical */
	if(xfrom == xto)
	{
		if(xfrom > right || xfrom < left ||
		   (yfrom > top && yto > top) ||
		   (yfrom < bottom && yto < bottom))
			return;
		if(yfrom < bottom)
			d_move(xfrom,bottom);
		else if(yfrom > top)
			d_move(xfrom,top);
		if(yto < bottom)
			d_line(xto,bottom);
		else if(yto > top)
			d_line(xto,top);
		else
			d_line(xto,yto);
		return;
	}

	/* draw horizontal */
	if(yfrom == yto)
	{
		if(yfrom > top || yfrom < bottom ||
		   (xfrom > right && xto > right) ||
		   (xfrom < left && xto < left))
			return;
		if(xfrom < left)
			d_move(left,yfrom);
		else if(xfrom > right)
			d_move(right,yfrom);
		if(xto < left)
			d_line(left,yto);
		else if(xto > right)
			d_line(right,yto);
		else
			d_line(xto,yto);
		return;
	}

	error_("Trying to draw slanted line for graphics number");
}

/****************************************/
/* SegSeq - draw a sequence of segments */
/****************************************/

static void SegSeq(x,y,xSize,ySize,left,bottom,right,top,segStr)
	int x;		/* x location to center segments */
	int y;		/* y location to center segments */
	int xSize;		/* x size of number */
	int ySize;		/* y size of number */
	int left;		/* left of window */
	int bottom;	/* bottom of window */
	int right;		/* right of window */
	int top;		/* top of window */
	char *segStr;		/* sequence of segments on a grid arranged */
				/* thus: */
				/* A---B---C */
				/* |   |   | */
				/* |   M   | */
				/* |   |   | */
				/* D-N-E-O-F */
				/* |   |   | */
				/* | G-H   | */
				/* | | |   | */
				/* I-J-K---L */
{
	int ox;			/* last x position drawn to */
	int oy;			/* last y position drawn to */
	int nx;			/* next x position to draw to */
	int ny;			/* next y position to draw to */
	double xOff;		/* x offset */
	double yOff;		/* y offset */
	static struct		/* offset table */
	{
		float x;		/* x offset (fraction of xSize) */
		float y;		/* y offset (fraction of ySize) */
	}
	offsets[] =
	{
		/* A */ {-.5,.5},
		/* B */ {0,.5},
		/* C */ {.5,.5},
		/* D */ {-.5,0},
		/* E */ {0,0},
		/* F */ {.5,0},
		/* G */ {-.25,-.25},
		/* H */ {0,-.25},
		/* I */ {-.5,-.5},
		/* J */ {-.25,-.5},
		/* K */ {0,-.5},
		/* L */ {.5,-.5},
		/* M */ {0,.25},
		/* N */ {-.25 * GASPECT,0},
		/* O */ {.25 * GASPECT,0}
	};

	/* find first point */
	xOff = offsets[*segStr - 'A'].x * xSize;
	if(xOff > 0)
		ox = x + (int)(xOff);
	else
		ox = x - (int)(-xOff);
	yOff = offsets[*segStr - 'A'].y * ySize;
	if(yOff > 0)
		oy = y + (int)(yOff);
	else
		oy = y - (int)(-yOff);
	if(ox >= left && ox <= right &&
	   oy >= bottom && oy <= top)
	{
		d_move(ox,oy);
	}

	/* draw remaining segments */
	segStr++;
	while(*segStr)
	{
		xOff = offsets[*segStr - 'A'].x * xSize;
		if(xOff > 0)
			nx = x + (int)(xOff);
		else
			nx = x - (int)(-xOff);
		yOff = offsets[*segStr - 'A'].y * ySize;
		if(yOff > 0)
			ny = y + (int)(yOff);
		else
			ny = y - (int)(-yOff);
		ClipSeg(ox,oy,nx,ny,left,bottom,right,top);
		ox = nx;
		oy = ny;
		segStr++;
	}
}

/************************************************/
/* GrNumChar - draw a graphics number character */
/************************************************/

static void GrNumChar(x,y,xSize,ySize,left,bottom,right,top,grChar)
	int x;			/* x place to draw it */
	int y;			/* y place to draw it */
	int xSize;		/* x size of characters */
	int ySize;		/* y size of characters */
	int left;		/* left of window */
	int bottom;		/* bottom of window */
	int right;		/* right of window */
	int top;		/* top of window */
	char grChar;		/* graphics character to draw */
{

# define segs(s) SegSeq(x,y,xSize,ySize,left,bottom,right,top,s)

	switch(grChar)
	{
		case '.':
			segs("GHKJG");
			break;
		case '+':
			segs("MH");
			segs("NO");
			break;
		case '-':
			segs("DF");
			break;
		case 'e':
			segs("CAIL");
			segs("DE");
			break;
		case '0':
			segs("ACLIA");
			break;
		case '1':
			segs("BK");
			break;
		case '2':
			segs("ACFDIL");
			break;
		case '3':
			segs("ACLI");
			segs("FN");
			break;
		case '4':
			segs("ADF");
			segs("CL");
			break;
		case '5':
			segs("CADFLI");
			break;
		case '6':
			segs("CAILFD");
			break;
		case '7':
			segs("ACL");
			break;
		case '8':
			segs("ACLIA");
			segs("DF");
			break;
		case '9':
			segs("FDACL");
			break;
		default:
			error_("Illegal character in graphics number");
	}
}

/***********************************/
/* DGrNum - draw a graphics number */
/***********************************/

static void DGrNum(x,y,xSize,ySize,left,bottom,right,top,number)
	int x;			/* x location to draw it at */
	int y;			/* y location to draw it at */
	int xSize;		/* x size of characters */
	int ySize;		/* y size of characters */
	int left;		/* left side of window */
	int bottom;		/* bottom of window */
	int right;		/* right side of window */
	int top;		/* top of window */
	double number;			/* number to draw */
{
	char numStr[20];	/* character version of number */
	char *ch;		/* character being drawn */
	int step;		/* size of step in x from character to */
				/* character */
	int cLeft;		/* left side of box to clear */
	int cBottom;		/* bottom of box to clear */
	int cRight;		/* right side of box to clear */
	int cTop;		/* top of box to clear */

	/* find location of first character */
	sprintf(numStr,"%g",number);
	step = (int)(GNUMSPACE * xSize + .5);
	if(step == xSize)
		step++;
	x = x - step * ((int)strlen(numStr) - 1) / 2;

	/* clear area under number if possible */
	if(hasClear)
	{
		cLeft = max(left,x - xSize / 2 - 1);
		cBottom = max(bottom,y - ySize / 2 - 1);
		cRight = min(right,x + step * ((int)strlen(numStr) - 1) +
			     xSize / 2 + 1);
		cTop = min(top,y + ySize / 2 + 1);
		if(cLeft < cRight && cBottom < cTop)
			d_clear(cLeft,cBottom,cRight,cTop);
	}

	/* draw characters */
	for(ch = numStr; *ch; ch++)
	{
		GrNumChar(x,y,xSize,ySize,left,bottom,right,top,*ch);
		x += step;
	}
}

/***************************************************/
/* PlaceText - draw a text string in a given place */
/***************************************************/

static void PlaceText(form,placeNum,text,textType)
	register GRFORMAT *form;	/* format in use */
	int placeNum;			/* number of place */
	register char *text;		/* pointer to character to be placed */
	int textType;			/* current char type 0-2 */
{
	register SPECPLACE *place;	/* pointer to structure for this place */
	char line[131];			/* one line of text */
	int lineNum;			/* line number */
	int charNum;			/* character number */
	register int x;			/* x location being drawn at */
	register int y;			/* y location being drawn at */
	int width;			/* number of characters on current */
					/* line */

	place = form->sPlace + placeNum;
	d_settxt(place->type);
	x = place->x;
	y = place->y;

	/* take special action for places 0,1,and 2 */
	switch(placeNum) {
	case 0 :
		x = (form->sWindow[1].left + form->sWindow[1].right +
			chX[textType] * x ) / (unsigned)2;
		y = form->sWindow[1].top + chY[textType] * y / 2;
		break;
	case 1 :
		if( ! place->isSideways) {
			x = ((int)form->sWindow[1].left +
				(int)form->sWindow[1].right +chX[textType] *
				x )/2;
			y = minLabY - chY[textType] * y / 2;
		}
		break;
	case 2 :
		if(place->isSideways) {
			x = minLabX - chY[textType] * x;
			y = ((int)form->sWindow[1].bottom +
				(int)form->sWindow[1].top + chX[textType] *
				y ) / 2;
		} else {
			y = form->sWindow[1].top + chY[textType] * y / 2;
			x = form->sWindow[1].left + chX[textType] * x / 2;
		}
		break;
	default:	/* don't draw higher places in a mosaic */
		if( (xBoxes | yBoxes) != 1)
			return;
	}
	width = place->width;
	if(width > sizeof(line))
		width = sizeof(line);
	lineNum = 0;

	while(strlen(text) > (unsigned)width)
	{
		charNum = width;
		while(charNum && text[charNum] != ' ' &&
		      text[charNum] != '	')
			charNum--;
		if(!charNum)
			charNum = width;
		if(lineNum == place->height - 1)
		{
			charNum = width - 2;
			strncpy(line,text,charNum);
			line[charNum] = '-';
			line[charNum + 1] = '>';
			line[charNum + 2] = 0;
			d_move(x,y);
			if(place->isCentered)
				d_ctext(line,place->isSideways);
			else
				d_text(line,place->isSideways);
			return;
		}
		else
		{
			strncpy(line,text,charNum);
			line[charNum] = 0;
			d_move(x,y);
			if(place->isCentered)
				d_ctext(line,place->isSideways);
			else
				d_text(line,place->isSideways);
		}
		if(lineNum == 0)
		{
			if(place->isSideways)
				y += place->indent * chX[place->type];
			else
				x += place->indent * chX[place->type];
			width -= place->indent;
		}
		if(place->isSideways)
			x += chY[place->type];
		else
			y -= chY[place->type];
		lineNum++;
		text += charNum + 1;
		while(*text && (*text == ' ' || *text == '	'))
			text++;
	}
	if(*text)
	{
		d_move(x,y);
		if(place->isCentered)
			d_ctext(text,place->isSideways);
		else
			d_text(text,place->isSideways);
	}
}

/*******************************************/
/* InitDevice - initialize a device driver */
/*******************************************/

/* returns 0 if driver exists, 1 if not */
static int InitDevice(device)
	char *device;		/* name of device to initialize */
{
	extern char *getenv();
	/* if device is null string, use terminal name in environment */
	if(!*device)
	{
		strncpy(device,getenv("TERM"),TERMNMLEN - 1);
		device[TERMNMLEN - 1] = 0;
	}

	/* make sure we're not going to get killed by a nasty device name */
	if(!strcmp(device,"end"))
		error_("Illegal device name");

	/* find correct driver */
	cDrive = drivers;
	while(1)
	{
		if(!strcmp(cDrive->name,device))
		{
			cDrive->driver();
			return(0);
		}
		if(!strcmp(cDrive->name,"end"))
			return(1);
		cDrive++;
	}
}

/*********************************************************************/
/* GetDevice - initialize a device asking the user for a replacement */
/*	       if the given device is not supported		     */
/*********************************************************************/

/* returns 1 if graphics should not continue */
static int GetDevice(device)
	char *device;		/* device name */
{
	if(noShow)
		return(1);
	while(InitDevice(device))
	{
		if(!strcmp(device,"none"))
		{
			hasClear = 1;
			hasCursor = 0;
			strcpy(termType,device);
			return(1);
		}
		fprintf(stderr,"\n");
		if(strcmp(device,"du"))
			fprintf(stderr,"No graphics for terminal '%s'\n",
				device);
		do
		{
			fprintf(stderr,"Enter name of device to use\n");
			fprintf(stderr,"(enter 'none' to abort plot),\n");
			fprintf(stderr,
			     "(enter 'help' for list of supported devices): ");
			scanf("%s",device);
			while(getchar() != '\n')
				;
			fprintf(stderr,"\n");
			if(!strcmp(device,"help"))
			{
				fprintf(stderr,"Supported devices:\n");
				cDrive = drivers;
				while(strcmp(cDrive->name,"end"))
				{
					fprintf(stderr," %s\n",cDrive->name);
					cDrive++;
				}
				fprintf(stderr,"\n");
			}
		}
		while(!strcmp(device,"help"));
	}
	return(0);
}
/****************************************/
/* Pabort - abort showing some graphics */
/****************************************/

void Pabort()
{
	/* only abort if graphics are in progress */
	if(plotting)
		d_cleanup();
}

/**************************************/
/* Pcursor - read the graphics cursor */
/**************************************/

/* returns graphics cursor read structure */
GCREAD Pcursor()
{
	int xRead;		/* x value read */
	int yRead;		/* y value read */
	PICTURE *picP;		/* picture that cursor read is in */
	GCREAD tempRead;	/* temporary graphics cursor read structure */
	char answer;		/* answer to whether or not this is last */
				/* cursor read (if device has no cursor) */
	double exp();

	/* initialize terminal */
	GetDevice(defTerm);

	/* if device has no cursor, enter location manually */
	if(!hasCursor)
	{
		fprintf(stderr,"No graphics input for this device\n");
		if(xBoxes == 1)
			tempRead.xBox = 1;
		else
		{
			fprintf(stderr,
				"	Enter column number of picture: ");
			scanf("%d",&tempRead.xBox);
			while(getchar() != '\n')
				;
		}
		if(yBoxes == 1)
			tempRead.yBox = 1;
		else
		{
			fprintf(stderr,"	Enter row number of picture: ");
			scanf("%d",&tempRead.yBox);
			while(getchar() != '\n')
				;
		}
		fprintf(stderr,"	Enter x: ");
		scanf("%f",&tempRead.x);
		while(getchar() != '\n')
			;
		fprintf(stderr,"	Enter y: ");
		scanf("%f",&tempRead.y);
		while(getchar() != '\n')
			;
		do
		{
			fprintf(stderr,
				"	Last graphics input? ('y' or 'n'): ");
			scanf("%c",&answer);
			if(getchar() != '\n')
			{
				answer = 0;
				while(getchar() != '\n')
					;
			}
		}
		while(answer != 'y' && answer != 'n');
		tempRead.endFlag = (answer == 'y');
		return(tempRead);
	}

	/* read cursor */
	d_cursor(&xRead,&yRead,&tempRead.endFlag);

	/* identify box containing cursor read */
	tempRead.xBox = (int)(xRead * xBoxes / (xRes + 1));
	tempRead.yBox = (int)(yRead * yBoxes / (yRes + 1));
	if(!(picP = boxes[tempRead.yBox * xBoxes + tempRead.xBox]))
	{
		tempRead.isPlot = 0;
		return(tempRead);
	}
	tempRead.isPlot = 1;

	/* scale cursor read */
	tempRead.x = (double)(xRead - (picP)->leftPel) *
		     (picP)->xPelScale + (picP)->left;
	tempRead.y = (double)(yRead - (picP)->bottomPel) *
		     (picP)->yPelScale + (picP)->bottom;
	if((picP)->xLog)
		tempRead.x = exp(tempRead.x);
	if((picP)->yLog)
		tempRead.y = exp(tempRead.y);

	return(tempRead);
}

/************************************************************/
/*  PFile() - return a FILE pointer to a valid printer file */
/*  Thu Mar 12 09:56:03 EST 1998 -- mwp
/************************************************************/
FILE *PFile() {

	FILE *fout;
	char *mktemp();
	if(strcmp(hcfile,"")==0) {/* we must create a temp file */
/* I guess do the error checking here rather than in the
 * drivers.  The strncmp should be ok for Solaris and Linux which
 * return different values on error. But both return the valid
 * filename on success. I assume HP does the same.
 */
  
  
		for(;;) {
		  strcpy (hcfile, "/tmp/comb.XXXXXX");
		  if( (strncmp(mktemp(hcfile),"/tmp/comb.",10)== 0) &&
		      (fout = FSafeOpen(hcfile,hcfile,"w")) != NULL)
			return(fout);
		  sleep(39); /* Wait for previous file to clear */
		}
	}
/* user supplied a file name. let FSafeOpen check for pilot errors */
	return(FSafeOpen(hcfile,hcfile,"w"));
}
