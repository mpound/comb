/*************/
/* Constants */
/*************/
#include <stdio.h>

# define F_X 0			/* x figure (for Pfigure) */
# define F_RECTANGLE 1		/* rectangle figure */
# define F_TRIANGLE 2		/* triangle figure */
# define F_DTRIANGLE 3		/* triangle pointing down */
# define F_CROSS 4		/* cross */
# define F_ELLIPSE 5		/* ellipse */
# define F_DIAMOND 6		/* diamond */
# define F_BAR 7		/* vertical bar "I" */
# define F_BAR_H 8		/* horizontal bar  MWP 01/04/95 */
# define F_OCTAGON 9
# define F_DOT 10		/* dot MWP 3/2/93 */
# define XDIM (int)65534	/* number of virtual pixels in x */
# define YDIM (int)65534	/* number of virtual pixels in y */
# define FORMNMLEN 20		/* maximum length of format length */
				/* (including null termination) */
# define TERMNMLEN 11		/* maximum length of a terminal name */
				/* (including null termination) */
# define DIRNMLEN 100		/* maximum length of directory name */
				/* (including null termination) */
# define UNKNOWN (int)(0x80000000) /* indicates an unknown int */
				/* (this is the highest neg (RWW) number) */

# define MAXWINDOWS 5		/* maximum number of windows in a format */
# define MAXPLACES 30		/* maximum number of places in a format */
# define TXTTYPES 3		/* number of text types */
# define LINTYPES 256		/* number of line types */
/* special line types for specific parts of a plot */
# define BORDERlt 200
# define TICSlt 201
# define GRID0lt 202
# define GRIDlt 203

/* Types for label formats G is the simple %g format with no idea about
 * the decimal point or precision.  F is for LabelPrint with a correspong
 * [hv]fmt value given.  HMS also uses the [hv]fmt. */
# define GFMT 0
# define FFMT 1
# define HMSFMT 2

/*******************************/
/* High level plotting options */
/*******************************/

