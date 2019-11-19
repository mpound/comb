# include <fcntl.h>
# include <stdio.h>
# include "C.h"
#if COMB
# include "../stacks/C.h"
# include "../coordsys/C.h"
# include "../stacks/include.h"
#endif /* COMB */
# include "../error/C.h"
# include "../misc/C.h"
# include "../parse/P.cleanuservars.h"

	/*******************/
	/* macro structure */
	/*******************/

typedef struct
{
	int nam;		/* name of macro */
	int loc;		/* location of macro in file */
}
Macro;

static Macro *macbuf = 0;		/* macro list */
static Macro *macbufe;			/* points 1 byte beyond allocated mem */
/* macros in directory 1 start at the beginning of macbuf */
static Macro *mmac2;			/* beg of macros in directory 2 */
static Macro *macl;			/* beg of local macros */
static Macro *macg;			/* beg of global macros */
static Macro *mace;			/* end of macro list */
char lfilnm[MAXDNLEN + 8];		/* local macro file name */
char gfilnm[MAXDNLEN + 8];		/* global macro file name */
#if COMB
char m1filnm[MAXDNLEN + 8] = "";	/* map macro file for directory 1 */
char m2filnm[MAXDNLEN + 8] = "";	/* map macro file for directory 2 */
#endif /* COMB */

/* memory resident linked list of complete macros that have been used */
struct MacLink {
	int nam;
	struct MacLink *lnk;
	char bod[1];		/* enough space will be allocated for the
				body.  The char bod will be for the NULL */
};
static struct MacLink *macLink = 0;	/* Points to head of linked list */

extern char *getenv();
extern char *strcpy();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* macro.c */
static Macro *rdmacfile P_((char *flnm, register Macro *to));
static void expargs P_((char *str));
static Macro *fmacro P_((register int *name));
static void gmacro P_((register Macro *macro, char *val));
static void RtMacro P_((int *name, char *val));
#undef P_

	/*************************************/
	/* rdmac - read macros in from files */
	/*************************************/

void rdmac()
{
	register FILE *fc;		/* file code */
	register struct MacLink *ml;
	static char noOpenFmt[] = "\n* * CAN'T OPEN MACRO FILE %s * *\n";
	static char noCreatFmt[] = "\n* * CAN'T CREATE %s * *\n";
	static char creatingFmt[] = "\n* * CREATING %s * *\n";

	/* make sure that there is a Macro list */
	if( macbuf == 0) {
		macbuf = (Macro *)ChkAlloc(64 * sizeof(Macro), "Macro List");
		macbufe = &macbuf[64];
	}

	/* clear the linked list of macros */

	while(ml = macLink) {
		macLink = ml->lnk;
		ChkFree((char *)ml);
	}

	/* initialize mace */
	mace = macbuf;

#if COMB
	/* read in macros from directory 1 if open */
	if( !notOpen[0]) {
	    sprintf(m1filnm,"%s/macros",dirName[0]);
	    if(access(m1filnm,0) == -1) {
		fprintf(stderr,creatingFmt,m1filnm);
		if((fc = fopen(m1filnm,"w")) == NULL)
			fprintf(stderr,noCreatFmt,m1filnm);
		else
			fclose(fc);
	    } else if( !(mace = rdmacfile(m1filnm,mace))) {
		mace = macbuf;
		fprintf(stderr,noOpenFmt, m1filnm);
	    }
	}
	mmac2 = mace;

	/* read in macros from directory 2 if open */
	if(notOpen[1] == 0) {
	    sprintf(m2filnm,"%s/macros",dirName[1]);
	    if(access(m2filnm,0) == -1) {
		fprintf(stderr,creatingFmt,m2filnm);
		if((fc = fopen(m2filnm,"w")) == NULL)
			fprintf(stderr,noCreatFmt,m2filnm);
		else
			fclose(fc);
	    } else if( !(mace = rdmacfile(m2filnm,mmac2))) {
		mace = mmac2;
		fprintf(stderr,noOpenFmt, m2filnm);
	    }
	}
#endif /* COMB */
	macl = mace;

	/* read in local macro file */
#if COMB
	sprintf(lfilnm,"%s/.LMACROS",getenv("HOME"));
#else /*COMB*/
	sprintf(lfilnm,"%s/.OBSMACROS",getenv("HOME"));
#endif /* COMB */
	if(access(lfilnm,0) == -1) {
		fprintf(stderr,creatingFmt,lfilnm);
		if((fc = fopen(lfilnm,"w")) == NULL)
			fprintf(stderr,noCreatFmt,lfilnm);
		else
			fclose(fc);
	} else if( !(mace = rdmacfile(lfilnm,macl))) {
		mace = macl;
		fprintf(stderr,noOpenFmt,lfilnm);
	}
	macg = mace;

	/* read in global macro file */
	if(access(gfilnm,0) == -1) {
		fprintf(stderr,creatingFmt,gfilnm);
		if((fc = fopen(gfilnm,"w")) == NULL)
			fprintf(stderr,noCreatFmt,gfilnm);
		else
			fclose(fc);
	} else if( !(mace = rdmacfile(gfilnm,macg))) {
		fprintf(stderr,noOpenFmt,gfilnm);
		mace = macg;
	}
}

	/*******************************************/
	/* rdmacfiles - read macros in from a file */
	/*******************************************/

