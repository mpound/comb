/*************/
/* Constants */
/*************/

# define MAXSTKNUM 262143	/* maximum stack number */
# define MAXSCNUM 32767		/* maximum scan number */
# define FULL 1			/* flag for full stack */
# define EMPTY 0		/* flag for empty stack */
# define OKTOMAKE 1		/* flag for making directory */
# define DONTMAKE 0		/* flag for not making directory */
# define FOREGROUND 0		/* foreground directory */
# define BACKGROUND 1		/* background directory */
# define DUMBINTERP 0		/* inverse distance "cone" interpolation */
# define BUMPINTERP 1		/* nearest point interpolation */
/*# define TRIINTERP 2 -- mwp  5/92*//* triangular interpolation */
# define SMARTINTERP 2		/* bessel interpolation */
# define GAUSSINTERP 3		/* gaussian interpolation */
# define NOINTERP   -1          /* no interpolation--to speed up
                                 * routines which use FillInArray
			 	 * to get a list of stacks, but which
				 * don't need to do any interpolation 
				 * e.g., lk() */
# define MAXCLEVELS 75
#define NWEIGHTS 15
				/* max number of contour lines */
# define MAXARRAYSIZE 1000	/* maximum size of arrays */
# define MAXCHN 16224     	/* Max number of channels in a stack -
				 * limited by length in struct of_indx being
				 * an unsigned short.  Must be a multiple
				 * of 32 for the allignment of use arrays */
# define FRSTONDISK 4		/* first stack number on disk */
# define NOTFS -9999		/* flag for refch to indicate that
				the observation was not freq switched */
# define MAXNSTK_ 32		/* size of nstk array in stacks */
# define MAXNSTKX 128		/* size of nstk array in stacks */

/*******************/
/* Data Structures */
/*******************/
/* Modified stack structure MWP 2/93 */

struct of_stk			/* structure of stacks in core */
{
	short numpt;			/* number of channels */
	short nver;			/* version number of header */
	short nprc;			/* data type */
	short nplc;			/* preferred plotting coordinates */
	short onstk;			/* old short stack number */
	short nwts;			/* number of weights */
	short locwt[NWEIGHTS];		/* locations of weights */
	short ibsln;			/* order of baseline removed */
	char kuse[64];			/* use array */
	short nstk[MAXNSTK_];		/* list of scan numbers */
	short nlist;			/* number of scan numbers in list */
	short indx;			/* last location in nstk */
	short subscan;			/* subscan # a la Kitt Peak: MWP 2/93*/
	short ftrm;			/* number of fourier components filtered */
	char label[64];			/* char array to replace isdum: MWP 12/92*/
					/* I feel better making sizeof label be
					 * a power of 2, so will retain a smaller
					 * isdum */
	int j2second;			/* seconds since J2000 (+/-). Same as J2Second
				         * keyword in BLSDF
					 */
	short isdum[2];			/* not used */
	int numst;			/* stack number */
	float fwid;			/* filter width */
	float expch;			/* expected channel */
	float refch;			/* expected line center */
	float vlsr;			/* LSR velocity of expch */
	float exwid;			/* expected line width */
	float wght[NWEIGHTS];		/* weights */
	float factt;			/* fudge factor */
	float time;			/* integration time */
	float pwrav;			/* average pwer per channel */
	float pwrer;			/* rms error in pwrav */
	float rms;			/* rms of channels in use array */
	float tmax;			/* peak temperature */
	float arerr;			/* error in the area */
	float area;			/* total area under the line */
	float vcent;			/* Velocity of the centroid */
	float fwhm;			/* FWHM or FWZP */
	float fwerr;			/* fitting error in fwhm */
	float fpeak;			/* velocity of peak of curve from */
					/* gaussian fit */
	float fperr;			/* fitting error in fpeak */
	float ftmax;			/* temperature at peak of gaussian */
					/* fit */
	float fterr;			/* fitting error in ftmax */
	float rmval;			/* temp subtracted from center chan */
	float ra;			/* ra of map center */
	float dec;			/* declination of map center */
	float l;			/* longitude of map center */
	float b;			/* latitude of map center */
	float dra;			/* ra offset */
	float ddec;			/* dec offset */
	float dl;			/* latitude offset */
	float db;			/* longitude offset */
	float dx;			/* horizontal user offset */
	float dy;			/* vertical user offset */
	float odra;			/* ra observing offset */
	float oddec;			/* dec observing offset */
	float epoch;			/* year of ra/dec coordinate system */
	float vpeak;			/* velocity of tmax in in */
	float chisq;			/* Chisq from fit (pf or gf) */
	float nfree;			/* (float)nfree from fit (pf or gf) */
	float rsdm[10];			/* not used */
	double freq;			/* assumed line frequency */
	float stak[MAXCHN];		/* values in stack */
	short nstkx[MAXNSTKX];		/* extension of scan number list */
};

