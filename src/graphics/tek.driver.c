/******************************
/* tektronix terminal drivers *
/*****************************/
/* History:
 *
 * Tue Apr  2 15:34:13 PST 1996 -- MWP
 *   vectfn replaced by hcfile (see hc.c) in order to have user-specified
 *   output file. If output is going to a printer, then stuff gets 
 *   spooled to /tmp/comb.XXXXXX as usual.
 */

# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include "C.h"
# include "include.h"
# include "../main/C.h"
# include "../main/machinedep.h"

# define ESC 27			/* ascii code for escape character */

static FILE *tout , *tin ;
static FILE *savetout;
FILE *FSafeOpen(), *FSafeClose();
static int cx = -20,cy = -20;
static int needsmove = 1;
static int xyvalid = 0;

/* The fflush associated with these patches seems to fix an illusive bug
 * which shows up in Solaris 2.x in which the \037 preceeding a text
 * write gets lost occasionally when writing to an xterm through a pty.
 */
#if SUN_OS && SYSTEM_V
static int iocount = 0;
#endif /* SUN_OS && SYSTEM_V */

#define NORMAL 0
#define XTERM 1
static int tType = 0;

/* these chars used by tekfile() driver -- MWP 2/93 */
/* reworked for tekplot() and tekfile() to eliminate
 * need for #if KITTPEAK -- Mon Mar 14 14:52:19 EST 1994 -- MWP
 */
static int ItsATekFile=0;
static int ItsATekPrinter=0;
static char *startqms = {
#include "startqms.h"
};
static char *endqms = {
#include "endqms.h"
}; 

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* tek.driver.c */
static void setvalues P_((void));
static void setup P_((void));
static void cleanup P_((void));
static void move P_((int x, int y));
static void line P_((int x, int y));
static void linetype P_((register int type));
static void text P_((char *str, int sideways));
static void t4112text P_((char *str, int sideways));
static void center P_((char *str, int sideways));
static void clear P_((int lx, int ly, int hx, int hy));
static void t4112clear P_((int lx, int ly, int hx, int hy));
static void t4112num P_((int n));
static void cursor P_((int *x, int *y, char *flag));
static void xy P_((register int x, register int y));
static void dmy P_(( void));
#undef P_

/*******************************/
/* output to a tektronix file  */
/*******************************/
void tekfile()
{
	static char name[] = "tekfile";
	if(strcmp (termType,name)) {
	   ItsATekFile=1;
	  *termType=0;
	   setvalues();
	   strcpy(termType,name);
	}
}
/****************************************************/
/* Tektronix printer --similar to above             */
/****************************************************/
void tekplot()
{
	static char name[] = "tekplot";
	if(strcmp (termType,name)) {
	   ItsATekPrinter=1;
	   *termType=0;
	   setvalues();
	   strcpy(termType,name);
	}
}
/**********************************/
/* vt100 - use driver for tek401x */
/**********************************/

vt100()
{
	static char name[] = "vt100";
	/* if not set up to drive vt100, set it up */
	if(strcmp(termType,name))
	{
		/* in case something happens while setting up */
		/* driver variables, make terminal illegal */
		*termType = 0;
		setvalues();
		tout = stdout;
		tin = stdin;

		/* indicate that driver is set for vt100 */
		strcpy(termType,name);
	}
}

/************************************/
/* xterm - use driver for tek401x */
/************************************/

xterm()
{
	static char name[] = "xterm";

	/* if not set up to drive xterm, set it up */
	if(strcmp(termType,name))
	{
		/* in case something happens while setting up */
		/* driver variables, make terminal illegal */
		*termType = 0;
		setvalues();
		tType = XTERM;
		tout = stdout;
		tin = stdin;

		/* indicate that driver is set for xterm */
		strcpy(termType,name);
	}
}

#if 0
/********************************************/
/* XtermSetup - set up the terminal for graphics */
/********************************************/

static void XtermSetup()
{
	fputs("\033[?38h",tout);
	linetype(255);
	plotting = 1;
	xyvalid = 0;
}

/***********************************/
/* XtermCleanup - clean up the terminal */
/***********************************/

static void XtermCleanup()
{
	fputs("\037\030\033\003", tout);
	fflush(tout);
	plotting = 0;
}
#endif /*0*/


/************************************/
/* tek4112 - use driver for tek401x */
/************************************/

