# include "../../main/C.h"
# include "../../graphics/C.h"
void op()
{
	extern char fitsFile[];
#include "op.dc.h"

#include "op.ec.h"
	if( !*projf) {
		SetProj(*proj);
	}
	if( !*htypef) {
		main_.pltd = 0;
		main_.htype = *htyp + 1;
	}
}
