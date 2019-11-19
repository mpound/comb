# include <setjmp.h>
# include <math.h>

	/****************************************************/
	/* variables for keeping track of current situation */
	/****************************************************/

extern Node *root;		/* current root */
extern Node *cnode;		/* current node pointer */
extern Node *nnode;		/* next value of current node pointer - */
				/*    used to combat YACC lookahead     */
				/*    (before YACC reduces a parameter  */
				/*    and stores it, LEX must read a    */
				/*    special character to indicate its */
				/*    end.  If this character set       */
				/*    cnode directly, then the para-	*/
				/*    meter would get stored in the	*/
				/*    wrong place)			*/
extern Node *rootstack[30];	/* stack of previouse values of root */
extern Node **rstkpnt;		/* root stack pointer */
extern int cda_type;		/* current data type */
extern int spor;		/* second part of range */
#if COMB
extern Scan curscn_;		/* current scan number */
#endif /*COMB*/
extern int hhmode;		/* set when in handhold mode */
extern jmp_buf hherr;		/* location to recover to in handhold */

	/******************/
	/* user variables */
	/******************/

extern double gg_[];		/* global numerics */
extern char *(sg_[]);		/* global strings */

	/*****************/
	/* string buffer */
	/*****************/
extern char *strbuf;		/* string buffer */
extern char *strpos;		/* pointer to current location in buffer */

	/********************************/
	/* global pointers to arguments */
	/********************************/

extern Node *tree;		/* tree structure */
extern char *pbuf;		/* parameter buffer */
extern char *str;		/* string being parsed */

	/******************************/
	/* key words for descriptions */
	/******************************/

extern char *(da_type[]);	/* data types */
extern char *(seq[]);		/* sequences */
extern char *(df_type[]);	/* default types */

	/***********************/
	/* temporary variables */
	/***********************/

extern Any mt1,mt2,mt3;		/* temporary variables for macros */
extern char errstr[];		/* temporary error string */

	/*************/
	/* functions */
	/*************/

Any *ferr();			/* fatal error */
char *index();			/* index of character in string */
double atof();			/* ascii to float */
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
char *hdrstrval();

	/******************/
	/* error messages */
	/******************/

extern char parent_err[];
extern char chld_err[];
extern char exc_err[];
extern char leaf_err[];
extern char rnge_err[];

	/*************************/
	/* error message buffers */
	/*************************/

extern char *sterr;		/* start of error - pointer to beginning */
				/* of string that resulted in error */
extern int errflag;		/* flag used to combat lookahead */

	/**********************/
	/* lex string buffers */
	/**********************/

extern char *unspt;		/* pointer to next character on unput stack */
extern char unstk[];		/* unput stack */
extern int yyleng;		/* length of token */

	/*****************/
	/* yacc variable */
	/*****************/

extern YYSTYPE yylval;

	/******************************/
	/* variables for cursor input */
	/******************************/

extern double xc[10];		/* x value */
extern double yc[10];		/* y value */
extern int crsr[10];		/* flag set if user did not type 'e' */

	/****************************/
	/* list of header variables */
	/****************************/

extern HdrVar headvars[];
extern HdrVar *headend;			/* end of header variable list */
