# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include "C.h"
# include "../../main/C.h"
# include "../../parse/P.tree.h"
# include "../../parse/C.h"
# include "../../parse/P.struct.h"
# include "../../parse/P.constant.h"
# include "../../error/C.h"


static int outFile = -1;		/* fd of file to receive stdout */
static int saveOut = -1;		/* fd of file to save original stdout */
static FILE *pipefd = 0;
static char outName[48];

/* Variables for redirecting to a string (by way of a temporary file */
int gsn=-1;		/* global string number */
int CopyToString=0;	/* flag for ResetStdout to read in tempFile and
			 * copy it to the given string */
HdrVar *hv; 		/* Pointer to header-type string variable which may
			 * be written to. */

#if __STDC__
extern FILE     *popen(const char *, const char *);
#endif

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* redirect.c */
static char *StdoutFile P_((void));
#undef P_

void rout()
{
	static int action = 0;
# include "rout.dc.h"

# include "rout.ec.h"

	if(!*actf)
		action = *act;
	switch(action) {
	case 0:
		RedirStdout(fname,(int)*trf);
		break;
	case 1:
		PipeStdout(pcmd);
		break;
	case 2: 
		RedirToStr(StrNam);
		break;
	case 3:
		ResetStdout();
		break;
	}
# include "rout.uc.h"
}

void rin()
{
	int action;
#include "rin.dc.h"
#include "rin.ec.h"
	if(!*actf)
		action = *act;
	else
		error("No acion given");
	switch(action) {
	case 0:
		RedirStdin(fname);
		break;
	case 1:
		combex_(exstr);
		break;
	}
# include "rin.uc.h"
}

void RedirStdout(fname,truncate)
char *fname;
register int truncate;
{
	if(saveOut >= 0)
		ResetStdout();
	if(outFile == -1 || strcmp(outName,fname) || truncate ) {
		if(outFile > 0) {
			close(outFile);
			outFile = -1;
		}
		truncate = (truncate)? O_TRUNC : O_APPEND ;
		outFile = ChkOpen(fname,fname,O_WRONLY | O_CREAT | truncate,
			0666);
		strcpy(outName,fname);
	}
	fflush(stdout);
	saveOut = dup(1);
	close(1);
	dup(outFile);
}

void PipeStdout(command)
char *command;
{
	if(saveOut >= 0)
		ResetStdout();
	if((pipefd = popen(command, "w")) == 0)
		error("Can't open pipe %s", command);
	fflush(stdout);
	saveOut = dup(1);
	close(1);
	dup(fileno(pipefd));
}

int PageStdout()
{
	if(saveOut < 0) {
		PipeStdout("${PAGER-more}");
		return(1);
	} else {
		return(0);
	}
}

void ResetStdout()
{

	if(CopyToString==1) { /* Take care of any outstanding ro str: */
	    int nbyte;
	    char *tstring;

            /* get the size of the string in dmyfile by lseeking 
             * to the EOF */
	    fflush(stdout);
	    nbyte = lseek(1, 0L, SEEK_CUR);

	    /* Make space and read the string */
	    tstring = ChkAlloc(nbyte + 1, "Memory to receive redir. stdout");
#if 0
fprintf(stderr, "String size is %d, tstring is at %d\n", nbyte, tstring);
#endif
	    lseek(1, 0L, SEEK_SET);
	    if(read(1, tstring, nbyte) != nbyte)
		error("trouble reading redirected string");
	    tstring[nbyte] = 0;
 
	    /* now put the string in its destination string */
            if(gsn > -1) {	/* global string */
		if(sg_[gsn]) 
		    ChkFree(sg_[gsn]);
		sg_[gsn] = tstring;
            } else {		/* hdr string variable */
		asgnstrhdr(hv, tstring);  /* This actually copies the string */
		ChkFree(tstring);
	    }
	    CopyToString=0;
	    gsn=-1;
	}

	/* Now do the regular stdout reset */
	if(saveOut > 0) {
		fflush(stdout);
		close(1);
		dup(saveOut);
		close(saveOut);
		saveOut = -1;
	}
	if(pipefd) {
		pclose(pipefd);
		pipefd = 0;
	}
}

static char *StdoutFile()
{
	if(!outName[0])
		sprintf(outName,"%s/comb.out",getenv("HOME") );
	return(outName);
}

void RedirStdin(fname)
char *fname;
{
	register FILE *fp;
	register int i;
	char newstr[NTTYLEN];

	fp = FSafeOpen(fname,fname,"r");
	while( !(i = ReadLine(newstr,fp)) ) {
		combex_(newstr);
	}
	FSafeClose(fp);
	if(i > 0) {
		strcpy(cmnd_.nc,"ri");
		error_("Combined command line too long");
	}
}

/* for "ro str:" -- MWP 6/20/96  with improvements by RWW 6/25/96 */
void RedirToStr(char *StrNam) 
{
	char *cp;
	HdrVar *findhdr(char *vname);
	static int dmyfd = -1;
	static char dmyname[] = "/tmp/combtoStr.XXXXXX";

	/* First do some checking on the string variable. 
	 * I think it is safer to do this before any redirection 
	 * to the temporary file.
	 */
        switch(StrNam[0]) {
        case '$': /* global string variable */
                if(isdigit(StrNam[1]))  /* get the number of the global string */
                        gsn=StrNam[1]-'0';
                else
                        gsn=0;
        	break;
        default: /* header string variable */
                if((hv = findhdr((*StrNam == '.') ? StrNam + 1 : StrNam))==NULL)
                        error("Variable %s not found",
			       (*StrNam == '.')? StrNam + 1: StrNam);
                if(hv->tp != HVSTRING && hv->tp != SCAN2STR && hv->tp != UVSTR)
                        error("%s is not a string variable",StrNam);
                break;
	}

	/* Has the file to catch the stdout for strings been opened yet? */
	if(dmyfd == -1) {
	    if(mktemp(dmyname) == 0)
		error("Could not make a temporary file");
	    dmyfd = ChkOpen(dmyname, dmyname, O_RDWR | O_CREAT | O_TRUNC,
		0666);
	    /* Unlink removes the directory entry but not the file. 
             * The file will remain until the last process is done with it.
             */
	    unlink(dmyname);
	}
	if(saveOut >= 0)
	    ResetStdout();
	else
	    fflush(stdout);
	lseek(dmyfd, 0, SEEK_SET);
	/* Save the file descriptor of the current stdout */
	saveOut = dup(1);
	close(1);
	/* dup is guaranteed to use the lowest available file descriptor
	 * which is 1 since we just closed it and 0 must be open */
	dup(dmyfd);
	CopyToString=1;
}

