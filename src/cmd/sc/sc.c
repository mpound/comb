#include <stdio.h>
#include <ctype.h>
#include "../../main/C.h"
#include "../../parse/P.tree.h"
#include "../../parse/C.h"
#include "../../parse/P.struct.h"
#include "../../parse/P.constant.h"
#if COMB
#include "../../stacks/C.h"
#endif /*COMB*/
# include "../../error/C.h"

static char inName[64];		/* Name of file to read from */
static FILE *inFile;
extern char *StdinFile();
extern char *getenv();
extern char *strcpy();
extern HdrVar *findhdr();

	/******************************************/
	/* sc - scanf like input command for comb */
	/******************************************/

void sc()
{
	char arg[10][128];
	char vname[32];
	HdrVar *hv;
	register int i,n,c;
	register char *cp;

# include "sc.dc.h"
# include "sc.ec.h"
	if( *srcn == 0 && (inFile == NULL || strcmp(inName,fname) ) ||
		*act ==2 ) {
		if(inFile != NULL) {
			fclose(inFile);
			inFile = NULL;
		}
		inFile = FChkOpen(fname,fname,"r");
		strcpy(inName,fname);
	}
	if(*srcn==2)
		inFile=stdin;
	switch(*act) {
	case 0:
	    if(*srcn == 1) {
		if(sg_[*gs] == 0)
			error_("Source string is empty");
		main_.test = sscanf(sg_[*gs], fmt, arg[0],arg[1],
			arg[2], arg[3], arg[4], arg[5], arg[6], arg[7],
			arg[8], arg[9]);
	    } else if(*srcn == 3) {
		main_.test = sscanf(scstr, fmt, arg[0],arg[1],
			arg[2], arg[3], arg[4], arg[5], arg[6], arg[7],
			arg[8], arg[9]);
 	    } else { 
		 main_.test = fscanf(inFile, fmt, arg[0],arg[1],
			arg[2], arg[3], arg[4], arg[5], arg[6], arg[7],
			arg[8], arg[9]);
	    }
	    if(main_.test == 0) {
		while((i = fgetc(inFile)) != '\n')
		    if(i == EOF) {
			main_.test = i;
			break;
		    }
	    }
	    if((main_.test <= 0) && (*eofw) ) {
		   warn("You are trying to read past the end of file marker");
	    }
	    for(i = 0,cp = args; (i < 10) && (c = *cp++); ) {
		if(isdigit(*cp))
			n = *cp++ - '0';
		else
			n = 0;
		switch(c) {
		case '$' :
			if(sg_[n]) {
				ChkFree(sg_[n]);
				sg_[n] = 0;
			}
			sg_[n] = ChkAlloc(strlen(arg[i]) + 1, "Global string");
			strcpy(sg_[n],arg[i++]);
			break;
		case '#':
			gg_[n] = *(double *)arg[i++];
			break;
		case '.':
			for(n = 0;isalnum(*cp); )
				vname[n++] = *cp++;
			vname[n] = 0;
			if(*cp == '(') {
				n = atoi(++cp);
				while(*++cp && *cp != ')')
					;
				cp++;
			} else {
				n = 1;
			}
			if((hv = findhdr(vname)) == NULL)
				error("Variable %s not found",vname);
			if(hv->tp == HVSTRING || hv->tp == SCAN2STR 
			    || hv->tp == UVSTR)  /* test for UVSTR - mwp 5/96 */
			    asgnstrhdr(hv, arg[i++]);
			else
			    asgnhdr(hv, n, *(double *)arg[i++]);
			break;
		case ' ':
		case ',':
		case '\t':
			break;
		default:
			error("Bad char %c in assignment list",c);
		}
	    }
	    break;
	case 1:
#if COMB
	    if(*srcn == 1)
		error_("Can't read 'into stack chans' from string");
	    main_.test = 0;
	    for(n = *stch1 - 1; n < *stch2; ) {
		if((i = fscanf(inFile, fmt, arg[0], arg[1],
				arg[2], arg[3], arg[4])) > 0) {
		    ++main_.test; 
/* MWP: Why is this a pointer to a double when stack vals are floats ? */
/* Because scanf put a double there RWW */
			stk_[0].stak[n++] = *(double *)arg[0];
		} else {
		    if(main_.test == 0 && i == EOF)
			main_.test = i;
		    break;
	 	}
	    }
	    break;
#else /*COMB*/
		error("No stack in obs");
#endif /*COMB*/
	case 2:		/* Only 'rewind' the file */
	    main_.test = 1;	/* indicate success */
	    break;
	}
	if(inFile == stdin) { 
	/* fixes a bug which caused all subsequent reads 
         * to be from stdin. Also don't want stdin to be fclosed 
	 * by the next if statement--that kills comb!  MWP 4/29/96 
         */
	  inFile=NULL;
	/* fixes a bug which caused first token of subsequent handhold
	 * mode reads (or any stdin read) to be skipped. MWP 05/01/96
         */
	  fflush(stdin);
	}
}

char *
StdinFile()
{
	if( ! inName[0] )
		sprintf(inName,"%s/comb.out",getenv("HOME") );
	return(inName);
}
