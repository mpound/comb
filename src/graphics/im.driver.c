/* History:
 *  Wed May 26 11:43:43 EDT 1993 -- MWP chnaged imagen,jmagen, etc to 
 * imagen1, imagen2, imagen3, imagen4
 * 
 * Tue Apr  2 15:34:13 PST 1996 -- MWP
 *   vectfn replaced by hcfile (see hc.c) in order to have user-specified
 *   output file. If output is going to a printer, then stuff gets 
 *   spooled to /tmp/comb.XXXXXX as usual.
 */

/*************************
/* Imagen printer driver *
/* multiple-machine ver. *
/*************************/

/* #define DEBUG	/*XX defined if we are */

# include <stdio.h>
# include <math.h>
# include <string.h>
# include "C.h"
# include "include.h"
# include "../main/C.h"
# include "imPcodes.h"

#define PHASESIZE	32

FILE * FSafeOpen ();

static FILE * fout ;
static int  cx = -20, cy = -20;
static int  xyvalid = 0;
static int  currX, currY;	/* current font size */
static int  dotted, phase, on;
static char *dots;
	/* numbers must total to PHASESIZE! */
static char dottype[4][16] = {	/* Pixels: 'ON' is first */
    { 16, 16, 0 },
    { 16, 5, 6, 5, 0 },
    { 10, 4, 5, 4, 5, 4, 0 },
    { 6, 10, 6, 10, 0 },
};

#ifdef DEBUG
char    impHead[] =
        "@Document(language impress, jobheader on, prerasterization on, owner whb)";
#else
char    impHead[] = "@Document(language impress, jobheader off, prerasterization on)";
#endif

#define NEITHER	0
#define IMAGEN1	1
#define IMAGEN2	2
#define IMAGEN3	3
#define IMAGEN4	4

static int  imjim = NEITHER;

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* im.driver.c */
static void setup P_((void));
static void cleanup P_((void));
static void setvalues P_((void));
static void move P_((int x, int y));
static void line P_((int x, int y));
static void path P_((int x, int y));
static void linetype P_((register int type));
static void text P_((char *str, int sideways));
static void texttype P_((int type));
static void center P_((char *str, int sideways));
static void dmy P_((void));
#undef P_

/**********************************/
/* ORIGINALLY:			  */
/* imagen - use driver for imagen */
/*     (Original - in M-208)      */
/* NOW 5/26/93: imagen1();	  */
/**********************************/

imagen1 () {
    static char name[] = "imagen1";

    if (strcmp (termType, name)) {
	*termType = 0;
	imjim = IMAGEN1;
	setvalues ();
	strcpy (termType, name);
    }
}

/**********************************/
/* ORIGINALLY:			  */
/* jmagen - use driver for jmagen */
/*        (New - in R-201)        */
/* NOW 5/26/93: imagen2();	  */
/**********************************/

imagen2 () {
    static char name[] = "imagen2";

    if (strcmp (termType, name)) {
	*termType = 0;
	imjim = IMAGEN2;
	setvalues ();
	strcpy (termType, name);
    }
}

/**********************************/
/* ORIGINALLY:			  */
/* kmagen - use driver1for kmagen */
/*        (New - in L-162)        */
/* NOW 5/26/93: imagen3();	  */
/**********************************/

imagen3 () {
    static char name[] = "imagen3";

    if (strcmp (termType, name)) {
	*termType = 0;
	imjim = IMAGEN3;
	setvalues ();
	strcpy (termType, name);
    }
}

/**********************************/
/* ORIGINALLY:			  */
/* lmagen - use driver for lmagen */
/*        (New - in R-121)        */
/* NOW 5/26/93: imagen4();	  */
/**********************************/

imagen4 () {
    static char name[] = "imagen4";

    if (strcmp (termType, name)) {
	*termType = 0;
	imjim = IMAGEN4;
	setvalues ();
	strcpy (termType, name);
    }
}