# define NO -1 *			/* negate option */
# define ENDOPT 0			/* end of option list */
# define V_XLIST 1
# define XLIST(a,l) V_XLIST,a,l		/* x list of values */
# define V_YLIST 2
# define YLIST(a,l) V_YLIST,a,l		/* y list of values */
# define V_XRANGE 3
# define XRANGE(f,t) V_XRANGE,f,t	/* range of x values */
# define V_YRANGE 4
# define YRANGE(f,t) V_YRANGE,f,t	/* range of y values */
# define V_XNOISE 5
# define XNOISE(n) V_XNOISE,n		/* noisiness of x values */
# define V_YNOISE 6
# define YNOISE(n) V_YNOISE,n		/* noisiness of y values */
# define V_XLOG 7
# define XLOG V_XLOG			/* make x logarithmic */
# define V_YLOG 8
# define YLOG V_YLOG			/* make y logarithmic */
# define V_XSTEP 9
# define XSTEP(s) V_XSTEP,s		/* step in x */
# define V_YSTEP 10
# define YSTEP(s) V_YSTEP,s		/* step in y */
# define V_KEYX 11
# define KEYX V_KEYX			/* key x to grid */
# define V_KEYY 12
# define KEYY V_KEYY			/* key y to grid */
# define V_LABELX 13
# define LABELX V_LABELX		/* draw labels on x axis */
# define V_LABELY 14
# define LABELY V_LABELY		/* draw labels on y axis */
# define V_GRID 15
# define GRID V_GRID			/* draw grid */
# define V_LINES 16
# define LINES V_LINES			/* use lines */
# define V_LTYPE 17
# define LTYPE(t) V_LTYPE,t		/* line type */
# define V_HIST 18
# define HIST V_HIST			/* use histogram */
# define V_FIGURE 19
# define FIGURE(f) V_FIGURE,f		/* use figure */
# define V_FWIDTH 20
# define FWIDTH(w) V_FWIDTH,w		/* width of figure */
# define V_FHEIGHT 21
# define FHEIGHT(h) V_FHEIGHT,h		/* height of figure */
# define V_FWLIST 22
# define FWLIST(a,l) V_FWLIST,a,l	/* list of figure widths */
# define V_FHLIST 23
# define FHLIST(a,l) V_FHLIST,a,l	/* list of figure heights */
# define V_XREVERSE 24
# define XREVERSE V_XREVERSE		/* reverse x */
# define V_YREVERSE 25
# define YREVERSE V_YREVERSE		/* reverse y */
# define V_XDIVS 26
# define XDIVS(d) V_XDIVS,d		/* maximum divisions in x */
# define V_YDIVS 27
# define YDIVS(d) V_YDIVS,d		/* maximum divisions in y */
# define V_HTICLEN 28
# define HTICLEN(l) V_HTICLEN,l		/* length of horizontal tic marks */
# define V_VTICLEN 29
# define VTICLEN(l) V_VTICLEN,l		/* length of vertical tic marks */
# define V_FLAGX0 30
# define FLAGX0 V_FLAGX0		/* flag tic or line where x = 0 */
# define V_FLAGY0 31
# define FLAGY0 V_FLAGY0		/* flag tic or line where y = 0 */
# define V_BORDER 32
# define BORDER V_BORDER		/* draw border */
# define V_fXLIST 33
# define fXLIST(a,l) V_fXLIST,a,l	/* x list of floats */
# define V_fYLIST 34
# define fYLIST(a,l) V_fYLIST,a,l	/* y list of floats */
# define V_fFWLIST 35
# define fFWLIST(a,l) V_fFWLIST,a,l	/* float list of figure widths */
# define V_fFHLIST 36
# define fFHLIST(a,l) V_fFHLIST,a,l	/* float list of figure heights */
# define V_iXLIST 37
# define iXLIST(a,l) V_iXLIST,a,l	/* x list of ints */
# define V_iYLIST 38
# define iYLIST(a,l) V_iYLIST,a,l	/* y list of ints */
# define V_iFWLIST 39
# define iFWLIST(a,l) V_iFWLIST,a,l	/* int list of figure widths */
# define V_iFHLIST 40
# define iFHLIST(a,l) V_iFHLIST,a,l	/* int list of figure heights */
# define V_ARRAY 41
# define ARRAY(a,r,c) V_ARRAY,a,r,c	/* array of doubles */
# define V_HATCH 42
# define HATCH(l,s) V_HATCH,l,s		/* hatch marks for contour map */
# define V_CADD 43
# define CADD(v,l,n) V_CADD,v,l,n	/* add contour levels */
# define V_CREM 44
# define CREM(v,n) V_CREM,v,n		/* remove contour levels */
# define V_CNUM 45
# define CNUM(n) V_CNUM,n		/* number of contour levels */
# define V_CLEVRANGE 46
# define CLEVRANGE(l,h) V_CLEVRANGE,l,h	/* range of contour levels */
# define V_CLEVLOG 47
# define CLEVLOG V_CLEVLOG		/* make contour levels logarithmic */
# define V_CLEVS 48
# define CLEVS(v) V_CLEVS,v		/* specific list of contour levels */
# define V_CLINTYPE 49
# define CLINTYPE(t) V_CLINTYPE,t	/* line type for all contours */
# define V_CLINSCALE 50
# define CLINSCALE V_CLINSCALE		/* scale line types to contour level */
# define V_CLINALT 51
# define CLINALT(t1,t2) V_CLINALT,t1,t2	/* alternate line types */
# define V_CLINS 52
# define CLINS(l) V_CLINS,l		/* specific list of contour lines */
# define V_CSTORE 53
# define CSTORE(v,l,n) V_CSTORE,v,l,n	/* store contour levels */
# define V_CLEVSTEP 54
# define CLEVSTEP(v) V_CLEVSTEP,v	/* specify setp between contours */
# define V_STEPSTOR 55
# define STEPSTOR(x,y) V_STEPSTOR,x,y	/* pointers to doubles for MkGrid to
					save xStep and yStep in */
# define V_CLINCOLOR 56
# define CLINCOLOR V_CLINCOLOR		/* monotonicaly increasing line types*/
# define V_CLEAROPT 57
# define CLEAROPT V_CLEAROPT
# define V_XLABHMS 58
# define XLABHMS V_XLABHMS
# define V_YLABHMS 59
# define YLABHMS V_YLABHMS
# define V_CIRCBORDER 60
# define CIRCBORDER V_CIRCBORDER
# define V_FILLED  61			/* Figure to be drawn is filled */
# define FILLED V_FILLED

/*******************/
/* Data structures */
/*******************/

typedef struct			/* general information about windows */
{
	unsigned char xIsScaled;	/* flag set if x should be scaled */
	unsigned char yIsScaled;	/* flag set if y should be scaled */
	unsigned char square;		/* flag set if window should be */
					/* adjusted so that one unit in */
					/* x is the same length as one */
					/* unit in y */
}
GENWINDOW;

