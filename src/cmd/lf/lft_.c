# include "../../parse/P.tree.h"

Node lft_[] =
{
/*000*/ "Fit a Lorentzian to part of a spectrum","lf",NORMAL,0,0,0,{1,
        4,8,0},
/*001*/ "Horizontal interval of fit (plot units)","h",NORMAL,0,0,0,{2,
        3,0},
/*002*/ "From","f",LEAF,1,0,0,{Btwo(0),Btwo(-1),16,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(20),Btwo(0),0},
/*003*/ "To","t",LEAF,1,1,0,{Btwo(8),Btwo(-1),16,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(22),Btwo(0),0},
/*004*/ "Action in stack 1","",EXCLSV,0,1,0,{Btwo(24),Btwo(-1),5,6,7,0
        },
/*005*/ "No action","na",LEAF,4,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Replace with parabola and overlay plot","see",LEAF,4,1,0,{
        Btwo(-1),Btwo(-1),0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),
        Btwo(0),0},
/*007*/ "Remove (subtract)","rm",LEAF,4,2,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*008*/ "Don't print","dp",LEAF,0,2,0,{Btwo(16),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(26),Btwo(0),0}
};
