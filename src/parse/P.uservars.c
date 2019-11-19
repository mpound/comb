/* Package for maintaining and accessing user declared variables.  Each
 * variable is represented here by a HdrVar structure as with header variables.
 * There are two separate lists containing these structures for Global and
 * Local variables.  Both lists are  kept in the order in which the variables
 * are declared.  Global variables are visible at the base level and inside
 * any macro.  They live for the remainder of the current comb session.
 * Local variables are only usable within the macro in which they are defined.
 * They are removed after the macro in which they are defined returns.
 * The variable main_.macroDepth which is maintained in main.c and macro.c
 * is used here.  Exmac will call CleanUserVars after the completion of each
 * macro and decrementing macroDepth.
 */
#include <stdio.h>
#include <string.h>
#include "P.tree.h"
#include "P.constant.h"
#include "P.struct.h"
#include "P.uservars.h"
#include "P.cleanuservars.h"
#include "../error/C.h"
#include "../main/C.h"

#define UVLISTSIZE 30
#define MINSTRLEN 15

typedef struct {
	HdrVar *hv;	/* Points to array of HdrVar structurs for user vars */
	int hvSize;	/* Size of allocated hv space (in HdrVars) */
	int hvNum;	/* Number of HdrVars in hv */
} UVLIST;
static UVLIST uv[2];	/* GLOBAL and LOCAL lists. */
static char typename[][8] = {"global", "local"};

HdrVar *NewUserVar(int where, char *name, int type, int length) {
	UVLIST *uvp;
	HdrVar *hv, *findhdr(char *vname);

	if(where == LOCAL && main_.macroDepth == 0)
	    error("Attempting to create local variable %s outside of a macro",
		name);

	/* Is variable name a reserve (scan, stack) word? */
	if( (hv = findhdr(name)) != NULL) {
		switch((int)hv->tp) {
		case DOUBLE:
		case FLOAT:
		case INTEGER: 
		case SHORT:
		case CHAR:
		case BIT:
		case HVSTRING:
			error("Sorry, %s is a stack or 'old scan' reserve word",name);
			break;
		case SCAN2NUM:
		case SCAN2STR:
			error("Sorry, %s is a Scan2 reserve word",name);
			break;
		case UVNUM:
		case UVSTR:
	/* SearchUserVar was already called by findhdr, 
 	 * but we don't know if it returned a local or global UV.
	 * Least complicated way is to call it again and waste a little
         * overhead.
	 *
	 * An alternative would be to define UVGNUM, UVGSTR, UVLNUM, UVLSTR
	 * as new hv types and modify this case statement according to the
	 * value of "where". This also would affect other parts of comb where
	 * a comparison is currently made against UVSTR or UVNUM.
	 */
		/* Does this user variable already exist? */
			if(hv = SearchUserVar(where, name)) {
	    		  if(hv->tp != type) {
				warn("Redeclaring %s from %s", name, (type == UVNUM)?
				"String to Numeric": "Numeric to String");
	    		  }
	    /* Deallocate present contents and then set up fresh */
	    		  if(hv->lc)
				ChkFree((void *)hv->lc);
			  goto finish;
			}
			break;
		} /* end switch */
	}/* endif */

	/* If we've  made it to here then the requested variable 
	 * is a new user var.
	 */
	uvp = (where == GLOBAL)? &uv[GLOBAL]: &uv[LOCAL];
	/* Has this variable list been set up yet? */
	if(!uvp->hv) {
	    uvp->hv = (HdrVar *)ChkAlloc(UVLISTSIZE * sizeof(HdrVar),
	   		 "User Variable List");
	    uvp->hvSize = UVLISTSIZE;
	    uvp->hvNum = 0;
	}
	/* Is there space for another HdrVar in this list? */
	if(uvp->hvNum >= uvp->hvSize) {
 	    uvp->hv = (HdrVar *)ChkRealloc((void *)uvp->hv,
	    		(uvp->hvSize += UVLISTSIZE) * sizeof(HdrVar),
			"User Variable List");
	}
	/* set up rest of HdrVar */
	hv = &uvp->hv[uvp->hvNum++];
	hv->nm = ChkAlloc(strlen(name) + 1, name);
	strcpy(hv->nm, name);

finish:
	if((hv->tp = type) == UVNUM) {
	    hv->lc = (int)ChkAlloc((hv->ln = length) * sizeof(double),
		name);
	    memset((void *)hv->lc, 0, length * sizeof(double));
	} else {
	    hv->lc = (int)ChkAlloc(hv->ln = MINSTRLEN + 1, hv->nm);
	    *(char *)hv->lc = 0;	/* initialize string empty */
	}
	hv->mt = (where == LOCAL)? main_.macroDepth: 0;

	return(hv);
}

