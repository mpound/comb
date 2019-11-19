/*******************/
/* versatek driver */
/*******************/

# include <stdio.h>
# include "C.h"
# include "include.h"
# include "/usr/local/src/vplt/phase1/ppepc.h"
static int x0,y0;			/* current 'pen' position */
static int textType = 0;		/* current text type */
static int pointSize[3] = {10,12,14};	/* default size in points for ch0,1,2 */
static int three = 3;

/**************************************/
/* versatek - use driver for versatek */
/**************************************/

versatek()
{
	int setup();
	int cleanup();
	int move(),line(),linetype();
	int text(),center(),texttype();
	int clear();
	int cursor();

	static char name[] = "versatek";

	/* if not set up to drive versatek, set it up */
	if(strcmp(termType,name))
	{
		/* in case something happens while setting up */
		/* driver variables, make terminal illegal */
		*termType = 0;

		/* set up text types */
		namfnt_("fix",&three);
		chX[0] = 16; chX[1] = 19; chX[2] = 25;
		chY[0] = 25; chY[1] = 30; chY[2] = 40;

		/* set up variables */
		xRes = 2111;
		yRes = 1535;
		xSize = 10.515;
		ySize = 7.675;
		hasClear = 0;
		hasCursor = 0;

		/* set up routines */
		d_start = setup;
		d_cleanup = cleanup;
		d_move = move;
		d_line = line;
		d_setlin = linetype;
		d_text = text;
		d_ctext = center;
		d_settxt = texttype;
		d_clear = clear;
		d_cursor = cursor;

		/* get formats */
		GtSpec("form.versatek");

		/* indicate that driver is set for versatek */
		strcpy(termType,name);
	}
}

/********************************************/
/* setup - set up the terminal for graphics */
/********************************************/

static setup()
{
	static int two = 2;
	noint_();
	plots_(&two);
	plotting = 1;
	yesint_();
}

/***********************************/
/* cleanup - clean up the terminal */
/***********************************/

static cleanup()
{
	noint_();
	if(ppep1_.nplot)
		vend_();
	else
		vercln_();
	plotting = 0;
	yesint_();
}

/**********************/
/* move - move cursor */
/**********************/

static move(x,y)
	unsigned short int x,y;
{
	/* save current position */
	x0 = x;
	y0 = y;
}

/********************/
/* line - draw line */
/********************/

static line(x,y)
	int x,y;
{
	/* line width correction to left and bottom borders of plot
	 see plot_(): LWC = (LINEWID + 1)/2 */
# define LWC 2

	if(ppep1_.rotate) {
		ppep1_.ix1 = ppep1_.nibsx - y0 + LWC;
		ppep1_.iy1 = x0 + LWC;
		ppep1_.ix2 = ppep1_.nibsx - y + LWC;
		ppep1_.iy2 = x + LWC;
	} else {
		ppep1_.ix1 = x0 + LWC;
		ppep1_.iy1 = y0 + LWC;
		ppep1_.ix2 = x + LWC;
		ppep1_.iy2 = y + LWC;
	}
	ppep1_.vpltd = 0;
	strip_();
	if(ppep1_.vpltd)
		ppep1_.nplot++;
	else
		ppep1_.nbad++;
	/* update present 'pen' location */
	x0 = x;
	y0 = y;
}

/****************************/
/* linetype - set line type */
/****************************/

static linetype(type)
	register int type;
{
	static char spectype[] = {29,29,9,0};
	if(type > GRIDlt)
		type = 29;
	else if(type >= BORDERlt)
		type = spectype[type - 200];
	else if(type >= 100)
		type -=100;
	ppep1_.jpen = type / 10;
	type -= ppep1_.jpen * 10;
	/* map [0,9] into legal types of increasing 'solidness' */
	ppep1_.ltype = 4 - (type >> 1);
	/* map [0,9] into legal widths */
	ppep1_.jpen >>= 1;
}

/********************/
/* text - draw text */
/********************/

static text(str,sideways)
	char *str;
	int sideways;
{
	PlotString(str,sideways,0);
}

/************************/
/* center - center text */
/************************/

static center(str,sideways)
	char *str;
	int sideways;
{
	PlotString(str,sideways,1);
}

/**********************************************
 * PlotString - plot a string on the versatek *
 **********************************************/

static PlotString(s,sideways,centered)
char *s;			/* string pointer */
int sideways,centered;		/* flags */
{
	char ts[140];
	float ang = (sideways)? 90.:0.;

	/* set reference position for strplt */
	if(sideways)
		x0 += chY[textType] >> 1;
	else
		y0 -= chY[textType] >> 1;
	ppep1_.pref[0] = x0 / ppep1_.xdots - ppep1_.rorg[0];
	ppep1_.pref[1] = y0 / ppep1_.ydots - ppep1_.rorg[1];

	sprintf(ts,"\\f3 \\s%d %.131s",pointSize[textType],s);
	strplt_(ts,&ang,&centered);
}

/****************************/
/* texttype - set text type */
/****************************/

static texttype(type)
	int type;
{
	textType = type;
}

/**************************************/
/* clear - clear a part of the screen */
/**************************************/

static clear(lx,ly,hx,hy)
	int lx,ly,hx,hy;
{
}

/*************************************/
/* cursor - read the graphics cursor */
/*************************************/

static cursor(x,y,flag)
	int *x,*y,*flag;
{
}
