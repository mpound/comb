# include "../../parse/P.tree.h"

Node routt_[] =
{
/*000*/ "Redirect output to a file or string variable","ro",NORMAL,0,
        0,0,{1,6,0},
/*001*/ "Action","",EXCLSV,0,0,0,{Btwo(8),Btwo(10),2,3,4,5,0},
/*002*/ "File name","fn",LEAF,1,0,0,{Btwo(22),Btwo(-1),3,1,Btwo(48),
        Btwo(0),Btwo(0),5,0,Btwo(12),Btwo(0),0},
/*003*/ "Pipeline for output","p",LEAF,1,1,0,{Btwo(70),Btwo(-1),3,1,
        Btwo(80),Btwo(0),Btwo(0),1,0,Btwo(14),Btwo(0),0},
/*004*/ "String variable name","str",LEAF,1,2,0,{Btwo(150),Btwo(-1),3,
        1,Btwo(65),Btwo(0),Btwo(0),7,0,Btwo(16),Btwo(0),0},
/*005*/ "Quit redirecting","q",LEAF,1,3,0,{Btwo(0),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(18),Btwo(0),0},
/*006*/ "Truncate before writing","t",LEAF,0,1,0,{Btwo(4),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(20),Btwo(0),0}
};
