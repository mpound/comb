/***************************************/
/* Routines for handling interpolation */
/***************************************/

# include <math.h>
# include <stdio.h>

# include "C.h"
# include "../coordsys/C.h"
#include "../misc/C.h"

# define iDisSqr(a,b) (double)((a.x - b.x) * (a.x - b.x) +\
			       (a.y - b.y) * (a.y - b.y) +\
			       (a.z - b.z) * (a.z - b.z))
# include "include.h"


#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* interp.c */
static void BumpInterp P_((XYZPOINT lc, STKLINK *list));
static void DumbInterp P_((XYZPOINT lc, STKLINK *list));
static void TriInterp P_((XYZPOINT lc, STKLINK *list));
static void SmartInterp P_((XYZPOINT lc, STKLINK *list));
static void GaussInterp P_((XYZPOINT lc, STKLINK *list));
#undef P_

char interpNames[][3] = { "ci", "ni", "bi", "gi"};
int interpType = DUMBINTERP; /* type of interpolation to be used */
static double gaussCoeff = -1;

/*******************************************************/
/* SetStkInterp - set type and radius of interpolation */
/*******************************************************/

void SetStkInterp(type,radius,effdia,oldgrid)
	int type;		/* type of interpolation */
	double radius;		/* interpolation radius (in arcminutes) */
	double effdia;		/* effective diameter of antenna */
	double oldgrid;		/* grid spacing in stack data */
{
	char errStr[100];		/* error string */
	double sin();

	if(radius <= 0.)
	{
		sprintf(errStr,"Illegal interpolation radius %g",radius);
		error_(errStr);
	}

	/* set interpolation type */
	interpType = type;

	/* set interpolation radius */
	iRad = 2 * sin(radius * M_PI / (180 * 60 * 2)) * SPHERERAD;
	iRadSqr = iRad * iRad;
	iIRad = (int)(iRad);
	if(iIRad < iRad)
		iIRad++;
	
	/* for bessel and gauss interpolation, set cut-off in u-v plane as
	   a function of frequency and effective antenna diameter */
	if (interpType == SMARTINTERP) {
		if (!coreFull[0]) error_("Stack 1 empty");
		qCut = effdia * stk_[0].freq * 0.97E-6;
		printf("effdia = %g\tqCut = %g\n",effdia,qCut);
		/* 0.97E-6 = 2.9e-4 rad/arcmin * e6 Hz/MHz / c  
		   this puts qCut in 1/arcmin.  Now convert. */
		qCut = 1 / (2 * sin( M_PI / (qCut * 180 * 60 * 2)) * SPHERERAD );
		gridL =  2 * sin( M_PI * oldgrid/ ( 180 * 60 * 2)) * SPHERERAD ; 
	} else if(interpType == GAUSSINTERP) {
		double r0;	/* half power radius of Gaussian in SPHERERADs*/

		/* effdia contains the fwhm of the Gaussian in this case */
		r0 = effdia * (M_PI * SPHERERAD / (180. * 60 * 2));
		gaussCoeff = -0.69314718 / (r0 * r0);
	}
}

/*******************************************************/
/* FillInWeights - fill in the weights in a stack list */
/*******************************************************/

void FillInWeights(lc,list)
	XYZPOINT lc;		/* location that weights refer to */
	STKLINK *list;		/* beginning of list */
{

	/* find weights */
	switch(interpType)
	{
		case BUMPINTERP:
			BumpInterp(lc,list);
			break;
		case DUMBINTERP:
			DumbInterp(lc,list);
			break;
/*		case TRIINTERP:
			TriInterp(lc,list);
			break;
mwp 5/92 */
		case SMARTINTERP:
			SmartInterp(lc,list);
			break;
		case GAUSSINTERP:
			GaussInterp(lc,list);
			break;
	}
}

/************************************************************/
/* BumpInterp - interpolate by averaging all closest stacks */
/************************************************************/

