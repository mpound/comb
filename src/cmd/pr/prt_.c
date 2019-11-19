# include "../../parse/P.tree.h"

Node prt_[] =
{
/*000*/ "Printf to standard output or a string variable","pr",NORMAL,
        0,0,0,{1,2,3,4,5,6,7,8,9,10,11,12,0},
/*001*/ "Format","fmt",LEAF,0,0,0,{Btwo(5234),Btwo(-1),3,1,Btwo(128),
        Btwo(0),Btwo(0),1,0,Btwo(5204),Btwo(0),0},
/*002*/ "Value","va",LEAF,0,1,0,{Btwo(0),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5206),Btwo(0),0},
/*003*/ "Value","vb",LEAF,0,2,0,{Btwo(520),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5208),Btwo(0),0},
/*004*/ "Value","vc",LEAF,0,3,0,{Btwo(1040),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5210),Btwo(0),0},
/*005*/ "Value","vd",LEAF,0,4,0,{Btwo(1560),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5212),Btwo(0),0},
/*006*/ "Value","ve",LEAF,0,5,0,{Btwo(2080),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5214),Btwo(0),0},
/*007*/ "Value","vf",LEAF,0,6,0,{Btwo(2600),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5216),Btwo(0),0},
/*008*/ "Value","vg",LEAF,0,7,0,{Btwo(3120),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5218),Btwo(0),0},
/*009*/ "Value","vh",LEAF,0,8,0,{Btwo(3640),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5220),Btwo(0),0},
/*010*/ "Value","vi",LEAF,0,9,0,{Btwo(4160),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5222),Btwo(0),0},
/*011*/ "Value","vj",LEAF,0,10,0,{Btwo(4680),Btwo(-1),19,1,Btwo(514),
        Btwo(0),Btwo(0),3,0,Btwo(5224),Btwo(0),0},
/*012*/ "Output","",EXCLSV,0,11,0,{Btwo(5226),Btwo(-1),13,14,15,0},
/*013*/ "Print to Stdout","so",LEAF,12,0,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*014*/ "Print to a Global String","gs",LEAF,12,1,0,{Btwo(5200),
        Btwo(5230),8,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(5228),Btwo(0),
        0},
/*015*/ "Print to a Named Variable","v",LEAF,12,2,0,{Btwo(5362),
        Btwo(-1),3,1,Btwo(33),Btwo(0),Btwo(0),7,0,Btwo(5232),Btwo(0),0
        }
};
