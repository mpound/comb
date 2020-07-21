/********************************************************
 * PCL5 printer driver for HP LaserJets (III and later) *
 ********************************************************/
/* History -- original version - RWW
 *
 * Tue Apr  2 15:34:13 PST 1996 -- MWP
 *   vectfn replaced by hcfile (see hc.c) in order to have user-specified
 *   output file. If output is going to a printer, then stuff gets 
 *   spooled to /tmp/comb.XXXXXX as usual.
 */


#include <ctype.h>
/* #define DEBUG	/*XX defined if we are */

# include <stdio.h>
# include <math.h>
# include <string.h>
# include "C.h"
# include "include.h"
# include "../main/C.h"

#define bzero(s, n) memset(s, 0, n)

FILE * FSafeOpen ();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* lj.driver.c */
static void setup P_((void));
static void cleanup P_((void));
static void setvalues P_((void));
static void move P_((int x, int y));
static void line P_((int x, int y));
static void linetype P_((register int type));
static void text P_((register char *str, int sideways));
static void texttype P_((int type));
static void center P_((char *str, int sideways));
static void TxtAngle P_((int sideways));
static void dmy P_((void));
#undef P_

static FILE * fout ;
static int cx = -20, cy = -20;	/* current x and y */
static int xyvalid = 0;
static int currX, currY;	/* current font size */
static int inLine = 0;		/* True if drawing a polyline */
static char lineTypeSet[9] = {0,0,0,0,0,0,0,0,0};
static int printer;

/*************************************************
 * Laser Jet PCL5 - similar to postscript driver *
 *************************************************/

#define LJ_PRINTER1	1
#define LJ_PRINTER2	2
#define LJ_PRINTER3 	3
#define LJ_PRINTER4 	4
#define PCL5_FILE	5
#define HPGL_PLOT	6

lj1() {
    static char name[] = "lj1";

    if (strcmp (termType, name)) {
	*termType = 0;
	setvalues ();
	printer = LJ_PRINTER1;
	strcpy (termType, name);
    }
}

lj2() {
    static char name[] = "lj2";

    if (strcmp (termType, name)) {
	*termType = 0;
	setvalues ();
	printer = LJ_PRINTER2;
	strcpy (termType, name);
    }
}

ljfile() {
    static char name[] = "ljfile";

    if (strcmp (termType, name)) {
	*termType = 0;
	setvalues ();
	printer = PCL5_FILE;
	strcpy (termType, name);
    }
}

hpglplot() {
    static char name[] = "hpglplot";

    if (strcmp (termType, name)) {
	*termType = 0;
	setvalues ();
	printer = HPGL_PLOT;
	strcpy (termType, name);
    }
}

/*******************************************/
/* setup - set up the graphics output file */
/*******************************************/

static void setup ()
{
    int    cc;
    FILE   *fphd;
    char tempname[DIRNMLEN + 10];

    fflush(stdout);
    fout=PFile();

    if( printer == HPGL_PLOT) {
	fprintf(fout, "INSP1RO90SC0,3328,0,2400,1WU1");
    } else {
/*  put out setup commands
		      reset,Landscape,PCLmod,Scale Isotropic,Rel Line Width */
	fprintf(fout, "\033E\033&l1O\033%%1BSP1SC0,3328,0,2400,1WU1");
    }
    bzero(lineTypeSet, sizeof(lineTypeSet));
    linetype (255);
    inLine = 0;
    plotting = 1;
}

/***********************************************/
/* cleanup - clean up the graphics output file */
/***********************************************/

static void cleanup ()
{
    char    ts[128];

    /* Okay, so print the page already! */
    if( printer != HPGL_PLOT) {
/*		      To PCL5, Eject page and reset */
	fprintf(fout,"\033%%0A\033E");
    }
    FSafeClose(fout);
    plotting = 0;
    if(printer == PCL5_FILE) {
	printf ("The LaserJet plot file is %s\n", hcfile);
    } else {
	sprintf(ts, "%s/lib/hc.lpr %s %s", main_.comb, termType,hcfile);
	eshell_(ts);
    }
    *termType = 0;
}

static void setvalues ()
{
 /* set up variables */
 /* cour10		cour12		cour14 */
    chX[0] = 25;	chX[1] = 30;	chX[2] = 35;
    chY[0] = 30;	chY[1] = 36;	chY[2] = 43;
 /* FYI: cour07: 17/21  cour08: 20/25 zurm20: 56/80 (or 30/80) */
 /* XX	zurm20 is NOT fixed-width! */


    xRes = 3328;
    yRes = 2400;
    xSize = 10.6;
    ySize = 7.5;
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
    GtSpec ("form.laser");
}

/**********************/
/* move - move cursor */
/**********************/

