# include <math.h>
# include <stdarg.h>

# include "C.h"
# include "include.h"
# include "step.h"

#define TWOPI 6.28318530718e+0

/********************************************/
/* MkGrid - make a grid in the graph window */
/********************************************/

/*VARARGS*/
void MkGrid(int firstOpt, ...)
{
	va_list opList;		/* list of options */
	char opt;		/* option */
	int xDivs;		/* maximum number of divisions in x */
	int yDivs;		/* maximum number of divisions in y */
	double xFrom;		/* beginning of grid in x */
	double yFrom;		/* beginning of grid in y */
	double xTo;		/* end of grid in x */
	double yTo;		/* end of grid in y */
	double xStep;		/* amount to increment x by between grid */
				/* marks */
	double yStep;		/* amount to increment y be between grid */
				/* marks */
	double hTicLen;		/* length of horizontal tics */
				/* (expressed as a fraction of the window) */
	double vTicLen;		/* length of vertical tics */
				/* (expressed as a fraction of the window) */
	int lineType;		/* type of line with which to draw grid */
	int defXbounds;		/* 1 if x bounds are to default */
	int defYbounds;		/* 1 if y bounds are to default */
	int defXStep;		/* 1 if xStep is to default */
	int defYStep;		/* 1 if yStep is to default */
	int keyX;		/* 1 if x axis should be keyed to grid */
	int keyY;		/* 1 if y axis should be keyed to grid */
	int defLineType;	/* 1 if lineType is to default */
	int flag0x;		/* 1 if x = 0 should be emphasized */
	int flag0y;		/* 1 if y = 0 should be emphasized */
	int labelX;		/* 1 if x axis should have numeric labels */
	int labelY;		/* 1 if y axis should have numeric labels */
	int labXPow;		/* 1 if x should be labelled with only */
				/* powers of 10 when x is logarithmic */
	int labYPow;		/* 1 if y should be labelled with only */
				/* powers of 10 when y is logarithmic */
	int yesGrid;		/* 1 if grid should be drawn */
	int yesBorder;		/* 1 if border should be drawn */
	int useLines;		/* 1 if grid should be drawn with lines */
				/* rather than tic marks */
	int circBorder;		/* 1 if border should be elliptical */
	double *xStepSave;	/* save xStep here if non zero */
	double *yStepSave;	/* save yStep here if non zero */
	double l;		/* left of window */
	double b;		/* bottom of window */
	double r;		/* right of window */
	double t;		/* top of window */
	double lt;		/* end of tics on left */
	double bt;		/* end of tics on bottom */
	double rt;		/* end of tics on right */
	double tt;		/* end of tics on top */
	double tmp;		/* temporary variable for switching */
				/* l with r or b with t */
	int xBack;		/* 1 if x is backward */
	int yBack;		/* 1 if y is backward */
	double x;		/* counter for x */
	double y;		/* counter for y */
	int xLabHms, yLabHms;	/* 1 if x (y) is labeled in hh:mm:ss fmt */

	double exp(),log(),log10();
	double ChooseLogStep();
	double ChooseStep();
	double Roundd();
	double fabs();

	struct stepvals xStepVals, yStepVals;

	/* add list of options to option list */
	va_start(opList, firstOpt);
	AddOpt(firstOpt, opList);
	va_end(opList);

	/* set up default values */
	xDivs = 8;
	yDivs = 8;
	hTicLen = .018;
	vTicLen = .018;
	defXbounds = 1;
	defYbounds = 1;
	defXStep = 1;
	defYStep = 1;
	xStepSave = (double *)0;
	yStepSave = (double *)0;
	keyX = 0;
	keyY = 0;
	defLineType = 1;
	flag0x = 1;
	flag0y = 1;
	labelX = 1;
	labelY = 1;
	labXPow = 1;
	labYPow = 1;
	yesGrid = 1;
	yesBorder = 1;
	circBorder = 0;
	useLines = 0;
	xLabHms = 0;
	yLabHms = 0;

	/* process options */
	while(opt = GtOpt()) switch(opt)
	{
		case V_XSTEP:
			xStep = Roundd(optArg[0].d);
			defXStep = 0;
			break;
		case V_YSTEP:
			yStep = Roundd(optArg[0].d);
			defYStep = 0;
			break;
		case V_STEPSTOR:
			xStepSave = optArg[0].l.point.d;
			yStepSave = optArg[1].l.point.d;
			break;
		case V_KEYX:
			keyX = 1;
			break;
		case V_KEYY:
			keyY = 1;
			break;
		case V_XRANGE:
			xFrom = Roundd(optArg[0].d);
			xTo = Roundd(optArg[1].d);
			defXbounds = 0;
			break;
		case V_YRANGE:
			yFrom = Roundd(optArg[0].d);
			yTo = Roundd(optArg[1].d);
			defYbounds = 0;
			break;
		case V_LABELX:
			labXPow = 0;
			break;
		case NO V_LABELX:
			labelX = 0;
			labXPow = 0;
			break;
		case V_LABELY:
			labYPow = 0;
			break;
		case NO V_LABELY:
			labelY = 0;
			labYPow = 0;
			break;
		case V_XLABHMS:
			xLabHms = 1;
			break;
		case V_YLABHMS:
			yLabHms = 1;
			break;
		case NO V_GRID:
			yesGrid = 0;
			break;
		case V_LINES:
			useLines = 1;
			break;
		case V_LTYPE:
			lineType = optArg[0].i;
			defLineType = 0;
			break;
		case V_XDIVS:
			xDivs = optArg[0].i;
			break;
		case V_YDIVS:
			yDivs = optArg[0].i;
			break;
		case V_HTICLEN:
			hTicLen = Roundd(optArg[0].d);
			break;
		case V_VTICLEN:
			vTicLen = Roundd(optArg[0].d);
			break;
		case NO V_FLAGX0:
			flag0x = 0;
			break;
		case NO V_FLAGY0:
			flag0y = 0;
			break;
		case NO V_BORDER:
			yesBorder = 0;
			break;
		case V_CIRCBORDER:
			circBorder = 1;
			break;
		case V_BORDER:
		case NO V_CIRCBORDER:
		case NO V_KEYX:
		case NO V_KEYY:
		case V_GRID:
		case NO V_LINES:
		case V_FLAGX0:
		case V_FLAGY0:
			break;
		default:
			error_("Invalid option to MkGrid");
	}

	/* make defaults that apply to both x and y axis */
	if(defLineType)
		if(useLines)
			lineType = GRIDlt;
		else
			lineType = TICSlt;

	/* x axis default values */
	xBack = ((cPic)->left > (cPic)->right);
	if((cPic)->xLog)
	{
		l = Roundd(exp((cPic)->left));
		r = Roundd(exp((cPic)->right));
		if(xBack)
		{
			tmp = l;
			l = r;
			r = tmp;
		}
		if(defXbounds)
		{
			if(defXStep) 
				xStep = ChooseLogStep(l,r,xDivs);
			if(keyX)
				KeyValues(1,&l,&r,xStep,1);
			xFrom = l;
			xTo = r;
			KeyValues(1,&xFrom,&xTo,xStep,0);
		}
		else
		{
			if(xFrom < l || xFrom > r || xTo < l || xTo > r)
				error_("X bounds of grid outside of window");
			if(xFrom > xTo)
			{
				tmp = xFrom;
				xFrom = xTo;
				xTo = tmp;
			}
			if(defXStep) 
				xStep = ChooseLogStep(xFrom,xTo,xDivs);
			if(keyX)
				KeyValues(1,&l,&r,xStep,1);
		}
		lt = Roundd(exp(log(l) + (log(r) - log(l)) * hTicLen));
		rt = Roundd(exp(log(r) - (log(r) - log(l)) * hTicLen));
/* guess at values for these two -- see comment below --- MWP 5/93 */
                xStepVals.exp = (int)(log10((double) xStep)+.5);
                xStepVals.ss.fmt = 1;
	}
	else
	{
		l = Roundd((cPic)->left);
		r = Roundd((cPic)->right);
		if(xBack)
		{
			tmp = l;
			l = r;
			r = tmp;
		}
		if(defXbounds)
		{
#if 0
			if(defXStep)
				xStep = ChooseStep(l,r,xDivs);
			if(keyX)
				KeyValues(0,&l,&r,xStep,1);
			xFrom = l;
			xTo = r;
			KeyValues(0,&xFrom,&xTo,xStep,0);
#else /*0*/
			if(!xLabHms || !(xLabHms = ChooseHmsStep(l, r,
					xDivs, keyX, &xStepVals)))
				ChooseDecStep(l, r, xDivs, keyX, &xStepVals);
			xStep = pow(10., (double)xStepVals.exp) /
				xStepVals.ss.div;
			xFrom = xStepVals.nl * xStep;
			xTo = xStepVals.nh * xStep;
			if(keyX) {
				l = xFrom;
				r = xTo;
			}
#endif /*0*/
		}
		else
		{
			if(xFrom < l || xFrom > r || xTo < l || xTo > r)
				error_("X bounds outside of window");
			if(xFrom > xTo)
			{
				tmp = xFrom;
				xFrom = xTo;
				xTo = tmp;
			}
			if(defXStep)
				xStep = ChooseStep(xFrom,xTo,xDivs);
			if(keyX)
				KeyValues(0,&l,&r,xStep,1);
		}
		lt = l + (r - l) * hTicLen;
		rt = r - (r - l) * hTicLen;
	}
	if(xStepSave)
		*xStepSave = xStep;

	/* y axis default values */
	yBack = ((cPic)->bottom > (cPic)->top);
	if((cPic)->yLog)
	{
		b = Roundd(exp((cPic)->bottom));
		t = Roundd(exp((cPic)->top));
		if(yBack)
		{
			tmp = b;
			b = t;
			t = tmp;
		}
		if(defYbounds)
		{
			if(defYStep) 
				yStep = ChooseLogStep(b,t,yDivs);
			if(keyY)
				KeyValues(1,&b,&t,yStep,1);
			yFrom = b;
			yTo = t;
			KeyValues(1,&yFrom,&yTo,yStep,0);
		}
		else
		{
			if(yFrom < b || yFrom > t || yTo < b || yTo > t)
				error_("Y bounds of grid outside of window");
			if(yFrom > yTo)
			{
				tmp = yFrom;
				yFrom = yTo;
				yTo = tmp;
			}
			if(defYStep) 
				yStep = ChooseLogStep(yFrom,yTo,yDivs);
			if(keyY)
				KeyValues(1,&b,&t,yStep,1);
		}
		bt = Roundd(exp(log(b) + (log(t) - log(b)) * vTicLen));
		tt = Roundd(exp(log(t) - (log(t) - log(b)) * vTicLen));
/*
 * Here we must put in a guess at reasonable values for yStepvals.exp
 * and yStepVals.ss.fmt, because they are referenced in the call to
 * Plabelfmt() below.  These structs are filled by ChooseDecStep() and
 * ChooseHmsStep() which are not called in the case of logarithmic
 * axes. That's why we have to give the numbers here.
 * MWP -- 5/6/93 
 */
		yStepVals.exp = (int)(log10((double) yStep)+.5);
		yStepVals.ss.fmt = 1;
/*printf("%g : %d : %d\n",yStep,yStepVals.exp, yStepVals.ss.fmt);*/
	}
	else
	{
		b = Roundd((cPic)->bottom);
		t = Roundd((cPic)->top);
		if(yBack)
		{
			tmp = b;
			b = t;
			t = tmp;
		}
		if(defYbounds)
		{
#if 0
			if(defYStep)
				yStep = ChooseStep(b,t,yDivs);
			if(keyY)
				KeyValues(0,&b,&t,yStep,1);
			yFrom = b;
			yTo = t;
			KeyValues(0,&yFrom,&yTo,yStep,0);
#else /*0*/
			if(!yLabHms || !(yLabHms = ChooseHmsStep(b, t,
					yDivs, keyY, &yStepVals)))
				ChooseDecStep(b, t, yDivs, keyY, &yStepVals);
			yStep = pow(10., (double)yStepVals.exp) /
				yStepVals.ss.div;
			yFrom = yStepVals.nl * yStep;
			yTo = yStepVals.nh * yStep;
			if(keyY) {
				b = yFrom;
				t = yTo;
			}
#endif /*0*/
		}
		else
		{
			if(yFrom < b || yFrom > t || yTo < b || yTo > t)
				error_("Y bounds of grid outside of window");
			if(yFrom > yTo)
			{
				tmp = yFrom;
				yFrom = yTo;
				yTo = tmp;
			}
			if(defYStep)
				yStep = ChooseStep(yFrom,yTo,yDivs);
			if(keyY)
				KeyValues(0,&b,&t,yStep,1);
		}
		bt = b + (t - b) * vTicLen;
		tt = t - (t - b) * vTicLen;
	}
	if(yStepSave)
		*yStepSave = yStep;

	/* rescale window if necessary */
	if(keyX || keyY) 
		if(!xBack && !yBack)
			ReBound(l,b,r,t,(cPic)->xLog,(cPic)->yLog);
		else if(!yBack)
			ReBound(r,b,l,t,(cPic)->xLog,(cPic)->yLog);
		else if(!xBack)
			ReBound(l,t,r,b,(cPic)->xLog,(cPic)->yLog);
		else
			ReBound(r,t,l,b,(cPic)->xLog,(cPic)->yLog);

	Plinetype(lineType);
	if ( ! circBorder ) 
	{
		/* Set up the formats for both axis labels */
		Plabelfmt((xLabHms)? HMSFMT: FFMT, xStepVals.ss.fmt - xStepVals.exp,
			(yLabHms)? HMSFMT: FFMT, yStepVals.ss.fmt - yStepVals.exp);
	
		/* draw x axis */
/*		xTo += 1e-8; */
		xTo += xStep / 100.;
		for(x = xFrom; x <= xTo; x += xStep)
		{
			/* x = Roundd(x); */
			if(labelX && (!(cPic)->xLog || !labXPow))
				Pxlabel(x);
			if(yesGrid)
			{
				if(flag0x && fabs(x) < 1e-14)
				{
					Plinetype(GRID0lt);
					Pmove(x,b);
					Pline(x,t);
					Plinetype(lineType);
				}
				else
				{
					Pmove(x,b);
					if(!useLines)
					{
						Pline(x,bt);
						Pmove(x,tt);
					}
					Pline(x,t);
				}
			}
			if((cPic)->xLog)
				if(fabs(log10(x) - (int)(log10(x))) < 1e-8)
				{
					if(labXPow) 
						Pxlabel(x);
					xStep = Roundd(xStep * 10.);
					x = xStep * (int)(Roundd(x / xStep));
				}
		}
	
		/* draw y axis */
/*		yTo += 1e-8; */
		yTo += yStep / 100;
		for(y = yFrom; y <= yTo; y += yStep)
		{
			/* y = Roundd(y); */
			if(labelY && (!(cPic)->yLog || !labYPow))
				Pylabel(y);
			if(yesGrid)
			{
				if(flag0y && fabs(y) < 1e-14)
				{
					Plinetype(GRID0lt);
					Pmove(l,y);
					Pline(r,y);
					Plinetype(lineType);
				}
				else
				{
					Pmove(l,y);
					if(!useLines)
					{
						Pline(lt,y);
						Pmove(rt,y);
					}
					Pline(r,y);
				}
			}
			if((cPic)->yLog)
				if(fabs(log10(y) - (int)(log10(y))) < 1e-8)
				{
					if(labYPow)
						Pylabel(y);
					yStep = Roundd(yStep * 10.);
					y = yStep * (int)(Roundd(y / yStep));
				}
		}
		if(yesBorder) 		/* draw border */
		{
			Plinetype(BORDERlt);
			Pborder();
		}
	}
	else		/* ticmarks for elliptical border */
	{
		if (yesGrid)
		{
 		    int ntics, itics;
		    double xcenter, ycenter, xhalfax, yhalfax;

		    ntics = 12;		/* tics at 30 degree angles */

		    xcenter = (l + r) / 2.;
		    ycenter = (b + t) / 2.;
		    xhalfax = fabs (r - l) / 2.;
		    yhalfax = fabs (t - b) / 2.;
		    for ( itics = 0; itics < ntics; itics ++)
		    {
			Pmove( xcenter + xhalfax * 
				cos((double)( TWOPI * itics)/ ntics ),
				ycenter + yhalfax * 
				sin((double)( TWOPI * itics)/ ntics ) );
			if (  !useLines)
				Pline( xcenter + xhalfax * (1.0-vTicLen) * 
					cos((double)( TWOPI * itics)/ ntics ),
					ycenter + yhalfax * (1.0-vTicLen) * 
					sin((double)( TWOPI * itics)/ ntics ) );
			else
				Pline( xcenter, ycenter);
		    }
		}
		if(yesBorder)
		{
			Plinetype(BORDERlt);
			Pcircborder();    /* elliptical border */
		}
	}
}

