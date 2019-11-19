#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "C.h"
#include "include.h"
# include "machinedep.h"


#if ! SYSTEM_V & ! LINUX
extern char *sys_siglist[];
#endif /*! SYSTEM_V */
/* mwp 1999-12-22. jansky does not have strsignal.
 * so make one up. just returns the signal back
 */
#if HP_700
char *strsignal(int sig) {
	char s[8];
	bzero(s);
	(void)sprintf(s," %d ",sig);
	return s;
}
#endif

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* faerr.c */
static void fkerr P_((char *s));
#undef P_

void faerr(int sig)
{
	int x, frkd, mfrkd;
	void (*istt)(), (*qstt)(), (*lstt)();
	time_t clock;
	char lbfr[128];
	char tempstr[128];
	char *ctime(), *getlogin();
	FILE *ofp, *fopen();
	int answer;

	/* clean up terminal */
#if COMB
	Pabort();
#endif /* COMB */
	fprintf(stderr,"*** \007SOFTWARE PROBLEM\007 ***\n");
	fprintf(stderr, "Caught signal: %s in command %s.\n",
#if ! SYSTEM_V && !LINUX
	sys_siglist[sig], cmnd_.nc);
#else /*! SYSTEM_V*/

	strsignal(sig), cmnd_.nc);
#endif /*! SYSTEM_V */
	fprintf(stderr,"Do you want to make a record? ");
	answer = getchar();
	while(getchar() != '\n')
		;
	if( answer != 'y' ) {
		error_(0);
	}

	fprintf(stderr,"Making record of error\n");
	fflush(stderr);
	istt = signal(SIGINT, SIG_IGN);
	qstt = signal(SIGQUIT, SIG_IGN);
	lstt = signal(SIGILL, SIG_DFL);
	if ((mfrkd = fork()) < 0) {
		signal(SIGINT, istt);
		signal(SIGQUIT, qstt);
		signal(SIGILL, lstt);
		error_("Can't make record of software problem");
	}
	if (mfrkd == 0) {
		fclose(stderr);		/* Ensure an open file pointer slot */
		sprintf(tempstr,"%s/lib/cerr", main_.comb);
		if ((ofp = fopen(tempstr, "a")) == NULL) {
			sprintf(tempstr, "Can't open %s/lib/cerr",main_.comb);
			fkerr(tempstr);
		}
		if ((frkd = fork()) < 0)
			fkerr("Can't fork for stack trace");
		if (frkd == 0) {
			abort();
			exit(0);	/* In case some clown removed abort() */
		}
		if (frkd > 0)
			wait(0);
		for (x = 1; x <= 33; x++)
			fputc('-', ofp);
		fputc('\n', ofp);
		time(&clock);
		fprintf(ofp, "Error at %s", ctime(&clock));
#if ! SYSTEM_V && !LINUX
		fprintf(ofp, "Caught signal: %s.\n", sys_siglist[sig]);
#else /*! SYSTEM_V*/
		fprintf(ofp, "Caught signal: %d.\n", sig);
#endif /*! SYSTEM_V */
		fprintf(ofp, "User: %s\n", getlogin());
		fprintf(ofp, "Command: %s\n", cmnd_.nc);
		fprintf(ofp, "Stack trace:\n");
		fclose(ofp);
		sprintf(lbfr,
#if LINUX
			"gdb %s/bin/%s core <%s/lib/gdbin >>%s/lib/cerr",
#else /* LINUX */
			"/bin/adb %s/bin/%s core <%s/lib/adbin >>%s/lib/cerr",
#endif /* LINUX */
			main_.comb,interrupt.name,main_.comb,main_.comb);
		eshell_(lbfr);
		sprintf(lbfr,"%s/lib/cerr",main_.comb); 
		chmod(lbfr, 0666);
		unlink("core");
		exit(0);
	}
	if (mfrkd > 0)
		wait(0);
	signal(SIGINT, istt);
	signal(SIGQUIT, qstt);
	signal(SIGILL, lstt);
	error_("Please inform a programmer that there is an error");
}

static void fkerr(char *s)	/* Error routine for the child */
{
	fprintf(stderr,"%s\n", s);
	exit(0);	/* This really just exits the child */
}
