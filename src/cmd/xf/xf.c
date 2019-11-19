# include "../../stacks/C.h"
# include "../../coordsys/C.h"
# include "../../stacks/include.h"
# include "../../main/C.h"
# include <stdio.h>
# include <sys/file.h>
# include <sys/types.h>
#if 0
# include <sys/dir.h>
#endif
# include <math.h>
# define USELEN sizeof(use_.nuse)

void xf()
{
# include "xf.dc.h"
	register  int sn;
	register int n;
	double tol;
	int totDropped = 0;
	int totCombined = 0;
	int totStored = 0;

# include "xf.ec.h"

	/* check that the background directory is open */
	curDir = BACKGROUND;
	CDirErr();

	curDir = FOREGROUND;
	CDirErr();
	OkStacks((int)*slh,(int)*sll);
	SetMaxDis(*ptol);
	MkStkList((int)*sn1, (int)*sn2);

	while(sn = NextStkOnList()) {
		if(*v > 1)
			fprintf(stderr,"Processing stack %d ",sn);
		if( !ChckData(sn) ) {
			fprintf(stderr,"stack %d does not exist\n",sn);
			continue;
		}
		getStack(sn,2);
		RmStkOnList(sn);

		/* if a filter width was given, only use stacks of that width */
		if(*FWID != 0.) {
			tol = *FWID/( stk_[1].numpt << 4);
			if( cifdif((double)stk_[1].fwid, *FWID, tol)) {
				fprintf(stderr,"fwid wrong\n");
				continue;
			}
		}
		/* If center freq out of range, skip this one */
		if(stk_[1].freq < *fr1 || stk_[1].freq > *fr2) {
			fprintf(stderr,"freq wrong\n");
			continue;
		}

		n = CombineSimilar((int)*v);
		totCombined += n & 0xffff;
		totDropped += n >> 16;
		for(n = 0;n < stk_[1].nwts;n++)
			if(stk_[1].wght[n] > *mxwt)
				stk_[1].wght[n] = *mxwt;
		curDir = BACKGROUND;
		n = NextStk(10,EMPTY);
		if(*v > 1 || (*v && (n % 50 == 0)) )
			fprintf(stderr,"  Stored in stack %d\n",n);
		noint_();
		totStored++;
	/*	ClrStkValue(n);	*/
		WrData(n,&stk_[1]);
		WrSearch(&stk_[1]);
		yesint_();
		curDir = FOREGROUND;
	}
	FreeStkList();
	/* Flush the search file buffer */
	curDir = BACKGROUND;
	OpenSearch();
	curDir = FOREGROUND;
	if(*v) {
		fprintf(stderr,"\n%d stacks stored in dt, ",totStored);
		fprintf(stderr,"%d combined with others, ",totCombined);
		fprintf(stderr,"%d not xfered (same scan(s))\n",totDropped);
	}
}