typedef struct			/* specific information about windows */
{
	char leftSame;			/* flag for whether left is the */
					/* same as graph window */
	char bottomSame;		/* flag for whether bottom is the */
					/* same as graph window */
	char rightSame;			/* flag for whether right is the */
					/* same as graph window */
	char topSame;			/* flag for whether top is the */
					/* same as graph window */
	unsigned short int left;	/* left hand side of window */
	unsigned short int bottom;	/* bottom of window */
	unsigned short int right;	/* right hand side */
	unsigned short int top;		/* top */
	unsigned short int xpels;	/* number of pixels in x */
	unsigned short int ypels;	/* number of pixels in y */
	int clx[TXTTYPES];		/* number of character widths to */
					/* move left of window over */
	int cly[TXTTYPES];		/* number of character heights to */
					/* move left of window over */
	int cbx[TXTTYPES];		/* number of character widths to */
					/* move bottom of window up */
	int cby[TXTTYPES];		/* number of character heights to */
					/* move bottom of window up */
	int crx[TXTTYPES];		/* number of character widths to */
					/* move right of window over */
	int cry[TXTTYPES];		/* number of character heights to */
					/* move right of window over */
	int ctx[TXTTYPES];		/* number of character widths to */
					/* move top of window down */
	int cty[TXTTYPES];		/* number of character heights to */
					/* move top of window down */
}
SPECWINDOW;

typedef struct			/* specific information about places */
{
	unsigned short int x;		/* x location of lower left hand */
					/* corner of first letter in place */
	unsigned short int y;		/* y location */
	unsigned short type;		/* text type used in place */
	unsigned short int width;	/* width in characters */
	unsigned short int height;	/* height in lines of text */
	unsigned short int indent;	/* number of characters to indent */
					/* after first line */
	unsigned char isSideways;	/* 1 if place is to be plotted */
					/* sideways (reading bottom to top) */
	unsigned char isCentered;	/* 1 if place is to be centered */
}
SPECPLACE;

typedef struct			/* graphics format */
{
	char name[FORMNMLEN];		/* name of format */
	unsigned char defined;		/* flag for whether specific */
					/* information is defined for this */
					/* terminal */
	unsigned short int xOvflow;	/* number of pixels between */
					/* x overflow lines and graph window */
					/* (window 0) */
	unsigned short int yOvflow;	/* number of pixels to y overflow */
					/* lines */
	unsigned short int xLab;	/* number of pixels between */	
					/* x labels and graph window */
	unsigned short int yLab;	/* number of pixels to y labels */
	unsigned char xIsSideways;	/* 1 if x labels are to be plotted */
					/* sideways */
	unsigned char yIsSideways;	/* 1 if y labels are sideways */
	unsigned short int numWindows;	/* number of windows */
	GENWINDOW gWindow[MAXWINDOWS];	/* general information about windows */
	SPECWINDOW sWindow[MAXWINDOWS];	/* specific information */
	unsigned short int numPlaces;	/* number of places */
	SPECPLACE sPlace[MAXPLACES];	/* specific information about places */
}
GRFORMAT;

typedef struct			/* picture */
{
	GRFORMAT *format;		/* pointer to format */
	unsigned short int formatNum;	/* number of screen format */
	char xLog;			/* whether x is a logarithmic scale */
	char yLog;			/* whether y is a logarithmic scale */
	double left;			/* value of left of graph window */
	double right;			/* value of right of graph window */
	double bottom;			/* value of bottom of graph window */
	double top;			/* value of top of graph window */
	double xScale;			/* scale factor for x */
	double yScale;			/* scale factor for y */
	double unitsRatio;		/* size ratio of x units (in pixels)
					 * to y units for a square plot */
	int leftPel;			/* pixel number of left of graphics */
					/* window (only meaningful for */
					/* cursor reads) */
	int bottomPel;			/* pixel number of bottom of */
					/* graphics window (only meaningful */
					/* for cursor reads) */
	double xPelScale;		/* x pixel scale (cursor reads) */
	double yPelScale;		/* y pixel scale (cursor reads) */
	unsigned short int lastx;	/* last x location shown on screen */
	unsigned short int lasty;	/* last y location shown on screen */
	unsigned char lastwindow;	/* last window used on screen */
	unsigned char lastltype;	/* last line type used */
	unsigned char lastttype;	/* last text type used */
	unsigned char windowNum;	/* window currently being plotted in */
	GENWINDOW *gWindow;		/* pointer to window structure */
					/* (general information) */
	int fullSize;			/* size of entire picture structure */
	int iBufSize;			/* size of intermediate code buffer */
					/* (amount of storage allocated) */
	int iBufEnd;			/* end of intermediate code buffer */
					/* (number of byte after last */
					/* instruction.  first byte is 0) */
	unsigned char lType;		/* current line type */
	unsigned char tType;		/* current text type */
	char htype;			/* htype of this plot */
	char numLinks;			/* Links to PICTURE pointers */
	int i1;				/* user info */
	int x;				/* current x location */
					/* (UNKNOWN if unknown) */
	int y;				/* current y location */
					/* (UNKNOWN if unknown) */
	char *start;			/* place to start displaying in */
					/* intermediate code buffer */
	char iBuf[1];			/* intermediate code buffer */
					/* (1 is just there to pacify */
					/* the compiler; iBuf may be any */
					/* length depending on how much */
					/* memory was allocated for this */
					/* picture. iBufSize gives the */
					/* actual length of iBuf in bytes) */
} PICTURE;

