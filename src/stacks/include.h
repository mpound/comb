/* must include ../coordsys/C.h before this */

/*************/
/* Constants */
/*************/

# define BLKSIZE 4096			/* size of disk block */
# define MAXDNLEN 64			/* max length of directory name */
# define MAXNMLEN 31			/* max length of default value */
					/* name */
# define FREQSCALE 100.			/* scaler for frequency */
#ifndef HALFSPACE
	/* half of the space occupied by the celestial sphere (2^30) */
# define HALFSPACE 1073741824
#endif /*HALFSPACE*/

/*******************/
/* Data structures */
/*******************/

typedef struct of_STKLINK		/* structure for linked stack list */
{
	int stkNum;				/* stack number */
	XYZPOINT loc;				/* location of stack */
	double weight;				/* weight of stack */
	struct of_STKLINK *next;		/* next in list */
}
STKLINK;

typedef struct				/* stack index structure */
{
	int x;					/* x location of stack */
	int y;					/* y location of stack */
	int z;					/* z location of stack */
	int freq;				/* frequency of stack */
	int stkNum;				/* stack number */
}
STKINDX;


/********************/
/* Global variables */
/********************/

extern int curDir;			/* current directory */
					/* (FOREGROUND or BACKGROUND) */
extern char dirName[2][MAXDNLEN];	/* names of open directories */
extern int notOpen[2];			/* flags set when directories */
					/* aren't open */
extern int stkReadOnly[2];		/* Don't write data in this directory
					 * data, index, or search is rd only */
extern int stkDefReadOnly[2];		/* Don't write defaults */
extern int lowStkNum;			/* low stack set by OkStks */
extern int highStkNum;			/* high stack set by OkStks */
extern double lowFreq;			/* low frequency set by OkFreqs */
extern double highFreq;			/* high frequency set by OkFreqs */
extern double iRad;			/* interpolation radius */
extern double iRadSqr;			/* interpolation radius squared */
extern int iIRad;			/* integer version of interpolation */
					/* radius */
extern double qCut;			/* cut-off radius in u-v plane bi: */
extern double gridL;			/* old grid spacing in stack */
extern int gtArrValues;			/* Flag for "Getting array values
					   message in Stk2Space */
extern char interpNames[][3];
extern int interpType;			/* code for type of interpolation */


#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* data.c */
void SetStackPosition P_((struct of_stk *stp, XYPOINT p));

/* interp.c */
void FillInWeights P_((XYZPOINT lc, STKLINK *list));

/* search.c */
STKINDX MkSrchIndx P_((register struct of_stk *stk));
int FindNear P_((XYZPOINT point, double range));
double dissquare P_((XYZPOINT *p1, XYZPOINT *p2));
void FillInArray P_((XYPOINT pLowerLeft, XYPOINT pUpperRight, int columns, int rows, int (*save)()));
float *Stk2Space P_((XYPOINT pLowerLeft, XYPOINT pUpperRight, int xRes, int yRes, double minWt));
float *Stk1Space P_((XYPOINT point1, XYPOINT point2, double low, int numCh, int spaceRes, double filtWid));

/* value.c */
double LocVal P_((STKLINK *list, double minWt));
int AvgStacks P_((register STKLINK *list, double filtWid, int catScanLst));

#undef P_
