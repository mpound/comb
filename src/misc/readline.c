/*
 * Read a line from a given FILE into a given buffer passing over '\' '\n'
 * pairs (escaped newlines).  Read a maximum of NTTYLEN chars.
 * Returns 0 normally, 1 if line too long, or EOF.
 */

#include <stdio.h>
#include "../main/C.h"
#include "../main/machinedep.h"

/* No libedit here */
int ReadLine(char *buf,FILE *fp)
{
	char *cp = buf;
	char  *endp = &buf[NTTYLEN];
	int c;
	int prevChar, inComment;

	prevChar = 0;
	inComment = 0;
	while(cp < endp) {
		if((c = getc(fp)) == EOF) {
			*cp = 0;
			return(EOF);
		}
		if(c == '\n') {
			if(prevChar == '\\') {
			    if(inComment)
				inComment = 0;	/* Comments stop at eol */
			    else
				cp--;		/* Erase it and ignore \n */
			} else {
				*cp = 0;
				return(0);
			}
		} else if(c == '/' && prevChar == '/') {
			inComment = 7;
			cp--;			/* Ignore both '/' */
		} else if(!inComment) {
			*cp++ = c;
		}
		prevChar = c;
	}
	return(1);
}

#if COMB
/* Used in macro.c */
int SkipLine(fp)
FILE *fp;
{
    for(;;) {
	switch(getc(fp)) {
	case '\\':
		getc(fp);
		break;
	case '\n':
		return(0);
	case EOF:
		return(EOF);
	}
    }
}
#endif /*COMB*/

/*
 * Read a line from the terminal using the ksh history mechanism if activated.
 * Do the same processing as for ReadLine.
 */
int TReadLine(char *buf)
{
	char ttybuf[NTTYLEN];		/* Char from edit_read or terminal */
	char *ip;			/* Pointer to next char in ttybuf */
	int cnt;			/* count of remaining char in ttybuf */
	char *cp = buf;
	char  *endp = &buf[NTTYLEN];
	int c;
	int prevChar, inComment;

	prevChar = 0;
	inComment = 0;
	cnt = 0;
	while(cp < endp) {
		if(cnt <= 0) {
#if HISTORY
			cnt = edit_read(0, ttybuf, NTTYLEN);
#else /*HISTORY*/
			fgets(ttybuf, NTTYLEN, stdin);
			cnt = strlen(ttybuf);
#endif /*HISTORY*/
			if(cnt <= 0) {
				*cp = 0;
				return(EOF);
			}
			ip = ttybuf;
		}
		c = *ip++; cnt--;
		if(c == '\n') {
			if(prevChar == '\\') {
			    if(inComment)
				inComment = 0;	/* Comments stop at eol */
			    else
				cp--;		/* Erase it and ignore \n */
			} else {
				*cp = 0;
				return(0);
			}
		} else if(c == '/' && prevChar == '/') {
			inComment = 7;
			cp--;			/* Ignore both '/' */
		} else if(!inComment) {
			*cp++ = c;
		}
		prevChar = c;
	}
	return(1);
}

#if 0
char buf[NTTYLEN];
main()
{
	while(ReadLine(buf,stdin) != EOF)
		printf("%s|\n",buf);
}
#endif