static void BumpInterp(lc,list)
	XYZPOINT lc;		/* location to interpolate */
	STKLINK *list;		/* list to be interpolated */
{
	static double best;	/* current best distance squared */
	static double disSqr;	/* new distance squared */
	static STKLINK *(soFar[11]); /* closest stacks so far */
	static STKLINK **lastSoFar; /* last in soFar */
	static int numSoFar;	/* number of stacks in soFar */

	lastSoFar = soFar - 1;
	soFar[10] = 0;
	numSoFar = 0;
	best = iRadSqr + 1.;
	while(list)
	{
		list->weight = 0.;
		disSqr = dissquare(&list->loc,&lc);
		if(disSqr < best)
		{
			numSoFar = 1;
			lastSoFar = soFar;
			*lastSoFar = list;
			best = disSqr;
		}
		else if(disSqr == best && numSoFar < 10)
		{
			numSoFar++;
			lastSoFar++;
			*lastSoFar = list;
		}
		list = list->next;
	}
	while(lastSoFar >= soFar)
	{
		(*lastSoFar)->weight = 1.0;
		lastSoFar--;
	}
}

/****************************************************************/
/* DumbInterp - set up weights for inverse square interpolation */
/****************************************************************/

static void DumbInterp(lc,list)
	XYZPOINT lc;		/* location to interpolate */
	STKLINK *list;		/* list to be interpolated */
{
	register STKLINK *curStk;/* stack whose weight is currently being */
				/* computed */

	curStk = list;
	while(curStk)
	{
		/* do cone interpolation */
		curStk->weight = (iRad - sqrt(dissquare(&curStk->loc,
				&lc))) / iRad;
		if(curStk->weight <= 0.)
			curStk->weight = 0.;
		curStk = curStk->next;
	}
}

/************************************************************/
/* TriInterp - compute weights for triangular interpolation */
/************************************************************/

