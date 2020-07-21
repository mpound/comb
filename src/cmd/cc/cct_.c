# include "../../parse/P.tree.h"

Node cct_[] =
{
/*000*/ "Change center channel","cc",NORMAL,0,0,0,{1,0},
/*001*/ "","",EXCLSV,0,0,0,{Btwo(32),Btwo(-1),2,3,4,5,0},
/*002*/ "New freq","fr",LEAF,1,0,0,{Btwo(0),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),5,0,Btwo(34),Btwo(0),0},
/*003*/ "New vel","vl",LEAF,1,1,0,{Btwo(8),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),5,0,Btwo(36),Btwo(0),0},
/*004*/ "Oscillator error (km/s to shift line)","oe",LEAF,1,2,0,{
        Btwo(16),Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(38),
        Btwo(0),0},
/*005*/ "Flip to opposite SSB, and new freq","ssbflip",LEAF,1,3,0,{
        Btwo(24),Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(40),
        Btwo(0),0}
};
