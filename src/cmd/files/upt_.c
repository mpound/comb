# include "../../parse/P.tree.h"

Node upt_[] =
{
/*000*/ "Update a stacks directory","up",NORMAL,0,0,0,{1,2,3,4,5,6,7,0
        },
/*001*/ "Name","n",LEAF,0,0,0,{Btwo(38),Btwo(-1),3,1,Btwo(129),
        Btwo(0),Btwo(0),7,0,Btwo(24),Btwo(0),0},
/*002*/ "Directory two","dt",LEAF,0,1,0,{Btwo(0),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(26),Btwo(0),0},
/*003*/ "From VAX (convert Vax stacks file to Sun","fvax",LEAF,0,2,0,{
        Btwo(4),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(28),
        Btwo(0),0},
/*004*/ 
        "Convert Big Endian stacks (Sun) to Little Endian (Dec) or reverse",
        "swap",LEAF,0,3,0,{Btwo(8),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),6,0,Btwo(30),Btwo(0),0},
/*005*/ "Test byte order only","tbo",LEAF,0,4,0,{Btwo(12),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(32),Btwo(0),0},
/*006*/ "From Hp","fhp",LEAF,0,5,0,{Btwo(16),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(34),Btwo(0),0},
/*007*/ "Fix up survey files","fix",LEAF,0,6,0,{Btwo(20),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(36),Btwo(0),0}
};
