# include <setjmp.h>
/* must have included ../main/C.h before here */
#if COMB
# include "../stacks/C.h"
#endif /*COMB*/
/* must have included P.struct.h before this */

	/****************************************************/
	/* variables for keeping track of current situation */
	/****************************************************/

Node *root;			/* current root */
Node *cnode;			/* current node pointer */
Node *nnode;			/* next value of current node pointer - */
				/*    used to combat YACC lookahead     */
				/*    (before YACC reduces a parameter  */
				/*    and stores it, LEX must read a    */
				/*    special character to indicate its */
				/*    end.  If this character set       */
				/*    cnode directly, then the para-	*/
				/*    meter would get stored in the	*/
				/*    wrong place)			*/
Node *rootstack[30];		/* stack of previouse values of root */
Node **rstkpnt;			/* root stack pointer */
int cda_type;			/* current data type */
int spor;			/* second part of range flag - used to */
				/* make '&' access the second variable */
				/* in a range if after '_' */
int hhmode;			/* set when in handhold mode, used to */
				/* recover from errors */
jmp_buf hherr;			/* location to recover to */

	/******************/
	/* user variables */
	/******************/

double gg_[10];			/* global numerics */
char *(sg_[10]);		/* global strings */

	/*****************/
	/* string buffer */
	/*****************/
char *strbuf;			/* string buffer */
char *strpos;			/* pointer to current location in buffer */

	/********************************/
	/* global pointers to arguments */
	/********************************/

Node *tree;				/* tree structure */
char *pbuf;				/* parameter buffer */
char *str;				/* string being parsed */

	/******************************/
	/* key words for descriptions */
	/******************************/

/* data types */
char *(da_type[]) =
{
	"",
	"COMMAND",
	"EXPRESSION",
	"STRING",
	"SCAN NUMBER",
	"SCAN NUMBER / COMMAND",
	"SCAN NUMBER / EXPRESSION",
	"SCAN NUMBER / STRING",
	"INTEGER",
	"INTEGER / COMMAND",
	"INTEGER / EXPRESSION",
	"INTEGER / STRING",
	"INTEGER / SCAN NUMBER",
	"INTEGER / SCAN NUMBER / COMMAND",
	"INTEGER / SCAN NUMBER / EXPRESSION",
	"INTEGER / SCAN NUMBER / STRING",
	"REAL",
	"REAL / COMMAND",
	"REAL / EXPRESSION",
	"REAL / STRING",
	"REAL / SCAN NUMBER",
	"REAL / SCAN NUMBER / COMMAND",
	"REAL / SCAN NUMBER / EXPRESSION",
	"REAL / SCAN NUMBER / STRING"
};

/* sequences */
char *(seq[]) =
{
	"",
	"SINGLE",
	"RANGE",
	"LIST"
};

/* default types */
char *(df_type[]) =
{
	"",
	"OLD",
	"TOGGLE",
	"SET",
	"AUTOMATIC",
	"AUTOMATIC",
	"FLAG",
	"ILLEGAL"
};

	/***********************/
	/* temporary variables */
	/***********************/

Any mt1,mt2;			/* temporary variables for macros */
char errstr[100];		/* temporary error string */

	/*************/
	/* functions */
	/*************/

Any *ferr();			/* fatal error */
Node *next(),*param();		/* routines for moving the current node */
Node *descend();		/* pointer (see P.pointer.c for details) */
Node *search(),*checknode();
char *ex_label();
double sin();			/* mathematical functions */
double cos();
double atan2();
double log();
double log10();
double pow();
double exp();
double hdrval();		/* header value */

	/******************/
	/* error messages */
	/******************/

char parent_err[] = "Parsing error: trying to access parent of root";
char chld_err[] = "Parsing error: trying to access child of leaf";
char exc_err[] = "Parsing error: trying to treat non-exclusive as exclusive";
char leaf_err[] = "Parsing error: trying to treat non-leaf as leaf";
char rnge_err[] = "Parsing error: accessing second parameter of non-range";

	/*************************/
	/* error message buffers */
	/*************************/

char *sterr;			/* start of error - pointer to beginning */
				/* of string that resulted in error */
int errflag;			/* flag used to combat lookahead */
				/* flag is set by special character, */
				/* reset by parameter.  if flag is */
				/* 1 when special character is read, */
				/* then that character is not being used */
				/* to end a parameter, and, since no */
				/* special characters involve YACC */
				/* lookahead, everything before it is */
				/* error free */

	/**********************/
	/* lex string buffers */
	/**********************/

char *unspt;			/* pointer to next character on unput stack */
char unstk[50];		/* unput stack */
extern int yyleng;		/* length of token */

	/******************************/
	/* variables for cursor input */
	/******************************/

double xc[10];			/* x value */
double yc[10];			/* y value */
int crsr[10];			/* flag set if user did not type 'e' */

	/****************************/
	/* list of header variables */
	/****************************/

HdrVar headvars[] =
{
# include "P.headvars.h"
};
HdrVar *headend = headvars + sizeof(headvars) / sizeof(HdrVar);
