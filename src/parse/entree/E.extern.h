# include <stdio.h>

	/*********/
	/* files */
	/*********/

extern char efile[100];		/* executable file name */
extern char dfile[100];		/* data file name */
extern char ifile[100];
extern char tfile[100];		/* tree file name */
extern char sfile[100];		/* source file name */
extern FILE *efc;		/* executable file code */
extern FILE *dfc;		/* data file code */
extern FILE *ifc;
extern FILE *tfc;		/* tree file code */
extern FILE *sfc;		/* source file code */

	/*****************************/
	/* entree to 'C' and fortran */
	/*****************************/

extern char *ctnam[TYPES];	/* type names for C */
extern char *ftnam[TYPES];
extern int da_ecf[];		/* table lookup to find correct boundery */
				/* type for given entree data type */
extern int da_fce[];
extern char tsub[];

	/*********************/
	/* current situation */
	/*********************/

extern int lastloc;		/* last location needed in buffer */
extern int clin;		/* current line */
extern int fortrn;		/* fortran flab (set when making fortran) */
extern int cnode;		/* current node */
extern Stack stack[10];		/* stack for keeping track of children */
extern Stack *stkpnt;		/* stack pointer */

	/*************/
	/* variables */
	/*************/

extern Var var[TYPES][1000];	/* variable structures */
extern int vars[TYPES];		/* number of variables of each type */
extern char comnam[100];	/* common name */
extern int siz[];		/* size of variable types */

	/****************/
	/* working tree */
	/****************/

extern Wnode wtree[100];

	/*********/
	/* misc. */
	/*********/

extern char *stpars;
