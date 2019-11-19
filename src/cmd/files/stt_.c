# include "../../parse/P.tree.h"

Node stt_[] =
{
/*000*/ "Store stack","st",NORMAL,0,0,0,{1,2,3,4,5,8,0},
/*001*/ "Stack number","n",LEAF,0,0,0,{Btwo(0),Btwo(22),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(20),Btwo(0),0},
/*002*/ "Directory name","f",LEAF,0,1,0,{Btwo(38),Btwo(26),3,1,
        Btwo(65),Btwo(0),Btwo(0),4,0,Btwo(24),Btwo(0),0},
/*003*/ "Use directory two","dt",LEAF,0,2,0,{Btwo(4),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(28),Btwo(0),0},
/*004*/ "Source stack","src",LEAF,0,3,0,{Btwo(8),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),3,0,Btwo(30),Btwo(0),0},
/*005*/ "Auto store","",EXCLSV,0,4,0,{Btwo(32),Btwo(-1),6,7,0},
/*006*/ "Don't auto store","dmy",LEAF,5,0,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "First free stack to use","a",LEAF,5,1,0,{Btwo(12),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(34),Btwo(0),0},
/*008*/ "Don't check dest","dc",LEAF,0,5,0,{Btwo(16),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(36),Btwo(0),0}
};
