# include <unistd.h>
# include "../../graphics/C.h"
# include "../../graphics/include.h"
# include "../../main/C.h"

char hcfile[DIRNMLEN+10];
void hc()
{
	PICTURE copy;
# include "hc.dc.h"
# include "hc.ec.h"

/* save the current picture, to be restored
 * after the hardcopy. This is to fix the cursor read
 * problem after an hc - mwp 12/01/98
 */
	copy=*cPic;
        switch(*typ) {
        case 0:
		strcpy (hcfile, "");
		orientation = (*ori == 0) ? LANDSCAPE: PORTRAIT; 
                Psend("post1");
                break;
        case 1:
		strcpy (hcfile, "");
		orientation = (*ori == 0) ? LANDSCAPE: PORTRAIT; 
                Psend("post2");
                break;
        case 2:
		strcpy (hcfile, "");
		orientation = (*ori == 0) ? LANDSCAPE: PORTRAIT; 
                Psend("post3");
                break;
        case 3:
		strcpy (hcfile, "");
		orientation = (*ori == 0) ? LANDSCAPE: PORTRAIT; 
                Psend("post4");
                break;
        case 4:
		strcpy (hcfile, "");
                Psend("imagen1");
                break;
        case 5:
		strcpy (hcfile, "");
                Psend("imagen2");
                break;
        case 6:
		strcpy (hcfile, "");
                Psend("lj1");
                break;
        case 7:
		strcpy (hcfile, "");
                Psend("lj2");
                break;
        case 8:
		strcpy (hcfile, "");
                Psend("hpglplot");
                break;
        case 9:
		strcpy (hcfile, "");
                Psend("tekplot");
                break;
        case 10:
		orientation = (*ori == 0) ? LANDSCAPE: PORTRAIT; 
                if( access(hcfn, F_OK) == 0) 
                        warn("%s exists and will be overwritten", hcfn);
		unlink(hcfn);
        	strcpy(hcfile, hcfn);
                Psend("postfile");
                break;
        case 11:
                if( access(hcfn, F_OK) == 0) 
                        warn("%s exists and will be overwritten", hcfn);
		unlink(hcfn);
        	strcpy(hcfile, hcfn);
                Psend("ljfile");
                break;
        case 12:
                if( access(hcfn, F_OK) == 0) 
                        warn("%s exists and will be overwritten", hcfn);
		unlink(hcfn);
        	strcpy(hcfile, hcfn);
		Psend("tekfile");
                break;
        }

	/* restore the picture */
	*cPic=copy;
	Pupdate();
}
