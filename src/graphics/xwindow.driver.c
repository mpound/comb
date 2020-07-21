/******************************
/* XWindow terminal driver    *
/*****************************/
/* History:
 *
 * Fri Aug 10 13:25:10 NZST 2001 -- CLM
 *   New driver for an XWindow based on Tek driver
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/cursorfont.h>
#include <ctype.h>

# include "C.h"
# include "include.h"
# include "../main/C.h"
# include "../main/machinedep.h"

#define ABS(a)          ((a) < 0 ? -(a) : (a))
#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#define MIN(a,b)        ((a) < (b) ? (a) : (b))
#define MAX_RGB_COLOR 65535

static Display *dpy = NULL;
static GC gc;
static XFontStruct *font;
static Window w=0;
static int screen;
static int cx = -20, cy = -20;
static int currX, currY;	/* current font size */
static XPoint *linepoints = NULL;
static int num_linepoints = 0;
static Colormap colormap;
static unsigned long pixel_map[NUM_PIXEL_COLORS];


#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* xwindow.driver.c */
static void setvalues P_((void));
static void setup P_((void));
static void cleanup P_((void));
static void move P_((int x, int y));
static void line P_((int x, int y));
static void linetype P_((register int type));
static void text P_((char *str, int sideways));
static void center P_((char *str, int sideways));
static void clear P_((int lx, int ly, int hx, int hy));
static void cursor P_((int *x, int *y, char *flag));
static void boxfill P_((int x, int y, short int color));
static void choose_colormap P_((int colormap_num));
static void dmy P_(( void));
static void purge_linepoints_buffer P_(( void));
#undef P_

xwindow() {
  static char name[] = "xwindow";
  if (strcmp (termType, name)) {
    *termType = 0;
    setvalues ();
    strcpy (termType, name);
  }
}

static void setvalues()
{
  /* set up variables */
  chX[0] = 10; chX[1] = 10; chX[2] = 10;
  chY[0] = 20; chY[1] = 20; chY[2] = 20;
  xRes = 1024;
  yRes = 780;
  xSize = 8;
  ySize = 6.09375;
  hasClear = 1;
  hasCursor = 0;
  
  /* set up routines */
  d_start = setup;
  d_cleanup = cleanup;
  d_move = move;
  d_line = line;
  d_setlin = linetype;
  d_text = text;
  d_ctext = center;
  d_settxt = dmy;
  d_clear = clear;
  d_cursor = cursor;
  d_boxfill = boxfill;
  
  /* get formats */
  GtSpec("form.xwindow");
}	

/********************************************/
/* setup - set up the terminal for graphics */
/********************************************/

static void setup()
{
  XSizeHints hints;
  XSetWindowAttributes attr;
  unsigned long mask = 0L;
  XGCValues gcvalues;
  unsigned long back, fore;

  if (dpy && w) {
    XRaiseWindow(dpy,w);
  } else {
    dpy = XOpenDisplay (NULL);
    if (!dpy) {
      fprintf (stderr, "COMB:  unable to open display '%s'\n",
	       XDisplayName (NULL));
      exit (1);
    }
    
    screen = DefaultScreen (dpy);
    
    /* select for all events */
    attr.event_mask = KeyPressMask | ButtonPressMask | StructureNotifyMask;
    
    hints.flags = PMinSize;
    hints.min_width = 1024;
    hints.min_height = 780;

    back = WhitePixel(dpy,screen);
    fore = BlackPixel(dpy,screen);
    
    attr.background_pixel = back;
    attr.border_pixel = fore;
    attr.backing_store = Always;
    attr.backing_planes = AllPlanes;
    attr.cursor =  XCreateFontCursor(dpy,XC_crosshair);
    mask |= (CWBackPixel | 
	     CWBorderPixel | 
	     CWEventMask | 
	     CWBackingStore |
	     CWBackingPlanes |
	     CWCursor);
    
    w = XCreateWindow (dpy, RootWindow (dpy, screen), 0, 0,
		       hints.min_width, hints.min_height, 2, 0,
		       InputOutput, (Visual *)CopyFromParent,
		       mask, &attr);
    
    XSetStandardProperties (dpy, w, "COMB graphics", NULL, (Pixmap) 0,
			    NULL, 0, &hints);
    
    XMapWindow (dpy, w);
    for(;;) {
      XEvent e;
      XNextEvent(dpy, &e);
      if (e.type == MapNotify)
	break;
    }
    
    gcvalues.line_width = 2;
    gcvalues.cap_style = CapRound;
    
    gc = XCreateGC(dpy,w,0,NULL);
    XSetForeground(dpy,gc,fore);
    XSetBackground(dpy,gc,back);
    XChangeGC(dpy,gc,
	      GCLineWidth |
	      GCCapStyle,
	      &gcvalues);
    
    font = XLoadQueryFont(dpy,"-*-fixed-*-r-*-*-20-*-*-*-*-*-*-*");
    XSetFont(dpy,gc,font->fid);

    colormap = XCreateColormap(dpy,w,DefaultVisual(dpy,screen),AllocNone);
    choose_colormap(3);
  }
  
  linetype(255);
  plotting = 1;
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
  XColor xcolor;
  
  if (old_colormap_num == colormap_num) return;

  if (old_colormap_num != -1)
    XFreeColors(dpy,colormap,pixel_map,NUM_PIXEL_COLORS,0);

  cutoff = 3*NUM_PIXEL_COLORS/8;

  for (i=0 ; i<NUM_PIXEL_COLORS ; i++) {
    switch (colormap_num) {
    case 1: /* gray-scale */
      xcolor.red   =
	xcolor.green =
	xcolor.blue  = i*((MAX_RGB_COLOR)/(NUM_PIXEL_COLORS-1));
      break;
    case 2: /* red */
      xcolor.red   = i*((MAX_RGB_COLOR)/(NUM_PIXEL_COLORS-1));
      xcolor.green =
	xcolor.blue  = 0;
      break;
    case 3: /* hot */
      xcolor.red=((i<cutoff) ? i/cutoff : 1)*MAX_RGB_COLOR;
      xcolor.green=((i<cutoff) ? 0 : 
			  ((i<(2*cutoff)) ? (i-cutoff)/cutoff : 1))
	*MAX_RGB_COLOR;
      xcolor.blue=((i<(2*cutoff)) ? 0 : (i-2*cutoff)/cutoff)*MAX_RGB_COLOR;
      break;
    case 0:
    default:
      xcolor.red   =
	xcolor.green =
	xcolor.blue  = MAX_RGB_COLOR;
      break;
    }
    XAllocColor(dpy,colormap,&xcolor);
    pixel_map[i] = xcolor.pixel;
  }
}

