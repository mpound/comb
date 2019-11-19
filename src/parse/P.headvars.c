# include "P.tree.h"
#include "../main/C.h"
#if COMB
#include "../scan/C.h"
#include "../stacks/C.h"
#endif /*COMB*/
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.extern.h"
# include "P.uservars.h"

extern HdrVar *GetScan2HdrVar(/*name*/);

	/************************************/
	/* findhdr - find a header variable */
	/************************************/
/* Changed to return null ptr if no header variable
 * found.  This makes filtering of reserve words in
 * "dv" easier.  -- mwp 6/26/96
 */

HdrVar *findhdr(char *vname)
/* vname is name of variable to be found */
{
	register HdrVar *look;		/* variable currently being checked */
	register int step;		/* amount to move look (for a */
					/* binary search) */
	register int cmp;		/* comparison */

	/* First check user defined variables */
	if(look = SearchUserVar(LOCAL, vname))
		return(look);
	if(look = SearchUserVar(GLOBAL, vname))
		return(look);

	/* find middle of list to start binary search */
	step = ((headend - headvars) >> 1) | 1;
	look = headvars + step - 1;

	/* search */
	while(1)
	{

		/* if search has taken us off top of list, */
		/* make sure we will go down */
		if(look < headvars)
			cmp = 1;

		/* if search has taken us off bottom of list, */
		/* make sure we will go up */
		else if(look >= headend)
			cmp = -1;

		/* otherwise, compare */
		else
		{
			cmp = strcmp(vname,look->nm);
			if(!cmp)
				return(look);
		}

		/* if search is done, give error */
		if(step == 1) {
		    if(look = GetScan2HdrVar(vname))
			return(look);
		    else
		 	return((HdrVar *) 0);
		    /*nferr("header variable not found");*/
		}

		/* continue search */
		step = (step + 1) >> 1;
		if(cmp > 0)
			look += step;
		else
			look -= step;
	}
}

	/**********************************/
	/* hdrval - return a header value */
	/**********************************/

double hdrval(var,ind)
	HdrVar *var;			/* pointer to variable whose */
					/* value is to be returned */
	int ind;			/* index into header array */
{
	/* subtract 1 from ind because astronomers are */
	/* used to fortran subscripts */
	ind = ind - 1;

	/* check that ind is in bounds */
	if(ind < 0 || (var->ln != 0 && ind >= var->ln))
		nferr("subscript out of bounds");

	/* return value */
	switch(var->tp)
	{
		case DOUBLE:
		case UVNUM:
			return(((double *)var->lc)[ind]);
		case FLOAT:
			return(((float *)var->lc)[ind]);
		case INTEGER:
			return(((int *)var->lc)[ind]);
		case SHORT:
			return(((short *)var->lc)[ind]);
		case CHAR:
			return(((char *)var->lc)[ind]);
		case BIT:
			ind = ind + 1;
			return(ibit_((char *)var->lc,&ind));
		case HVSTRING:
			nferr("header variable is type string");
		case SCAN2NUM:
			return(Scan2HdrNumVal(var->lc, ind));
	}
}

	/********************************************/
	/* hdrstrval - return a string header value */
	/********************************************/

char *hdrstrval(var)
	HdrVar *var;			/* pointer to variable whose */
					/* value is to be returned */
{

	/* return value */
	switch(var->tp) {
	case HVSTRING:
	case UVSTR:
		return((char *)var->lc);
		break;
	case SCAN2STR:
		return(Scan2HdrStrVal(var->lc));
		break;
	}
}

	/*************************************************/
	/* asgnhdr - assign a value to a header variable */
	/*************************************************/

asgnhdr(var,ind,val)
	HdrVar *var;			/* variable to which value is to */
					/* be assigned */
	int ind;			/* index into header array */
	double val;			/* value to be assigned */
{
	int temp;			/* temporary variable for calling */
					/* fortran routine */

	/* subtract 1 from ind because astronomers are used to fortran */
	ind = ind - 1;

	/* check that ind is in range */
	if(ind < 0 || ind >= var->ln)
		nferr("subscript out of bounds");

	/* assign value */
	switch(var->tp)
	{
		case DOUBLE:
		case UVNUM:
			((double *)var->lc)[ind] = val;
			break;
		case FLOAT:
			((float *)var->lc)[ind] = val;
			break;
		case INTEGER:
			((int *)var->lc)[ind] = val;
			break;
		case SHORT:
			((short *)var->lc)[ind] = val;
			break;
		case CHAR:
			((char *)var->lc)[ind] = val;
			break;
		case BIT:
			ind = ind + 1;
			temp = val;
			filbts_((char *)var->lc,&temp,&ind,&ind);
			break;
		case SCAN2NUM:
			Scan2HdrNumPut(val, var->lc, ind);
			break;
	}
}

	/**************************************************/
	/* asgnstrhdr - assign a value to a string header */
	/**************************************************/

void asgnstrhdr(var, val)
	HdrVar *var;			/* pointer to variable whose */
					/* value is to be returned */
	char *val;
{

	/* return value */
	switch(var->tp) {
	case HVSTRING:
		strncpy((char *)var->lc, val, var->ln);
		break;
	case SCAN2STR:
		Scan2HdrStrPut(val, var->lc);
		break;
	case UVSTR:
		StoreUserStringVar(var, val);
	}
}
