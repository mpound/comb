# include "../../parse/P.tree.h"

Node dat_[] =
{
/*000*/ "Define an area of an image","da",NORMAL,0,0,0,{1,7,8,9,10,11,
        0},
/*001*/ "Specify boundary","",EXCLSV,0,0,0,{Btwo(2068),Btwo(2070),2,3,
        6,0},
/*002*/ "No change","nc",LEAF,1,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*003*/ "Points on boundary (offset from center in current units)",
        "po",NORMAL,1,1,0,{4,5,0},
/*004*/ "Horizontal","x",LEAF,3,0,0,{Btwo(0),Btwo(-1),16,1,Btwo(1),
        Btwo(128),Btwo(0),7,0,Btwo(2072),Btwo(2074),0},
/*005*/ "Vertical","y",LEAF,3,1,0,{Btwo(1024),Btwo(-1),16,1,Btwo(1),
        Btwo(128),Btwo(0),7,0,Btwo(2076),Btwo(2078),0},
/*006*/ "Cursor read for polygon","cr",LEAF,1,2,0,{Btwo(-1),Btwo(-1),
        0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "Area number (1-5)","an",LEAF,0,1,0,{Btwo(2048),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(2080),Btwo(0),0},
/*008*/ "Print vertices of area","p",LEAF,0,2,0,{Btwo(2052),Btwo(-1),
        8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(2082),Btwo(0),0},
/*009*/ "Plot area on current plot","pl",LEAF,0,3,0,{Btwo(2056),
        Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(2084),Btwo(0),0}
        ,
/*010*/ "Line type","lt",LEAF,0,4,0,{Btwo(2060),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(2086),Btwo(0),0},
/*011*/ "Print area in area","pa",LEAF,0,5,0,{Btwo(2064),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(2088),Btwo(0),0}
};
