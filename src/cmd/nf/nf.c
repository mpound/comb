# include "../../main/C.h"
# include "../../scan/C.h"

void nf()
{
# include "nf.dc.h"
# include "nf.ec.h"
	if(*prn) { 
		printf("Scan file - %s\n",scan_.datfn);
	}
	scan_.ndfn = strlen(nfln);
	strcpy(curscn_.fil, &nfln[scan_.ndfn - 3]);
}
