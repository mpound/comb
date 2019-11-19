# include "../../parse/P.tree.h"

Node ist_[] =
{
/*000*/ "Interpolate a spectrun for a given position","is",NORMAL,0,0,
        0,{1,4,5,13,16,17,18,19,0},
/*001*/ "Relative coordinate","rc",NORMAL,0,0,0,{2,3,0},
/*002*/ "ra/odra/dl/l/dx","x",LEAF,1,0,0,{Btwo(0),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(88),Btwo(0),0},
/*003*/ "dec/oddec/db/b/dy","y",LEAF,1,1,0,{Btwo(8),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(90),Btwo(0),0},
/*004*/ "Interpolation radius(')","ir",LEAF,0,1,0,{Btwo(16),Btwo(-1),
        16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(92),Btwo(0),0},
/*005*/ "Interpolation type","",EXCLSV,0,2,0,{Btwo(94),Btwo(96),6,7,8,
        11,0},
/*006*/ "Cone","ci",LEAF,5,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),Btwo(0),
        Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "Near point","ni",LEAF,5,1,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*008*/ "Bessel","bi",NORMAL,5,2,0,{9,10,0},
/*009*/ "Effective Antenna Diameter","efd",LEAF,8,0,0,{Btwo(24),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(98),Btwo(0),0},
/*010*/ "Grid Spacing of Stack Data (')","gs",LEAF,8,1,0,{Btwo(32),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(100),Btwo(0),0}
        ,
/*011*/ "Gaussian","gi",NORMAL,5,3,0,{12,0},
/*012*/ "FWHM of Gaussian ('Arc)","fwhm",LEAF,11,0,0,{Btwo(40),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(102),Btwo(0),0}
        ,
/*013*/ "Stack number limits","sn",NORMAL,0,3,0,{14,15,0},
/*014*/ "Low","l",LEAF,13,0,0,{Btwo(72),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(104),Btwo(0),0},
/*015*/ "High","h",LEAF,13,1,0,{Btwo(76),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(106),Btwo(0),0},
/*016*/ "Center Frequency (MHz)","fr",LEAF,0,4,0,{Btwo(48),Btwo(-1),
        16,2,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(108),Btwo(0),0},
/*017*/ "Filter width","fwid",LEAF,0,5,0,{Btwo(64),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(110),Btwo(0),0},
/*018*/ "Stack use array","u",LEAF,0,6,0,{Btwo(80),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),2,0,Btwo(112),Btwo(0),0},
/*019*/ "Test existence (.test)","t",LEAF,0,7,0,{Btwo(84),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(114),Btwo(0),0}
};
