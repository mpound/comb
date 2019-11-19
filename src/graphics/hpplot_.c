/* routines to interface HP7580B pen plotter    March 1985   A. A. Stark */

#include <stdio.h>
#include <sgtty.h>
#include <signal.h>
#include <setjmp.h>
#include <ctype.h>

#define BUFSIZE 81

static int fd;		/* File descriptor for connection to plotter */
static char devname[] = "/dev/plotter";
static char buff[BUFSIZE];

			
openhpplot_(ok)
int *ok;
/*	This routine opens the HP 7580B penplotter on /dev/plotter, and
	checks that it is ready.  If it is not ready, it initiates
	a dialog with stdin and stdout, asking for the user to fix the plotter
	up.  When the plotter is ready, *ok is set to fd.  *ok is set to
	-1 on an unsuccesful open.

	openhpplot must be called before any other HP penplotter routine
*/

{
	void timeout();
	int try, statn;
	static struct sgttyb sgtty = { B1200,B1200,'\b','@',CRMOD|ECHO|ANYP|CBREAK};

	fd = -1;
	fprintf(stderr,"\nOpening HP penploter");
	fflush(stdout);

	(void) alarm(0);
	signal(SIGALRM, timeout);
	try = 0;
	while ( (void) alarm(3), (fd = open(devname, 2)) < 0 ) {
		(void) alarm(0);
		if ( try++ >= 3 || tryagain() == 0 ) goto broken;
	}
	(void) alarm(0);
	ioctl(fd,TIOCEXCL,NULL);		/* Ask for exclusive open */
	ioctl(fd,TIOCSETP,&sgtty);		/* Set basic parameters */
	ioctl(fd,TIOCSDTR,NULL);		/* Set data terminal ready */
	sleep(2);
	fprintf(stderr," - plotter open\n");
	try = 0;
	do {
		if ( try >= 3 || (try && tryagain() == 0) ) goto broken;
		try++;
		putshp("\033.Y\033.@9999;14:\033.J\033.KDF;RO;IP;");
		putshp("\033.M50;0;10;13;0:\033.N10;19;0:\033.I80;0;17;0:");
		status_( &statn );
	} while ( statn != 8 );
	*ok = fd;
	return;

broken:	if ( fd != -1 ) closehpplot_();
	fprintf(stderr,"\n\007ERROR opening HP penplotter\007\n");
	fflush(stdout);
	*ok = -1;
	return;
}

tryagain()
{
	void timeout();

	signal(SIGALRM, timeout);
	(void) alarm(300);

	buff[0] = 'n';
	write(1,"\007\nHP penplotter not ready - try again? \007", 39);
	read( 0, buff, BUFSIZE);

	(void) alarm(0);

	sscanf("%s", buff);

	return ( (buff[0] == 'y') ? 1 : 0 );
}

closehpplot_()
/*	closehpplot sets the HP penplotter in the "view" mode, and
	closes /dev/plotter.
*/
{
	int dummy;
	dummy = 0;
	tellpen_(&dummy);			/* Put pen away */
	putshp("NR;");
	fprintf(stderr," Closing plotter\n");
	fflush(stdout);
	sleep(4);
	ioctl(fd,TIOCNXCL,NULL);		/* Turn off exclusive use */
	ioctl(fd,TIOCCDTR,NULL);		/* Turn off data term ready */
	close(fd);
}

zzget_(dummy,iarray,isize,iend)
int *dummy, *iarray, *isize, *iend;
/*
c
c
c     gets a record from the plotter and converts from
c         hollerith to ascii codes
c
c
c     dummy = lu     -- unit number (not used)
c
c     iarry  -- pointer to buffer
c
c     iend   -- location of the end-of-string flag
c
c     isize  -- size of -iarry-.  (2.le.isize .and. isize.le.73)
c
c     ***************************************************************
c     *                                                             *
c     *   this routine reads a record from the plotter into the     *
c     *   buffer provided by the calling program, one character     *
c     *   per array element.  each character is then converted to   *
c     *   its equivalent ascii code.  an end-of-string flag (-1)    *
c     *   is stored immediately after the rightmost character.      *
c     *   the location of this flag is also returned.               *
c     *                                                             *
c     *   when this routine exits, each element of -iarry- (up to   *
c     *   the end-of-string) contains the ascii code for one        *
c     *   character.                                                *
c     *                                                             *
c     ***************************************************************
c
*/
{
	int flag;
	int *endarray;
	register char *pc;

	if ( *iend < 0 ) flag = 0; else flag = 1;

	endarray = iarray + abs(*isize);
	*iend = 1;
	getshp( buff, BUFSIZE);
	pc = buff;
	while ( *pc ) {
		if (flag == 0 && ( *pc < '!' || *pc > '~') ) break;
		*iarray++ = *pc++;
		(*iend)++;
		if ( iarray >= endarray ) {
			fprintf(stderr,"\nBuffer exceeded reading hpplotter in zzget");
			break;
		}
	}
	*iarray++ = -1;

	while (iarray < endarray) *iarray++ = (int) ' ';
	return;

}

