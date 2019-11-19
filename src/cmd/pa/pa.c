#include <sys/time.h>
# include "../../main/C.h"
# include "../../main/machinedep.h"

void pa()
{
	static int oldtype,msecs;
	
# include "pa.dc.h"
# include "pa.ec.h"
	if(!*tpfl)
		oldtype = *type;
	if(oldtype == 0) {
		wait_();
	} else {
		msecs = (int)(*time*1.0E6);
#if SYSTEM_V || !SUN_OS 		/* MWP Tue Aug 31 12:07:48 EDT 1993 */
		sleep((unsigned)*time);
#else 
		usleep((unsigned)msecs);
#endif
	}
}
