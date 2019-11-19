# include "../../parse/P.tree.h"

Node pht_[] =
{
/*000*/ "Print in hms format","ph",NORMAL,0,0,0,{1,2,3,0},
/*001*/ "Value","v",LEAF,0,0,0,{Btwo(0),Btwo(-1),16,1,Btwo(1),Btwo(0),
        Btwo(0),3,0,Btwo(16),Btwo(0),0},
/*002*/ "No line feed","nl",LEAF,0,1,0,{Btwo(8),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(18),Btwo(0),0},
/*003*/ "Output","",EXCLSV,0,2,0,{Btwo(20),Btwo(-1),4,5,6,0},
/*004*/ "Print to Stdout","so",LEAF,3,0,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*005*/ "Print to a Global String","gs",LEAF,3,1,0,{Btwo(12),Btwo(24),
        8,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(22),Btwo(0),0},
/*006*/ "Print to a Named Variable","nv",LEAF,3,2,0,{Btwo(28),
        Btwo(-1),3,1,Btwo(33),Btwo(0),Btwo(0),7,0,Btwo(26),Btwo(0),0}
};
