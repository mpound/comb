# include "../../main/C.h"
/*
 * command e - execute a shell command
 */
void ex()
{
# include "ex.dc.h"
# include "ex.ec.h"
	if(*cmdf)
		error_("No shell command");
	eshell_(cmd);
}
