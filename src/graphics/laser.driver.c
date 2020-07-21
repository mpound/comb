/*****************************
/* Postscript printer driver *
/* multiple-machine version  *
/*****************************/

/* 1/24/91 -mwp added space for 3rd and 4th printers, standardized
 * printer definitions.
 *
 * Wed May 26 11:23:13 EDT 1993 -- MWP changed laser, aaser etc to 
 * post1(), post2() and maser() to postfile()
 *
 * Tue Dec 27 14:14:41 EST 1994 -- landscape or portrait mode available -- MWP
 *				  (also changes to C.h include.h and hc.[ct])
 *			          Conforming postscript is written out even
 *				  to printer. This simplifies things.
 *
 * Tue Apr  2 15:34:13 PST 1996 -- MWP
 *   vectfn replaced by hcfile (see hc.c) in order to have user-specified
 *   output file. If output is going to a printer, then stuff gets 
 *   spooled to /tmp/comb.XXXXXX as usual.
 *
 * Thu Mar 12 10:47:55 EST 1998 -- mwp new routine FILE* PFile() in show.c
 *   used to get output file name.
 */
#include <ctype.h>
/* #define DEBUG	/*XX defined if we are */

# include <stdio.h>
# include <math.h>
# include <string.h>
# include "C.h"
# include "include.h"
# include "../main/C.h"

#define PHASESIZE	32

FILE * FSafeOpen (); 

static FILE * fout ;
static int cx = -20, cy = -20;
static int xyvalid = 0;
static int currX, currY;	/* current font size */
static int lwnmb;
static int needsStroke = 0;	/* number of lines which have been defined,
				 * but not stroked */

#define MAX_RGB_COLOR 1.0

static struct {
  double red;
  double green;
  double blue;
} pixel_map[NUM_PIXEL_COLORS]; /* rgb values of the colormap */

int orientation;         	/* Page Orientation: default is LANDSCAPE,
				   which is set in hc.c  */
#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* laser.driver.c */
static void setup P_((void));
static void cleanup P_((void));
static void setvalues P_((void));
static void move P_((int x, int y));
static void line P_((int x, int y));
static void linetype P_((register int type));
static void text P_((register char *str, int sideways));
static void texttype P_((int type));
static void center P_((char *str, int sideways));
static void dmy P_((void));
static void boxfill P_((int x, int y, short int color));
static void choose_colormap P_((int colormap_num));
#undef P_

/****************************************/
/* laser - similar to driver for imagen */
/****************************************/

#define PS_PRINTER1	1
#define PS_PRINTER2	2
#define PS_PRINTER3 	3
#define PS_PRINTER4 	4
#define PS_FILE		5

post1() {
    static char name[] = "post1";

    if (strcmp (termType, name)) {
	*termType = 0;
	lwnmb=PS_PRINTER1;
	setvalues ();
	strcpy (termType, name);
    }
}

post2() {
    static char name[] = "post2";

    if (strcmp (termType, name)) {
	*termType = 0;
	lwnmb=PS_PRINTER2;
	setvalues ();
	strcpy (termType, name);
    }
}

post3() {
    static char name[] = "post3";

    if (strcmp (termType, name)) {
	*termType = 0;
	lwnmb=PS_PRINTER3;
	setvalues ();
	strcpy (termType, name);
    }
}

post4() {
    static char name[] = "post4";

    if (strcmp (termType, name)) {
	*termType = 0;
	lwnmb=PS_PRINTER4;
	setvalues ();
	strcpy (termType, name);
    }
}