/***************************************************************************/
/* ChooseLogStep - choose a step size to start with in a logarithmic scale */
/***************************************************************************/

/* returns the step size */
double ChooseLogStep(low,high,maxDivs)
	double low;		/* low value of scale */
	double high;		/* high value of scale */
	int maxDivs;		/* maximum number of divisions */
{
	static double steps[] =	/* different legal step sizes */
	{
		1.,
		2.,
		2.5,
		5.,
		0.
	};
	int topPow;		/* top power of 10 in range */
	int bottomPow;		/* bottom power of 10 in range */
	int powsOf10;		/* number of powers of 10 in range */
	double top;		/* 10 ^ topPow */
	double bottom;		/* 10 ^ bottomPow */
	double extra;		/* extra stuff at top and bottom (outside */
				/* powers of 10) */
	double diff;		/* difference between low and high */
				/* scaled to reflect the way that */
				/* logarithmic scale is drawn (starts over */
				/* at each power of 10) */
	double step;		/* step size */
	double log10(),exp();
	double StepSize();
	double fabs();

	/* find scaled difference between high and low */
	topPow = (int)(log10(high));
	bottomPow = (int)(log10(low)) + 1;
	if(bottomPow < topPow)
		powsOf10 = topPow - bottomPow;
	else
		powsOf10 = 0;
	top = pow(10.,(double)(topPow));
	bottom = pow(10.,(double)(bottomPow));
	extra = (high - top) / top + (bottom - low) * 10 / bottom;
	diff = extra + 10 * powsOf10;

	/* return step size */
	step = StepSize(diff,steps,maxDivs);
	if(step > 10. + 1e-8)
		error_("Not enough allowable divisions for logarithmic scale");
	step = step * bottom / 10;
	if(fabs(log10(low) - (int)(log10(low))) < 1e-8)
		step /= 10;
	return(step);
}