static void move (x, y)
int  x, y;
{
    fprintf(fout,"PU%d,%d",x,y);
    inLine = 0;
    cx = x;
    cy = y;
}

/********************/
/* line - draw line */
/********************/

static void line (x, y)
int  x, y;
{
    if(inLine)
	fprintf(fout,",%d,%d",x,y);
    else {
	fprintf(fout,"PD%d,%d",x,y);
	inLine = 1;
    }
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
    static oldtype = -99999;
    int width, dotted;

    if(type == oldtype)
	return;
    oldtype = type;

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

    dotted = 1 + type % 10;
    width = (type / 10) % 10;
    if(dotted < 8) {
	if(dotted == 1) {
	    fprintf(fout, "UL1,20,80LT1,.4");
	} else {
	    fprintf(fout, "LT%d,%s", dotted, (dotted < 4)? ".5": "1");
	}
	fprintf(fout, "PW%.2f", (2+width) * 0.012835);
    } else
	fprintf(fout, "PW%.2fLT", (2+width) * 0.012835);
    inLine = 0;
}

/********************/
/* text - draw text */
/********************/

static void text (str, sideways)
register char   *str;
int     sideways;
{
    TxtAngle(sideways);
    fprintf(fout,"LO2LB%s\003", str);
    inLine = 0;

#if 0
/* adjust char position:
 *	Comb origin is middle of left edge
 *	Postscript origin is lower left corner
 */

    if(strlen(str) == 1) {
        int xOff = -currX * 3 / 8;
 
        if(islower(*str))
            fprintf(fout,"%d %d rm\n", xOff, -currY * 9 / 43);
        else if(*str == '*')
            fprintf(fout,"%d %d rm\n", xOff, -currY * 13 / 32);
        else if(*str == '.')
            fprintf(fout,"%d %d rm\n", xOff, -currY / 20);
        else    /* This is correct for upper case and some others  eg. + */
            fprintf(fout,"%d %d rm\n", xOff, -currY * 9 / 32);
    } else
        fprintf(fout,"%d %d rm\n", 0, -currY / 4);
    fprintf(fout,"(%s) sh\n",str);
    if (sideways) {
        fprintf(fout,"-90 rt\n");    /* 90 deg clockwise */
    }

    xyvalid = 0;
#endif /*0*/
}

/**************************************/
/* texttype - set the text type       */
/**************************************/

static void texttype (type)
int     type;
{
    static int oldtype = -999999;
    static struct {
	float x, y;
    } txtsize[3] = {{.58,1.11},{.69,1.36},{.82,1.62}};

 /* type is 0,1,2 (1 : 1.4 : 2) */
 /* same typeface, different sizes */
    if (type > 2 || type < 0)
	type = 2;
    if(type == oldtype)
	return;
    oldtype = type;

 /* Families are numbered 1, 2, 3 */

    currX = chX[type]*1.5;
    currY = chY[type]*1.5;

    fprintf(fout,"SR%.2f,%.2f", txtsize[type].x, txtsize[type].y);
    inLine = 0;
}

/************************/
/* center - center text */
/************************/

static void center (str, sideways)
char   *str;
int     sideways;
{
    TxtAngle(sideways);
    fprintf(fout,"LO5LB%s\003", str);
    inLine = 0;

#if 0
 /* Good thing we're using a fixed-width font */
 /* XX Of course, sideways text needs to be moved in the Y axis! */
    if(needsStroke) {
	fprintf(fout, "st\n");
	needsStroke = 0;
    }
    cx = cy = -20;
    if (sideways){
	cy -= strlen (str) * currX / 3;
        fprintf(fout,"%d %d rm\n",cx,cy);
        fprintf(fout,"90 rt\n");    /* 90 deg counterclockwise */
        fprintf(fout,"(%s) sh\n",str);
        fprintf(fout,"-90 rt\n");     /* 90 deg clockwise */
    } else {
	cx -= strlen (str) * currX / 3;
        fprintf(fout,"%d %d rm\n",cx,cy);
        fprintf(fout,"(%s) sh\n",str);
    }
    /*printf("center %s, %d, %d, %d\n",str,sideways,cx,cy);*/
#endif /*0*/
}

static void TxtAngle(sideways)
{
    static int txtIsSideways = 0;

    if (sideways) {
	if(!txtIsSideways) {
            fprintf(fout,"DR0,1");    /* 90 deg counterclockwise */
	    txtIsSideways = sideways;
	}
    } else {
	if(txtIsSideways) {
            fprintf(fout,"DR1,0");    /* Normal */
	    txtIsSideways = sideways;
	}
    }
}

/****************************
 * dmy - do nothing routine *
 ****************************/

static void dmy ()
{
}
