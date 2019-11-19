# include "P.tree.h"
#include "../main/C.h"
#if COMB
#include "../scan/C.h"
#endif /*COMB*/
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.extern.h"
#include <stdio.h>

	/*****************************************************/
	/* ferr - fatal error prints message and causes dump */
	/*****************************************************/

#define DEBUG 0
Any *ferr(message)
	char *message;			/* message to be printed */
{
#if DEBUG
	int abort = 0;			/* variable for causing dump */
#endif
	if(message)
		fprintf(stderr,"Fatal error in Parse: %s\n",message);
	else
		fprintf(stderr, "Fatal Error in Parse\n");
#if DEBUG
	abort = 1 / abort;
#else
	exit(7);
#endif
	return (Any *)0;		/* keep lint happy */
}

	/***************************/
	/* nferr - non-fatal error */
	/***************************/

nferr(message)
	char *message;			/* message to be printed */
{
	char string[NTTYLEN];		/* string which caused error */

	/* construct error string */
	strncpy(string,sterr,str - sterr - (unspt - unstk));
	string[str - sterr - (unspt - unstk)] = 0;

	/* print it */
	fprintf(stderr,"Error on \"%s\": %s\n",string,message);

	/* if in handhold mode, return, else, print more information */
	if(hhmode)
		longjmp(hherr,0);
	fprintf(stderr,"	Expecting: ");
	prnode(0,cnode);

	/* call error */
	error_("\n	Parsing parameters");
}

	/*******************************/
	/* yerror - yacc error routine */
	/*******************************/

yyerror(errstr)
char *errstr;
{
	nferr(errstr);
}