postfile() {
    static char name[] = "postfile";

    if (strcmp (termType, name)) {
	*termType = 0;
	lwnmb=PS_FILE;
	setvalues ();
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
 /* put out header
  * including:	coordinate system "Modified"
  *	setup for font families
  */
     sprintf(tempname, "%s/src/graphics/%s", main_.comb,
	(orientation == LANDSCAPE) ? "landscape.head": "portrait.head");
/*      (lwnmb == PS_FILE)? "eps.head": "laser.head");*/
/*   sprintf(tempname, "%s/src/graphics/laser.head", main_.comb); */
     fphd = FSafeOpen("Post Script Hdr", tempname, "r");
     while((cc=getc(fphd)) != EOF){
         putc(cc,fout);
     }
     FSafeClose(fphd);

 /* 0x0D: 00 Origin: Unchanged (40: Upper Left of NEW page)
  *	 08 Axes:	Invert H-V (0: unchanged)
  *	 05 Orient:	90 deg from phys. x axis (?)
  * 0x0D is GOOD : (0,0) is Lower Left
  */

 /* available fonts (built-in):
  *	cour07 cour10 cour08 cour12 cour14 zurm20
  */

/*    setvalues (); */
    choose_colormap(3);
    linetype (255);
    plotting = 1;
    needsStroke = 0;
    xyvalid = 0;
}

/***********************************************/
/* cleanup - clean up the graphics output file */
/***********************************************/

static void cleanup ()
{
    char    ts[128];

 /* Okay, so print the page already! */
    if(needsStroke) {
	fprintf(fout, "st\n");
	needsStroke = 0;
    }
    fprintf(fout,"showpage\n");
    plotting = 0;
    switch(lwnmb){
        case PS_PRINTER1:
        case PS_PRINTER2:
        case PS_PRINTER3:
        case PS_PRINTER4:
	    fprintf(fout,"%%%%Trailer\n");
	    fprintf(fout,"%%%%EOF\n");
    	    FSafeClose(fout);
	    sprintf(ts, "%s/lib/hc.lpr %s %s", main_.comb, termType,hcfile);
	    /* sprintf(ts,"lpr -Pqpost -h -r %s", hcfile); */
	    /* sprintf(ts,"prt -dqpost %s;rm %s", hcfile, hcfile); */
            eshell_(ts);
            break;
        case PS_FILE:
	    fprintf(fout,"%%%%Trailer\n");
	    fprintf(fout,"%%%%EOF\n");
            FSafeClose(fout);
            printf ("The PostScript plot file is %s\n", hcfile);
            break;
	default:
	    unlink(hcfile);
	    error("No such printer");
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
    xSize = 11.0933;
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
    d_boxfill = boxfill;

 /* get formats */
    GtSpec ("form.laser");
}

/**********************/
/* move - move cursor */
/**********************/

static void move (x, y)
int  x, y;
{
    if(needsStroke) {
	fprintf(fout, "st\n");
	needsStroke = 0;
    }
    fprintf(fout,"%d %d mt\n",x,y);
    cx = x;
    cy = y;
}

/********************/
/* line - draw line */
/********************/

static void line (x, y)
int  x, y;
{
    if(needsStroke > 1000) {
	move(cx, cy);
    }
    fprintf(fout,"%d %d lt\n",x,y);
    cx = x;
    cy = y;
    needsStroke++;
}

/***************************************************/
/* choose_colormap - set up the requested colormap */
/***************************************************/

static void choose_colormap(colormap_num)
     int colormap_num;
{
  static int old_colormap_num = -1;
  int i;
  double cutoff;
  
  if (old_colormap_num == colormap_num) return;
  
  cutoff = 3*NUM_PIXEL_COLORS/8;

  for (i=0 ; i<NUM_PIXEL_COLORS ; i++) {
    switch (colormap_num) {
    case 1: /* gray-scale */
      pixel_map[i].red   =
	pixel_map[i].green =
	pixel_map[i].blue  = i*((MAX_RGB_COLOR)/(NUM_PIXEL_COLORS-1));
      break;
    case 2: /* red */
      pixel_map[i].red   = i*((MAX_RGB_COLOR)/(NUM_PIXEL_COLORS-1));
      pixel_map[i].green =
	pixel_map[i].blue  = 0;
      break;
    case 3: /* hot */
      pixel_map[i].red=((i<cutoff) ? i/cutoff : 1)*MAX_RGB_COLOR;
      pixel_map[i].green=((i<cutoff) ? 0 : 
			  ((i<(2*cutoff)) ? (i-cutoff)/cutoff : 1))
	*MAX_RGB_COLOR;
      pixel_map[i].blue=((i<(2*cutoff)) ? 0 : (i-2*cutoff)/cutoff)*MAX_RGB_COLOR;
      break;
    case 0:
    default:
      pixel_map[i].red   =
	pixel_map[i].green =
	pixel_map[i].blue  = MAX_RGB_COLOR;
      break;
    }
  }  
}

/*************************************/
/* boxfill - create a filled box     */
/*************************************/

static void boxfill(x,y,color)
	int x,y;
	short int color;
{
  int ulx,uly,width,height;
  
  if(needsStroke)
    move(cx, cy);

  fprintf(fout,"%d %d lt %d %d lt %d %d lt %f %f %f bf\n",
	  x,cy,x,y,cx,y,
	  pixel_map[color].red,
	  pixel_map[color].green,
	  pixel_map[color].blue);
  move(x,y);
  
  cx=x;
  cy=y;
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

    if(type == oldtype)
	return;
    oldtype = type;

    if(needsStroke) {
	fprintf(fout, "st\n");
	needsStroke = 0;
    }
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

    switch((type % 10) / 2){
            case 0:
                fprintf(fout,"[2] 0 setdash\n");
                break;
            case 1:
                fprintf(fout,"[4] 0 setdash\n");
                break;
            case 2:
                fprintf(fout,"[.5 0 2 2 0] 0 setdash\n");
                break;
            case 3:
                fprintf(fout,"[6] 0 setdash\n");
                break;
	    case 4:
        	fprintf(fout,"[] 0 setdash\n");
    }

    type = (type / 10) % 10;		/* mask off 10's digit */
    /* @300 dpi use 1 to 5 dots line width */
    fprintf(fout,"%5.2f setlinewidth\n", (type + 2) * .12);
}

/********************/
/* text - draw text */
/********************/

static void text (str, sideways)
register char   *str;
int     sideways;
{

    if(needsStroke) {
	fprintf(fout, "st\n");
	needsStroke = 0;
    }
    if (sideways) {
        fprintf(fout,"90 rt\n");    /* 90 deg counterclockwise */
    }

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
}

/**************************************/
/* texttype - set the text type       */
/**************************************/

static void texttype (type)
int     type;
{
    static int oldtype = -999999;
    float txt_sz;
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

    fprintf(fout,"/Courier    %d  sf\n",currY);
 /* Inter-word space: 1 char width */
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
}

/****************************
 * dmy - do nothing routine *
 ****************************/

static void dmy () { }
