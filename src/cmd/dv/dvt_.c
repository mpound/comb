# include "../../parse/P.tree.h"

Node dvt_[] =
{
/*000*/ "Declare user Variables","dv",NORMAL,0,0,0,{1,2,3,4,5,0},
/*001*/ "Numeric variables","n",LEAF,0,0,0,{Btwo(26),Btwo(14),3,1,
        Btwo(1025),Btwo(0),Btwo(0),4,0,Btwo(12),Btwo(0),0},
/*002*/ "String variables","s",LEAF,0,1,0,{Btwo(1051),Btwo(18),3,1,
        Btwo(1025),Btwo(0),Btwo(0),4,0,Btwo(16),Btwo(0),0},
/*003*/ "Make variable local to macro","l",LEAF,0,2,0,{Btwo(0),
        Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(20),Btwo(0),0},
/*004*/ "Print global user variables","pg",LEAF,0,3,0,{Btwo(4),
        Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(22),Btwo(0),0},
/*005*/ "Print local user variables","pl",LEAF,0,4,0,{Btwo(8),
        Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(24),Btwo(0),0}
};
