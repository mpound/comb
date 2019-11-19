# include "../../main/C.h"
/*
# include "../cm/S.struct.h"
*/

/*
 * q - quit comb
 */
void quit()
{
/*
	extern SFILE sfile[];
*/
# include "quit.dc.h"
# include "quit.ec.h"
/*
	if(sfile[0])
		Sclose(sfile[0]);
	if(sfile[1])
		Sclose(sfile[1]);
*/
#if 0
	f_exit();
#endif /*0*/
	exit(0);
}