/***************************************************************/
/* ChooseStep - choose a step size for a given range of values */
/***************************************************************/

/* returns step size */
double ChooseStep(low,high,maxDivs)
	double low;		/* low value of range */
	double high;		/* high value of range */
	int maxDivs;		/* maximum number of steps allowed */
{
	static double steps[] =	/* different legal step sizes */
	{
		1.,
		2.,
		2.5,
		4.,
		5.,
		0.
	};
	double StepSize();

	/* return step size */
	return(StepSize(high - low,steps,maxDivs));
}

/********************************************************************/
/* StepSize - find a good step size for dividing a given difference */
/********************************************************************/

/* returns step size */
double StepSize(diff,stepIndx,maxDivs)
	double diff;		/* difference to be divided */
	double *stepIndx;	/* list of legal step sizes */
	int maxDivs;		/* maximum number of divisions */
{
	double chkStep;		/* step size being checked */
	double realExp;		/* real exponent of diff */
	int exponent;		/* exponent of diff */
	double mantissa;	/* mantissa of diff */
	double bestStep;	/* best step size so far */
	int bestDivs;		/* number of divisions resulting from */
				/* best step size */
	double log10(),pow();
	double Roundd();

	/* put diff in scientific notation */
	realExp = log10(diff);
	exponent = (int)(realExp);
	if(exponent > 0 && exponent < realExp)
		exponent++;
	else if(exponent < 0 && exponent > realExp)
		exponent--;
	else if(exponent == 0)
		exponent = 1;
	mantissa = diff / pow(10., (double)(exponent));

	/* find best step size */
	bestDivs = 0;
	while(*stepIndx)
	{
		chkStep = *stepIndx;
		if((int)(mantissa / chkStep) <= maxDivs)
		{
			do
				chkStep /= 10.;
			while((int)(Roundd(mantissa / chkStep)) <= maxDivs);
			chkStep *= 10;
			if((int)(Roundd(mantissa / chkStep)) > bestDivs)
			{
				bestDivs = (int)(Roundd(mantissa / chkStep));
				bestStep = chkStep;
			}
		}
		stepIndx++;
	}

	/* return it */
	return(Roundd(bestStep * pow(10.,(double)(exponent))));
}

