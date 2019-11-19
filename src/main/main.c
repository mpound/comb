# include <stdio.h>
# include <fcntl.h>
# include <ctype.h>
# include <signal.h>
# include <string.h>
# include <stdarg.h>
# include "C.h"
# include "include.h"
# include "machinedep.h"
# include "dragon.h"
# include "../misc/C.h"
# include "../parse/P.cleanuservars.h"
# if COMB
# include "../stacks/C.h"
# include "../scan/C.h"
# include "../coordsys/C.h"
# include "../stacks/include.h"
# include "../graphics/C.h"
# include "../image/C.h"
#endif /* COMB */

# include "../error/C.h"


static int ncmds;		/* number of commands in this version of comb*/
extern char gfilnm[];

char *getenv();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* faerr.c */
void faerr P_((int sig));

#undef P_

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* main.c */
static void CombInit P_((void));
static void history P_((char *cmd, char *hist));
static void rdtty P_((char *string));
static void excmd P_((void));
static void beep2 P_((void));
static void cleanup P_((void));
static void onintr P_((int));
static void inton_ P_((void));
#undef P_

	/****************************/
	/* main - main comb routine */
	/****************************/

main(argc,argv) 
	int argc;		/* number of arguments */
	char **argv;		/* values of arguments */
{
					/* updated) */
	static void (*istat)();		/* status of interrupt */
	char newstr[NTTYLEN];		/* new command string */

	/* the following is static to avoid a possible hpux problem that
	 * automatic variables may be undefined on return from a longjmp
	 */
	static char hist[NTTYLEN];	/* old command string */

#if HISTORY
#if 0
	static char stdinbuf[BUFSIZ];
#endif
	static char histfile[48];
	/* save pristene tty state for restoring after an interrupt in which
	 * libedit had the tty in raw mode */
/*	ioctl(0, TCGETS, &termState); */
	/* set history file name in the environment for libedit */
	sprintf(histfile, "HISTFILE=%s/.combhistory", getenv("HOME"));
	putenv(histfile);
#if 0
	setbuf(stdin, stdinbuf);
#endif
#else /*HISTORY*/
#if ! SYSTEM_V
	(void)setlinebuf(stdout);
	(void)setlinebuf(stderr);
#endif /*! SYSTEM_V */
#endif /* HISTORY */

	setjmp(interrupt.sjbuf);
	/* print greeting */
	printf("Welcome to %s %s\n\n",argv[0],VERSION);

	/* store name used to invoke comb for other routines to reference */
	interrupt.name = argv[0];

#if 0
	/* initialize fortran */
	f_init();
#endif

	/* initialize a few things */
	CombInit();
	/* initialize signal handling */
#if 0
	set_stak (argv [0]);		/* initialize stack trace variables */
#endif
        istat = signal(SIGINT,SIG_IGN);
	(void)signal(SIGPIPE,SIG_IGN);
	setjmp(interrupt.sjbuf);
	setjmp(interrupt.retbuf);
	signal(SIGFPE, faerr);
	signal(SIGBUS, faerr);
	signal(SIGILL, faerr);
	if(istat != SIG_IGN)
		signal(SIGINT,onintr);
	interrupt.intflg = 0;
	interrupt.depth = 0;
#if HISTORY
	fflush(stderr);
	fflush(stdout);
/*	ioctl(0, TCSETS, &termState); */
	tty_cooked(0);
#if ! SYSTEM_V
	setlinebuf(stdout);
	setlinebuf(stderr);
#endif /* ! SYSTEM_V */
#endif /* HISTORY */
	ResetStdout();		/* This fixes up redirections of lc and ?? */

	/* main loop */
	while(1)
	{

		/* Make sure that macroDepth is set and user local variables
		 * have been deleted */
		main_.macroDepth = 0;
		CleanUserVars();

		/* get command string */
		printf("\n-> ");
#if LINUX
		fflush(stdout);
#endif
		rdtty(newstr);

		/* if first character of newstr is '.', execute */
		/* history commands */
		if(*newstr == '.')
			history(newstr + 1,hist);

		/* otherwise, execute command string */
		else
		{
			strcpy(hist,newstr);
			combex_(newstr);
		}
		ResetStdout();

#if COMB
		SearchFlush(-1);  /* Flush search file buffers if suppressed */
		/* if plot flag is set after executing command, */
		/* execute pl */
		if( main_.pltd == 0 && main_.apfl && coreFull[0])
			combex_("pl");
#endif /* COMB */
	}
}

	/**************************************
	 * CombInit - initialize a few things *
	 **************************************/

