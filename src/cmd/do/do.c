# include "../../main/C.h"

void docmd()
{
	register int cnt = 0;	/* will be set non zero if cmd executed */
	char *texp;
# include "do.dc.h"
# include "wexp.dc.h"
# include "do.ec.h"
	if(*tol) {
		texp = exp;
# include "wexp.ec.h"
		while(*val) {
			combex_(cmd);
			cnt++;
			if(*tol == 2)
				break;
			strcpy(cmnd_.nc,"do");
			texp = exp;
# include "wexp.ec.h"
		}
	} else
		for( ; cnt < *to; cnt += 1)
			combex_(cmd);

	/* if nothing has been done yet, check for an else clause */
	if( !cnt && !*elfl)
		combex_(elcmd);
}
