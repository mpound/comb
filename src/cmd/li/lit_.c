# include "../../parse/P.tree.h"

Node lit_[] =
{
/*000*/ "Fit and remove a polynomial baseline","li",NORMAL,0,0,0,{1,2,
        3,0},
/*001*/ "Order of poly","o",LEAF,0,0,0,{Btwo(8),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(12),Btwo(0),0},
/*002*/ "Width to exclude around sig & ref(plot units)","wid",LEAF,0,
        1,0,{Btwo(0),Btwo(16),16,1,Btwo(1),Btwo(0),Btwo(0),4,0,
        Btwo(14),Btwo(0),0},
/*003*/ "Action","",EXCLSV,0,2,0,{Btwo(18),Btwo(-1),4,5,6,0},
/*004*/ "Remove baseline from stack1","rm",LEAF,3,0,0,{Btwo(-1),
        Btwo(-1),0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*005*/ "Replace stack1 by poly","see",LEAF,3,1,0,{Btwo(-1),Btwo(-1),
        0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Extend removal to stack2","xt",LEAF,3,2,0,{Btwo(-1),Btwo(-1),
        0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0}
};
