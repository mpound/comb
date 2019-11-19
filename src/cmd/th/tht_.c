# include "../../parse/P.tree.h"

Node tht_[] =
{
/*000*/ "Temperature Histogram","th",NORMAL,0,0,0,{1,2,5,10,11,12,13,
        14,15,16,17,18,19,0},
/*001*/ "Temperature bin size (Kelvin)","tbin",LEAF,0,0,0,{Btwo(0),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(72),Btwo(0),0},
/*002*/ "Stack number limits","sn",NORMAL,0,1,0,{3,4,0},
/*003*/ "Low","l",LEAF,2,0,0,{Btwo(56),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(74),Btwo(0),0},
/*004*/ "High","h",LEAF,2,1,0,{Btwo(60),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(76),Btwo(0),0},
/*005*/ "Specify velocity limits","",EXCLSV,0,2,0,{Btwo(78),Btwo(80),
        6,7,0},
/*006*/ "All velocities","a",LEAF,5,0,0,{Btwo(-1),Btwo(-1),0,1,
        Btwo(1),Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*007*/ "Velocity limits","vl",NORMAL,5,1,0,{8,9,0},
/*008*/ "Low vel","l",LEAF,7,0,0,{Btwo(8),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(82),Btwo(0),0},
/*009*/ "High vel","h",LEAF,7,1,0,{Btwo(16),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(84),Btwo(0),0},
/*010*/ "Scale factor for x","xfact",LEAF,0,3,0,{Btwo(24),Btwo(-1),16,
        1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(86),Btwo(0),0},
/*011*/ "Scale factor for y","yfact",LEAF,0,4,0,{Btwo(32),Btwo(-1),16,
        1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(88),Btwo(0),0},
/*012*/ "Minimum x value to include","minx",LEAF,0,5,0,{Btwo(40),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(90),Btwo(0),0},
/*013*/ "Maximum x value to include","maxx",LEAF,0,6,0,{Btwo(48),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(92),Btwo(0),0},
/*014*/ "Plot x axis as log","logx",LEAF,0,7,0,{Btwo(64),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(94),Btwo(0),0},
/*015*/ "Plot y axis as log","logy",LEAF,0,8,0,{Btwo(68),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(96),Btwo(0),0},
/*016*/ "File Name for temperature histogram data","fn",LEAF,0,9,0,{
        Btwo(112),Btwo(-1),3,1,Btwo(65),Btwo(0),Btwo(0),5,0,Btwo(98),
        Btwo(0),0},
/*017*/ "Horizontal label","hlb",LEAF,0,10,0,{Btwo(177),Btwo(102),3,1,
        Btwo(41),Btwo(0),Btwo(0),4,0,Btwo(100),Btwo(0),0},
/*018*/ "Vertical label","vlb",LEAF,0,11,0,{Btwo(218),Btwo(106),3,1,
        Btwo(41),Btwo(0),Btwo(0),4,0,Btwo(104),Btwo(0),0},
/*019*/ "Main label","mlb",LEAF,0,12,0,{Btwo(259),Btwo(110),3,1,
        Btwo(41),Btwo(0),Btwo(0),4,0,Btwo(108),Btwo(0),0}
};