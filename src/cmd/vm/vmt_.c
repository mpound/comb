# include "../../parse/P.tree.h"

Node vmt_[] =
{
/*000*/ "Calculate Virial Mass","vm",NORMAL,0,0,0,{1,2,3,4,12,15,18,
        19,20,21,22,0},
/*001*/ "Area Number","an",LEAF,0,0,0,{Btwo(96),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(112),Btwo(0),0},
/*002*/ "Spacing of points('Arc)","sp",LEAF,0,1,0,{Btwo(0),Btwo(-1),
        16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(114),Btwo(0),0},
/*003*/ "Interpolation radius(')","ir",LEAF,0,2,0,{Btwo(8),Btwo(-1),
        16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(116),Btwo(0),0},
/*004*/ "Interpolation type","",EXCLSV,0,3,0,{Btwo(118),Btwo(120),5,6,
        7,10,0},
/*005*/ "Cone","ci",LEAF,4,0,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),Btwo(0),
        Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*006*/ "Near point","ni",LEAF,4,1,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "Bessel","bi",NORMAL,4,2,0,{8,9,0},
/*008*/ "Effective Antenna Diameter","efd",LEAF,7,0,0,{Btwo(16),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(122),Btwo(0),0}
        ,
/*009*/ "Grid Spacing of Stack Data (')","gs",LEAF,7,1,0,{Btwo(24),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(124),Btwo(0),0}
        ,
/*010*/ "Gaussian","gi",NORMAL,4,3,0,{11,0},
/*011*/ "FWHM of Gaussian ('Arc)","fwhm",LEAF,10,0,0,{Btwo(32),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(126),Btwo(0),0}
        ,
/*012*/ "Stack number limits","sn",NORMAL,0,4,0,{13,14,0},
/*013*/ "Low","l",LEAF,12,0,0,{Btwo(100),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(128),Btwo(0),0},
/*014*/ "High","h",LEAF,12,1,0,{Btwo(104),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(130),Btwo(0),0},
/*015*/ "Velocity limits of integration","vl",NORMAL,0,5,0,{16,17,0},
/*016*/ "Low vel","l",LEAF,15,0,0,{Btwo(40),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(132),Btwo(0),0},
/*017*/ "High vel","h",LEAF,15,1,0,{Btwo(48),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(134),Btwo(0),0},
/*018*/ "Center Frequency (MHz)","fr",LEAF,0,6,0,{Btwo(56),Btwo(-1),
        16,2,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(136),Btwo(0),0},
/*019*/ "Filter width","fwid",LEAF,0,7,0,{Btwo(72),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(138),Btwo(0),0},
/*020*/ "Cloud distance (pc) for vm calc","dist",LEAF,0,8,0,{Btwo(80),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(140),Btwo(0),0}
        ,
/*021*/ "Scale for mass from integrated line","scale",LEAF,0,9,0,{
        Btwo(88),Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(142),
        Btwo(0),0},
/*022*/ "Be verbose","v",LEAF,0,10,0,{Btwo(108),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(144),Btwo(0),0}
};
