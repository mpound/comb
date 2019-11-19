# include <stdarg.h>
#include <math.h>

# include "C.h"
# include "include.h"
#include "../main/dragon.h"

/**************************/
/* MkGraph - make a graph */
/**************************/

/*VARARGS*/
void MkGraph(int firstOpt, ...)
{
	va_list opList;		/* list of options */
	char opt;		/* option */
	LISTPOINTER xList;	/* list of x values */
	int xLen;		/* length of xList */
	LISTPOINTER yList;	/* list of y values */
	int yLen;		/* length of yList */
	int numPoints;		/* number of points on graph */
	double xLow;		/* low value of x if range */
	double yLow;		/* low value of y if range */
	double xHigh;		/* high value of x if range */
	double yHigh;		/* high value of y if range */
	int xIsList;		/* 1 if x is list of values */
	int yIsList;		/* 1 if y is list of values */
	int graphType;		/* 0 = lines, 1 = histogram, 2 = figures */
	int lineType;		/* line type to make graph with */
	int fgType;		/* type of figure to use (if graphType == 2) */
	int fgfilled=0;         /* figure is filled, rather than open */
	double fgWidth;		/* figure width */
	double fgHeight;	/* figure height */
	LISTPOINTER wList;	/* list of widths */
	int wLen;		/* length of wList */
	LISTPOINTER hList;	/* list of heights */
	int hLen;		/* length of hList */
	int wIsList;		/* 1 if figure widths come from a list */
	int hIsList;		/* 1 if figure heights come from a list */
	int reverseX;		/* 1 if x is to be reversed */
	int reverseY;		/* 1 if y is to be reversed */
	double temp;		/* general temporary variable */
	int lens[4];		/* possible numbers of points on graph */
	double xStep;		/* step between x's if !xIsList */
	double yStep;		/* step between y's if !yIsList */
	int givenXs=0; 		/* are xStep and/or  yStep  in arglist? */
	int givenYs=0;		/* Needed for histogram * plotting (th) 
				 * -- MWP 5/4/94
				 */
	double x;		/* x location being plotted */
	double y;		/* y location being plotted */
	double ox;		/* previous x location plotted */
	double oy;		/* previous y location plotted */
	double NextInList();
	int count;		/* general purpose counter */
	int drawing;		/* flag for whether lines are being drawn */

	/* add list of options to option list */
	va_start(opList, firstOpt);
	AddOpt(firstOpt, opList);
	va_end(opList);

	/* set up defaults */
	numPoints = 0;
	if((cPic)->left < (cPic)->right)
	{
		xLow = (cPic)->left;
		xHigh = (cPic)->right;
	}
	else
	{
		xLow = (cPic)->right;
		xHigh = (cPic)->left;
	}
	if((cPic)->xLog)
	{
		xLow = exp(xLow);
		xHigh = exp(xHigh);
	}
	if((cPic)->bottom < (cPic)->top)
	{
		yLow = (cPic)->bottom;
		yHigh = (cPic)->top;
	}
	else
	{
		yLow = (cPic)->top;
		yHigh = (cPic)->bottom;
	}
	if((cPic)->yLog)
	{
		yLow = exp(yLow);
		yHigh = exp(yHigh);
	}
	xIsList = 0;
	yIsList = 0;
	graphType = 0;
	lineType = LINTYPES - 1;
	fgWidth = (xHigh - xLow) / 40;
	fgHeight = (yHigh - yLow) / 40;
	wIsList = 0;
	hIsList = 0;
	reverseX = 0;
	reverseY = 0;

	/* process options */
	while(opt = GtOpt()) switch(opt)
	{
/* MWP 5/4/93 XSTEP, YSTEP needed for historgram plotting (th) */
                case V_XSTEP:
                        xStep = Roundd(optArg[0].d);
                        givenXs = 1;
                        break;
                case V_YSTEP:
                        yStep = Roundd(optArg[0].d);
                        givenYs = 1;
                        break;
		case V_XLIST:
		case V_fXLIST:
		case V_iXLIST:
			xList = optArg[0].l;
			xLen = optArg[1].i;
			xIsList = 1;
			break;
		case V_YLIST:
		case V_fYLIST:
		case V_iYLIST:
			yList = optArg[0].l;
			yLen = optArg[1].i;
			yIsList = 1;
			break;
		case V_XRANGE:
			xLow = optArg[0].d;
			xHigh = optArg[1].d;
			xIsList = 0;
			break;
		case V_YRANGE:
			yLow = optArg[0].d;
			yHigh = optArg[1].d;
			yIsList = 0;
			break;
		case V_LTYPE:
			lineType = optArg[0].i;
			break;
		case V_LINES:
			graphType = 0;
			break;
		case V_HIST:
			graphType = 1;
			break;
		case V_FIGURE:
			fgType = optArg[0].i;
			graphType = 2;
			break;
		case V_FWIDTH:
			fgWidth = optArg[0].d;
			wIsList = 0;
			break;
		case V_FHEIGHT:
			fgHeight = optArg[0].d;
			hIsList = 0;
			break;
		case V_FILLED:
			fgfilled = 1;
			break;
		case V_FWLIST:
		case V_fFWLIST:
		case V_iFWLIST:
			wList = optArg[0].l;
			wLen = optArg[1].i;
			wIsList = 1;
			break;
		case V_FHLIST:
		case V_fFHLIST:
		case V_iFHLIST:
			hList = optArg[0].l;
			hLen = optArg[1].i;
			hIsList = 1;
			break;
		case V_XREVERSE:
			reverseX = 1;
			break;
		case V_YREVERSE:
			reverseY = 1;
			break;
		default:
			error_("Invalid option for MkGraph");
	}

	/* check for errors */
	if(graphType == 1 && xIsList && yIsList)
		error_("Histogram must have one RANGE dimension");

	/* reverse x or y if necessary */
	if(reverseX && !xIsList)
	{
		temp = xLow;
		xLow = xHigh;
		xHigh = temp;
	}
	if(reverseY && !yIsList)
	{
		temp = yLow;
		yLow = yHigh;
		yHigh = temp;
	}

	/* find number of points on graph */
	count = 0;
	if(xIsList)
		lens[count++] = xLen;
	if(yIsList)
		lens[count++] = yLen;
	if(graphType == 2)
	{
		if(wIsList)
			lens[count++] = wLen;
		if(hIsList)
			lens[count++] = hLen;
	}
	if(count--)
	{
		numPoints = lens[count];
		while(count--)
			if(numPoints > lens[count])
				numPoints = lens[count];
	}

	/* do nothing if there is nothing to do */
	if(numPoints == 0)
		return;

	/* find x and y steps if ranges */
	if(!xIsList && !givenXs) /* MWP 5/4/93 */
		if(numPoints > 1)
			xStep = (xHigh - xLow) / (numPoints - 1);
		else
			xStep = 0.;
	if(!yIsList && !givenYs) /* MWP 5/4/93 */
		if(numPoints > 1)
			yStep = (yHigh - yLow) / (numPoints - 1);
		else
			yStep = 0.;

	/* make graph */
	drawing = 0;
	Plinetype(lineType);
	if(!xIsList)
		x = xLow - xStep;
	else
		x = 0;
	if(!yIsList)
		y = yLow - yStep;
	else
		y = 0;
	for(count = 0; count < numPoints; count++)
	{
		ox = x;
		if(xIsList)
			x = NextInList(&xList);
		else
			x += xStep;
		oy = y;
		if(yIsList)
			y = NextInList(&yList);
		else
			y += yStep;
		if(x == DRAGON || y == DRAGON) {
			if(graphType == 1 && drawing)
				Pline(ox,oy);
			drawing = 0;
		} else {
			if(fabs(x) > MAXVALUE)
				x = MAXVALUE;
			if(fabs(y) > MAXVALUE)
				y = MAXVALUE;
			switch(graphType)
			{
				case 0:
					if(!drawing)
					{
						Pmove(x,y);
						drawing = 1;
					}
					else
						Pline(x,y);
					break;
				case 1:
					if(!drawing)
					{
						Pmove(x,y);
						drawing = 1;
					}
					else
					{
						if(!xIsList)
						{
							Pline(x - xStep/2,oy);
							Pline(x - xStep/2,y);
						}
						else if(!yIsList)
						{
							Pline(ox,y - yStep/2);
							Pline(x,y - yStep/2);
						}
					}
					break;
				case 2:
					if(wIsList)
						fgWidth = NextInList(&wList);
					if(hIsList)
						fgHeight = NextInList(&hList);
					Pfigure(x,y,fgType,fgWidth,fgHeight,fgfilled);
					break;
			}
		}
	}
	if(graphType == 1 && drawing)
		Pline(x,y);
}
