# include "../../parse/P.tree.h"

Node dot_[] =
{
/*000*/ "Loop through a command string","do",NORMAL,0,0,0,{1,5,6,0},
/*001*/ "Type of loop","",EXCLSV,0,0,0,{Btwo(4),Btwo(-1),2,3,4,0},
/*002*/ "Number of times to execute","n",LEAF,1,0,0,{Btwo(0),Btwo(-1),
        8,1,Btwo(1),Btwo(0),Btwo(0),7,0,Btwo(6),Btwo(0),0},
/*003*/ "While","w",LEAF,1,1,0,{Btwo(16),Btwo(-1),2,1,Btwo(81),
        Btwo(0),Btwo(0),7,0,Btwo(8),Btwo(0),0},
/*004*/ "If","i",LEAF,1,2,0,{Btwo(16),Btwo(-1),2,1,Btwo(81),Btwo(0),
        Btwo(0),7,0,Btwo(8),Btwo(0),0},
/*005*/ "Command string","c",LEAF,0,1,0,{Btwo(97),Btwo(-1),1,1,
        Btwo(1001),Btwo(0),Btwo(0),7,0,Btwo(10),Btwo(0),0},
/*006*/ "Else","el",LEAF,0,2,0,{Btwo(1098),Btwo(14),1,1,Btwo(1001),
        Btwo(0),Btwo(0),4,0,Btwo(12),Btwo(0),0}
};
