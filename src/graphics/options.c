# include <stdarg.h>

# include "C.h"
# include "include.h"
#include "../main/dragon.h"

/* option list */
static char options[MAXOPTS];
static char *lastOpt = options;
static char *opt = options;
static ANYTYPE optArgs[MAXOPTS * AVGARGS];
static ANYTYPE *lastArg = optArgs;
ANYTYPE *optArg = optArgs;
static char numArgs[MAXOPTS];
static char *lastNum = numArgs;
static char *narg = numArgs;

/*************************************/
/* MkOpt - set some plotting options */
/*************************************/

/*VARARGS*/
MkOpt(int firstOpt, ...)
{
	va_list opList;		/* list of options */

	va_start(opList, firstOpt);
	AddOpt(firstOpt, opList);
	va_end(opList);
}

/*******************************************************/
/* AddOpt - add a bunch of options to the current list */
/*******************************************************/

/*VARARGS*/
void AddOpt(firstOpt, opList)
	int firstOpt;
	va_list opList;
{
	char newOpt;		/* option being added to list */

	/* add options */
	for(newOpt = firstOpt; newOpt; newOpt = va_arg(opList,int))
	{
		*lastOpt++ = newOpt;
		switch((newOpt < 0) ? -newOpt:newOpt)
		{
			/* options with 1 list and 1 integer arguments */
			case V_XLIST:
			case V_YLIST:
			case V_FWLIST:
			case V_FHLIST:
			case V_CREM:
				*lastNum++ = 2;
				lastArg->l.type = T_DOUBLE;
				lastArg++->l.point.d = va_arg(opList,double *);
				lastArg++->i = va_arg(opList,int);
				break;
			case V_fXLIST:
			case V_fYLIST:
			case V_fFWLIST:
			case V_fFHLIST:
				*lastNum++ = 2;
				lastArg->l.type = T_FLOAT;
				lastArg++->l.point.f = va_arg(opList,float *);
				lastArg++->i = va_arg(opList,int);
				break;
			case V_iXLIST:
			case V_iYLIST:
			case V_iFWLIST:
			case V_iFHLIST:
				*lastNum++ = 2;
				lastArg->l.type = T_INT;
				lastArg++->l.point.i = va_arg(opList,int *);
				lastArg++->i = va_arg(opList,int);
				break;

			/* the CSTORE option is a bit of a kluge */
			case V_CSTORE:
				*lastNum++ = 3;
				lastArg++->l.point.i = va_arg(opList,int *);
				lastArg++->l.point.i = va_arg(opList,int *);
				lastArg++->l.point.i = va_arg(opList,int *);
				break;

			/* options with 1 list and 2 integer arguments */
			case V_ARRAY:
				*lastNum++ = 3;
				lastArg->l.type = T_DOUBLE;
				lastArg++->l.point.d = va_arg(opList,double *);
				lastArg++->i = va_arg(opList,int);
				lastArg++->i = va_arg(opList,int);
				break;

			/* options with 2 double arguments */
			case V_XRANGE:
			case V_YRANGE:
			case V_HATCH:
			case V_CLEVRANGE:
				*lastNum++ = 2;
				lastArg++->d = va_arg(opList,double);
				lastArg++->d = va_arg(opList,double);
				break;

			/* options with 1 double argument */
			case V_XNOISE:
			case V_YNOISE:
			case V_XSTEP:
			case V_YSTEP:
			case V_HTICLEN:
			case V_VTICLEN:
			case V_FWIDTH:
			case V_FHEIGHT:
			case V_CLEVSTEP:
				*lastNum++ = 1;
				lastArg++->d = va_arg(opList,double);
				break;

			/* options with no arguments */
			case V_XLOG:
			case V_YLOG:
			case V_KEYX:
			case V_KEYY:
			case V_LABELX:
			case V_LABELY:
			case V_GRID:
			case V_LINES:
			case V_HIST:
			case V_XREVERSE:
			case V_YREVERSE:
			case V_FLAGX0:
			case V_FLAGY0:
			case V_BORDER:
			case V_CIRCBORDER:
			case V_CLEVLOG:
			case V_CLINSCALE:
			case V_CLINCOLOR:
			case V_XLABHMS:
			case V_YLABHMS:
			case V_FILLED:
				*lastNum++ = 0;
				break;

			/* options with 1 integer argument */
			case V_LTYPE:
			case V_XDIVS:
			case V_YDIVS:
			case V_FIGURE:
			case V_CNUM:
			case V_CLINTYPE:
				*lastNum++ = 1;
				lastArg++->i = va_arg(opList,int);
				break;

			/* options with 1 list argument */
			case V_CLEVS:
				*lastNum++ = 1;
				lastArg->l.type = T_DOUBLE;
				lastArg++->l.point.d = va_arg(opList,double *);
				break;
			case V_CLINS:
				*lastNum++ = 1;
				lastArg->l.type = T_INT;
				lastArg++->l.point.i = va_arg(opList,int *);
				break;

			/* options with 2 list arguments */
			case V_STEPSTOR:
				*lastNum++ = 2;
				lastArg->l.type = T_DOUBLE;
				lastArg++->l.point.d = va_arg(opList,double *);
				lastArg->l.type = T_DOUBLE;
				lastArg++->l.point.d = va_arg(opList,double *);
				break;

			/* options with 2 integer arguments */
			case V_CLINALT:
				*lastNum++ = 2;
				lastArg++->i = va_arg(opList,int);
				lastArg++->i = va_arg(opList,int);
				break;

			/* options with 2 list and 1 integer arguments */
			case V_CADD:
				*lastNum++ = 3;
				lastArg->l.type = T_DOUBLE;
				lastArg++->l.point.d = va_arg(opList,double *);
				lastArg->l.type = T_INT;
				lastArg++->l.point.i = va_arg(opList,int *);
				lastArg++->i = va_arg(opList,int);
				break;
			case V_CLEAROPT:
				opt = lastOpt = options;
				optArg = lastArg = optArgs;
				narg = lastNum = numArgs;
				break;

			default:
				error("Illegal high level graphics option %d",
				newOpt);
		}
	}
}

/*********************************************/
/* GtOpt - get an option off the option list */
/*********************************************/

/* returns the option number (0 at end) */
int GtOpt()
{
	/* check for end of list */
	if(opt == lastOpt)
	{
		opt = lastOpt = options;
		optArg = lastArg = optArgs;
		narg = lastNum = numArgs;
		return(0);
	}

	/* return next option */
	if(opt != options)
		optArg += *narg++;
	return(*opt++);
}

/******************************************/
/* NextInList - get next number in a list */
/******************************************/

double NextInList(list)
	LISTPOINTER *list;	/* list from which number is to be gotten */
{
	switch(list->type)
	{
		case T_DOUBLE:
			return(*list->point.d++);
		case T_FLOAT:
			return((double)(*list->point.f++));
		case T_INT:
			if(*list->point.i++ == iDRAGON)
				return(DRAGON);
			else
				return((double)(list->point.i[-1]));
		default:
			error("NextInList: internal error - bad list type");
	}
	/*NOTREACHED*/
}

#if 0
/* this seems to be incorrect and unused */

/*********************************************/
/* InList - return a given element of a list */
/*********************************************/

double InList(list,index)
	LISTPOINTER list;	/* list to get number from */
	int index;		/* element to look at */
{
	switch(list.type)
	{
		case T_DOUBLE:
			return(*list.point.d);
		case T_FLOAT:
			return((double)(*list.point.f));
		case T_INT:
			if(*list.point.i == iDRAGON)
				return(DRAGON);
			else
				return((double)(*list.point.i));
	}
}
# endif /*0*/
