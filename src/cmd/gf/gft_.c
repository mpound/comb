# include "../../parse/P.tree.h"

Node gft_[] =
{
/*000*/ "Fit a gaussian function to part of a spectrum","gf",NORMAL,0,
        0,0,{1,4,5,9,0},
/*001*/ "Horizontal interval of fit (plot units)","h",NORMAL,0,0,0,{2,
        3,0},
/*002*/ "From","f",LEAF,1,0,0,{Btwo(0),Btwo(-1),16,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(28),Btwo(0),0},
/*003*/ "To","t",LEAF,1,1,0,{Btwo(8),Btwo(-1),16,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(30),Btwo(0),0},
/*004*/ "Specify fwhm of gaussian(plot units)","wid",LEAF,0,1,0,{
        Btwo(16),Btwo(34),16,1,Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(32),
        Btwo(0),0},
/*005*/ "Action in stack 1","",EXCLSV,0,2,0,{Btwo(36),Btwo(-1),6,7,8,0
        },
/*006*/ "No action","na",LEAF,5,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "Replace with Gaussian and overlay plot","see",LEAF,5,1,0,{
        Btwo(-1),Btwo(-1),0,1,Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),
        Btwo(0),0},
/*008*/ "Remove (subtract)","rm",LEAF,5,2,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*009*/ "Don't print","dp",LEAF,0,3,0,{Btwo(24),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(38),Btwo(0),0}
};
