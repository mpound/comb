# include "../../parse/P.tree.h"

Node spt_[] =
{
/*000*/ "Make a scatter plot comparing two images","sp",NORMAL,0,0,0,{
        1,2,3,4,5,6,7,8,9,10,11,14,17,18,19,24,25,26,27,0},
/*001*/ "Image number for x axis","imx",LEAF,0,0,0,{Btwo(96),Btwo(-1),
        8,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(128),Btwo(0),0},
/*002*/ "Image number for y axis (< 0 for none)","imy",LEAF,0,1,0,{
        Btwo(100),Btwo(-1),8,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(130),
        Btwo(0),0},
/*003*/ "Area number","an",LEAF,0,2,0,{Btwo(104),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),1,0,Btwo(132),Btwo(0),0},
/*004*/ "File Name for scatter plot data","fn",LEAF,0,3,0,{Btwo(190),
        Btwo(-1),3,1,Btwo(65),Btwo(0),Btwo(0),5,0,Btwo(134),Btwo(0),0}
        ,
/*005*/ "Scale factor for x","xfact",LEAF,0,4,0,{Btwo(0),Btwo(-1),16,
        1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(136),Btwo(0),0},
/*006*/ "Scale factor for y","yfact",LEAF,0,5,0,{Btwo(8),Btwo(-1),16,
        1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(138),Btwo(0),0},
/*007*/ "Minimum x value to include","minx",LEAF,0,6,0,{Btwo(16),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(140),Btwo(0),0}
        ,
/*008*/ "Minimum y value to include","miny",LEAF,0,7,0,{Btwo(24),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(142),Btwo(0),0}
        ,
/*009*/ "Maximum x value to include","maxx",LEAF,0,8,0,{Btwo(32),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(144),Btwo(0),0}
        ,
/*010*/ "Maximum y value to include","maxy",LEAF,0,9,0,{Btwo(40),
        Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(146),Btwo(0),0}
        ,
/*011*/ "Vertical range to plot","v",NORMAL,0,10,0,{12,13,0},
/*012*/ "From","f",LEAF,11,0,0,{Btwo(48),Btwo(150),16,1,Btwo(1),
        Btwo(0),Btwo(0),4,0,Btwo(148),Btwo(0),0},
/*013*/ "To","t",LEAF,11,1,0,{Btwo(56),Btwo(154),16,1,Btwo(1),Btwo(0),
        Btwo(0),4,0,Btwo(152),Btwo(0),0},
/*014*/ "Horizontal range to plot","h",NORMAL,0,11,0,{15,16,0},
/*015*/ "From","f",LEAF,14,0,0,{Btwo(64),Btwo(158),16,1,Btwo(1),
        Btwo(0),Btwo(0),4,0,Btwo(156),Btwo(0),0},
/*016*/ "To","t",LEAF,14,1,0,{Btwo(72),Btwo(162),16,1,Btwo(1),Btwo(0),
        Btwo(0),4,0,Btwo(160),Btwo(0),0},
/*017*/ "Plot x axis as log","logx",LEAF,0,12,0,{Btwo(108),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(164),Btwo(0),0},
/*018*/ "Plot y axis as log","logy",LEAF,0,13,0,{Btwo(112),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),6,0,Btwo(166),Btwo(0),0},
/*019*/ "Plot Symbol","ps",NORMAL,0,14,0,{20,21,22,23,0},
/*020*/ "Figure Number","fign",LEAF,19,0,0,{Btwo(116),Btwo(-1),8,1,
        Btwo(1),Btwo(0),Btwo(0),3,0,Btwo(168),Btwo(0),0},
/*021*/ "Figure height","fh",LEAF,19,1,0,{Btwo(80),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(170),Btwo(0),0},
/*022*/ "Figure width","fw",LEAF,19,2,0,{Btwo(88),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(172),Btwo(0),0},
/*023*/ "Filled","fill",LEAF,19,3,0,{Btwo(120),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(174),Btwo(0),0},
/*024*/ "Horizontal label","hlb",LEAF,0,15,0,{Btwo(255),Btwo(178),3,1,
        Btwo(41),Btwo(0),Btwo(0),4,0,Btwo(176),Btwo(0),0},
/*025*/ "Vertical label","vlb",LEAF,0,16,0,{Btwo(296),Btwo(182),3,1,
        Btwo(41),Btwo(0),Btwo(0),4,0,Btwo(180),Btwo(0),0},
/*026*/ "Main label","mlb",LEAF,0,17,0,{Btwo(337),Btwo(186),3,1,
        Btwo(41),Btwo(0),Btwo(0),4,0,Btwo(184),Btwo(0),0},
/*027*/ "Overlay","ovl",LEAF,0,18,0,{Btwo(124),Btwo(-1),8,1,Btwo(1),
        Btwo(0),Btwo(0),6,0,Btwo(188),Btwo(0),0}
};