void tek4112()
{
	static char name[] = "tek4112";

	/* if not set up to drive tek4112, set it up */
	if(strcmp(termType,name))
	{
		/* in case something happens while setting up */
		/* driver variables, make terminal illegal */
		*termType = 0;
		setvalues();
		d_text = t4112text;
		d_clear = t4112clear;
		tout = stdout;
		tin = stdin;

		/* indicate that driver is set for tek4112 */
		strcpy(termType,name);
	}
}

void tty5620()
{
	static char name[] = "5620";
	static char ptyname[11] = "/dev/ttyp0";
	static FILE *ptyout = NULL;

	if(strcmp(termType,name)) {
	    if(tout != ptyout) {
		*termType = 0;
		if(ptyout != NULL && ptyout != stdout) {
			fclose(ptyout);
		}
		if(tin != stdin)
			fclose(tin);

tryagain:
		fprintf(stderr, "Please enter the name of the tek4014 window eg. 'ttyp0'  ");
		scanf("%s",ptyname + 5);
		while(getchar() != '\n') ;
		ptyout = tout = fopen(ptyname,"w");
		tin = fopen(ptyname,"r");
		if(tout == NULL|| tin == NULL) {
			printf("\07Can't open %s\n",ptyname);
			goto tryagain;
		}
	    }
	    setvalues();
	    strcpy(termType,name);
	}
}

static void setvalues()
{
	/* set up variables */
	chX[0] = 14; chX[1] = 14; chX[2] = 14;
	chY[0] = 22; chY[1] = 22; chY[2] = 22;
	xRes = 1024;
	yRes = 780;
	xSize = 8;
	ySize = 6.09375;
	hasClear = 0;
	hasCursor = 1;
	tType = NORMAL;

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
	d_boxfill = dmy;

	/* get formats */
	GtSpec("form.tek401x");
}	

/********************************************/
/* setup - set up the terminal for graphics */
/********************************************/

static void setup()
{
	if(tType == XTERM)
		fputs("\033[?38h",tout);
	if(ItsATekFile||ItsATekPrinter) {
		tout=PFile();
		fprintf(tout,"%s",startqms); /* Magic characters */
	}
	linetype(255);
	plotting = 1;
	xyvalid = 0;
}

/***********************************/
/* cleanup - clean up the terminal */
/***********************************/

static void cleanup()
{
	char ts[128];
	if(tType == XTERM)
		fputs("\037\030\033\003", tout);
	else
		fputs("\037\030", tout);
	if(ItsATekFile||ItsATekPrinter) { 
		fprintf(tout,"%s",endqms);  /* More magic chars to make it work */
		FSafeClose(tout);
		if(ItsATekFile)
	   	    printf("The tektronix plot file is %s\n",hcfile);
		if(ItsATekPrinter) {
		   sprintf(ts,"%s/lib/hc.lpr %s %s",main_.comb,termType,hcfile);
		   eshell_(ts);
		}
		*termType=0; 
		tout=stdout;
		tin=stdin;
		ItsATekFile=0;
		ItsATekPrinter=0;
	}
	else /* it's a regular screen plot */
		fflush(tout);
#if SUN_OS && SYSTEM_V
	iocount = 0;
#endif /* SUN_OS && SYSTEM_V */
	plotting = 0;
}

/**********************/
/* move - move cursor */
/**********************/

static void move(x,y)
	int x,y;
{
	cx = x;
	cy = y;
	needsmove++;
}

/********************/
/* line - draw line */
/********************/

static void line(x,y)
	int x,y;
{
	if(needsmove) {
		putc(29,tout);
		xy(cx,cy);
		needsmove = 0;
	}
	xy((int)x,(int)y);
}

/****************************/
/* linetype - set line type */
/****************************/

static void linetype(type)
	register int type;
{
	static int otype = -1;
	static char ch[] = "acbd`";	/* See 4014 man appendix F */
	/* dotted,short dashed,dot dashed,longdashed,continuous */

	if(type >= 200)
		type = (type == GRIDlt)? 0:9;
	else if(type < 100)
		type = type % 10;
	else
		type = (type - 100)/10;
	type >>= 1;
	if(type == otype)
		return;
	putc(ESC,tout);
	putc(ch[type],tout);
	otype = type;
}

/********************/
/* text - draw text */
/********************/