zzput_(dummy,iaray,nchars)
int *dummy ,*iaray ,*nchars;
/*
c
c
c     writes ascii characters to output device
c
c
c     dummy = idevot -- output device
c
c     pointer to iaray  -- array containing ascii codes for the characters
c                 to be written.  each element contains the ascii
c                 code for one character.
c
c     *nchars -- number of characters in the string.  (not .gt. 71)
c
c
c     ***************************************************************
c     *                                                             *
c     *   this routine writes a record to the plotter.  the calling *
c     *   program provides data to this routine in the form of an   *
c     *   array of ascii codes.                                     *
c     *                                                             *
c     ***************************************************************
c
*/
{
	char buf[BUFSIZE];

	zzpack_( iaray, buf, nchars);
	write( fd, buf, *nchars);
}

zzpack_(isourc,idest,nchars)
int *isourc, *idest, *nchars;
/*
c
c     packs characters--converts from ascii codes to hollerith data.
c
c
c     isourc -- source array.  each element contains the ascii
c                 code for one character.
c
c     idest  -- destination array, returned to calling pgm.
c                 will contain the hollerith representation of
c                 the character string specified by isourc.
c
c     *nchars -- number of characters in the string.
c
c
c     ***************************************************************
c     *                                                             *
c     *   this routine converts a string from ascii codes, stored   *
c     *   one character per array element, to the appropriate       *
c     *   representation for hollerith data in the host processor.  *
c     *                                                             *
c     ***************************************************************
c
c
*/
{
	register int i;
	int *isourcp;
	char *idestp;

	isourcp = isourc;
	idestp = (char *) idest;
	for ( i = 0; i < *nchars; i++ ) *idestp++ = (char) *isourcp++;

}

zzlow_(isourc,idest)
int *isourc, *idest;
/*
c
c     returns the ascii code of the character stored in the
c     low-order position of an integer.
c
c
c     isourc -- pointer to source integer
c
c     idest  -- pointer to destination integer
c
c
c     ***************************************************************
c     *                                                             *
c     *   the ascii code of the character contained in the least    *
c     *   significant byte of *isourc is returned in *idest .       *
c     *                                                             *
c     ***************************************************************
c
*/
{
	*idest = (int) ( (char) *isourc );
}

zzextr_(isourc,dummy,iptr,idest)
int *isourc, *dummy, *iptr, *idest;
/*
c
c
c     extract ascii codes from hollerith data
c
c
c     isourc = source array containing one or more characters in
c                 the hollerith format of the host processor
c
c     issize = size of -isourc-
c
c     iptr   = pointer defining which character is to be
c                 extracted. (iptr=1 designates the leftmost
c                 character.)
c
c     idest  = destination integer to be loaded with the ascii
c                 code for the desired character
c
c
c     ***************************************************************
c     *                                                             *
c     *   given an array containing hollerith data, this routine    *
c     *   extracts the character specified by -iptr- and returns    *
c     *   its ascii code.                                           *
c     *                                                             *
c     ***************************************************************
c
c
*/
{
	*idest = (int) *( (char *) isourc + *iptr - 1 );
}

puthpplot(c)
char c;
{
	write( fd, &c, 1);
}

putshp(pc)
char *pc;
{
	write( fd, pc, strlen(pc)); 
}

getshp( pc, max)
char *pc;
int max;
{
	register int i;
	void timeout();

	signal(SIGALRM, timeout);
	(void) alarm(200);

	for ( i = 0; read( fd, pc, 1) > 0; pc++, i++) {
		if ( i >= max ) {
			fprintf(stderr,"\n Buffer exceeded in getshp\007");
			return(-1);
		}
		if (*pc == '\n') break;
	}
	(void) alarm(0);
	*pc = 0;
	return(i);
}

