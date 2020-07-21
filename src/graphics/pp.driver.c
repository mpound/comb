/* uses routines in hpplot_.c
*/
/******************************
/* hp7580  pen plotter driver *
/*****************************/

# include <stdio.h>
# include "C.h"
# include "include.h"

#define bzero(s, n) memset(s, 0, n)

static int fd = -1;			/* file discriptor for plotter */
static int dummy;
static int cx = -20,cy = -20;
static int needsmove = 1;
static int pendown = 0;
static char tempbuff[81];
static int sfact = 1;
static int sacc = 5;
static int drawing = 0;			/* True if the intermediate code is
					calling for the pen which is loaded */
static int curLineType,lastLineType;	/* Used changing line type for text */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* pp.driver.c */
static setvalues P_((void));
static setup P_((void));
static cleanup P_((void));
static move P_((int x, int y));
static line P_((int x, int y));
static linetype P_((register int type));
static text P_((char *str, int sideways));
static center P_((char *str, int sideways));
static texttype P_((int type));
static cursor P_((int *x, int *y, int *flag));
static dmy P_((void));
static xy P_((register int x, register int y));
static label P_((char *str));
static movepen P_((void));
#undef P_

hp7580()
{
	*termType=0;
	setvalues();
	strcpy(termType,"hp7580");
}


		
static setvalues()
{
	int setup();
	int cleanup();
	int move(),line(),linetype();
	int text(),center(),texttype(),dmy();
	int clear();
	int cursor();

	/* set up variables */
	chX[0] = 180; chX[1] = 340; chX[2] = 450;	/* 1.12 times width */
	chY[0] = 440; chY[1] = 840; chY[2] = 1240;	/* 2 times height */
	xRes = 31270;		/* pixels between p1x and p2x */
	yRes = 20132;		/* pixels between p1y and p2y */
	xSize = 30.778;		/* inches between p1x and p2x */
	ySize = 19.815;		/* inches between p1y and p2y */
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
	d_clear = dmy;
	d_cursor = cursor;
	d_boxfill = dmy;

	/* get formats */
	GtSpec("form.hp7580");
}	

/******************************/
/* setup - set up the plotter */
/******************************/

static setup()
{
	int p1x, p1y, p2x, p2y;
	int lmon = 1;		/* monitor mode: 0=none; 1=parse; 2=receive */
	int temp;

	openhpplot_(&fd);
	if ( fd < 0 ) {
		ChkAccess("hp penplotter on /dev/plotter","/dev/plotter",6,0,"");
		error_("Can't open plotter");
	}
	plotting = 1;
	asksize_(&p1x,&p1y,&p2x,&p2y);
	if ( abs(p1x) < abs(p1y) ) {
		rotate_();
		asksize_(&p1x,&p1y,&p2x,&p2y);
	}
	sfact = (float) xRes * sacc / (p2x-p1x) + 0.5 ;
	if ( (temp = (float)yRes*sacc/(p2y-p1y)+0.5 ) > sfact ) sfact = temp;

	sprintf(tempbuff,"SC0,%d,0,%d;",(p2x-p1x)*sfact/sacc-1,(p2y-p1y)*sfact/sacc-1);
	putshp(tempbuff);
	putshp("PU;");
	pendown = 0;			/* pen is up */
}

/********************************/
/* cleanup - finish up the plot */
/********************************/

static cleanup()
{
	(void)askpendwn();
	closehpplot_();
	plotting = 0;
}

/*******************/
/* move - move pen */
/*******************/

static move(x,y)
	unsigned short int x,y;

{
	cx = x;
	cy = y;
	needsmove++;
}

/********************/
/* line - draw line */
/********************/

static line(x,y)
	unsigned short int x,y;
{
	if(!drawing) {
		cx = x;
		cy = y;
		needsmove++;
		return;
	}
	if(needsmove) movepen();
	if (!pendown) putshp("PD;");
	pendown = 1;
	xy((int)x,(int)y);
}

