#if 0
/* Subroutine which converts a modified Julian date into an 8 char string
suitable for printing. Mjd=0 all day Jan 1, 1950. Mjd=9861+day+(year-1977)
+ient((year-1977)/4). This subroutine only works from March 1,1900 till
Feb 28 2100. The algorithm comes from the hp67 calendar function in the 
standard pac write up.*/

# include <string.h>
# include <stdio.h>
#include "../main/C.h"

char *jdate(short mjd)
{
	static char s[9];
	register int day,m,y;
	day = mjd+18326;
	y = (day - 122.1)/365.25;
	day -= (int)(365.25*y);
	m = day/30.6001;
	day -= (int)(m*30.6001);
	if( m > 13)
		m -= 13;
	else
		m--;
	if( m <= 2 )
		y++;
	sprintf(s,"%d/%d/%d",m,day,y);
	return(s);
}
#endif  /*0*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "C.h"

char *jdate(time_t clock)
{
	static char s[20];
	struct tm *tm;
	tm = gmtime(&clock);
/* change to 4 digit year and use FITS date format CCYY-MM-DDThh:mm:ss
 * - mwp 1999-12-16
 */
	sprintf(s, "%4d-%02d-%02dT%02d:%.2d:%.2d", 
		1900+tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);
	return(s);
}


/**
 * return the formatted time given the J2Second value.
 * J2Second is seconds (+/-) since J2000. See blsdf
 */
char *j2sToDate(int j2second) {
	return jdate(j2sToUnix(j2second));
}

/**
 * return the J2Second value given a properly formatted date
 * improper format returns 0, which is ignored by pl.c and image.c.
 * Format must be FITS date format CCYY-MM-DD[Thh:mm:ss[.sss...]]
 */
int dateToJ2S(char *s) {

	int length;
	char timestr[9];
	char yearstr[5];
	char monstr[3];
	char daystr[3];
	char hour[3], min[3], *second;
	double fsec;
	int i;
	struct tm obstime;

	bzero(timestr,9);
	bzero(monstr,3);
	bzero(daystr,3);
	bzero(yearstr,5);
	bzero(min,3);
	bzero(hour,3);

	length=strlen(s);
	if(length<10) return 0; /* illegal, CCYY-MM-DD is the minimum*/
	if(length<19) return 0; /* if a time tag, then CCYY-MM-DDThh:mm:ss
				 * is the minimum 
			         */
	if(s[10]!='T') return 0; /* must have time marker */
	if(s[13]!=':') return 0; /* must have colon-separated tokens */
	if(s[16]!=':') return 0; /* must have colon-separated tokens */

/* convert ymd */
	strncpy(yearstr,s,4);
	obstime.tm_year=atoi(yearstr)-1900;

	strncpy(monstr,&s[5],2);
	obstime.tm_mon=atoi(monstr)-1;

	strncpy(daystr,&s[8],2);
	obstime.tm_mday=atoi(daystr);

/* now convert hms */
	strncpy(hour,&s[11],2);
	obstime.tm_hour=atoi(hour);

	strncpy(min,&s[14],2);
	obstime.tm_min=atoi(min);

	second=&s[17];
	fsec=atof(second);
	obstime.tm_sec=round(fsec);

/*fprintf(stderr, "year month-1 day hr min %d %d %d %d %d %d\n", 
		obstime.tm_year,obstime.tm_mon,obstime.tm_mday,
		obstime.tm_hour,obstime.tm_min,obstime.tm_sec);
*/
 	putenv("TZ=GMT"); /* set the timezone to UT */
	return unixToJ2S(mktime(&obstime));
	
}


/**
 * return J2Second value given unix time (seconds since 1970-01-01)
 */
int unixToJ2S(time_t clock) {
	int j2second;
	j2second = clock-(10957 * 86400 + 43200);
	return j2second;
}

/**
 * return unix time given j2second
 */
time_t j2sToUnix(int j2second) {
	time_t converted;
 /* convert to unix time, seconds since 1970-01-01, e.g. scan_.iobbsec */
	converted=(time_t) (j2second+(10957 * 86400 + 43200));
	return converted;
}

#if 0
void prdate_(clock)
time_t *clock;			/* Time as sec since 1/1/70 in UNIX tradition */
{
	puts(jdate(*clock));
}
#endif /*0*/

#if 0
void prdate_(mjd,fday)
short *mjd;			/* modified Julian date as in OBS scan hdr */
short *fday;			/* Time as 15 bit fraction of day */
{
	char ts[32];

	strcpy(ts,jdate(*mjd));
	HmsPrint(ts,0,*fday / 682.66666667);
	puts(ts);
}
#endif /*0*/
