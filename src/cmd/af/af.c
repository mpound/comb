#include "../../main/C.h"
#include "../../stacks/C.h"

/********************************************************************
 * Attach a FITS file to an image.  Read in the header, but no data *
 ********************************************************************/

void af()
{
	static int action = 0;
	int nDragon=0;

# include "af.dc.h"
# include "af.ec.h"

	if(!*actf)
		action = *act;
	if(action == 0)
		OpenFits(*imn, ff);
	else {
		if( *stn < 1 || *stn >= FRSTONDISK)
			error("Can only write stacks in memory now");
		ReadFitsSpectrum(&stk_[*stn - 1],ff);
		mrkful_((int *)stn);
	/* check for incoming DRAGONs -- MWP 5/8/96 */
        	if(main_.drgmes!=0) {
        	  if( (nDragon=ChkDrag(*stn)) > 0)
          	    switch(main_.drgmes) {
          	      case 1:
              	        /* no message */
                        break;
        	      case 2:
                	printf(" Found and eliminated %d BLANK%s in stack %d\n",
				nDragon,(nDragon==1)?"":"s",stn);
                	printf(" Use 'op msg:' to eliminate this message.\n");
                	break;
         	    }
        	}
		main_.pltd = 0;
	}
}
