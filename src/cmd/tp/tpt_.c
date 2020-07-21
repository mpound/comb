# include "../../parse/P.tree.h"

Node tpt_[] =
{
/*000*/ 
        "Total power - average chans in stack1 weighted by cal in stack3",
        "tp",NORMAL,0,0,0,{1,2,3,4,5,0},
/*001*/ "Store power at this elevation","st",LEAF,0,0,0,{Btwo(0),
        Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(20),Btwo(0),0},
/*002*/ "Reduce stored skydip power values","reduce",LEAF,0,1,0,{
        Btwo(4),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(22),
        Btwo(0),0},
/*003*/ "Clear stored values from memory without reducing","clr",LEAF,
        0,2,0,{Btwo(8),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,
        Btwo(24),Btwo(0),0},
/*004*/ "Print stored values from memory","prt",LEAF,0,3,0,{Btwo(12),
        Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(26),Btwo(0),0},
/*005*/ "Output skydip reduction in tabular form","tab",LEAF,0,4,0,{
        Btwo(16),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(28),
        Btwo(0),0}
};