typedef struct			/* use array stuff */
{
	int kusfl;			/* set use array from stack? */
	char muse[MAXCHN / 8];		/* stored use array */
	char nuse[MAXCHN / 8];		/* active use array */
} USE;

typedef struct			/* array of values from sky */
{
	int xres;			/* horizontal resolution */
	int yres;			/* vertical resolution */
	double lowH;			/* coord value of low horizontal */
	double lowV;			/* coord value of low vertical */
	double highH;			/* coord value of high horizontal */
	double highV;			/* coord value of high vertical */
	double lowVel;			/* low velocity */
	double highVel;			/* high velocity */
	float data[1];			/* the array of values itself */
					/* (note: the 1 is meaningless; */
					/* it's just there to pacify the */
					/* compiler) */
}
SKYARRAY;

/********************/
/* Global Variables */
/********************/

extern struct of_stk stk_[FRSTONDISK - 1];/* stacks in core */
extern int coreFull[FRSTONDISK - 1];	/* flags for whether core stacks */
					/* are full */
extern USE use_;
extern int stkValFlag;			/* flag set when computing a value */
					/* of a stack */
extern double stkValue;			/* value of the stack */

/* The short isDrag[] is currently unused. I had used it in earlier
 * attempts to deal with DRAGONs in stacks, when I thought it would be
 * useful to keep track of which channels in the current stack were
 * DRAGON. It was declared in data.c and send by ChkDrag.  I'm leaving 
 * this in as a comment for now, because it  might be useful in the 
 * future. Probably not though. -- MWP 5/96
 */
/*extern short isDrag[MAXCHN];  /* Array that says where DRAGON(s) are     */
			      /* in a stack.  isDrag[i] gets the value:  */
                              /* 1 if channel i is DRAGON  		 */
                              /* 0 otherwise              		 */

/**************************
 * define global routines *
 **************************/
/* NB: search.c: MkSrchIndx Can't be defined here, but is defined in
 * include.h */

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* data.c */
void mrkful_ P_((int *sn));
void CreateData P_((void));
void OpenData P_((void));
void CloseData P_((void));
int NextStk P_((register int stkNum, int find));
int StkIsFull P_((int stkNum));
int stfull_ P_((int *n));
int NumInds P_((void));
int ChckData P_((int stkNum));
void EmData P_((int stkNum));
void UnEmData P_((int stkNum));
void WrData P_((int stkNum, register struct of_stk *stk));
void gstak_ P_((register int *s1, register int *s2));
void getStack P_((register int src, register int dest));
int RdData P_((int stkNum, register struct of_stk *stk));
int RdVaxData P_((int stkNum, register struct of_stk *stk));
int CvtBEndianToLEndian P_((int stkNum, register struct of_stk *stk));
char *TestByteOrder(int expectCorrect);
void MkStkList P_((register int first, register int last));
int NextStkOnList P_((void));
void RmStkOnList P_((register int stack));
void FreeStkList P_((void));
int IsSameData P_((register struct of_stk *stk1, register struct of_stk *stk2));
int ChkDrag P_((int stkNum));
int ClrDrag P_((int stkNum,int startch));


/* default.c */
void CreateDefault P_((void));
void OpenDefault P_((void));
void CloseDefault P_((void));
int GtStkDflt P_((char *name, char *format, char *address, int bomb));
void WrStkDflt P_((char *name, char *format, ...));
void RmDefault P_((char *name));

/* errors.c */
void DirErr P_((char *message));
void CDirErr P_((void));
void StkNumErr P_((int stkNum, char *message));
void StkDskErr P_((int stkNum, char *message, int flag));
void StkDskWrn P_((int stkNum, char *message, int flag));

/* interp.c */
void SetStkInterp P_((int type, double radius, double effdia, double oldgrid));

/* search.c */
void CreateSearch P_((void));
void OpenSearch P_((void));
void CloseSearch P_((void));
void WrSearch P_((struct of_stk *stk));
void RmSearch P_((struct of_stk *stk));
void SearchFlush P_((int flush));
void SearchHdrPrint P_((int dir));
void OkFreqs P_((double high, double low));
void OkStacks P_((int high, int low));
void SetMaxDis P_((double range));
void FreeArray P_((void));
void InterpAtPoint P_((double x, double y, double filtWid, int catScanLst));
int ExecuteAtPoint P_((double x, double y, char *string));
void SearchFileError(int stkNum);
int CombineSimilar P_((int verbose));

/* value.c */
void InitValues P_((void));
void ClrStkValue P_((int stkNum));
char *StkValMacro P_((char *macAndArgs, int order));

#undef P_
