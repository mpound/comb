/**************************************/
/* Driver for setlayer on the tty5620 */
/**************************************/

# include <stdio.h>
# include "C.h"
# include "include.h"
#include <ctype.h>

#define FASTMODS

#define LINE 'L'
#define MOVE 'M'
#define ERASE 'E'
#define CHAR 'C'
#define RECT 'R'
#define STRING 'S'
#define QUERY 'Q'
#define GRAPH_BEGIN '\035'
#define GRAPH_END '\037'
#define ESCAPE '\033'		/* ascii code for escape character */

#ifdef FASTMODS
static int drawingVectors = 0;
#else
static int needsmove = 1;
#endif

static int cx,cy;		/* current x any y in terminal coordinates */

/* Terminal coordinates increase to the right and down from the upper left */

setlayer()
{
	int setup();
	int cleanup();
	int move(),line(),linetype();
	int text(),center(),texttype();
	int clear();
	int cursor();

	static char name[] = "setlayer";

	/* if not set up to drive setlayer, set it up */
	if(strcmp(termType,name))
	{
		/* in case something happens while setting up */
		/* driver variables, make terminal illegal */
		*termType = 0;

		/* set up variables */
		chX[0] = 9; chX[1] = 9; chX[2] = 9;
		chY[0] = 14; chY[1] = 14; chY[2] = 14;
		xRes = 719;
		yRes = 599;
		xSize = 10.5;
		ySize = 8.57;
		hasClear = 0;
		hasCursor = 1;

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
		GtSpec("form.setlayer");

		/* indicate that driver is set for setlayer */
		strcpy(termType,name);
	}
}

/********************************************/
/* setup - set up the terminal for graphics */
/********************************************/

static setup()
{
	plotting = 1;
	putchar(GRAPH_BEGIN);
}

/***********************************/
/* cleanup - clean up the terminal */
/***********************************/

static cleanup()
{
#ifdef FASTMODS
	stopDrawingVectors();
#endif
	putchar(GRAPH_END);
	fflush(stdout);
	plotting = 0;
}

/**********************/
/* move - move cursor */
/**********************/

static move(x,y)
	unsigned short int x,y;
{
#ifdef FASTMODS
	stopDrawingVectors();
#else
	needsmove++;
#endif
	cx = x;
	cy = yRes - y;
}

/********************/
/* line - draw line */
/********************/

#ifndef FASTMODS
static line(x,y)
	unsigned short int x,y;
{
	if(needsmove) {
		putchar(MOVE);
		PutShort(cx);
		PutShort(cy);
		needsmove = 0;
	}
	putchar(LINE);
	PutShort(x);
	PutShort(yRes -y);
}

#else
static line(x,y)
int x,y;
{
	if(!drawingVectors) {
		putchar('V');
		drawingVectors++;
		xy(cx,cy);
	}
	xy(x,(int)yRes - y);
}
#endif

/****************************/
/* linetype - set line type */
/****************************/

/*ARGSUSED*/
static linetype(typ)
	unsigned char typ;
{
#if 0
	static int otype = -1;
	static char lt[10] = {7,9,6,6,4,8,10,5,5,1};
	register int type = typ;

	if(type >= 200)
		type = (type == GRIDlt)? 0:9;
	else if(type < 100)
		type %= 10;
	else
		type = (type - 100)/10;
	if(type == otype)
		return;
	otype = type;
	makemode(NONE);
	printf("%c*m2a%dB",ESC,lt[type]);
#endif
}

#ifndef FASTMODS

/********************/
/* text - draw text */
/********************/

static text(str,sideways)
	register char *str;
	int sideways;
{
	putchar(MOVE);
	PutShort(cx);
	PutShort(cy - 5);
	while(*str) {
		putchar(CHAR);
		putchar(*str++);
	}
}

#else
/********************/
/* text - draw text */
/********************/