typedef struct		/* graphics cursor read structure */
{
	double x;		/* x location of cursor */
	double y;		/* y location of cursor */
	int xBox;		/* x box containing cursor */
	int yBox;		/* y box containing cursor */
	char isPlot;		/* 1 if there is a graph in the chosen box */
	char endFlag;		/* 1 if end was indicated, 0 if not */
}
GCREAD;

typedef struct			/* list pointer for option arguments */
{
	char type;			/* data type of elements of list */
	union				/* pointer to list */
	{
		double *d;
		float *f;
		int *i;
	}
	point;
}
LISTPOINTER;

typedef union				/* union for option arguments */
{
	char c;
	int i;
	float f;
	double d;
	LISTPOINTER l;
}
ANYTYPE;

extern short bxCol,bxRow;	/* column and row of current plotting box */
extern int labelsUC;		/* translate all label alphas to upper case */
extern float xSquish, ySquish;	/* Factors for adjusting plot scales */
extern int noShow;		/* flag to tell Pshow not to actually show */

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* graph.c */
void MkGraph P_((int firstOpt, ...));

#undef P_
#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* grid.c */
void MkGrid P_((int firstOpt, ...));
double ChooseLogStep P_((double low, double high, int maxDivs));
double ChooseStep P_((double low, double high, int maxDivs));
double StepSize P_((double diff, double *stepIndx, int maxDivs));
void KeyValues P_((int isLog, double *low, double *high, double step, int out));
double Roundd P_((double v));

#undef P_
#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* guts.c */
void Pstart P_((void));
void Pscreen P_((int ixBox, int iyBox));
int Pfull P_((int ixBox, int iyBox));
void PsetType P_((int type));
int PgetType P_((void));
void Pformdir P_((char *name));
void Pformat P_((char *formName));
void Pwindow P_((int windowNum));
void Pcreate P_((PICTURE **pic, int size));
void Plink P_((PICTURE *picP, PICTURE **pic));
void Punlink P_((PICTURE **pic));
void Pbounds P_((double left, double bottom, double right, double top));
void Punitsratio P_((double ratio));
void Plogbounds P_((double left, double bottom, double right, double top, int xIsLog, int yIsLog));
void ReBound P_((double left, double bottom, double right, double top, int xIsLog, int yIsLog));
int Povrflow P_((int flag));
void Pborder P_((void));
void Pcircborder P_((void));
void Pfigure P_((double x, double y, int figure, double width, double height, int filled));
void Pmove P_((double x, double y));
void Pline P_((double x, double y));
void Pboxfill P_((double x, double y, short int color));
void Ptext P_((char *text));
void Pstext P_((char *text));
void Pplace P_((int place, register char *text));
void Pctext P_((char *text));
void Pcstext P_((char *text));
void Plabelfmt P_((int htype, int hfmt, int vtype, int vfmt));
void Pxlabel P_((double x));
void Pylabel P_((double y));
void Pgrnum P_((double x, double y, double num));
void Ptexttype P_((int type));
void Plinetype P_((int type));
int PushInstr P_((int instr, int arglen));
void GtSpec P_((char *fileName));

#undef P_
#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* map.c */
void MkMap P_((int firstOpt, ...));

#undef P_
#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* options.c */
int MkOpt P_((int firstOpt, ...));
/* Prototype AddOpt defined iff file which
 * include $G/C.h also includes <stdarg.h> first
 */
#ifdef va_start
void AddOpt P_((int firstOpt, va_list opList));
#endif

int GtOpt P_((void));
double NextInList P_((LISTPOINTER *list));

#undef P_
#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* scale.c */
int MkScale P_((int firstOpt, ...));

#undef P_
#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* show.c */
void Pterminal P_((char *terminal));
void Pboxes P_((int xIn, int yIn));
void Pshow P_((int ixBox, int iyBox));
void Psend P_((char *device));
void Pupdate P_((void));
void Pabort P_((void));
FILE *PFile P_((void));
GCREAD Pcursor P_((void));

#undef P_