static Macro *rdmacfile(flnm,to)
	char *flnm;			/* name of file */
	register Macro *to;		/* place to read macros into */
{
	FILE *fc;			/* file code */
	char tempnam[5];

	/* open file */
	if((fc = fopen(flnm,"r")) == NULL)
		return(0);

	/* read in macros */
	/* note that the null termination for nam spills over into loc, but
	that is harmless */
	while(fscanf(fc,"%4s",tempnam) != EOF)
	{
		if(to >= macbufe) {
			int size = macbufe - macbuf + 16;
			Macro *new = (Macro *)ChkRealloc((char *)macbuf, size *
				sizeof(Macro), "Macro List");
			if(new != macbuf) {
				int diff = new - macbuf;
				mmac2 += diff;
				macl += diff;
				macg += diff;
				mace += diff;
				to += diff;
				macbuf = new;
			}
			macbufe = &macbuf[size];
		}
		strncpy(&to->nam, tempnam, 4);
		to++->loc = ftell(fc) + 3;
		(void)SkipLine(fc);	/* skip over the macro body */
	}
	fclose(fc);

	/* return location after last macro */
	return(to);
}

	/*************************/
	/* exmac - execute macro */
	/*************************/


void exmac()
{
	char val[NTTYLEN];	/* expansion of macro */

	/* get macro */
	RtMacro((int *)cmnd_.nc,val);

	/* expand macro */
	expargs(val);

	/* execute macro */
	main_.macroDepth++;
	combex_(val);
	main_.macroDepth--;
	CleanUserVars();
}

	/************************************/
	/* expargs - expand macro arguments */
	/************************************/