static text(str,sideways)
	register char *str;
	int sideways;
{
	register int c;

	stopDrawingVectors();
	putchar(MOVE);
	PutShort(cx);
	PutShort(cy - 5);
	putchar(STRING);
	while( c = *str++ ) {
		if(isprint(c))
			putchar(c);
	}
	putchar(GRAPH_BEGIN);
}
#endif

/************************/
/* center - center text */
/************************/

static center(str,sideways)
	char *str;
	int sideways;
{
	cx -= strlen(str)*chX[0]/2;
	text(str,sideways);
}

/****************************/
/* texttype - set text type */
/****************************/

/*ARGSUSED*/
static texttype(type)
	int type;
{
#if 0
	static int otype = -1;

	if(type == otype)
		return;
	makemode(NONE);
	if(type == 2)
		printf("%c*mO",ESC);
	else if(otype == 2 || otype == -1)
		printf("%c*mP",ESC);
	otype = type;
#endif
}

/**************************************/
/* clear - clear a part of the screen */
/**************************************/

/*ARGSUSED*/
static clear(lx,ly,hx,hy)
	int lx,ly,hx,hy;
{
#ifdef FASTMODS
	stopDrawingVectors();
#endif
	putchar(ERASE);
}

/*************************************/
/* cursor - read the graphics cursor */
/*************************************/

#include <sgtty.h>

static cursor(x,y,flag)
	int *x,*y;
	char *flag;
{
	struct sgttyb Tty;
	short int SaveFlags;

	/* tell user how to use the cursor once */
	static int informed = 0;
	if(!informed) {
		printf("Place cross, click button 1 or 2 (2 indicates end)\n");
		informed++;
	}

	noint_();
	/* turn echo off, Cbreak on */
	ioctl(1,TIOCGETP,&Tty.sg_ispeed);
	SaveFlags = Tty.sg_flags;
	Tty.sg_flags &= ~ECHO;
	Tty.sg_flags |= CBREAK;
	ioctl(1,TIOCSETP,&Tty.sg_ispeed);

	/* ask the 5620 */
	setup();
	putchar(QUERY);
	fflush(stdout);

	/* get the coordinates */
	*x = GetShort() - 1;
	*y = yRes - (cy = GetShort() - 1);
	*flag = (getchar() != '2');

	/* restore tty conditions */
	Tty.sg_flags = SaveFlags;
	ioctl(1,TIOCSETP,&Tty.sg_ispeed);
	yesint_();

	/* mark the point on the screen */
	cx = *x - 4;
	text("X",0);
	cleanup();
}

#define IMASK5	0037	/* bit pattern: 0000000000011111 */
#define BIT7ON	0100	/* bit pattern: 0000000001000000 */

static GetShort()
{
	register int x;

	x = (getchar() & IMASK5);
	x |= (getchar() & IMASK5) << 5;

	return x;
}


PutShort(x)
register int x;
{
	register int c;
	c = (x & IMASK5) | BIT7ON;
	putchar(c);
	c = ((x >> 5) & IMASK5) | BIT7ON;
	putchar(c);
 
}

#ifdef FASTMODS

/**********************************************/
/* xy - send xy coordinates in tek401x format */
/**********************************************/

static xy(x,y)
register int x,y;
{
	register int yh1,yl1,xh1;
	int xl1;
	static int yh2 = -1,yl2 = -1,xh2 = -1;

	yh1 = ((y >> 5) & 31) | 040;
	yl1 = (y & 31) | 0140;
	xh1 = ((x >> 5) & 037) | 040;
	xl1 = (x & 31) | 0100;

	if(yh1 != yh2) {
		putchar(yh1);
		yh2 = yh1;
	}
	if(yl1 != yl2 || (xh1 != xh2 && yh1 == yh2)) {
		putchar(yl1);
		yl2 = yl1;
	}
	if(xh1 != xh2) {
		putchar(xh1);
		xh2 = xh1;
	}
	putchar(xl1);
	cx = x;
	cy = y;
}

stopDrawingVectors()
{
	if(drawingVectors) {
		drawingVectors = 0;
		putchar(GRAPH_END);
	}
}
#endif
