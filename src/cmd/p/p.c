#include "../../main/C.h"
#include "../../parse/P.tree.h"
#include "../../parse/P.struct.h"
#include "../../parse/C.h"
#include <stdio.h>
#include "../../error/C.h"

HdrVar *findhdr(char *vname);
void asgnstrhdr(HdrVar *nvar, char *val);

	/*********************/
	/* p - print a value */
	/*********************/

void p()
{
# include "prnn.dc.h"
# include "prnn.ec.h"
	if(!*dp) switch(*vart)
	{
		case REAL:
			printf("%g",*varr);
			break;
		case STRING:
			printf("%s",var);
			break;
	}
	if(!*dp)
	{
		if(!*nl)
			printf("\n");
		else
			printf("  ");
	}
#include "prnn.uc.h"
}

/*
 * c - calculate just like 'p', but don't ever print.
 */

void c()
{
# include "crn.dc.h"
# include "crn.ec.h"
#include "crn.uc.h"
}

void ph()
{
	char ts[128];
# include "ph.dc.h"
# include "ph.ec.h"
	ts[0] = 0;
	HmsPrint(ts, 1, *var);
	if(*act==0) {
		printf((*nl)? "%s ": "%s\n", ts);
	} else {
            if(*act == 1) {
                if(sg_[*gs])
                        ChkFree(sg_[*gs]);
                sg_[*gs] = ChkAlloc(strlen(ts) + 1, "Global string");
                strcpy(sg_[*gs], ts);
            } else {
                HdrVar  *writevar;
                if((writevar = findhdr((*nvar == '.')? nvar + 1: nvar)) !=NULL)
                        asgnstrhdr(writevar, ts);
                else
                        error("Variable %s not found",(*nvar == '.')? nvar + 1: nvar);
            }

       }
}
