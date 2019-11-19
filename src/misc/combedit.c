/*
 * edit - edit a string according to edstr
 *	  source is the string to be edited
 *	  edstr is in the form ".substr.replacement."
 *	      where "." is any character, substr is the substring to
 *	      be replaced and replacement is the replacement
 */

# include <string.h>
# include "../main/machinedep.h"
 
#if SYSTEM_V
#define bcopy(s, d, n) memmove(d, s, n)
#define bzero(s, n) memset(s, 0, n)
#endif /*SYSTEM_V*/

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* combedit.c */
static void replace P_((char *rplb, char *rple, char *stre, char *str));
#undef P_

char *edit(char *source,char *iedstr,int mlen)
{
	char edbuf[100];
	char *stp,*rplc;
	register char *strt,*edstr = edbuf;
	int len;
	if(strlen(iedstr) > (unsigned)98)
		error_("Edit string too long");
	(void)strcpy(edstr,iedstr);
	if(!(rplc = strchr(edstr+2,*edstr)))
		error_("Bad syntax of edit string");
	*rplc++ = 0;
	if((strt = strchr(rplc,*edbuf)))
		*strt = 0;
	len = strlen(++edstr);
	if((int)strlen(rplc) - len > mlen)
		error_("Replacement string too long");
	strt = source - 1;
	do
		if(!(strt = strchr(strt + 1,*edstr)))
			error_("Substring not found");
	while(strncmp(edstr,strt,len));
	stp = strt + len;
	replace(strt,stp,strchr(strt,0),rplc);
	return(iedstr + (strchr(rplc,0) - edbuf) + 1);
}

/*
 * replace - replace a portion of a string
 */
static void replace(char *rplb,char *rple,char *stre,char *str)
{
	int mov;
	char *movt;
	mov = stre - rple + 1;
	movt = rplb + strlen(str);
	bcopy(rple,movt,mov);
	while(*str)*rplb++ = *str++;
}
