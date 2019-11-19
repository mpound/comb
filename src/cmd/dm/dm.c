# include <stdio.h>
# include "../../main/C.h"
# include "../../main/include.h"

extern char lfilnm[],gfilnm[]
#if COMB
	,m1filnm[],m2filnm[]
#endif /*COMB*/
	;
extern FILE *FChkOpen();
extern int notOpen[];
static char *macFiles[] = {lfilnm, gfilnm
#if COMB
	, m1filnm, m2filnm
#endif /*COMB*/
	};

void dm()
{
# include "dm.dc.h"
	char ccmd[128];
	char *s,*getenv();
# include "dm.ec.h"

	if(*glob >= 2
#if COMB
		&& notOpen[*glob - 2]
#endif /*COMB*/
		)
		error_("Stacks directory not open");
	if(*act == 0) {
		if((s = getenv("EDITOR")) == 0)
			s = "/bin/ed";
		sprintf(ccmd,"%s %s",s,macFiles[*glob]);
		system(ccmd);
		rdmac();
		return;
	} else if(*act == 1) {
		if(*pfl) {
			sprintf(ccmd,"cat %s", macFiles[*glob]);
		} else {
			sprintf(ccmd,"matchmac \"%s\" %s", pat,macFiles[*glob]);
		}
		eshell_(ccmd);
	}
}