/*******************************************/
/* setup - set up the graphics output file */
/*******************************************/

static void setup () {
    fflush(stdout);
    fout=PFile();
#ifdef DEBUG
    puts (hcfile);
#endif

 /* put out Impress header
  * including:	coordinate system "Modified"
  *	setup for font families
  */
    fputs (impHead, fout);

    putc (imP_SET_HV_SYSTEM, fout);
 /* 0x0D: 00 Origin: Unchanged (40: Upper Left of NEW page)
  *	 08 Axes:	Invert H-V (0: unchanged)
  *	 05 Orient:	90 deg from phys. x axis (?)
  * 0x0D is GOOD : (0,0) is Lower Left
  */
    putc (0x0D, fout);

 /* available fonts (built-in):
  *	cour07 cour10 cour08 cour12 cour14 zurm20
  */
    putc (imP_CREATE_FAMILY_TABLE, fout);
    putc (1, fout);		/* family: 1	 */
    putc (1, fout);		/* pairs:  1	 */
    putc (0, fout);		/* map_name: 0 (builtin) */
    fputs ("cour10", fout);	/* font_name		 */
    putc (0, fout);		/* ... null-terminated	 */

/* For imagen1 use big BOLD characters that JB likes*/
    if (imjim == IMAGEN1) {
    	putc (imP_CREATE_FAMILY_TABLE, fout);
    	putc (2, fout);		/* family: 2	 */
    	putc (1, fout);		/* pairs:  1	 */
    	putc (0, fout);		/* map_name: 0 (builtin) */
    	fputs ("zurm20", fout);	/* font_name		 */
   	putc (0, fout);		/* ... null-terminated	 */
    } else {
	putc (imP_CREATE_FAMILY_TABLE, fout);
	putc (2, fout);		/* family: 2	 */
	putc (1, fout);		/* pairs:  1	 */
	putc (0, fout);		/* map_name: 0 (builtin) */
	fputs ("cour12", fout);	/* font_name		 */
	putc (0, fout);		/* ... null-terminated	 */
    }

/* For imagen1, use big BOLD characters that JB likes*/
    if (imjim == IMAGEN1) {
    	putc (imP_CREATE_FAMILY_TABLE, fout);
    	putc (3, fout);		/* family: 3	 */
    	putc (1, fout);		/* pairs:  1	 */
   	putc (0, fout);		/* map_name: 0 (builtin) */
    	fputs ("zurm20", fout);	/* font_name: was zurm20 */
    	putc (0, fout);		/* ... null-terminated	 */
    } else {
	putc (imP_CREATE_FAMILY_TABLE, fout);
	putc (3, fout);		/* family: 3	 */
	putc (1, fout);		/* pairs:  1	 */
	putc (0, fout);		/* map_name: 0 (builtin) */
	fputs ("cour14", fout);	/* font_name:            */
	putc (0, fout);		/* ... null-terminated	 */
    }

    setvalues ();
    linetype (255);
    plotting = 1;
    xyvalid = 0;
}

/***********************************************/
/* cleanup - clean up the graphics output file */
/***********************************************/

static void cleanup()
{
    char    ts[200];

 /* Okay, so print the page already! */
    putc (imP_ENDPAGE, fout);
    FSafeClose (fout);
    plotting = 0;

    sprintf(ts, "%s/lib/hc.lpr %s %s", main_.comb, termType, hcfile);
    /* sprintf(ts, "/usr/ucb/lpr -P%cmagen -r %s", 'i' + imjim - 1, hcfile); */
    eshell_(ts);
    *termType = 0;
}

