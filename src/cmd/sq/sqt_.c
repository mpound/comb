# include "../../parse/P.tree.h"

Node sqt_[] =
{
/*000*/ "Squish - increase or decrease chan width","sq",NORMAL,0,0,0,{
        1,2,0},
/*001*/ "New chan wid/old wid","r",LEAF,0,0,0,{Btwo(0),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(16),Btwo(0),0},
/*002*/ "Center of squish(horiz plot units)","h",LEAF,0,1,0,{Btwo(8),
        Btwo(20),16,1,Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(18),Btwo(0),0}
};