/****************************/
/* linetype - set line type */
/****************************/

static linetype(type)
	register int type;
{
	register int i;
	int units, tens;
	static char penUse[9];		/* one byte per pen for pens 1 -8
					codes are: 0 not needed, 1 needed,
					2 is being or has been plotted */
	static int passPen;		/* the pen being used on this pass */

	if (type >= 200) {
		type = 17;
	}
	if (type >= 100) type -= 100;
	lastLineType = curLineType;
	curLineType = type;
	tens = type/10;
	if (tens == 0) tens = 1;
	if (tens == 9) tens = 8;

	/* The following code controls multiple passes through the intermediate
	code - one for each pen that is needed */

	switch(ppState) {
	case FRESHSTART :
		bzero(penUse,9);
		penUse[1]++;
		return;
	case NEWPASS :
		for(i = 1;i <=8;i++) {	/* find a pen which is needed */
			if(penUse[i] == 1) {
				penUse[i] = 2;
				passPen = i;
				tellpen_(&passPen);
				ppState = PLOTTING;
				break;
			}
		}
		if(i > 8)
			error_("pen plotter pass control error\n");
	/* This falls through to PLOTTING */
	case PLOTTING :
	case MOREPENS :
		drawing = (tens == passPen);
		if(!drawing) {
			if(penUse[tens] == 0)
				penUse[tens] = 1;
			else if(penUse[tens] >= 2)
				return;
			ppState = MOREPENS;
			return;
		}
		break;
	default :
		error("Impossible ppState = %d\n",ppState);
	}

	units = type%10 + 1;
	telldash_(&units);
	return;
}

/********************/
/* text - draw text */
/********************/

static text(str,sideways)
	char *str;
	int sideways;
{
	linetype(17);
	if(!drawing) {
		linetype(lastLineType);
		return;
	}
	if (needsmove) movepen();
	if(sideways)
		putshp("LO2;DI0,1;");
	else
		putshp("LO2;DI1,0;");
	label(str);
	linetype(lastLineType);
	pendown = 1;
}

/************************/
/* center - center text */
/************************/

static center(str,sideways)
	char *str;
	int sideways;
{
	linetype(17);
	if(!drawing) {
		linetype(lastLineType);
		return;
	}
	if (needsmove) movepen();
	if (sideways)
		putshp("LO5;DI0,1;");
	else
		putshp("LO5;DI1,0;");
	label(str);
	linetype(lastLineType);
	pendown = 1;
}

/**************************************/
/* texttype - set the text type       */
/**************************************/

static texttype(type)
int type;
{
	putshp("CS10;CA15;");		/* select standard and alt. fonts */
	if ( type > 2 || type < 0 ) type = 2;
	sprintf(tempbuff,"SI%6.3f,%6.3f;",chX[type]*.0025*0.89*sacc/sfact,chY[type]*.0025*0.5*sacc/sfact);
	putshp(tempbuff);
	putshp("ES,0.12");		/* put in extra space between chars */
					/* the multipler for chX is 1/(1+es) */
}


/****************************/
/* cursor - read the cursor */
/****************************/

static cursor(x,y,flag)
	int *x,*y,*flag;
{
	dummy = 1;
	digitize_(x,y,&dummy);
	*flag = ( !askpendwn() );
}

/****************************
 * dmy - do nothing routine *
 ****************************/

static dmy()
{
}

/*********************************************/
/* xy - send xy coordinates in hp7580 format */
/*********************************************/

static xy(x,y)
register int x,y;
{
	sprintf(tempbuff,"PA%d,%d;",x,y);
	putshp(tempbuff);
}

/**************************/
/* label - transmit label */
/**************************/

static label(str)
char *str;
{
	putshp("LB");
	putshp(str);
	puthpplot(3);
}

static movepen()
{
	if (pendown) putshp("PU;");
	pendown = 0;
	xy(cx,cy);
	needsmove = 0;
}
