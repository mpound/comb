# include "../../parse/P.tree.h"

Node sct_[] =
{
/*000*/ "Scanf from a file, stdin, or a string variable","sc",NORMAL,
        0,0,0,{1,2,6,11,0},
/*001*/ "Format","fmt",LEAF,0,0,0,{Btwo(34),Btwo(-1),3,1,Btwo(128),
        Btwo(0),Btwo(0),1,0,Btwo(16),Btwo(0),0},
/*002*/ "Action","",EXCLSV,0,1,0,{Btwo(18),Btwo(-1),3,4,5,0},
/*003*/ "Variables to assign results to","v",LEAF,2,0,0,{Btwo(162),
        Btwo(-1),3,1,Btwo(129),Btwo(0),Btwo(0),1,0,Btwo(20),Btwo(0),0}
        ,
/*004*/ "Read into stack1 chans","stch",LEAF,2,1,0,{Btwo(0),Btwo(-1),
        8,2,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(22),Btwo(0),0},
/*005*/ "Rewind file only","rw",LEAF,2,2,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Source","",EXCLSV,0,2,0,{Btwo(24),Btwo(-1),7,8,9,10,0},
/*007*/ "File name","fn",LEAF,6,0,0,{Btwo(291),Btwo(-1),3,1,Btwo(65),
        Btwo(0),Btwo(0),5,0,Btwo(26),Btwo(0),0},
/*008*/ "Global String to read","gs",LEAF,6,1,0,{Btwo(8),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(28),Btwo(0),0},
/*009*/ "Standard input","stdin",LEAF,6,2,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*010*/ "A string or string variable","str",LEAF,6,3,0,{Btwo(356),
        Btwo(-1),3,1,Btwo(8193),Btwo(0),Btwo(0),7,0,Btwo(30),Btwo(0),0
        },
/*011*/ "Warn if end of file is encountered","w",LEAF,0,3,0,{Btwo(12),
        Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),2,0,Btwo(32),Btwo(0),0}
};
