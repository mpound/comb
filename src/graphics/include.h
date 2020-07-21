/*************/
/* Constants */
/*************/

# define MCURSOR 0		/* move cursor instruction */
# define DLINE 4		/* draw line instruction */
# define PTEXT 8		/* place text instruction */
# define DTEXT 9		/* draw text instruction */
# define DGRNUM 10		/* draw graphics number instruction */
# define DXLABEL 11		/* draw a numeric x label */
# define DYLABEL 12		/* draw a numeric y label */
# define SETWINDOW 13		/* set the window to be used */
# define SETLTYPE 14		/* set the line type to be used */
# define SETTTYPE 15		/* set the text type to be used */
# define CTEXT 16		/* center text */
# define LOVRFLOW 17		/* indicate an overflow from the graph */
				/* window on the left */
# define BOVRFLOW 18		/* indicate an overflow on the bottom */
# define ROVRFLOW 19		/* indicate an overflow on the right */
# define TOVRFLOW 20		/* indicate an overflow on the top */
# define LABELFMT 21		/* indicate format for x & y axis labels */

# define DBOXFILL 22            /* draw a filled box */

# define PICBLOCK 4096		/* number of bytes that intermediate code */
				/* buffer is incremented by when it */
				/* overflows */
# define MAXFORMS 10		/* maximum number of different formats */
# define MAXOPTS 40		/* maximum number of high level options */
# define AVGARGS 3		/* maximum average number of option */
				/* arguments */
# define T_DOUBLE 0		/* data type double */
# define T_FLOAT 1		/* data type float */
# define T_INT 2		/* data type int */

#define FRESHSTART 0		/* pen plotter should restart pen cycle */
#define NEWPASS 1		/* pen plotter prepare for next pen */
#define PLOTTING 2		/* pen plotter is in the middle of a pass */
#define MOREPENS 3		/* pen plotter will need another pass */

#define NUM_PIXEL_COLORS 500    /* number of possible boxfill pixel colors */

/*******************/
/* Data structures */
/*******************/

typedef struct			/* move cursor arguments */
{
	unsigned short int x;		/* x location to move to */
	unsigned short int y;		/* y location to move to */
}
MC_ARGS;

typedef struct			/* draw line arguments */
{
	unsigned short int x;		/* x location to draw to */
	unsigned short int y;		/* y location to draw to */
}
DL_ARGS;

typedef struct			/* place text arguments */
{
	unsigned char place;		/* place number */
}
PT_ARGS;

typedef struct			/* draw text arguments */
{
	char sideways;			/* 1 if text is to be sideways */
}
DT_ARGS;

typedef struct			/* draw graphics number arguments */
{
	unsigned short int x;		/* x location to draw number at */
	unsigned short int y;		/* y location to draw number at */
	float num;			/* number to be drawn */
}
DG_ARGS;

typedef struct			/* draw x label arguments */
{
	float num;			/* number to draw there */
	unsigned short int x;		/* x location to draw label at */
}
DX_ARGS;

typedef struct			/* draw y label arguments */
{
	float num;			/* number to draw there */
	unsigned short int y;		/* y location to draw label at */
}
DY_ARGS;

typedef struct			/* set window arguments */
{
	unsigned char window;		/* window to set to */
}
SW_ARGS;

typedef struct			/* set line type arguments */
{
	unsigned char type;		/* line type */
}
SL_ARGS;

typedef struct			/* set text type arguments */
{
	unsigned char type;		/* text type */
}
ST_ARGS;

typedef struct			/* center text arguments */
{
	char sideways;			/* 1 if text is to be sideways */
}
CT_ARGS;

typedef struct			/* left overflow arguments */
{
	unsigned short int from;	/* start of overflow */
	unsigned short int to;		/* end of overflow */
}
LO_ARGS;

typedef struct			/* bottom overflow arguments */
{
	unsigned short int from;	/* start of overflow */
	unsigned short int to;		/* end of overflow */
}
BO_ARGS;

typedef struct			/* right overflow arguments */
{
	unsigned short int from;	/* start of overflow */
	unsigned short int to;		/* end of overflow */
}
RO_ARGS;

typedef struct			/* top overflow arguments */
{
	unsigned short int from;	/* start of overflow */
	unsigned short int to;		/* end of overflow */
}
TO_ARGS;

typedef struct {
	char htype, vtype;	/* Type of formats ie. g, f, or hms*/
	short int hfmt, vfmt;	/* control for decimal points */
} 
LF_ARGS;

typedef struct			/* box fill arguments */
{
  unsigned short int x;		/* x location to draw to */
  unsigned short int y;		/* y location to draw to */
  short int color;               /* gray scale shade of box */
}
BF_ARGS;

