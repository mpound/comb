# include "C.h"
# include "include.h"
#include "dragon.h"

INTERRUPT interrupt;		/* info for interrupts and error recovery */
struct CMNDH cmnd_;		/* info for command arg parsing */

/* miscellaneous things needed by many cmds */
#if COMB
struct MAIN main_ = {3,0,0,"",2,0, "",0,0};
#else /*COMB*/
struct MAIN main_ = {0,"/usr/nobs",0};
#endif /*COMB*/

struct of_dragon dragon_;
struct of_constants constants_;
