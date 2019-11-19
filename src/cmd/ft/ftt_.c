# include "../../parse/P.tree.h"

Node ftt_[] =
{
/*000*/ "Fourier operations on baseline","ft",NORMAL,0,0,0,{1,0},
/*001*/ "Action","",EXCLSV,0,0,0,{Btwo(8),Btwo(-1),2,3,4,0},
/*002*/ "Replace stack1 by amplitude, stack2 by phase","see",LEAF,1,0,
        0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),
        Btwo(0),0},
/*003*/ "Inverse transform stack1 and stack2","ift",LEAF,1,1,0,{
        Btwo(-1),Btwo(-1),0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),
        Btwo(0),0},
/*004*/ "Remove Fourier components from stack1","rm",NORMAL,1,2,0,{5,
        6,0},
/*005*/ "Number of components to filter (chan)","nc",LEAF,4,0,0,{
        Btwo(0),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(10),
        Btwo(0),0},
/*006*/ 
        "Number of low-frequency channels to exclude from filter (chan)",
        "ex",LEAF,4,1,0,{Btwo(4),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),
        1,0,Btwo(12),Btwo(0),0}
};
