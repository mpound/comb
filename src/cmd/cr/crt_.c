# include "../../parse/P.tree.h"

Node crt_[] =
{
/*000*/ "Cursor read","cr",NORMAL,0,0,0,{1,2,3,0},
/*001*/ "Number of reads to make","n",LEAF,0,0,0,{Btwo(0),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(20),Btwo(0),0},
/*002*/ "Change to box with cursor","cb",LEAF,0,1,0,{Btwo(4),Btwo(-1),
        8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(22),Btwo(0),0},
/*003*/ "Print cursor coordinates","",EXCLSV,0,2,0,{Btwo(24),Btwo(26),
        4,5,6,0},
/*004*/ "Print both x and y","p",LEAF,3,0,0,{Btwo(8),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(28),Btwo(0),0},
/*005*/ "Print x only","px",LEAF,3,1,0,{Btwo(12),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(30),Btwo(0),0},
/*006*/ "Print y only","py",LEAF,3,2,0,{Btwo(16),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(32),Btwo(0),0}
};
