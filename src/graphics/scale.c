/*Date: Sun, 05 Feb 95 10:28:52 -0500
 From: Bob Wilson <rww@big.att.com>

Marc,

This version doesn't get hung up with incomplete stacks the way the old
one does.  It just checks for reasonable limits to the plots.  The odd
looking test: left != left is looking for NaNs.

Bob
 
Fri Apr  5 11:31:27 EST 1996
  
Now I have disabled the isNoisy option if the cutOff (.rms) parameter is zero
or NaN.
*/

# include <math.h>
# include <stdarg.h>

# include "C.h"
# include "include.h"
#include "../main/dragon.h"

# define TOLERANCE 20.		/* maximum ratio of difference between */
				/* a datum and the average of its */
				/* neighbors with the noisiness of the */
				/* data that can be tolerated before */
				/* datum is considered bad */
# define BREATH 15.		/* fraction of window to be used for */
				/* "breathing room" */


#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* scale.c */
static double ChkNext P_((LISTPOINTER *listp));
static void ScaleList P_((int isLog, LISTPOINTER list, int len,
	int isNoisy, double cutOff, double *low, double *high));
static double eToTheX P_((double x));
#undef P_

/***************************************/
/* MkScale - scale the current picture */
/***************************************/

/*VARARGS*/
MkScale(int firstOpt, ...)
{
	va_list opList;		/* list of options */
	char opt;		/* option being handled */
	double left;		/* left value of window */
	double bottom;		/* bottom value of window */
	double right;		/* right value of window */
	double top;		/* top value of window */
	int xLog;		/* 1 if x is to be logarithmic */
	int yLog;		/* 1 if y is to be logarithmic */
	int xIsList;		/* 1 if x boundaries should be set from list */
	int yIsList;		/* 1 if y boundaries should be set from list */
	LISTPOINTER xList;	/* list of values for x */
	int xLen;		/* length of xList */
	LISTPOINTER yList;	/* list of values for y */
	int yLen;		/* length of yList */
	int xNoisy;		/* 1 if x should be treated as noisy */
	int yNoisy;		/* 1 if y should be treated as noisy */
	double xCutOff;		/* this is compared with the difference */
				/* between each element of xList and the */
				/* average of its neighbors to see if that */
				/* element should be used to set bounds */
	double yCutOff;		/* same as xCutOff but for y */
	int xRev;		/* 1 if x dimension should be reversed */
	int yRev;		/* 1 if y dimension should be reversed */
	double temp;		/* temporary variable for switching doubles */

	/* set default values */
	left = 0.;
	bottom = 0.;
	right = 1.;
	top = 1.;
	xLog = 0;
	yLog = 0;
	xIsList = 0;
	yIsList = 0;
	xNoisy = 0;
	yNoisy = 0;
	xRev = 0;
	yRev = 0;

	/* add list of options to option list */
	va_start(opList, firstOpt);
	AddOpt(firstOpt, opList);
	va_end(opList);

	/* process options */
	while(opt = GtOpt()) switch(opt)
	{
		case V_XLIST:
		case V_fXLIST:
		case V_iXLIST:
			xIsList = 1;
			xList = optArg[0].l;
			xLen = optArg[1].i;
			break;
		case V_XRANGE:
			left = optArg[0].d;
			right = optArg[1].d;
			break;
		case V_YLIST:
		case V_fYLIST:
		case V_iYLIST:
			yIsList = 1;
			yList = optArg[0].l;
			yLen = optArg[1].i;
			break;
		case V_YRANGE:
			bottom = optArg[0].d;
			top = optArg[1].d;
			break;
		case V_XNOISE:
			xNoisy = 1;
			xCutOff = optArg[0].d * 2 * TOLERANCE;
			break;
		case V_YNOISE:
			yNoisy = 1;
			yCutOff = optArg[0].d * TOLERANCE;
			break;
		case V_XLOG:
			xLog = 1;
			break;
		case V_YLOG:
			yLog = 1;
			break;
		case V_XREVERSE:
			xRev = 1;
			break;
		case V_YREVERSE:
			yRev = 1;
			break;
		case NO V_XLOG:
		case NO V_YLOG:
		case NO V_XREVERSE:
		case NO V_YREVERSE:
			break;
		default:
			error_("Invalid option to MkScale");
	}

	/* find left and right boundaries */
	if(xIsList)
		ScaleList(xLog,xList,xLen,xNoisy,xCutOff,&left,&right);
	temp = fabs(right - left);
	if(temp < 1e-6) {
		right += 1;
	}
	if(temp > 1e6 || left != left || right != right) {
		left = 0;
		right = 1;
	}
	if(xRev)
	{
		temp = left;
		left = right;
		right = temp;
	}

	/* find bottom and top boundaries */
	if(yIsList)
		ScaleList(yLog,yList,yLen,yNoisy,yCutOff,&bottom,&top);
	temp = fabs(top - bottom);
	if(temp < 1e-6) {
		top += 1e-6;
		bottom -= 1e-6;
	}
	if(temp > 1e6) {
		top = 1e6;
		bottom = -1e6;
	}
	if(yRev)
	{
		temp = bottom;
		bottom = top;
		top = temp;
	}

	/* set boundaries */
	Plogbounds(left,bottom,right,top,xLog,yLog);
}

