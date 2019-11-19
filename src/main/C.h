#include "History.h"
#define COMB 1
#define OBS 0
#define VERSION "2000.2"

/*************/
/* Constants */
/*************/

# define NTTYLEN 8192		/* maximum length of a command */
# define PATHLENGTH 128

# if COMB
/* Values for htype and c1 in a picture description */
# define CHAN 1	
# define FREQ 2
# define VEL 3
# define AGST 4
# define SP_SP 5
# define SP_VEL 6
# define XY_PLOT 7
# endif /*COMB*/

/*********************
   Global Structures
**********************/
struct CMNDH
{
	char *gstr;	/* The argument string to be parsed in this cmd */
	int cmdsig;	/* A number which changes each time the tty is read */
	char nc[6];	/* The current command name */
};
extern struct CMNDH cmnd_;
struct MAIN
{
# if COMB
	int htype;	/* Type of horizontal coordinate on current plot */
	int pltd;	/* non zero iffi the current plot corresponds
			to stack 1 */
	int apfl;	/* non zero for auto plotting */
	char stkpth[PATHLENGTH];/* Path to default location for stacks directories */
			/* total stacks name length will be 64 (MAXDNLEN)  */
	int drgmes;     /* message level when encountering a DRAGON in a stack */
			/* 0: don't check for DRAGON, no message
			/* 1: check for DRAGON, no message
			/* 2: message when stack is brought into memory (getStack)*/
# endif /*COMB*/
	int test;	/* logical result of tests in various commands
				- a 'header variable' for parse */
	char comb[PATHLENGTH];	/* Path name of the basic comb directory */
	int warny;	/* If true, warn will assume a 'y' answer */
	int macroDepth;	/* Depth of macro execution, 0 if at base level */
#if OBS
	char apfl;	/* Controls automatic move to main map of new posn */
	char newPoint;	/* A new center position has been given */
#endif /*OBS*/
};
extern struct MAIN main_;

#ifndef NODEDEFINED
/* This is also in ../parse/P.tree.h, but it is needed in all commands
 * and this is the easiest way to get it in and not include extras */
/* tree structure */
typedef struct
{
	char *des;			/* description of node */
	char *label;			/* label of node */
	char type;			/* type of node */
	char parent;			/* parent of node */
	char branch;			/* branch number of node */
	char extra;			/* so info will start on an even byte */
	unsigned char info[32];		/* Info is documented in P.tree.h */
} Node;

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* P.parse.c */

void parse_ P_((char **str_arg, Node *tree_arg, char *pbuf_arg));

#undef P_

#define NODEDEFINED
#endif /*NODEDEFINED*/

/* macros form f2c.h which are generally useful */

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define DABS(x) (double)ABS(x)
#define MIN(a,b) ((a) <= (b) ? (a) : (b))
#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#define DMIN(a,b) (double)MIN(a,b)
#define DMAX(a,b) (double)MAX(a,b)

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* main.c */
int main P_((int argc, char **argv));
void combex_ P_((char *str));
void error_ P_((char *sp));
void error P_((char *s, ...));
void warn_ P_((char *sp));
void warn P_((char *s, ...));
void wait_ P_((void));
void noint_ P_((void));
void yesint_ P_((void));

#undef P_
#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* macro.c */
void rdmac P_((void));
void exmac P_((void));
void GetMacro P_((char *macAndArgs, char *expMac));

#undef P_
