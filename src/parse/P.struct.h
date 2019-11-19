	/**************/
	/* yacc union */
	/**************/

typedef union
{
	double real;
	char *string;
}
YYSTYPE;

	/**********************/
	/* union of all types */
	/**********************/

typedef union
{
	double d;
	float f;
	int i;
	short sh;
	short *shp;
	char *st;
	char c;
	Node *n;
}
Any;

	/***************************************************************/
	/* structure returned by handhold for making humanized command */
	/***************************************************************/

typedef struct
{
	int entered;			/* whether or not the node was */
					/* entered */
	int labeled;			/* whether or not the returned */
					/* command begins with a label */
	char cmd[400];			/* command */
}
hhstruct;

	/***********************************/
	/* description of header variables */
	/***********************************/

typedef struct
{
	char *nm;			/* name of variable */
	int lc;				/* location of value or valDes number*/
	short ln;			/* length (for using subscripts) */
	char tp;			/* type of variable */
	char mt;                        /* type of memory variable is in */
}
HdrVar;