static void TriInterp(lc,list)
	XYZPOINT lc;		/* location to be interpolated */
	STKLINK *list;		/* list of stacks to interpolate it with */
{
	STKLINK *(stk1[10]);	/* stacks at location 1 */
	STKLINK *(stk2[10]);	/* stacks at location 2 */
	STKLINK *(stk3[10]);	/* stacks at location 3 */
	STKLINK **stk1Last;	/* last stack in stk1 */
	STKLINK **stk2Last;	/* last stack in stk2 */
	STKLINK **stk3Last;	/* last stack in stk3 */
	double y2z3_y3z2,x2z3_x3z2,x2y3_x3y2; /* variables for computing */
				/* determinant of matrix */
	double y1z3_y3z1,x1z3_x3z1,x1y3_x3y1;
	double y1z2_y2z1,x1z2_x2z1,x1y2_x2y1;
	XYZPOINT lc1;		/* location 1 */
	XYZPOINT lc2;		/* location 2 */
	XYZPOINT lc3;		/* location 3 */
	double disSqr;		/* square of distance from a stack to lc */
	double disSqr2;
	double disSqr3;
	double tempDisSqr;
	STKLINK *curStk;	/* stack currently being checked */
	double weight;		/* weight of stacks */
	double dt;
	double tempDt;
	double fabs();

	/* find all stacks at location 2 */
	/* This value for disSqr assures that the first stack found will have
	 * tempDisSqr < disSqr and thus stk2Last will be properly initialized */
	disSqr = iRadSqr + 1.;
	for(curStk = list; curStk; curStk = curStk->next)
	{
		curStk->weight = 0.;
		tempDisSqr = iDisSqr(curStk->loc,lc);
		if(tempDisSqr < 1000.)
		{
			BumpInterp(lc,list);
			return;
		}
		if(tempDisSqr == disSqr)
		{
			if(stk2Last - stk2 < 9)
				*++stk2Last = curStk;
		}
		else if(tempDisSqr < disSqr)
		{
			stk2Last = stk2;
			*stk2Last = curStk;
			disSqr = tempDisSqr;
		}
	}
	lc2 = (*stk2)->loc;
	disSqr2 = disSqr;

	/* find all stacks at location 3 */
	stk3Last = 0;
	disSqr = iRadSqr + 1.;
	for(curStk = list; curStk; curStk = curStk->next)
	{
		if((tempDisSqr = iDisSqr(curStk->loc,lc)) > disSqr2)
		{
			if(tempDisSqr == disSqr)
			{
				if(stk3Last - stk3 < 9)
					*++stk3Last = curStk;
			}
			else if(tempDisSqr < disSqr)
			{
				stk3Last = stk3;
				*stk3Last = curStk;
				disSqr = tempDisSqr;
			}
		}
	}
	if(!stk3Last)
		return;
	lc3 = (*stk3)->loc;
	disSqr3 = disSqr;

	/* set up to compute determinant of [lc1 lc2 lc3] */
	y2z3_y3z2 = (double)(lc2.y * lc3.z) - (double)(lc3.y * lc2.z);
	x2z3_x3z2 = (double)(lc2.x * lc3.z) - (double)(lc3.x * lc2.z);
	x2y3_x3y2 = (double)(lc2.x * lc3.y) - (double)(lc3.x * lc2.y);

	/* find all stacks at location 1 */
	stk1Last = 0;
	disSqr = iRadSqr + 1.;
	for(curStk = list; curStk; curStk = curStk->next)
	{
		if((tempDisSqr = iDisSqr(curStk->loc,lc)) > disSqr3 &&
		   fabs(tempDt = (curStk->loc.x * y2z3_y3z2) -
		                 (curStk->loc.y * x2z3_x3z2) +
		                 (curStk->loc.z * x2y3_x3y2)) > 0.)
		{
			if(tempDisSqr == disSqr)
			{
				if(stk1Last - stk1 < 9)
					*++stk1Last = curStk;
			}
			else if(tempDisSqr < disSqr)
			{
				stk1Last = stk1;
				*stk1Last = curStk;
				disSqr = tempDisSqr;
				dt = tempDt;
			}
		}
	}
	if(!stk1Last)
		return;
	lc1 = (*stk1)->loc;

	/* set up to compute other determinants */
	y1z3_y3z1 = (double)(lc1.y * lc3.z) - (double)(lc3.y * lc1.z);
	x1z3_x3z1 = (double)(lc1.x * lc3.z) - (double)(lc3.x * lc1.z);
	x1y3_x3y1 = (double)(lc1.x * lc3.y) - (double)(lc3.x * lc1.y);
	y1z2_y2z1 = (double)(lc1.y * lc2.z) - (double)(lc2.y * lc1.z);
	x1z2_x2z1 = (double)(lc1.x * lc2.z) - (double)(lc2.x * lc1.z);
	x1y2_x2y1 = (double)(lc1.x * lc2.y) - (double)(lc2.x * lc1.y);

	/* compute weights */
	weight = (lc.x * y2z3_y3z2 - lc.y * x2z3_x3z2 + lc.z * x2y3_x3y2) /
		 (dt * (stk1Last - stk1 + 1));
	while(stk1Last >= stk1)
		(*stk1Last--)->weight = weight;
	weight = -(lc.x * y1z3_y3z1 - lc.y * x1z3_x3z1 + lc.z * x1y3_x3y1) /
		 (dt * (stk2Last - stk2 + 1));
	while(stk2Last >= stk2)
		(*stk2Last--)->weight = weight;
	weight = (lc.x * y1z2_y2z1 - lc.y * x1z2_x2z1 + lc.z * x1y2_x2y1) /
		 (dt * (stk3Last - stk3 + 1));
	while(stk3Last >= stk3)
		(*stk3Last--)->weight = weight;
}

/**************************************/
/* SmartInterp - Bessel interpolation */
/**************************************/

static void SmartInterp(lc,list)
	XYZPOINT lc;		/* location to interpolate */
	STKLINK *list;		/* list to be interpolated */

{
	register STKLINK *curStk;/* stack whose weight is currently being */
				/* computed */
	curStk = list;
	while(curStk)
	{
		double r, r2;

		r = sqrt( r2=dissquare(&curStk->loc,&lc) );
		curStk->weight = gridL*gridL*qCut*qCut*j1ox(qCut*r)
			*exp(- 2. * M_PI *r2/(iRad*iRad) );
		curStk = curStk->next;
	}
}

/****************************************/
/* GaussInterp - Gaussian interpolation */
/****************************************/

static void GaussInterp(lc,list)
	XYZPOINT lc;		/* location to interpolate */
	STKLINK *list;		/* list to be interpolated */

{
	register STKLINK *curStk;/* stack whose weight is currently being */
				/* computed */

	curStk = list;
	while(curStk)
	{
		curStk->weight = exp(gaussCoeff * dissquare(&curStk->loc,&lc)) ;
		curStk = curStk->next;
	}
}
