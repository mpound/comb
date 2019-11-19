# include "../../main/C.h"
# include "../../main/include.h"
extern Icmd icmd[];
extern int numcmds();

void lc()
{
	/* The following are static to avoid a hpux problem that automatic
	 * variables may be undefined on return from a longjmp */
	static short int cntr;
	static short int paging;
	static Icmd *ip;
	char dumstr[20];
# include "lc.dc.h"
# include "lc.ec.h"
	ip = icmd;
	cntr = numcmds();
	paging = PageStdout();
	setjmp(interrupt.retbuf);
	if(!cntr--) {
		if(paging)
			ResetStdout();
		longjmp(interrupt.sjbuf, 0);
	}
	sprintf(dumstr,"%2.2s ?d",ip++);
	combex_(dumstr);
}