/***********************************/
/* cleanup - clean up the terminal */
/***********************************/

static void cleanup()
{
  /* XCloseDisplay (dpy); */
  purge_linepoints_buffer();
  XFlush(dpy);
  plotting = 0;
}

/**********************/
/* move - move cursor */
/**********************/

static void move(x,y)
	int x,y;
{
  purge_linepoints_buffer();
  cx = x;
  cy = y;
}

/********************/
/* line - draw line */
/********************/

static void line(x,y)
	int x,y;
{

  if (num_linepoints) {
    num_linepoints++;
    linepoints = (XPoint *) realloc(linepoints,num_linepoints*sizeof(XPoint));
    linepoints[num_linepoints-1].x=x;
    linepoints[num_linepoints-1].y=yRes-y;
  } else {
    num_linepoints=2;
    linepoints = (XPoint *) realloc(linepoints,num_linepoints*sizeof(XPoint));
    linepoints[0].x=cx;
    linepoints[0].y=yRes-cy;
    linepoints[1].x=x;
    linepoints[1].y=yRes-y;
  }
    
  cx = x;
  cy = y;

}

/**********************************************************************/
/* purge_linepoints_buffer - draw the lines which have been requested */
/**********************************************************************/

static void purge_linepoints_buffer()
{
  if (num_linepoints) {
    XDrawLines(dpy,w,gc,linepoints,num_linepoints,CoordModeOrigin);
    XFlush(dpy);
    num_linepoints=0;
    free(linepoints);
    linepoints=NULL;
  }
}

/****************************/
/* linetype - set line type */
/****************************/