/***********************************************************/
/* KeyValues - key upper and lower bounds to the step size */
/***********************************************************/

void KeyValues(isLog,low,high,step,out)
	int isLog;		/* 1 if this is a logarithmic scale */
	double *low;		/* lower bound */
	double *high;		/* upper bound */
	double step;		/* step size */
	int out;		/* 1 if low and high should be keyed by */
				/* moving them away from each other, 0 if */
				/* by moving them toward each other */
{
	double lowKey;		/* lower of the two keyed values */
	double highKey;		/* upper of the two keyed values */
	double hStep;		/* step around high */
	double lStep;		/* step around low */
	double k1,k2,k3,k4;	/* four possible values to key to */
	double log10();
	double Roundd();

	/* put low and high into lowKey and highKey */
	lowKey = *low;
	highKey = *high;
	if(isLog)
	{
		if(fabs(log10(lowKey) - (int)(log10(lowKey))) < 1e-8)
			lStep = step * 10.;
		else
			lStep = step;
		hStep = lStep * pow(10.,(double)((int)(log10(highKey))
					- (int)(log10(lowKey))));
	}

	/* find four possible values to key to */
	if(isLog)
	{
		k1 = lStep * (int)(Roundd(lowKey / lStep));
		if(k1 < 1e-8)
		{
			k1 = pow(10.,(double)((int)(log10(lowKey))));
			k2 = lStep;
		}
		else
			k2 = k1 + lStep;
		k3 = hStep * (int)(Roundd(highKey / hStep));
		if(k3 < 1e-8)
		{
			k3 = pow(10.,(double)((int)(log10(highKey))));
			k4 = hStep;
		}
		else
			k4 = k3 + hStep;
	}
	else
	{
		k1 = step * (int)(Roundd(lowKey / step));
		if(lowKey > 0)
			k2 = k1 + step;
		else
		{
			k2 = k1;
			k1 = k2 - step;
		}
		k3 = step * (int)(Roundd(highKey / step));
		if(highKey > 0)
			k4 = k3 + step;
		else
		{
			k4 = k3;
			k3 = k4 - step;
		}
	}

	/* key values */
	if(out)
	{
		if(k2 - lowKey < 1e-8)
			lowKey = k2;
		else
			lowKey = k1;
		if(highKey - k3 < 1e-8)
			highKey = k3;
		else
			highKey = k4;
	}
	else
	{
		if(lowKey - k1 < 1e-8)
			lowKey = k1;
		else
			lowKey = k2;
		if(k4 - highKey < 1e-8)
			highKey = k4;
		else
			highKey = k3;
	}

	/* put values back in high and low */
	*low = Roundd(lowKey);
	*high = Roundd(highKey);
}

/*********************************************************************/
/* Roundd - round a double variable to a reasonable number of digits */
/*********************************************************************/

# define MULT 1e6
/* returns rounded value */
double Roundd(v)
	double v;		/* variable to round */
{
	double r;		/* rounded value */
	double nonInt;		/* non integral part of v */

	if(fabs(v) > 1e8) {
		register float f = v;
		return( (double)f );
	} else {
		nonInt = v - (int)(v);
		if(nonInt < 0)
		    r = (int)(v) + (double)((int)(nonInt * MULT - .5) / MULT);
		else
		    r = (int)(v) + (double)((int)(nonInt * MULT + .5) / MULT);
		return(r);
	}
}
