# include "../../parse/P.tree.h"

Node pdt_[] =
{
/*000*/ "Print data","pd",NORMAL,0,0,0,{1,0},
/*001*/ "Action","",EXCLSV,0,0,0,{Btwo(4),Btwo(-1),2,3,4,5,6,7,8,9,0},
/*002*/ "Stack 1 Header","st",LEAF,1,0,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*003*/ "Scan Header","sc",LEAF,1,1,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*004*/ "Stack 1 Data","stdt",LEAF,1,2,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*005*/ "Scan Data","scdt",LEAF,1,3,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Main header","main",LEAF,1,4,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "List scan numbers","sn",LEAF,1,5,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*008*/ "Put first scan file name in global string","gs",LEAF,1,6,0,{
        Btwo(0),Btwo(8),8,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(6),
        Btwo(0),0},
/*009*/ "Print date and time of stack 1","date",LEAF,1,7,0,{Btwo(-1),
        Btwo(-1),0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0}
};