static void CombInit()
{
	register char *cp;
	int fd;
	int n;
	char buff[1024];

	/* get the main directory path for comb */
	if((cp = getenv("COMB")) != NULL)
		strcpy(main_.comb,cp);
	else {
		fprintf(stderr, "Please define COMB in your environment before running comb\n");
		exit(1);
	}
	sprintf(buff,"%s/lib/hello", main_.comb);
	if((fd = open(buff, O_RDONLY)) > 0) {
		while((n = read(fd, buff, sizeof(buff)))
			> 0)
			write(1,buff,n);
		close(fd);
	}

/* print the news headlines */
	system("grep '^[1-9]' $COMB/doc/online/news.doc");

        if((cp = getenv("EDITOR")) == NULL)
               fprintf(stderr,"\nYou don't have EDITOR defined in your environment.\nThe default is /bin/ed.\n");

#if COMB
	if((cp = getenv("LSTACKS")) != NULL)
		strncpy(main_.stkpth,cp,PATHLENGTH);
	else
		*main_.stkpth = '\0';
	if((cp = getenv("SCANFILE")) != NULL) {
		strncpy(scan_.datfn,cp,PATHLENGTH);
		scan_.ndfn = strlen(cp);
		memcpy(curscn_.fil, &cp[scan_.ndfn-3], 4);
		curscn_.num = 1;
	}
#endif /* COMB */

	/* read in list of macros */
	sprintf(gfilnm, "%s/lib/.GMACROS", main_.comb);
	rdmac();

	/* set ncmds */
	ncmds = numcmds();

	/* set up dragon */
	for(dragon_.f = 2; dragon_.f < 1e10;dragon_.f *= 2 * dragon_.f)
		;
	dragon_.f *= dragon_.f * 127. / (float)64;
	dragon_.d = dragon_.f;

	/* positive quiet NaN independent of byte order */
	*(int *)&dragon_.fNaN = 0xffffffff;
	*(int *)&dragon_.NaN = 0x7fffffff;
	((int *)&dragon_.NaN)[1] = 0xffffff7f;

	/* set up constants */

	constants_.AMU=1.660540E-24;
	constants_.C=2.99792458E10;
	constants_.E=2.7182818284590452354;
	constants_.G=6.67259E-8;
	constants_.K=1.380658E-16;
	constants_.MSUN=1.989E33;
	constants_.PC=3.085678021E18;
	constants_.Pi=3.14159265358979323846;
}


	/*********************************************/
	/* combex - execute a command string in comb */
	/*********************************************/

void combex_(str)
	char *str;		/* string to be executed */
{
	char *tstr;		/* temporary string pointer to save previous
				 * cmnd_.gstr and allow recursive calls */
	register char *cstr;		/* command string pointer */
	char old_nc[4];		/* save command name for errors in commands
				that call combex */

	/* check that command string is worth executing */
	if(!*str)
		return;

	/* set the global string pointer */
	tstr = cmnd_.gstr;
	cmnd_.gstr = str;
	strncpy(old_nc,cmnd_.nc,4);

	/* execute commands while the global string pointer points to them */
	do
	{

		/* pass all spaces and tabs */
		while(*cmnd_.gstr == ' ' || *cmnd_.gstr == '	')
			cmnd_.gstr++;

		/* extract command */
		for(cstr = cmnd_.nc;cstr < cmnd_.nc + 4;cstr++)
		{
			if((*cstr = *cmnd_.gstr++) == ' ' ||
			   *cstr == '	' ||
			   *cstr == ',' ||
			   *cstr == ';' ||
			   !*cstr)
				break;
		}
		cmnd_.gstr--;
		*cstr = ' ';

               /* If there is something, execute either command or macro */
                if(cstr == cmnd_.nc + 4)
                        exmac();        /* check for the macro name too long? */
                else if(cstr > cmnd_.nc) {
                        *++cstr = 0;
                        excmd();
                } else if(*cmnd_.gstr) {
                        cmnd_.gstr++;
                }

	}
	while(*cmnd_.gstr);

	/* restore cmnd_.gstr */
	cmnd_.gstr = tstr;
	strncpy(cmnd_.nc,old_nc,4);
}

	/**************************************/
	/* history - execute history commands */
	/**************************************/