typedef union			/* all possible arguments */
{
	MC_ARGS mc;
	DL_ARGS dl;
	PT_ARGS pt;
	DT_ARGS dt;
	DG_ARGS dg;
	DX_ARGS dx;
	DY_ARGS dy;
	SW_ARGS sw;
	SL_ARGS sl;
	ST_ARGS st;
	CT_ARGS ct;
	LO_ARGS lo;
	BO_ARGS bo;
	RO_ARGS ro;
	TO_ARGS to;
	LF_ARGS lf;
        BF_ARGS bf;
}
ALLARGS;

typedef union			/* pointer to any argument */
{
	MC_ARGS *mc;
	DL_ARGS *dl;
	PT_ARGS *pt;
	DT_ARGS *dt;
	DG_ARGS *dg;
	DX_ARGS *dx;
	DY_ARGS *dy;
	SW_ARGS *sw;
	SL_ARGS *sl;
	ST_ARGS *st;
	CT_ARGS *ct;
	LO_ARGS *lo;
	BO_ARGS *bo;
	RO_ARGS *ro;
	TO_ARGS *to;
	LF_ARGS *lf;
	char *chr;			/* added for accessing strings */
					/* in intermediate code buffer */
}
PALLARGS;			/* NOTE: PALLARGS is different from */
				/* *ALLARGS on machines that enforce */
				/* various kinds of boundaries for their */
				/* data.  if short integers have to be */
				/* on two byte boundaries and characters */
				/* don't, for example, *ALLARGS must be */
				/* on a two byte boundary (because some */
				/* structures start with short integers) */
				/* while PALLARGS does not (unless the */
				/* field being used points to a structure */
				/* that starts with a short integer). */

/********************/
/* Global variables */
/********************/

extern PICTURE *cPic;		/* current picture */
extern PICTURE *oPic;		/* old picture (scratch memory) */
extern short int numForms;	/* number of formats */
extern char plotting;		/* flag set while plotting */
extern int xBoxes;		/* number of boxes in x */
extern int yBoxes;		/* number of boxes in y */
extern PICTURE **boxes;		/* array of pointers to pictures in boxes */
extern char defTerm[TERMNMLEN];	/* name of default terminal type */
extern char fDirName[DIRNMLEN]; /* name of directory from which to get */
				/* formats */
extern char termType[TERMNMLEN]; /* name of terminal for which following */
				/* global variables apply */
extern short int chX[TXTTYPES]; /* x size of characters (size of a */
				/* space character for devices with */
				/* proportional spacing) */
extern short int chY[TXTTYPES]; /* y size of characters */
extern int xRes;		/* x resolution of screen */
extern int yRes;		/* y resolution of screen */
extern float xSize;		/* x size of screen (in inches) */
extern float ySize;		/* y size of screen (in inches) */
extern void (*d_start)();	/* driver routine for starting graphics */
extern void (*d_move)();	/* driver routine for moving cursor */
extern void (*d_line)();	/* driver routine for drawing line */
extern void (*d_text)();	/* driver routine for drawing text */
extern void (*d_ctext)();	/* driver routine for drawing centered text */
extern void (*d_setlin)();	/* driver routine for setting line type */
extern void (*d_settxt)();	/* driver routine for setting text type */
extern void (*d_cleanup)();	/* driver routine for cleaning up terminal */
				/* when done plotting */
extern void (*d_clear)();	/* driver routine for clearing part of the */
				/* screen */
extern char hasClear;		/* flag for whether driver has a clear */
				/* routine (d_clear) */
extern void (*d_cursor)();	/* driver routine for reading cursor */
extern char hasCursor;		/* flag for whether driver has a cursor */
				/* routine (d_cursor) */
extern void (*d_boxfill)();     /* driver routine for drawing filled boxes */
extern GRFORMAT formats[MAXFORMS]; /* catalog of formats */
extern ANYTYPE *optArg;		/* option arguments */
extern int ppState;		/* Used to coordinate multiple passes through
				the intermediate code for the pen plotter */
extern int ccCarousel;		/* Which carousel of pens is mounted on pp */
extern int orientation;         /* Orientation for hardcopy: LANDSCAPE (default)
				   or PORTRAIT. Currently, postscript only. 
				 	-- MWP Tue Dec 27 13:11:56 EST 1994
				 */
/* Orientation types. Currently only used for PostScript hardcopy,
 * but eventually other printer types should have it
 *    -- MWP Tue Dec 27 13:13:45 EST 1994
 */
# define LANDSCAPE 0
# define PORTRAIT 1
extern char hcfile[DIRNMLEN + 10]; /* hardcopy file. For hc.c to optionally
				    * pass a file name to *.driver.c */