#if 0
static void cleanup () {
    char    ts[128];
    char   *jipr = "jpr";	/* default to JMAGEN */

    *termType = 0;

 /* Okay, so print the page already! */
    putc (imP_ENDPAGE, fout);
    FSafeClose (fout);
    plotting = 0;

    switch (imjim) {
	case IMAGEN:
	    jipr = "ipr";
	    break;
	case JMAGEN:
	    jipr = "jpr";
	    break;
	case KMAGEN:
	    jipr = "kpr";
	    break;
	case LMAGEN:
	    jipr = "lpr";
	    break;
    }

#ifdef DEBUG
    sprintf (ts, "<%s uux - -n \"hoh-1!%s\"", hcfile, jipr);
    puts (ts);
#else
    sprintf (ts, "<%s uux - -n \"hoh-1!%s\" ; rm %s", hcfile, jipr, hcfile);
#endif
    system (ts);
}
#endif /*0*/

static void setvalues ()
{
 /* set up variables */
 /* cour10		cour12		cour14 */
    chX[0] = 25;	chX[1] = 30;	chX[2] = 35;
    chY[0] = 30;	chY[1] = 36;	chY[2] = 43;
 /* FYI: cour07: 17/21  cour08: 20/25 zurm20: 56/80 (or 30/80) */
 /* XX	zurm20 is NOT fixed-width! */

 /* For JB's favorite BIG font on imagen1       */
    if(imjim==IMAGEN1){
 	/* cour10		zurm20		zurm20 */
    	chX[0] = 25;	chX[1] = 50;	chX[2] = 45;
    	chY[0] = 30;	chY[1] = 80;	chY[2] = 60;
    }

    xRes = 3328;
    yRes = 2400;
    /* xRes / yRes must equal xSize / ySize for square plots to work */
    xSize = 11.09333;
    ySize = 8.0;
    hasClear = 0;
    hasCursor = 0;

 /* set up routines */
    d_start = setup;
    d_clear = dmy;
    d_cleanup = cleanup;
    d_move = move;
    d_line = line;
    d_setlin = linetype;
    d_text = text;
    d_ctext = center;
    d_settxt = texttype;
    d_cursor = dmy;
    d_boxfill = dmy;

 /* get formats */
    GtSpec ("form.im");
}

/**********************/
/* move - move cursor */
/**********************/

static void move (x, y)
int  x, y;
{

    putc (imP_SET_ABS_H, fout);	/* set-current-pos: x */
    putwImp ((int) x, fout);
    putc (imP_SET_ABS_V, fout);	/* set-current-pos: y */
    putwImp ((int) y, fout);

    cx = x;
    cy = y;
}

/********************/
/* line - draw line */
/********************/

static void line (x, y)
int  x, y;
{
 /* XX still under construction */
    if (dots != NULL) {
	char   *dptr;
	int     ccnt, dx, dy, origx, origy;
	int nx, ny;
	double  vectlen, vdone;

	dx = x - cx;
	dy = y - cy;
	vectlen = sqrt ((double) (dx * dx) + (double) (dy * dy));
	vdone = 0;
	origx = cx;
	origy = cy;

	    if (phase)	/*XX experimental */
		on = !on;
	dptr = dots;
	do {
	    ccnt = 0;
	    while ((ccnt + *dptr) <= phase) {
		ccnt += *dptr++;
		if (*dptr == '\0')
		    dptr = dots;
	    }
	    ccnt = *dptr + ccnt - phase;
	    phase += ccnt;
	    if (phase >= PHASESIZE) {
		phase = 0;
	    }
	    vdone += (double) ccnt;
	    if (vdone > vectlen) {
		nx = x;
		ny = y;
		phase = (phase + (int) (vdone - vectlen) + 16) % 16;
	    }
	/* XX is there a need, for the following cast, to first
	 * add/subtract 0.5?
	 */
	    else {
		nx = (origx + (int) (((double) dx * vdone) / vectlen));
		ny = (origy + (int) (((double) dy * vdone) / vectlen));
	    }
	    if (on)
		path (nx, ny);
	    else
		move (nx, ny);
	    /*XX if (vdone <= vectlen) */
		on = !on;
	} while (vdone < vectlen);
    }
    else
	path (x, y);
}

