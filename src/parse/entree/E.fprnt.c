# include <stdarg.h>
# include <stdio.h>
# include <string.h>
# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"


	/****************************************************************/
	/* fprnt - print something into a file with a line length limit */
	/****************************************************************/

#if 0
fprnt(fc,max,cnt,strt,va_alist)
	FILE *fc;			/* file to be printed into */
	int max;			/* maximum line length */
	int *cnt;			/* pointer to character counter */
	char *strt;			/* string to put at the start of line */
	struct ... args;		/* arguments for sprintf */
#endif /*0*/

void fprnt(
	FILE *fc,			/* file to be printed into */
	int max,			/* maximum line length */
	int *cnt,			/* pointer to character counter */
	char *strt,			/* string to put at the start of line */
	char *fmt,			/* format for sprintf */
	...)
{
	char str[1000];			/* temporary string buffer */
	char *tmp;			/* temporary pointer to character */
	int len;			/* length of string */
	int tlen;			/* length of string before line feed */
	va_list ap;

	va_start(ap, fmt);
	vsprintf(str, fmt, ap);
	va_end(ap);

	/* print line feed and strt if string goes off end of line */
	len = strlen(str);
	tlen = (tmp = strchr(str,'\n')) ? tmp - str : len;
	if(*cnt + tlen > max)
	{
		fprintf(fc,"\n%s",strt);
		*cnt = strlen(strt);
	}

	/* compute new cnt */
	*cnt = (tmp = strrchr(str,'\n')) ? str + len - tmp - 1 : *cnt + len;

	/* print string into file */
	fprintf(fc,"%s",str);
}