HdrVar *SearchUserVar(int where, char *name) {
	UVLIST *uvp;
	HdrVar *hv;

	uvp = (where == GLOBAL)? &uv[GLOBAL]: &uv[LOCAL];
	/* Has this variable list been set up yet? */
	if(!uvp->hv) {
	    return((HdrVar *)NULL);
	}
	for(hv = &uvp->hv[uvp->hvNum - 1]; hv >= uvp->hv; hv--) {
	    if(where == LOCAL && hv->mt < main_.macroDepth)
		return((HdrVar *)NULL);
	    if(!strcmp(name, hv->nm))
		return(hv);
	}
	return((HdrVar *)NULL);
}

void StoreUserStringVar(HdrVar *hv, char *string) {
	int newlen;

	if((newlen = strlen(string)) >= hv->ln) {
	    ChkFree((void *)hv->lc);
	    hv->lc = (int)ChkAlloc(hv->ln = newlen + 1, hv->nm);
	}
	strcpy((char *)hv->lc, string);
}

void CleanUserVars(void) {
	UVLIST *uvp;
	HdrVar *hv;

	/* Has this variable list been set up yet? */
	if(!uv[LOCAL].hv) {
	    return;
	}

	/* Step through the LOCAL list from the end down and deallocate the
	 * memory associated with each HdrVar for all macro depths greater
	 * than the current depth. */
	uvp = &uv[LOCAL];
	for(hv = &uvp->hv[uvp->hvNum - 1]; uvp->hvNum && hv->mt >
		main_.macroDepth; hv--, uvp->hvNum--) {
	    ChkFree(hv->nm);
	    ChkFree((void *)hv->lc);
	}
}

void PrintUserVars(int where) {
	UVLIST *uvp;
	HdrVar *hv;

	if(where == LOCAL) {
	    uvp = &uv[LOCAL];
	    printf("Local user variables (current macroDepth = %d):\n",
		main_.macroDepth);
	    if(!uvp->hv) return;
	    for(hv = &uvp->hv[uvp->hvNum - 1]; hv >= uvp->hv; hv--) {
		if(hv->tp == UVNUM) {
		    printf("N %s(%d) depth %d   %g\n", hv->nm, hv->ln, hv->mt,
			*(double *)hv->lc);
		} else {
		    printf("S %s(%d) depth %d   %s\n", hv->nm, hv->ln, hv->mt,
		        (hv->lc)? (char *)hv->lc:"");
		}
	    }
	} else {
	    uvp = &uv[GLOBAL];
	    printf("Global user variables:\n");
	    if(!uvp->hv) return;

	    for(hv = &uvp->hv[uvp->hvNum - 1]; hv >= uvp->hv; hv--) {
		if(hv->tp == UVNUM) {
		    printf("N %s(%d)   %g\n", hv->nm, hv->ln,
			*(double *)hv->lc);
		} else {
		    printf("S %s(%d)   %s\n", hv->nm, hv->ln,
		        (hv->lc)? (char *)hv->lc:"");
		}
	    }
	}
}