static void text(str,sideways)
	char *str;
	int sideways;
{
	if(sideways)
		error_("Tek401x can't write sideways");
#if SUN_OS && SYSTEM_V
	if(++iocount > 10) {
		if(!ItsATekFile && !ItsATekPrinter)
			fflush(tout);
		iocount = 0;
	}
#endif /* SUN_OS && SYSTEM_V */
	putc(29,tout);
	if(strlen(str) == 1 && !isdigit(*str)) {
		int yOff;

		if(islower(*str))
			yOff = 4;
		else if(*str == '*')
			yOff = 7;
		else if(*str == '.')
			yOff = 1;
		else
			yOff = 6;
		xy(cx - 5,cy - yOff);
	} else
		xy(cx,cy - 8);
	fprintf(tout,"\037%s",str);
	xyvalid = 0;
}

/********************************/
/* text - draw text for tek4112 */
/********************************/

static void t4112text(str,sideways)
	char *str;
	int sideways;
{
	putc(29,tout);
	xy(cx,cy - 8);
	putc(31,tout);
	if(sideways)
		fprintf(tout,"\033MRE:0");
	fprintf(tout,"\033LT");
	t4112num(strlen(str));
	fprintf(tout,"%s",str);
	if(sideways)
		fprintf(tout,"\033MR00");
	xyvalid = 0;
}

/************************/
/* center - center text */
/************************/

static void center(str,sideways)
	char *str;
	int sideways;
{
	cx -= strlen(str)*chX[0]/2;
	d_text(str,sideways);
}

/**************************************/
/* clear - clear a part of the screen */
/**************************************/

static void clear(lx,ly,hx,hy)
	int lx,ly,hx,hy;
{
	fprintf(tout,"\035\033\f");
	xyvalid = 0;
}

/**************************************************/
/* clear - clear a part of the screen for tek4112 */
/**************************************************/

static void t4112clear(lx,ly,hx,hy)
	int lx,ly,hx,hy;
{
	fprintf(tout,"\033SC\033SK1\033SO1");
	xyvalid = 0;
}

/*************************************/
/* cursor - read the graphics cursor */
/*************************************/

static void cursor(x,y,flag)
	int *x,*y;
	char *flag;
{
	char response[20];
	int ch;
	static int informed = 0;
	if(!informed) {
		printf("Place cursor, hit space (e to end sequence)\n");
		if(tType == XTERM)
			printf("You must type rtn after 'space' or 'e'\n");
		informed++;
	}
	if(tType == XTERM)
		fputs("\033[?38h",tout);
	fprintf(tout,"\035\033\032");		/* Print mode, turn on cursor */
	fflush(tout);
	fgets(response,20,tin);
	if(*response == '\n')
		fgets(response + 1,20,tin);
	*x = ((response[1] & 31) << 5) | (response[2] & 31);
	*y = ((response[3] & 31) << 5) | (response[4] & 31);
	*flag = (*response != 'e');
	xyvalid = 0;
	cx = *x - 4;
	cy = *y;
	text("X",0);
	cleanup();
	/* fflush(tout); */
}

/****************************
 * dmy - do nothing routine *
 ****************************/

static void dmy( void)
{
}

/**********************************************/
/* xy - send xy coordinates in tek401x format */
/**********************************************/

static void xy(x,y)
register int x,y;
{
	char yh1,yl1,xh1,xl1;
	static char yh2,yl2,xh2;
	yh1 = 32 + ((y) >> 5);
	yl1 = 96 + ((y) & 31);
	xh1 = 32 + ((x) >> 5);
	xl1 = 64 + ((x) & 31);
	if(!xyvalid) {
		fprintf(tout,"%c%c%c%c",yh1,yl1,xh1,xl1);
		xyvalid++;
	} else {
		if(yh1 != yh2)
			putc(yh1,tout);
		if(yl1 != yl2 || xh1 != xh2)
			putc(yl1,tout);
		if(xh1 != xh2)
			putc(xh1,tout);
		putc(xl1,tout);
	}
	yh2 = yh1;
	yl2 = yl1;
	xh2 = xh1;
	cx = x;
	cy = y;
#if SUN_OS && SYSTEM_V
	iocount++;
#endif /* SUN_OS && SYSTEM_V */
}

/*****************************************
 * t4112num send an <int> to the tek4112 *
 *****************************************/

static void t4112num(n)
register int n;
{
	char lo,mid,hi;

	lo = n & 15;
	mid = (n >>= 4) & 63;
	hi = (n >> 6) & 63;

	if(hi)
		putc(hi | 64,tout);
	if(mid || hi)
		putc(mid | 64,tout);
	putc(lo | 48,tout);
}
