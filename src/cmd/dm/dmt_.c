# include "../../parse/P.tree.h"

Node dmt_[] =
{
/*000*/ "Define macro","dm",NORMAL,0,0,0,{1,6,0},
/*001*/ "Macro file","",EXCLSV,0,0,0,{Btwo(0),Btwo(-1),2,3,4,5,0},
/*002*/ "Local","l",LEAF,1,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),Btwo(0),
        Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*003*/ "Global","g",LEAF,1,1,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*004*/ "Stack directory 1","st",LEAF,1,2,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*005*/ "Directory two","dt",LEAF,1,3,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Action","",EXCLSV,0,1,0,{Btwo(2),Btwo(-1),7,8,0},
/*007*/ "Edit with $EDITOR","ed",LEAF,6,0,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*008*/ "Print macros with optional reg exp match test","p",LEAF,6,1,
        0,{Btwo(8),Btwo(6),3,1,Btwo(32),Btwo(0),Btwo(0),3,0,Btwo(4),
        Btwo(0),0}
};
