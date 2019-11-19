#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
# include "../../main/C.h"
#include "../../stacks/C.h"
# include "../../main/machinedep.h"
#if LYNX
#define vfork fork
#endif /*LYNX*/

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* uf.c */
static void CloseError P_((int *pfd1, int *pfd2, char *s));
#undef P_

void uf()
{
	/* Pipe will set up each of these so that [0] is for reading and [1]
	 * is for writing */
	int uinpfd[2], uoutpfd[2];	/* user input pipe, user output pipe */
	int childPid, deadPid;
	int n, nread, nwrite;
	void (*sigSave)();
#if SYSTEM_V || LINUX
	int status;
#else
	union wait status;
#endif
# include "uf.dc.h"
# include "uf.ec.h"
	uinpfd[0] = uinpfd[1] = 0;
	uoutpfd[0] = uoutpfd[1] = 0;
	if(pipe(uinpfd) < 0 || pipe(uoutpfd) < 0)
		CloseError(uinpfd, uoutpfd, "Making pipes");
	if((childPid = vfork()) == 0) {	/* The Child */
		char *shortName;
		char pin[4], pout[4];

		close(uinpfd[1]);
		close(uoutpfd[0]);
		sprintf(pin, "%d", uinpfd[0]);
		sprintf(pout, "%d", uoutpfd[1]);
		if(shortName = strrchr(nam, '/'))
			shortName++;
		else
			shortName = nam;
		(void)execlp(nam, shortName, pin, pout, (char *)0);
		_exit(127);
	} else if(childPid < 0)
		CloseError(uinpfd, uoutpfd, "Forking");

	/* If we get here, we are the parent comb process */
	close(uinpfd[0]);
	close(uoutpfd[1]);
	sigSave = signal(SIGPIPE, SIG_IGN);
	if((nwrite=write(uinpfd[1], &stk_[0], sizeof(struct of_stk))) !=
			sizeof(struct of_stk)) {
		fprintf(stderr, "Write to outgoing pipe failed (%d)\n",nwrite);
		perror("Failure was ");
	}
	signal(SIGPIPE, sigSave);
	close(uinpfd[1]);
	for(nread = 0; nread < sizeof(struct of_stk); nread += n) {
		if((n = read(uoutpfd[0], nread + (char *)&stk_[0],
				sizeof(struct of_stk) - nread)) <= 0) {
			fprintf(stderr, "Only %d bytes of stack returned\n",
				nread);
			break;
		}
	}
	if(coreFull[0] = (nread == sizeof(struct of_stk) && stk_[0].numpt > 1
			&& stk_[0].nlist > 0))
		main_.pltd = 0;
	close(uoutpfd[0]);
	while((deadPid = wait(&status)) != childPid && deadPid != -1) {
		printf("Child %d died\n", deadPid);
	}
	if(!WIFEXITED(status))
		error("Error executing user function");
#if 0
	printf("Child %d died\n", deadPid);
#endif /*0*/
}

static void CloseError(pfd1, pfd2, s)
int *pfd1, *pfd2;
char *s;
{
	if(pfd1[0] > 0) close(pfd1[0]);
	if(pfd1[1] > 0) close(pfd1[1]);
	if(pfd2[0] > 0) close(pfd2[0]);
	if(pfd2[1] > 0) close(pfd2[1]);
	perror("");
	error(s);
}