static double ChkNext(listp)
LISTPOINTER *listp;
{
	double value;
	double NextInList();

	return( ((value = NextInList(listp)) != DRAGON && fabs(value) >
		MAXVALUE / 2.)? MAXVALUE: value);
}

/*********************************************************************/
/* ScaleList - find the high and low values in a list ignoring noise */
/*********************************************************************/

static void ScaleList(isLog,list,len,isNoisy,cutOff,low,high)
	int isLog;		/* whether scale is logarithmic */
	LISTPOINTER list;	/* list to be scaled */
	int len;		/* length of list */
	int isNoisy;		/* whether compensation should be made */
				/* for noise (1 if yes) */
	double cutOff;		/* noisiness cutoff (see xCutOff above) */
	double *low;		/* low value in list */
	double *high;		/* high value in list */
{
	double prevEl;		/* previous element of list */
	double thisEl;		/* current element of list */
	double nextEl;		/* next element of list */
	double diff;		/* difference between low and high */
	int count;		/* counter for number of elements processed */
	int ok;			/* 1 if thisEl is within noise limits */
	double InList();
	double log(),eToTheX();
	double fabs();

	/* First a sanity check for looking at noisy data */
	if(cutOff <= 0 || cutOff != cutOff)
		isNoisy = 0;
	/* handle trivial case */
	if(len == 1)
	{
		*low = ChkNext(&list);
		*high = *low;
		return;
	}

	/* find first valid element of list */
	thisEl = ChkNext(&list);
	nextEl = ChkNext(&list);
	count = 2;
	if(isNoisy || thisEl == DRAGON)
	{
		if(thisEl == DRAGON || nextEl == DRAGON)
			ok = 0;
		else
			ok = fabs(thisEl - nextEl) <= cutOff;
		while(count < len && (!ok || thisEl == DRAGON))
		{
			prevEl = thisEl;
			thisEl = nextEl;
			nextEl = ChkNext(&list);
			count++;
			if(prevEl == DRAGON ||
			   thisEl == DRAGON ||
			   nextEl == DRAGON)
				ok = 0;
			else
				ok = fabs(thisEl - (prevEl + nextEl) / 2)
				     <= cutOff;
		}
	}

	/* check values in the middle */
	*high = thisEl;
	*low = thisEl;
	while(count < len)
	{
		prevEl = thisEl;
		thisEl = nextEl;
		nextEl = ChkNext(&list);
		count++;
		if(prevEl == DRAGON ||
		   thisEl == DRAGON ||
		   nextEl == DRAGON)
			ok = 0;
		else
			ok = fabs(thisEl - (prevEl + nextEl) / 2) <= cutOff;
		if(thisEl != DRAGON && (!isNoisy || ok))
			if(thisEl > *high)
				*high = thisEl;
			else if(thisEl < *low)
				*low = thisEl;
	}

	/* check last value */
	if(nextEl != DRAGON) if(!isNoisy || fabs(thisEl - nextEl) <= cutOff)
		if(nextEl > *high)
			*high = nextEl;
		else if(nextEl < *low)
			*low = nextEl;

	/* give data room to breath */
	if(isLog)
	{
		if(*high <= 0)
			error_("No data above 0 in logarithmic scale");
		if(*low <= 0)
			*low = *high / 20.;		/* arbitrary */
		diff = log(*high) - log(*low);
		*high = eToTheX(log(*high) + diff / BREATH);
		*low = eToTheX(log(*low) - diff / BREATH);
	}
	else
	{
		diff = *high - *low;
		*high += diff / BREATH;
		*low -= diff / BREATH;
	}

	/* make sure that range is at least TOLERANCE times the noise */
	if(isNoisy)
	{
		diff = *high - *low;
		if(diff < cutOff)
			if(isLog)
				*high += (cutOff - diff);
			else
			{
				*high += (cutOff - diff) / 2;
				*low -= (cutOff - diff) / 2;
			}
	}
}

static double eToTheX(x)
	double x;
{
	double exp();

	return((x < 0)?(1 / exp(-x)):exp(x));
}


