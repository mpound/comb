# include "../../parse/P.tree.h"

Node cot_[] =
{
/*000*/ "Combine two stacks, result in 1 & 2","co",NORMAL,0,0,0,{1,2,
        3,4,5,0},
/*001*/ "Locadd","ad",LEAF,0,0,0,{Btwo(0),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),3,0,Btwo(20),Btwo(0),0},
/*002*/ "Locsum","su",LEAF,0,1,0,{Btwo(4),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),3,0,Btwo(22),Btwo(0),0},
/*003*/ "Add stacks","add",LEAF,0,2,0,{Btwo(8),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(24),Btwo(0),0},
/*004*/ "Don't check positions","dc",LEAF,0,3,0,{Btwo(12),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(26),Btwo(0),0},
/*005*/ "Align center channels","ac",LEAF,0,4,0,{Btwo(16),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(28),Btwo(0),0}
};