static void path (x, y)
int  x, y;
{
 /* Create_Path, 2 points: (cx,cy), (x,y), Draw_Path */
    putc (imP_CREATE_PATH, fout);
    putwImp (2, fout);
    putwImp (cx, fout);
    putwImp (cy, fout);
    putwImp ((int) x, fout);
    putwImp ((int) y, fout);
    putc (imP_DRAW_PATH, fout);
    putc (15, fout);		/* draw a black line */

    cx = x;
    cy = y;
}

/****************************/
/* linetype - set line type */
/****************************/

/*XX	For nos. 100 to 199:
 * Units: Dottedness: smaller = more space
 * Tens:  Width: bigger = wider
 */

static void linetype (type)
register int    type;
{
    switch (type) {
	case 255: 		/* reset pen width */
	    type = 19;
	    break;
	case BORDERlt: 		/* borders a bit wider   */
	case TICSlt: 		/* ...ditto for markings */
	    type = 29;
	    break;
	case GRIDlt: 		/* grids dotted */
	    type = 10;
	    break;
	case GRID0lt: 		/* also use array "grid" */
	    type = 19;
	    break;
    }

    if ((dotted = (type % 10) / 2) != 4) {
	dots = dottype[dotted];
	phase = 0;
	on = 1;
    }
    else
	dots = NULL;

    type %= 100;		/* mask off 10's digit */
    type /= 10;
    type = (type / 2) + 1;

    putc (imP_SET_PEN, fout);
    putc (type, fout);
}

/********************/
/* text - draw text */
/********************/

static void text (str, sideways)
char   *str;
int     sideways;
{
    register char  *tstr = str;

    if (sideways) {
	putc (imP_SET_ADV_DIRS, fout);	/* change to "going up" */
	putc (6, fout);			/* 270 deg clockwise */
    }

 /* adjust char position:
  *	Comb origin is middle of left edge
  * Impress origin is lower left corner
  *XX Is this right for sideways?
  */
    move (cx, (cy - (currY / 2)));

 /* Impress, in it's wisdom, does not use ASCII 0x20 for 'space', so */
 /* 	change the spaces into Impress spaces (oh, brother)	     */
    do {
	if (*tstr == ' ')
	    putc (imP_SP, fout);
	else
	    putc (*tstr, fout);
    } while (*++tstr);

    if (sideways) {
	putc (imP_SET_ADV_DIRS, fout);	/* change to normal */
	putc (0, fout);
    }

    xyvalid = 0;
}

/**************************************/
/* texttype - set the text type       */
/**************************************/

static void texttype (type)
int     type;
{
 /* type is 0,1,2 (1 : 1.4 : 2) */
 /* same typeface, different sizes */
    if (type > 2 || type < 0)
	type = 2;

 /* Families are numbered 1, 2, 3 */
    putc (imP_SET_FAMILY, fout);
    putc (type + 1, fout);

    currX = chX[type];
    currY = chY[type];

 /* Inter-word space: 1 char width */
    putc (imP_SET_SP, fout);
    putwImp (currX, fout);
}

/************************/
/* center - center text */
/************************/

static void center (str, sideways)
char   *str;
int     sideways;
{
 /* Good thing we're using a fixed-width font */
 /* XX Of course, sideways text needs to be moved in the Y axis! */
    if (sideways)
	cy -= strlen (str) * currX / 2;
    else
	cx -= strlen (str) * currX / 2;
    text (str, sideways);
}

/****************************
 * dmy - do nothing routine *
 ****************************/

static void dmy () {}

/**************************************************
 * putwImp : put signed 16-bit quantity to imagen *
 **************************************************/

putwImp (word, fp)
int     word;
FILE * fp;
{
 /* word is in range (-16384 to 16383)
  * imPress puts high-byte first
  */
    putc ((word >> 8) & 0xFF, fp);
    putc (word & 0xFF, fp);
}