void timeout()
{
	if ( fd != -1 ) closehpplot_();
	error("\n\007Timeout accessing HP plotter\007\n");
}

status_(istatus)
int *istatus;
{
	sleep(2);

	putshp("\033.O");
	
	if ( getshp( buff, BUFSIZE) <= 0 ) {
		*istatus = -1;
		return;
	}
	*istatus = atoi(buff);

}

asksize_( p1x, p1y, p2x, p2y)
int *p1x, *p1y, *p2x, *p2y;

/*	Queries the HP penplotter about the coordinates of the upper-left
	and lower-right points (p1 and p2, respectively).  Unless these
	points have been re-set by the user ( it is not expected that this
	would normally be done ), these coordinates are a measure of the
	size of paper that has been put in the plotter.

	The x axis is normally along the direction of paper travel,
	and is usually the long dimension of the paper, with the
	notable exception of 8.5" x 11" paper, where it is required
	that the x axis be the 8.5" side.  Upper-left and lower-right
	(p1 and p2) refer to the paper orientation when it is in the
	"view" position.  In this position, increasing x is down,
	and increasing y is to the right ( this assumes that the
	coordinate system has not been rotated).
*/
{
	putshp("OP;");
	getshp( buff, BUFSIZE );
	sscanf( buff,"%d,%d,%d,%d", p1x, p1y, p2x, p2y);
}

rotate_()

/*	rotate the x,y coordinate system counterclockwise by 90 degrees.
	This is for paper that has the short dimension along the
	direction of paper travel.
*/
{
	int x1, y1, x2, y2;

	asksize_(&x1,&y1,&x2,&y2);
	putshp("RO90;");
	tellsize_(&y1,&x1,&y2,&x2);
}

tellsize_(p1x,p1y,p2x,p2y)
int *p1x, *p1y, *p2x, *p2y;

/*	re-set the coordinates of the p1 and p2 points */

{
	putshp("IP");
	sprintf(buff,"%d,%d,%d,%d;",*p1x,*p1y,*p2x,*p2y);
	putshp(buff);
}

telldash_(type)
int *type;

/*	Start making dashed lines.  Type > 6 will result in solid lines.
	Type = 0 will put points only at array values.  Types less than 0
	will result in the pattern of dashes being scaled to the distance
	between points.  Increasing type yields decreasing dottedness.
*/
{
	static int oldtype = 7;

	if ( *type == oldtype ) return;
	oldtype = *type;
	if ( *type > 6 || *type < -6 ) {
		putshp("LT;");			/* set solid line */
		return;
	}
	sprintf(buff,"LT%d,0.5;",*type);	/* the 0.5 says make dash
						   pattern 0.5% of paper size */
	putshp(buff);
}

tellpen_(type)
int *type;

/*	Choose a new pen.  Type = 0 puts the pen away.
*/
{
	static int oldtype = -1;

	if (*type == oldtype) return;
	sprintf(buff,"SP%d;", *type % 9 );
	putshp(buff);
	oldtype = *type;
}

digitize_(px,py,n)
int px[], py[], *n;

/*	Interactively digitize an array of n points, using the manual controls.
*/
{
	register int i;
	int *pdummy;

	putshp("SP0;");			/* put pen away */
	sprintf(buff,"\n\007Begin digitization of %d points.\n",*n);
	write(0,buff,strlen(buff));
	for ( i = 0; i < *n; i++) {
		write(0,"move cursor, push enter: ",25);
		putshp("DP;");
		while ( !digitready() )
			sleep(1);
		putshp("OD;");
		getshp(buff,BUFSIZE);
		sscanf(buff,"%d,%d,%d",&px[i],&py[i],pdummy);
		sprintf(buff,"   point  %d,  x = %d, y = %d \007\n",i, px[i], py[i] );
		write(0,buff,strlen(buff));
	}
	putshp("DC;");
}

digitready()
{
	putshp("OS;");
	getshp(buff,BUFSIZE);
	return( atoi(buff) & 04 );
}

askpendwn()
{
	putshp("OS;");
	getshp(buff,BUFSIZE);
	return( atoi(buff) & 01 );
}

