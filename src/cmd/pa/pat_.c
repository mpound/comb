# include "../../parse/P.tree.h"

Node pat_[] =
{
/*000*/ "Pause in execution","pa",NORMAL,0,0,0,{1,0},
/*001*/ "Type of wait","",EXCLSV,0,0,0,{Btwo(8),Btwo(10),2,3,0},
/*002*/ "Wait for 'n'","w",LEAF,1,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*003*/ "Time interval to pause (sec)","t",LEAF,1,1,0,{Btwo(0),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(12),Btwo(0),0}
};
