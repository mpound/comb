	/**********************/
	/* variable structure */
	/**********************/

typedef struct
{
	int loc;		/* location of variable in parse buffer */
	char name[MAXLEN];	/* name of variable */
	int jm;			/* set when variable just made */
	int chl;		/* number of characters if variable is strig */
	int lln;		/* number of elements if variable is list */
	int rng;		/* 1 if variable is range */
	int sec;		/* -1 if variable is not secondary */
				/* number of parent if it is */
	int ptp;		/* type of parent */
}
Var;

	/**************************/
	/* working node structure */
	/**************************/

typedef struct
{
	char des[100];		/* node description */
	char lbl[10];		/* label */
	int typ;		/* node type */
	int prn;		/* node parent */
	int brn;		/* branch */
	int chd[20];		/* children */
	int var;		/* variable number */
	int fvr;		/* 1 if node has variable */
	int flg;		/* number of variable for flag */
	int ffl;		/* 1 if node has flag */
	int dtp;		/* data type */
	int sdt;		/* starting data type */
	int chl;		/* number of characters in string */
	int rng;		/* 1 if node is range */
	int lln;		/* length of list (0 if node isn't list) */
	int dft;		/* default type */
	char dfv[100];		/* value for default */
	int lin;		/* line number of node description */
	int ext;		/* 1 if node is external */
	char evr[100];		/* external variable */
	char eds[100];		/* description of external variable */
	int tfv;		/* variable number of type flag */
	int efv;		/* variable number of elements */
}
Wnode;

	/****************************/
	/* children stack structure */
	/****************************/

typedef struct
{
	int branch;		/* branch of node */
	int parent;		/* parent of nodes to come */
}
Stack;
