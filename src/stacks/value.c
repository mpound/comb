# include "C.h"
# include "../coordsys/C.h"
# include "include.h"
# include "../main/C.h"
# include "../main/dragon.h"
# include <stdio.h>
# include "../error/C.h"
# include <string.h>

/* static variables */
static float *valArray[2] = {0,0};	/* arrays of computed values */
static float valDragon = 1.7003214522644e38;	/* dragon in value file
					to indicate that value has not been
					computed yet */
static int valArrayLen[2] = {0,0};	/* length of those two arrays */
static int valArrayHow[2];		/* 1 = integrate, 2 = apply macro then
					average, 3 = average then apply macro */
static char macro[NTTYLEN][2];		/* expansion of macros used to find
					value */
static char badValueStr[] = "Internal error: value array not assigned ";

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* value.c */
static void ClearValues P_((int start));
static double StkVal(int stkNum);
#undef P_

int Already_warned;	/* Warned about differing filterwidths? (AvgStacks)*/
/*******************************************************/
/* InitValues - initialize value array for a directory */
/*******************************************************/

void InitValues()
{
	if(valArray[curDir]) {
		noint_();
		ChkFree((char *)valArray[curDir]);
		valArray[curDir] = 0;
		yesint_();
	}
	valArrayLen[curDir] = NumInds() + 100;
	valArray[curDir] = (float *)(ChkAlloc(valArrayLen[curDir] *
		sizeof(float),"ValArray"));
	ClearValues(0);
}

/******************************************************************/
/* StkValMacro - set up the macro to be used for computing values */
/******************************************************************/

char *StkValMacro(macAndArgs,order)
	char *macAndArgs;	/* macro name with its arguments */
	int order;		/* 0 = apply macro then average, 1 = average */
				/* then apply macro */
{
	char tempMacro[NTTYLEN];

	GetMacro(macAndArgs,tempMacro);
	if(valArrayHow[curDir] != order + 2 || strcmp(tempMacro,macro[curDir]))
	{
		noint_();
		ClearValues(0);
		strcpy(macro[curDir],tempMacro);
		valArrayHow[curDir] = order + 2;
		yesint_();
	}
	/* return pointer to expanded macro */
	return(macro[curDir]);
}

/************************************************************************/
/* LocVal - compute a value for a location from a linked list of stacks */
/************************************************************************/

double LocVal(list, minWt)
STKLINK *list;		/* linked list */
double minWt;		/* minimum weight to appear in map */
{
	double value;		/* computed value */
	double tempVal;
	double totalWeight;	/* total of all the weights */

	if(valArrayHow[curDir] == 3)
	{
		if(AvgStacks(list,0.0,0))
			value = DRAGON;
		else
		{
			stkValFlag = 1;
			combex_(macro[curDir]);
			stkValFlag = 0;
			value = stkValue;
		}
	}
	else
	{
		value = 0.;
		totalWeight = 0.;
		while(list)
		{
			if((tempVal = StkVal(list->stkNum)) != DRAGON) {
				value += list->weight * tempVal;
/*
printf("\nvalue= %g   list->weight= %g  tempVal= %g ",value,list->weight,tempVal);
*/
				totalWeight += list->weight;
			}
			list = list->next;
		}
		if(totalWeight <= minWt)
			value = DRAGON;
		else if (interpType != SMARTINTERP)
			value /= totalWeight;
	}
	return(value);
}

/******************************/
/* StkVal - get a stack value */
/******************************/

static double StkVal(stkNum)
	int stkNum;		/* number of stack to evaluate */
{
	if( ! valArray[curDir])
		error_(badValueStr);
	if(stkNum > valArrayLen[curDir] || stkNum < FRSTONDISK)
		error("Trying to get value for stk %d which is beyond valArray"
			,stkNum);
	if(valArray[curDir][stkNum] != valDragon)
		return((double)valArray[curDir][stkNum]);
	if(!gtArrValues) {
		fprintf(stderr,"Getting stack values\n");
		gtArrValues++;
	}

	switch(valArrayHow[curDir]) {
		case 0:
			error_("No way of getting single value for stack");
		case 1:
			error_("Not yet implemented");
		case 2:
		case 3:
			getStack(stkNum,1);
			if(!coreFull[0])
			    SearchFileError(stkNum);
			stkValFlag = 1;
			combex_(macro[curDir]);
			stkValFlag = 0;
			valArray[curDir][stkNum] = stkValue;
			break;
	}
	return(stkValue);
}

/******************************************************/
/* AvgStacks - construct a weighted average of stacks */
/******************************************************/

AvgStacks(list,filtWid,catScanLst)	/* return true if data was found */
	register STKLINK *list;	/* list of stacks to average */
	double filtWid;		/* if non zero, only use stacks with this
				filter width */
	int catScanLst;
{
	int weightNum;		/* number of weight being computed */
	int init;		/* init value for cmbn_ */
	static int savesig = -1;	/* save command line serial number to
				control error messages in cmbn */
	float tol;		/* variable for argument of fortran subroutine */

	/* add stacks together */
	coreFull[1] = 0;	/* mark stack 2 empty */
	/* in init 4 prevents position comparisons and 16 suppresses combining
	 * scan number lists */
	init = (catScanLst)? 4: 20;
	if(savesig != cmnd_.cmdsig)
		/* If first time through on this command line, clear cmbn's
		 * memory of discrepencies */
		init++;
	while(list)
	{
		if(list->weight) {
/*
printf("stk# = %d, list->wght = %g, nwts2 = %d\n", list->stkNum, list->weight, stk_[1].nwts);
*/
		    getStack(list->stkNum,1);
		    if(!coreFull[0])
			SearchFileError(list->stkNum);

		/* if a filter width was given, only use stacks of that width */
		    tol = filtWid/( stk_[0].numpt << 4);
		    if(filtWid == 0. || !cifdif((double)stk_[0].fwid,
			(double)filtWid,(double)tol)) {
			for(weightNum = 0;weightNum < stk_[0].nwts;weightNum++)
				stk_[0].wght[weightNum] *= list->weight;
			cmbn_(&init);
			init &= 0xfe;
			savesig = cmnd_.cmdsig;
		    }
		    else if(!Already_warned) {
			warn("Filter widths differ--ignoring some stacks");
			savesig = cmnd_.cmdsig;
			Already_warned = 1;
		    }
		}
		list = list->next;
	}
	getStack(2,1);
	return(coreFull[0]);
}

/********************************************************************
 * ClrStkValue() - clear the value of a given stack (set to valDragon) *
 ********************************************************************/

void ClrStkValue(stkNum)
int stkNum;
{
	if( ! valArray[curDir])
		error_(badValueStr);
	if(stkNum >= valArrayLen[curDir]) {
		int new = valArrayLen[curDir];

		noint_();
		valArrayLen[curDir] = stkNum + 100;
		valArray[curDir] = (float *)ChkRealloc((char *)valArray[curDir],
			(unsigned)(valArrayLen[curDir] * sizeof(float)),
			"valArray");
		ClearValues(new);
		yesint_();
	} else {
		valArray[curDir][stkNum] = valDragon;
	}
}

/************************************************************************/
/* ClearValues - clear the values in the current directory beyond start */
/************************************************************************/

static void ClearValues(start)
int start;
{
	register float *vp,*end,ldragon = valDragon;

	vp = valArray[curDir];
	end = vp + valArrayLen[curDir];
	vp += start;

	while(vp < end)
		*vp++ = ldragon;
}