static void history(cmd,hist)
char *cmd;		/* command string */
char *hist;		/* old command string */
{
	static char arch[10][NTTYLEN];	/* list of archived command strings */
	char errstr[26];	/* error string */
	char *edit();

	do
	{
		switch(*cmd)
		{

			/* if user typed only '.' or typed an 'e', */
			/* execute old command string */
			case 0:
			case 'e':
				combex_(hist);
				break;

			/* if user typed 'p', print old command string */
			case 'p':
				printf("%s\n",hist);
				break;

			/* if user typed 'a', archive command string */
			case 'a':
				if(*++cmd < '0' || *cmd > '9')
					error_("Illegal archive number");
				strcpy(arch[*cmd - '0'],hist);
				break;

			/* if user typed just a number, recover archive */
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				strcpy(hist,arch[*cmd - '0']);
				break;

			/* if user typed 's', edit string */
			case 's':
				cmd = edit(hist,cmd + 1,
					   NTTYLEN - strlen(hist) - 1) - 1;
				break;

			/* ignore spaces, tabs, and commas */
			case ' ':
			case '	':
			case ',':
				break;

			/* print error message if character isn't found */
			default:
				sprintf(errstr,"'%c' - bad history command",
					*cmd);
				error_(errstr);
		}
	}
	while(*cmd && *++cmd);
}

	/*********************************************/
	/* rdtty - read command string from terminal */
	/*********************************************/

static void rdtty(string)
char *string;		/* string to read into */
{
	register int i;

	/* set command to COMB */
	strncpy(cmnd_.nc,"COMB",4);

	/* read string */
	if( (i = TReadLine(string)) ) {
		if(i == EOF) {
			putc('\n', stdin);
			strcpy(string,"q");
			return;
		} else {
			rdtty(string);
			error_("Command too long");
		}
	}

	/* get string signature */
	cmnd_.cmdsig++;
}

	/***************************/
	/* excmd - execute command */
	/***************************/

static void excmd()
{
	extern Icmd icmd[];	/* command list */
	Icmd *cmd = icmd + ncmds;	/* command being checked */
	Icmd *tcmd;
	register int step = ncmds + 1;
	register int diff;
	register int c0;			/* first char of command */

	c0 = cmnd_.nc[0];
	do {
		step++;
		step >>= 1;
		if((tcmd = cmd - step) >= icmd) {
		    if((diff = tcmd->cmd[0] - c0)>= 0) {
			if(diff == 0) {
				if((diff = (tcmd->cmd[1] - cmnd_.nc[1])) < 0)
					continue;
			}
			if(diff == 0) {
				(*tcmd->cp)();
				if(interrupt.depth) {
					fprintf(stderr,
			"Internal error - %s completed with interrupt off",
					cmnd_.nc);
					inton_();
				}
				return;
			}
			cmd = tcmd;
		    }
		}
	} while(step > 1);
	error_("Command not found");
}
	
