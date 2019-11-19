#include "ctype.h"
#include "../../parse/P.tree.h"
#include "../../parse/P.constant.h"
#include "../../parse/P.struct.h"
#include "../../parse/P.uservars.h"
#include "../../main/C.h"

#define VARNAMELEN 63

static char *getnext(char *ip, char *name, int *lenp);

void dv()
{
	char *cp;
	char name[VARNAMELEN + 1];
	int length;
#include "dv.dc.h"
#include "dv.ec.h"

	if(!*nfl) {
	    for(cp = nnam; *cp;) {
		cp = getnext(cp, name, &length);
		NewUserVar((*lclf)? LOCAL: GLOBAL, name, UVNUM, length);
	    }
	}
	if(!*sfl) {
	    for(cp = snam; *cp;) {
		cp = getnext(cp, name, &length);
		NewUserVar((*lclf)? LOCAL: GLOBAL, name, UVSTR, length);
	    }
	}
	if(*pgf)
	    PrintUserVars(GLOBAL);
	if(*plf)
	    PrintUserVars(LOCAL);
}

static char *getnext(char *ip, char *name, int *lenp) {
	char *op = name;

	while(isspace(*ip) || *ip == ',')
	    ip++;
	if(*ip == '.')
	    ip++;
	if(!isalpha(*op++ = *ip++))
	    error("First char of variable name must be alphabetical");
	while(!isspace(*op = *ip) && *op != '.' && *op != ','
		 && *op !='(' && *op !=0) {

/* these confuse the parser, so don't let user have them */
            if(*ip=='#' || *ip=='$')
		error("You can't use # or $ in a variable name");
	    op++;
	    ip++;
	    if(op > &name[VARNAMELEN])
		error("Variable name too long");
	}
	*op = 0;
	while(isspace(*ip))
	    ip++;
	if(*ip == '(') {
	    ip++;
	    if(sscanf(ip, "%d", lenp) != 1 || *lenp <= 0)
		error("Bad length for %s", name);
	    while(*ip != ')') {
		if(!*ip++)
		    error("Missing ')' for %s", name);
	    }
	    ip++;
	} else {
	    *lenp = 1;
	}
	return(ip);
}
