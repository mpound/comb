# include "../../parse/P.tree.h"

Node wft_[] =
{
/*000*/ "Write an image to a FITS file","wf",NORMAL,0,0,0,{1,2,6,7,8,
        9,10,11,12,0},
/*001*/ "Fits file name","ff",LEAF,0,0,0,{Btwo(64),Btwo(-1),3,1,
        Btwo(65),Btwo(0),Btwo(0),1,0,Btwo(40),Btwo(0),0},
/*002*/ "Action","",EXCLSV,0,1,0,{Btwo(42),Btwo(44),3,4,5,0},
/*003*/ "Image number","im",LEAF,2,0,0,{Btwo(24),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(46),Btwo(0),0},
/*004*/ "Stack number","st",LEAF,2,1,0,{Btwo(28),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(48),Btwo(0),0},
/*005*/ "Close tape","ct",LEAF,2,2,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Object","o",LEAF,0,2,0,{Btwo(129),Btwo(-1),3,1,Btwo(69),
        Btwo(0),Btwo(0),3,0,Btwo(50),Btwo(0),0},
/*007*/ "Telescope","tel",LEAF,0,3,0,{Btwo(198),Btwo(-1),3,1,Btwo(69),
        Btwo(0),Btwo(0),3,0,Btwo(52),Btwo(0),0},
/*008*/ "Reverse file in y","revy",LEAF,0,4,0,{Btwo(32),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(54),Btwo(0),0},
/*009*/ "Min Image Data Value for scaling","dmin",LEAF,0,5,0,{Btwo(0),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(56),Btwo(0),0},
/*010*/ "Max Image Data Value for scaling","dmax",LEAF,0,6,0,{Btwo(8),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(58),Btwo(0),0},
/*011*/ "Bitpix to use","bitpix",LEAF,0,7,0,{Btwo(36),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(60),Btwo(0),0},
/*012*/ "Blanking Value:","blank",LEAF,0,8,0,{Btwo(16),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(62),Btwo(0),0}
};
