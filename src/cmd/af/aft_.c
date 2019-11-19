# include "../../parse/P.tree.h"

Node aft_[] =
{
/*000*/ "Attach a FITS file to an image","af",NORMAL,0,0,0,{1,2,0},
/*001*/ "Fits file name","ff",LEAF,0,0,0,{Btwo(18),Btwo(-1),3,1,
        Btwo(65),Btwo(0),Btwo(0),7,0,Btwo(8),Btwo(0),0},
/*002*/ "Action","",EXCLSV,0,1,0,{Btwo(10),Btwo(12),3,4,0},
/*003*/ "Image number","im",LEAF,2,0,0,{Btwo(0),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(14),Btwo(0),0},
/*004*/ "Stack number","st",LEAF,2,1,0,{Btwo(4),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(16),Btwo(0),0}
};
