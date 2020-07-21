# include "../../parse/P.tree.h"

Node gst_[] =
{
/*000*/ "Put STO data in st 1","gs",NORMAL,0,0,0,{1,2,4,6,0},
/*001*/ "File number","fn",LEAF,0,0,0,{Btwo(0),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(16),Btwo(0),0},
/*002*/ "Specify backend","",EXCLSV,0,1,0,{Btwo(18),Btwo(-1),3,0},
/*003*/ "Backend number","be",LEAF,2,0,0,{Btwo(4),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),5,0,Btwo(20),Btwo(0),0},
/*004*/ "Specify reference stack","",EXCLSV,0,2,0,{Btwo(22),Btwo(-1),
        5,0},
/*005*/ "stack number","refstk",LEAF,4,0,0,{Btwo(8),Btwo(26),8,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(24),Btwo(0),0},
/*006*/ "Specify hot stack","",EXCLSV,0,3,0,{Btwo(28),Btwo(-1),7,0},
/*007*/ "stack number","hotstk",LEAF,6,0,0,{Btwo(12),Btwo(32),8,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(30),Btwo(0),0}
};
