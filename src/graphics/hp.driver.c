/***********************/
/* hp terminal drivers */
/***********************/

# include <stdio.h>
# include "C.h"
# include "include.h"

# define ESC 27			/* ascii code for escape character */

# define NONE 0			/* no mode, line type, text type, etc. */
# define PLOT 1			/* plotting mode */
# define HTEXT 2		/* horizontal text mode */
# define VTEXT 3		/* vertical text mode */
# define CHTEXT 4		/* centered horizontal text mode */
# define CVTEXT 5		/* centered vertical text mode */

static int flag;		/* 1 if plotting in binary absolute */
				/* 2 if plotting in binary increments */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* hp.driver.c */
static void setterm P_((void));
static void setup P_((void));
static void cleanup P_((void));
static void move P_((int x, int y));
static void line P_((int x, int y));
static void linetype P_((register int type));
static void text P_((char *str, int sideways));
static void center P_((char *str, int sideways));
static void texttype P_((int type));
static void clear P_((int lx, int ly, int hx, int hy));
static void cursor P_((int *x, int *y, char *flag));
static void makemode P_((int mode));
static void xy P_((int x, int y));
#undef P_

/**********************************/
/* hp2648 - use driver for hp2648 */
/**********************************/

hp2648()
{

	/* if not set up to drive hp2648, set it up */
	if(strcmp(termType,"hp2648"))
	{
		/* in case something happens while setting up */
		/* driver variables, make terminal illegal */
		*termType = 0;

		/* set up variables */
		chX[0] = 7; chX[1] = 7; chX[2] = 7;
		chY[0] = 10; chY[1] = 10; chY[2] = 10;
		xRes = 719;
		yRes = 359;
		xSize = 10.5;
		ySize = 5.2427;

		setterm();

		/* get formats */
		GtSpec("form.hp2648");

		/* indicate that driver is set for hp2648 */
		strcpy(termType,"hp2648");
	}
}

/**********************************/
/* hp2623 - use driver for hp2623 */
/**********************************/

hp2623()
{
	static char name[] = "hp2623";

	/* if not set up to drive hp2623, set it up */
	if(strcmp(termType,name))
	{
		/* in case something happens while setting up */
		/* driver variables, make terminal illegal */
		*termType = 0;

		/* set up variables */
		chX[0] = 7; chX[1] = 7; chX[2] = 7;
		chY[0] = 10; chY[1] = 10; chY[2] = 10;
		xRes = 511;
		yRes = 389;
		xSize = 10.5;
		ySize = 7.9932;

		setterm();

		/* get formats */
		GtSpec("form.hp2623");

		/* indicate that driver is set for hp2623 */
		strcpy(termType,name);
	}
}

static void setterm()
{
	hasClear = 1;
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
}

/********************************************/
/* setup - set up the terminal for graphics */
/********************************************/

static void setup()
{
	linetype(9);
	texttype(-1);
	plotting = 1;
	printf("%c*dcS",ESC);
}

/***********************************/
/* cleanup - clean up the terminal */
/***********************************/

static void cleanup()
{
	makemode(NONE);
	printf("%c*dT",ESC);
	plotting = 0;
}

/**********************/
/* move - move cursor */
/**********************/

static void move(x,y)
	int x,y;
{
	makemode(PLOT);
	printf("a");
	xy(x,y);
}

/********************/
/* line - draw line */
/********************/

static void line(x,y)
	int x,y;
{
	makemode(PLOT);
	xy(x,y);
}

/****************************/
/* linetype - set line type */
/****************************/

static void linetype(type)
	register int type;
{
	static int otype = -1;
	static char lt[10] = {7,9,6,6,4,8,10,5,5,1};

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
/* Matt's scheme follows
	if(type != -1)
	{
		printf("%c*m2a",ESC);
		if(type * 16 / LINTYPES < 15)
			printf("170 %dc2B",type * 16 / LINTYPES + 1);
		else
			printf("1B");
	}
	otype = type;
*/
}

/********************/
/* text - draw text */
/********************/

static void text(str,sideways)
	char *str;
	int sideways;
{
	if(sideways)
		makemode(VTEXT);
	else
		makemode(HTEXT);
	printf("%s",str);
}

/************************/
/* center - center text */
/************************/

static void center(str,sideways)
	char *str;
	int sideways;
{
	if(sideways)
		makemode(CVTEXT);
	else
		makemode(CHTEXT);
	printf("%s\n",str);
}

/****************************/
/* texttype - set text type */
/****************************/

static void texttype(type)
	int type;
{
	static int otype = -1;

	if(type == otype)
		return;
	makemode(NONE);
	if(type == 2)
		printf("%c*mO",ESC);
	else if(otype == 2 || otype == -1)
		printf("%c*mP",ESC);
	otype = type;
}

/**************************************/
/* clear - clear a part of the screen */
/**************************************/

static void clear(lx,ly,hx,hy)
	int lx,ly,hx,hy;
{
	makemode(NONE);
	if(lx == 0 && ly == 0 && hx == xRes && hy == yRes)
		printf("%c*dA",ESC);
	else
	{
		printf("%c*m1a1b%d,%d %d,%de2A",ESC,lx,ly,hx,hy);
		linetype(-1);
	}
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
		informed++;
	}
	printf("%c*dC",ESC);
	printf("%c*s4^%c",ESC,17);
	scanf("%s",response);
	while(getchar() != '\n')
		;
	sscanf(response,"%d,%d,%d",x,y,&ch);
	*flag = (ch != 'e');
	setup();
	move(*x - 3,*y);
	text("X",0);
	cleanup();
	fflush(stdout);
}

/****************************************************/
/* makemode - make sure terminal is in a given mode */
/****************************************************/

static void makemode(mode)
	int mode;
{
	static int cmode = NONE;

	if(mode == cmode)
		return;
	if(cmode == PLOT)
		printf("Z");
	switch(mode)
	{
		case PLOT:
			flag = 0;
			printf("%c*p",ESC);
			break;
		case HTEXT:
			printf("%c*m1n2Q",ESC);
			break;
		case VTEXT:
			printf("%c*m2n2Q",ESC);
			break;
		case CHTEXT:
			printf("%c*m1n5Q",ESC);
			break;
		case CVTEXT:
			printf("%c*m2n5Q",ESC);
			break;
	}
	cmode = mode;
}

/**********************************************/
/* xy - send xy coordinates in hp2648A format */
/**********************************************/

static void xy(x,y)
	int x,y;
{
	static int cx = -20,cy = -20;
	int dx,dy;
	char c1,c2,c3,c4;

	dx = x - cx;
	dy = y - cy;

	if(dx > 15 || dx < -16 || dy > 15 || dy < -16 || flag == 0)
	{
		c1 = 32 | (x >> 5);
		c2 = 32 | (x & 31);
		c3 = 32 | (y >> 5);
		c4 = 32 | (y & 31);
		if(flag != 1)
		{
			printf("i");
			flag = 1;
		}
		printf("%c%c%c%c",c1,c2,c3,c4);
	}

	else
	{
		c1 = 32 | (dx & 31);
		c2 = 32 | (dy & 31);
		if(flag != 2)
		{
			printf("j");
			flag = 2;
		}
		printf("%c%c",c1,c2);
	}

	cx = x;
	cy = y;
}
