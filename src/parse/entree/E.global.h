# include <stdio.h>

	/*********/
	/* files */
	/*********/

char efile[100];		/* executable file name */
char dfile[100];		/* data file name */
char ifile[100];		/* initialization file for fortran */
char tfile[100];		/* tree file name */
char sfile[100];		/* source file name */
FILE *efc;			/* executable file code */
FILE *dfc;			/* data file code */
FILE *ifc;			/* initialization file code */
FILE *tfc;			/* tree file code */
FILE *sfc;			/* source file code */

	/*****************************/
	/* entree to 'C' and fortran */
	/*****************************/

char *ctnam[TYPES] =		/* type names for C */
{
	"double",
	"int",
	"",
	"short",
	"char"
};
char *ftnam[TYPES] =
{
	"real*8",
	"integer",
	"",
	"integer*2",
	"character"
};
int da_ecf[] =			/* table lookup to find correct boundery */
{				/* type for given entree data type */
	-1,
	CHRPT,
	CHRPT,
	CHRPT,
	SCNNM,
	SCNNM,
	SCNNM,
	SCNNM,
	INT,
	INT,
	INT,
	INT,
	INT,
	INT,
	INT,
	INT,
	DOUBLE,
	DOUBLE,
	DOUBLE,
	DOUBLE,
	DOUBLE,
	DOUBLE,
	DOUBLE,
	DOUBLE
};

int da_fce[] =			/* table lookup to find correct entree */
{				/* type from boundery type */
	REAL,
	INTN,
	SCAN,
	0,
	STRING
};

char tsub[] =			/* table lookup for data type subscript */
{
	'r',
	'i',
	's',
	0,
	'c'
};

	/*********************/
	/* current situation */
	/*********************/

int lastloc;			/* last location needed in buffer */
int clin;			/* current line */
int fortrn;			/* fortran flag (set when making fortran) */
int cnode;			/* current node */
Stack stack[10];		/* stack for keeping track of children */
Stack *stkpnt;			/* stack pointer */

	/*************/
	/* variables */
	/*************/

Var var[TYPES][1000];		/* variable structures */
int vars[TYPES];		/* number of variables of each type */
char comnam[100];		/* common name */
int siz[TYPES] =		/* size of variable types */
{
	8,
	4,
	6,
	2,
	-1
};

	/****************/
	/* working tree */
	/****************/

Wnode wtree[100];

	/*********/
	/* misc. */
	/*********/

char *stpars;
