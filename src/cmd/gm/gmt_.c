# include "../../parse/P.tree.h"

Node gmt_[] =
{
/*000*/ "Graphics Manipulation","gm",NORMAL,0,0,0,{1,29,0},
/*001*/ "Action","",EXCLSV,0,0,0,{Btwo(76),Btwo(-1),2,5,8,11,15,18,21,
        28,0},
/*002*/ "Current box","cb",NORMAL,1,0,0,{3,4,0},
/*003*/ "Current column\0","cc",LEAF,2,0,0,{Btwo(48),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),5,1,Btwo(78),Btwo(0),0},
/*004*/ "Current row\0","cr",LEAF,2,1,0,{Btwo(52),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),5,1,Btwo(80),Btwo(0),0},
/*005*/ "Number of boxes","nb",NORMAL,1,1,0,{6,7,0},
/*006*/ "Number of columns","nc",LEAF,5,0,0,{Btwo(56),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(82),Btwo(0),0},
/*007*/ "Number of rows","nr",LEAF,5,1,0,{Btwo(60),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(84),Btwo(0),0},
/*008*/ "Scale","sc",NORMAL,1,2,0,{9,10,0},
/*009*/ "X scale\0","xs",LEAF,8,0,0,{Btwo(0),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),5,1,Btwo(86),Btwo(0),0},
/*010*/ "Y scale\0","ys",LEAF,8,1,0,{Btwo(8),Btwo(-1),16,1,Btwo(1),
        Btwo(0),Btwo(0),5,1,Btwo(88),Btwo(0),0},
/*011*/ "Title","ti",NORMAL,1,3,0,{12,13,14,0},
/*012*/ "text","t",LEAF,11,0,0,{Btwo(106),Btwo(-1),3,1,Btwo(65),
        Btwo(0),Btwo(0),1,0,Btwo(90),Btwo(0),0},
/*013*/ "Horizontal position","h",LEAF,11,1,0,{Btwo(16),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(92),Btwo(0),0},
/*014*/ "Vertical position","v",LEAF,11,2,0,{Btwo(24),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(94),Btwo(0),0},
/*015*/ "Move","mv",NORMAL,1,4,0,{16,17,0},
/*016*/ "Horizontal position","h",LEAF,15,0,0,{Btwo(16),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(92),Btwo(0),0},
/*017*/ "Vertical position","v",LEAF,15,1,0,{Btwo(24),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(94),Btwo(0),0},
/*018*/ "Line","li",NORMAL,1,5,0,{19,20,0},
/*019*/ "Horizontal position","h",LEAF,18,0,0,{Btwo(16),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(92),Btwo(0),0},
/*020*/ "Vertical position","v",LEAF,18,1,0,{Btwo(24),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(94),Btwo(0),0},
/*021*/ "Draw Figure","df",NORMAL,1,6,0,{22,23,24,25,26,27,0},
/*022*/ "Figure number","fign",LEAF,21,0,0,{Btwo(64),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(96),Btwo(0),0},
/*023*/ "Horizontal position","h",LEAF,21,1,0,{Btwo(16),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(92),Btwo(0),0},
/*024*/ "Vertical position","v",LEAF,21,2,0,{Btwo(24),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),4,0,Btwo(94),Btwo(0),0},
/*025*/ "Figure height","fh",LEAF,21,3,0,{Btwo(32),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),7,0,Btwo(98),Btwo(0),0},
/*026*/ "Figure width","fw",LEAF,21,4,0,{Btwo(40),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),7,0,Btwo(100),Btwo(0),0},
/*027*/ "Filled","fill",LEAF,21,5,0,{Btwo(68),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(102),Btwo(0),0},
/*028*/ "Re-plot","rp",LEAF,1,7,0,{Btwo(-1),Btwo(-1),0,1,Btwo(1),
        Btwo(0),Btwo(0),0,0,Btwo(0),Btwo(0),0},
/*029*/ "Line type","lt",LEAF,0,1,0,{Btwo(72),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(104),Btwo(0),0}
};
