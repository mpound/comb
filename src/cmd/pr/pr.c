#include <stdarg.h>
#include "../../main/C.h"
#include "../../main/machinedep.h"
#include "../../parse/P.tree.h"
#include "../../parse/P.struct.h"
#include "../../parse/C.h"
#include <stdio.h>
#include "../../error/C.h"

HdrVar *findhdr(char *vname);
void asgnstrhdr(HdrVar *var, char *val);


#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* pr.c */
static void DoEscSeq P_((char *str));
#undef P_

	/**********************************/
	/* pr - printf like print command */
	/**********************************/

void pr()
{
	/* array to hold values for passing to printf */
	char args[80];
	char *ap;		/* pointer into args */

	int *vp;		/* Pointer into values from parse */
	int d;			/* size of a value entry */
	short *tp;		/* Pointer to types of values */

# include "pr.dc.h"
# include "pr.ec.h"
 
	/* Move values into args */
	vp = (int *)var0;
	d = (int *)var1 - (int *)var0;
#if HP_700
	ap = &args[80];
#else /*HP_700*/
	ap = args;
#endif /*HP_700*/
	for(tp = var0t; tp <= var9t; ) {
		switch(*tp++) {
		case REAL:
/*
printf("REAL = %g\n",*(double *)vp);
*/
#if HP_700
/*
 * The Hp 700 series stacks up arguments on the varags list in the opposite
 * order from everyone else.  They are also all on 8 byte boundaries.
 */
			if((int)ap & 4)
				ap -= 4;
			ap -= sizeof(double);
			*(int *)ap = *vp;
			((int *)ap)[1] = vp[1];
#else /*HP_700*/
			*(int *)ap = *vp;
			ap += sizeof(int);
			*(int *)ap = vp[1];
			ap += sizeof(int);
#endif /*HP_700*/
			break;
		case STRING :
/*
printf("STRING = %s\n",vp);
*/

#if HP_700
			ap -= sizeof(char *);
			*(char **)ap = (char *)vp;
#else /*HP_700*/
			*(char **)ap = (char *)vp;
			ap += sizeof(char *);
#endif /*HP_700*/

			break;
		}
		vp += d;
	}
	/* Do the output */
	DoEscSeq(fmt);
	if(*act == 0) {
#if HP_700
		vprintf(fmt, (va_list)&args[80]);
#else /*HP_700*/
		vprintf(fmt, args);
#endif /*HP_700*/
		fflush(stdout);
	} else {
	    char ts[NTTYLEN];

#if HP_700
	    vsprintf(ts, fmt, (va_list)&args[80]);
#else /*HP_700*/
	    vsprintf(ts, fmt, args);
#endif /*HP_700*/

	    if(*act == 1) {
		if(sg_[*gs])
			ChkFree(sg_[*gs]);
		sg_[*gs] = ChkAlloc(strlen(ts) + 1, "Global string");
		strcpy(sg_[*gs], ts);
	    } else {
		HdrVar  *writevar;
		if((writevar = findhdr((*var == '.')? var + 1: var)) !=NULL)
			asgnstrhdr(writevar, ts);
		else
			error("Variable %s not found",(*var == '.')? var + 1: var);
	    }
	}
}

static void DoEscSeq(str)
char *str;
{
	register char *ip, *op;
	register int c;
	char *svip;
	for(ip = op = str; *ip; ) {
		if(*ip == '\\') {
			switch(ip[1]) {
			case 'n' :
				*++ip = '\n';
				break;
			case 't' :
				*++ip = '\t';
				break;
			case 'v' :
				*++ip = '\v';
				break;
			case 'b' :
				*++ip = '\b';
				break;
			case 'r' :
				*++ip = '\r';
				break;
			case 'f' :
				*++ip = '\f';
				break;
			case 'a' :
				*++ip = '\a';
				break;
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
				svip = ++ip;
				for(c = *ip++ - '0'; *ip >= '0' && *ip < '8' &&
						ip - svip < 3; ){
					c <<= 3;
					c += *ip++ - '0';
				}
				*--ip = c;
				break;
			case 'x' :
				for(c = 0, svip = ip += 2;
						((*ip >= '0' && *ip <= '9') ||
						(*ip >= 'a' && *ip <= 'f')) &&
						ip - svip < 2; ){
					c <<= 4;
					if(*ip <= '9')
						c += *ip++ - '0';
					else
						c += *ip++ - 'a' + 10;
				}
				*--ip = c;
				break;
			case '\\':
				++ip;
				break;
			}
		}
		*op++ = *ip++;
	}
	*op++ = '\0';
}