static void expargs(str)
	char *str;		/* macro string being expanded */
{
	char arg[10][100];	/* list of arguments */
	register char *argp;	/* pointer to argument being entered */
	register int args;	/* number of arguments entered */
	int arglen[10];		/* length of arguments */
	int qfl;		/* quote flag */
	char quote;		/* quotation mark */
	int bracks;		/* number of brackets */
	char temp[NTTYLEN];	/* temporary string for holding macro */
	register char *p0;	/* pointer to str */
	register char *p1;	/* pointer to temp */

	/* get arguments */
	args = 0;
	argp = arg[0];
	qfl = 0;
	quote = 0;
	bracks = 0;
	cmnd_.gstr++;
	if(*cmnd_.gstr == ';' || !*cmnd_.gstr)
		cmnd_.gstr--;
	while((qfl || bracks || *++cmnd_.gstr != ';') && *cmnd_.gstr)
	{
		if(*cmnd_.gstr == '\'' || *cmnd_.gstr == '"')
		{
			if(qfl)
				if(*cmnd_.gstr == quote)
					qfl = 0;
			else
			{
				qfl = 1;
				quote = *cmnd_.gstr;
			}
		}
		if(*cmnd_.gstr == '{')
			bracks++;
		if(*cmnd_.gstr == '}')
			bracks--;
		if(*cmnd_.gstr == ',' && !qfl && !bracks)
		{
			if(args == 10)
				error_("Too many arguments");
			*argp = 0;
			arglen[args] = argp - arg[args];
			args++;
			argp = arg[args];
		}
		else
		{
			if(*cmnd_.gstr == '\\')
				cmnd_.gstr++;
			*argp++ = *cmnd_.gstr;
		}
	}
	if(arglen[args] = argp - arg[args])
		args++;
	*argp = 0;
	if(*cmnd_.gstr == ';')
		cmnd_.gstr++;
	/* expand macro */
	p0 = str;
	p1 = temp;
	while(*p1 = *p0++)
	{
		if(*p1 == '\\' && *p0 == '!')
			*p1++ = *p0++;
		else if(*p1 == '!')
		{
			if(*p0 < '0' || *p0 > '9')
				error_("Illegal character after '!'");
			if(*p0 - '0' >= args)
				error_("Not enough arguments given");
			strcpy(p1,arg[*p0 - '0']);
			p1 += arglen[*p0 - '0'];
			p0++;
		}
		else
			p1++;
	}
	strcpy(str,temp);
}

	/***********************/
	/* fmacro - find macro */
	/***********************/

static Macro *fmacro(name)
register int *name;		/* name of macro to be found */
{
	register Macro *macro;		/* macro being checked */

	for(macro = macbuf;macro < mace && macro->nam != *name;macro++);
	if(macro == mace)
		return(0);
	return(macro);
}

	/*******************************/
	/* gmacro - get value of macro */
	/*******************************/

static void gmacro(macro,val)
register Macro *macro;		/* macro to get */
char *val;			/* value of macro */
{
	FILE *fc;
#if COMB
	if(macro < mmac2)
		fc = fopen(m1filnm,"r");
	else if(macro < macl)
		fc = fopen(m2filnm,"r");
	else if(macro < macg)
#else /*COMB*/
	if(macro < macg)
#endif /* COMB */
		fc = fopen(lfilnm,"r");
	else
		fc = fopen(gfilnm,"r");
	fseek(fc,macro->loc,0);
	if(ReadLine(val,fc) > 0)
		error("macro body too long");
	fclose(fc);
}

	/**************************************************************
	 * RtMacro - get macro from disk or memory, if not in memory, *
	 * put on linked list
	 **************************************************************/

static void RtMacro(int *name,char *val)
{
	register struct MacLink *ml;
	Macro *macro;
	register int nam = *name;

	/* first try linked list in memory */
	for(ml = macLink; ml; ml = ml->lnk) {
		if(nam == ml->nam) {
			strcpy(val,ml->bod);
			return;
		}
	}
	if( !(macro = fmacro(name)))
		error_("Macro not found");
	gmacro(macro,val);
	/* save this macro at the head of the linked list */
	ml = (struct MacLink *)ChkAlloc(strlen(val) + sizeof(struct MacLink),
		"Macro body");
	ml->nam = nam;
	ml->lnk = macLink;
	strcpy(ml->bod, val);
	macLink = ml;
}

	/*************************************************************/
	/* GetMacro - externally callable routine for getting a macro */
	/**************************************************************/

void GetMacro(macAndArgs,expMac)
	char *macAndArgs;	/* macro with arguments */
	char *expMac;		/* result of expanding value of macro */
{
	char *temp;		/* temporary storage for gstr */

	temp = cmnd_.gstr;
	RtMacro((int *)macAndArgs,expMac);
	noint_();
	cmnd_.gstr = macAndArgs + 3;
	expargs(expMac);
	cmnd_.gstr = temp;
	yesint_();
}
