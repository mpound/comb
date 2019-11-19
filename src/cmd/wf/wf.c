/*************************************************
 * wf - Write an image as a Fits file *
 *************************************************/
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../stacks/C.h"
#include "../../image/C.h"
char *mktemp();
void WriteFits();

void wf()
{
	static int action = 0;
# include "wf.dc.h"
# include "wf.ec.h"

	if(!*actf)
		action = *act;
	switch(action) {
	case 0:
	    {
	    register struct of_imagehdr *ihp = imageHdr[*imn];
	    ChkImage(*imn);
		if(*revy) {
		    ihp->cdelt2 = -ihp->cdelt2;
		    ihp->crpix2 = 1 + ihp->naxis2 - ihp->crpix2;
		}
	/* change BITPIX for writing -- MWP 5/22/96*/
	        if(*bitpx!=ihp->bitpix) {
		    double svbzero, svbscale;
		    int svbitpix, svblank;
	            svbitpix=ihp->bitpix;
	            ihp->bitpix=*bitpx;
		    printf(" Changing BITPIX from %d to %d in %s.\n", 
			svbitpix,ihp->bitpix,ff);
	            svbzero = ihp->fbzero;
	            svbscale = ihp->bscale;
		    svblank = ihp->blank;
		    if(*dmin != DRAGON && *dmax != DRAGON) {
		       ihp->fbzero = (*dmin + *dmax) * 0.5;
		       ihp->bscale = (*dmax - *dmin);
		    }
		    else { 
		       if(ihp->datamin==ihp->datamax)
			     SetDataScale(*imn);
	               ihp->fbzero = (ihp->datamin + ihp->datamax) * 0.5;
	               ihp->bscale = (ihp->datamax - ihp->datamin);
		    }
		    switch(ihp->bitpix) {
		    case 8:
			ihp->bscale /= 254;
			break;
		    case 16:
			ihp->bscale /= 65534;
			break;
		    case 32:
			ihp->bscale /= 4294967294.;
			break;
		    case -32:
			ihp->bscale = 1.0;
			ihp->fbzero = 0.0;
			ihp->blank = 0;
			break;
		    }
		    WriteFits(*imn, ff, obj, tel);
	            ihp->bitpix=svbitpix; /* Don't want to change the real ones */
		    ihp->fbzero = svbzero;
		    ihp->bscale = svbscale;
		    ihp->blank = svblank;
		} else
		    WriteFits(*imn, ff, obj, tel);
		if(*revy) {
		    ihp->cdelt2 = -ihp->cdelt2;
		    ihp->crpix2 = 1 + ihp->naxis2 - ihp->crpix2;
		}
	    }
	    break;
	case 1:
		if(*stn >= FRSTONDISK)
			error("Can only write stacks in memory now");
		if( !StkIsFull(*stn))
			error("Stack %d is empty", *stn);
		WriteFitsSpectrum(&stk_[*stn - 1], ff, obj, tel);
		break;
	case 2:
		CloseFitsOut();
		break;
	}
}
