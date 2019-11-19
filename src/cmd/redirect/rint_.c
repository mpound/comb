# include "../../parse/P.tree.h"

Node rint_[] =
{
/*000*/ "Redirect command input from a file or string","ri",NORMAL,0,
        0,0,{1,0},
/*001*/ "Action","",EXCLSV,0,0,0,{Btwo(0),Btwo(2),2,3,0},
/*002*/ "File name","fn",LEAF,1,0,0,{Btwo(8),Btwo(-1),3,1,Btwo(129),
        Btwo(0),Btwo(0),7,0,Btwo(4),Btwo(0),0},
/*003*/ "Execute a string as comb commands","str",LEAF,1,1,0,{
        Btwo(137),Btwo(-1),3,1,Btwo(8193),Btwo(0),Btwo(0),7,0,Btwo(6),
        Btwo(0),0}
};
