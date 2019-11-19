# include "../../parse/P.tree.h"

Node adt_[] =
{
/*000*/ "Add scans to stack2","ad",NORMAL,0,0,0,{1,2,3,8,9,10,0},
/*001*/ "Scan number(s)","sn",LEAF,0,0,0,{Btwo(20),Btwo(34),4,2,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(32),Btwo(0),0},
/*002*/ "Step size","st",LEAF,0,1,0,{Btwo(0),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),3,0,Btwo(36),Btwo(0),0},
/*003*/ "Specify backend","",EXCLSV,0,2,0,{Btwo(38),Btwo(-1),4,5,6,7,0
        },
/*004*/ "Backend number","be",LEAF,3,0,0,{Btwo(4),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),5,0,Btwo(40),Btwo(0),0},
/*005*/ "Broad backend","bbe",LEAF,3,1,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Narrow backend","nbe",LEAF,3,2,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "Expander","exp",LEAF,3,3,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*008*/ "Specify Subscan Number","ss",LEAF,0,3,0,{Btwo(8),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(42),Btwo(0),0},
/*009*/ "Care level","cr",LEAF,0,4,0,{Btwo(12),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),3,0,Btwo(44),Btwo(0),0},
/*010*/ "Matching headers only","mh",LEAF,0,5,0,{Btwo(16),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(46),Btwo(0),0}
};
