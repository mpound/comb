# include "../../parse/P.tree.h"

Node int_[] =
{
/*000*/ "Integrate part of a spectrum","in",NORMAL,0,0,0,{1,4,7,0},
/*001*/ "Horizontal integration interval(plot units)","h",NORMAL,0,0,
        0,{2,3,0},
/*002*/ "from","f",LEAF,1,0,0,{Btwo(0),Btwo(-1),16,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(36),Btwo(0),0},
/*003*/ "to","t",LEAF,1,1,0,{Btwo(8),Btwo(-1),16,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(38),Btwo(0),0},
/*004*/ "Clip Level","clip",NORMAL,0,1,0,{5,6,0},
/*005*/ "For Zeroth and First Moments","area",LEAF,4,0,0,{Btwo(16),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(40),Btwo(0),0},
/*006*/ "For Second Moment","disp",LEAF,4,1,0,{Btwo(24),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(42),Btwo(0),0},
/*007*/ "Don't print","dp",LEAF,0,2,0,{Btwo(32),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(44),Btwo(0),0}
};
