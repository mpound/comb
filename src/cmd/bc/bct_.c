# include "../../parse/P.tree.h"

Node bct_[] =
{
/*000*/ "Designate bad channels","bc",NORMAL,0,0,0,{1,2,5,0},
/*001*/ "Range of horiz values(plot units)","r",LEAF,0,0,0,{Btwo(0),
        Btwo(22),16,2,Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(20),Btwo(0),0},
/*002*/ "Action","",EXCLSV,0,1,0,{Btwo(24),Btwo(-1),3,4,0},
/*003*/ "Add to list","ad",LEAF,2,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*004*/ "Clear old list first","cl",LEAF,2,1,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*005*/ "Print currently flagged bad channels","p",LEAF,0,2,0,{
        Btwo(16),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(26),
        Btwo(0),0}
};