static void linetype(type)
	register int type;
{
  static int oldtype = -1;
  char dash_pattern[3];
  int linestyle;
  int dash_pattern_num;
  
  purge_linepoints_buffer();

  if(type == oldtype)
    return;
  oldtype = type;

  switch (type) {
  case 255: 		/* reset pen width */
    type = 19;
    break;
  case BORDERlt: 	/* borders a bit wider   */
  case TICSlt: 		/* ...ditto for markings */
    type = 29;
    break;
  case GRIDlt: 		/* grids dotted */
    type = 10;
    break;
  case GRID0lt: 	/* also use array "grid" */
    type = 19;
    break;
  }

  type = type % 100;
  switch (type % 10) {
  case 10:
  case 0: /* most dotted */
    linestyle = LineOnOffDash;
    dash_pattern[0] = 1; dash_pattern[1] = 5;
    dash_pattern_num=2;
    break;
  case 1:
    linestyle = LineOnOffDash;
    dash_pattern[0] = 2; dash_pattern[1] = 5;
    dash_pattern_num=2;
    break;
  case 3:
    linestyle = LineOnOffDash;
    dash_pattern[0] = 2; dash_pattern[1] = 4;
    dash_pattern_num=2;
    break;
  case 4:
    linestyle = LineOnOffDash;
    dash_pattern[0] = 3; dash_pattern[1] = 4;
    dash_pattern_num=2;
    break;
  case 5:
    linestyle = LineOnOffDash;
    dash_pattern[0] = 3; dash_pattern[1] = 3;
    dash_pattern_num=2;
    break;
  case 6:
    linestyle = LineOnOffDash;
    dash_pattern[0] = 3; dash_pattern[1] = 2;
    dash_pattern_num=2;
    break;
  case 7:
    linestyle = LineOnOffDash;
    dash_pattern[0] = 4; dash_pattern[1] = 2;
    strcpy(dash_pattern,"24");
    dash_pattern_num=2;
    break;
  case 8:
    linestyle = LineOnOffDash;
    dash_pattern[0] = 5; dash_pattern[1] = 1;
    strcpy(dash_pattern,"15");
    dash_pattern_num=2;
    break;
  case 9: /* solid */
    linestyle = LineSolid;
    break;
  }
  
  type = (type / 10) % 10;		/* mask off 10's digit */
  /* Type now defines line thickness */
  XSetLineAttributes(dpy,gc,(type/2)+1,linestyle,CapButt,JoinBevel);
  if (linestyle != LineSolid) 
    XSetDashes(dpy,gc,0,dash_pattern,dash_pattern_num);

}

/********************/
/* text - draw text */
/********************/

static void text(str,sideways)
	char *str;
	int sideways;
{
  XCharStruct textextent;
  int direction_return,font_ascent_return,font_descent_return;
  Pixmap pixmap;
  XImage *ximage1,*ximage2;
  int x,y;
  unsigned long pixel;

  purge_linepoints_buffer();

  XTextExtents(font,str,strlen(str),
	       &direction_return,
	       &font_ascent_return,
	       &font_descent_return,
	       &textextent);

  if (sideways) {
    pixmap = XCreatePixmap(dpy,w,
			   textextent.width,
			   textextent.ascent+textextent.descent,
			   DefaultDepth(dpy,screen));
    XDrawImageString(dpy,pixmap,gc,
		     0,textextent.ascent,
		     str,strlen(str));
    ximage1 = XGetImage(dpy,pixmap,0,0,
			textextent.width,
			textextent.ascent+textextent.descent,
			AllPlanes,ZPixmap);
    ximage2 = XGetImage(dpy,w,0,0,
			textextent.ascent+textextent.descent,
			textextent.width,
			AllPlanes,ZPixmap);
    for (x=0 ; x<textextent.width ; x++)
      for (y=0 ; y<textextent.ascent+textextent.descent ; y++) {
	pixel = XGetPixel(ximage1,x,y);
	XPutPixel(ximage2,
		  y,
		  textextent.width-x-1,
		  pixel);
      }
    XPutImage(dpy,w,gc,ximage2,0,0,
	      cx,yRes-cy-textextent.width,
	      textextent.ascent+textextent.descent,
	      textextent.width);
    XDestroyImage(ximage2);
    XDestroyImage(ximage1);
    XFreePixmap(dpy,pixmap);
  } else {
    XDrawString(dpy,w,gc,cx,
		yRes-cy+(textextent.ascent/2),
		str,strlen(str));
  }
  XFlush(dpy);
}

/************************/
/* center - center text */
/************************/

static void center(str,sideways)
	char *str;
	int sideways;
{
  if (sideways) 
    cy -= strlen(str)*chX[0]/2;
  else
    cx -= strlen(str)*chX[0]/2;
  d_text(str,sideways);
}

/**************************************/
/* clear - clear a part of the screen */
/**************************************/

static void clear(lx,ly,hx,hy)
	int lx,ly,hx,hy;
{
  XClearArea(dpy,w,lx,yRes-hy,hx-lx,hy-ly,0);
}

/*************************************/
/* cursor - read the graphics cursor */
/*************************************/

static void cursor(x,y,flag)
	int *x,*y;
	char *flag;
{
}

/*************************************/
/* boxfill - create a filled box     */
/*************************************/

static void boxfill(x,y,color)
	int x,y;
	short int color;
{
  int ulx,uly,width,height;
  
  ulx=MIN(x,cx);
  width=ABS(x-cx)+1;
  height=ABS(y-cy)+1;
  uly=yRes-MAX(y,cy);

  XSetForeground(dpy,gc,pixel_map[color]);
  XFillRectangle(dpy,w,gc,ulx,uly,width,height);
  XSetForeground(dpy,gc,BlackPixel(dpy,screen));
  
  cx=x;
  cy=y;
}

/****************************
 * dmy - do nothing routine *
 ****************************/

static void dmy( void)
{
}