#if 0
	register Icmd *cmd;	/* command being checked */
	register Icmd *end;	/* last cmd in list */

	end = icmd + ncmds;
	for(cmd = icmd; cmd < end; cmd++)
	{
		if(cmd->n == *(short *)cmnd_.nc)
		{
			(*cmd->cp)();
			if(interrupt.depth) {
				fprintf(stderr,
		"Internal error - command %s completed with interrupt off",
				cmnd_.nc);
				inton_();
			}
			return;
		}
	}
	error_("Command not found");
}
#endif

	/*********************************/
	/* error - general error routine */
	/*********************************/

void error_(sp)
char *sp;		/* message to be printed */
{
	if(sp) {
		cleanup();
		fprintf(stderr,"\07%s in cmd %s\n",sp,cmnd_.nc);
		beep2();
		longjmp(interrupt.sjbuf,0);
	}
	longjmp(interrupt.retbuf,0);
}

/*VARARGS*/
void error(char *s, ...)
{
	char buf[1024];
	va_list ap;

	va_start(ap, s);
	vsprintf(buf, s, ap);
	va_end(ap);
	error_(buf);
}

static void beep2()
{
	int i;
	for(i = 0;i < 1000;i++) ;
	putc('\07',stderr);
}

	/***********************************************************/
	/* warn - error routine which asks if you want to continue */
	/***********************************************************/

void warn_(sp)
	char *sp;		/* message to be printed */
{
	char answer;		/* answer to warning */

#if COMB
	/* clean up terminal */
	Pabort();
#endif /* COMB */

	if(main_.warny) {
		fprintf(stderr, "\nIn command %s %s.\nAutomatic continue\n",
			cmnd_.nc, sp);
		return;
	}
	fprintf(stderr,"\nIn command %s %s.\nDo you want to continue\07? ",
		cmnd_.nc,sp);
	answer = tolower(getchar()); /* Y is ok */
	while(getchar() != '\n')
		;
	if( answer != 'y' ) {
		cleanup();
		longjmp(interrupt.sjbuf,0);
	}
}

/*VARARGS*/
void warn(char *s, ...)
{
	char buf[1024];
	va_list ap;

#if COMB
	Pabort();
#endif /* COMB */

	va_start(ap, s);
	vsprintf(buf, s, ap);
	va_end(ap);
	warn_(buf);
}

/*
 * cleanup things for error and warn
 */
static void cleanup()
{
#if COMB
	Pabort();
#endif /* COMB */
	ResetStdout();
#if COMB
	CloseFitsOut();
	SearchFlush(-1);	/* Flush search file buffers if suppressed */
#endif /* COMB */
	CloseAll();
/* if command was ns or up, close the directory */
	if(strcmp(cmnd_.nc,"ns")==0 || strcmp(cmnd_.nc,"up")==0) 
		CloseDir();
	ReleaseAllSafe();
}
	/*******************************************/
	/* wait - wait for the user to type return */
	/*******************************************/

void wait_()
{
	while(getchar() != '\n')
		;
}

	/*******************************/
	/* onintr - called on interrupt */
	/*******************************/

static void onintr(dmy)
int dmy;
{
	/* if depth > 0, then the current program
	can't be interrupted.  increment intflg to indicate
	an interrupt was sent */

	if(interrupt.depth) {
		interrupt.intflg++;
	} else {

	/* otherwise, clean up and go back to main loop */
		cleanup();
		fprintf(stderr,"\nInterrupt\n");
		longjmp(interrupt.sjbuf,0);
	}
}

/***********************************************************************
 * noint_ suspend responding to interrupts  or increase inhibit depth **
 ***********************************************************************/

void noint_()
{
	interrupt.depth++;
}

/*********************************************************************
 * yesint_ turn on interrupt or decrease depth of interrupt inhibit **
 *********************************************************************/

void yesint_()
{
	if(--interrupt.depth <= 0) {
		if(interrupt.depth < 0) {
			interrupt.depth = 0;
			error_("internal error - more yesint than noint calls");
		}
		if(interrupt.intflg)
			onintr(0);
	}
}

/**********************************************************
 * inton_ turn interrupts on regardless of inhibit depth **
 **********************************************************/

static void inton_()
{
	if(interrupt.intflg)
		onintr(0);
	interrupt.depth = 0;
}
