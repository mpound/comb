# include "../../parse/P.tree.h"

Node xft_[] =
{
/*000*/ 
        "Transfer stacks to directory 2 after making them unique (1/position)",
        "xf",NORMAL,0,0,0,{1,2,3,4,5,8,9,0},
/*001*/ "Range of stack number(s) to move","n",LEAF,0,0,0,{Btwo(40),
        Btwo(-1),8,2,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(60),Btwo(0),0},
/*002*/ "Center Frequency (MHz)","fr",LEAF,0,1,0,{Btwo(0),Btwo(-1),16,
        2,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(62),Btwo(0),0},
/*003*/ "Filter width","fwid",LEAF,0,2,0,{Btwo(16),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(64),Btwo(0),0},
/*004*/ "Position tolerance for combining (')","ptol",LEAF,0,3,0,{
        Btwo(24),Btwo(-1),16,1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(66),
        Btwo(0),0},
/*005*/ "Stack number limits for combining","sn",NORMAL,0,4,0,{6,7,0},
/*006*/ "Low","l",LEAF,5,0,0,{Btwo(48),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(68),Btwo(0),0},
/*007*/ "High","h",LEAF,5,1,0,{Btwo(52),Btwo(-1),8,1,Btwo(1),Btwo(0),
        Btwo(0),1,0,Btwo(70),Btwo(0),0},
/*008*/ "Maximum weight","mxwt",LEAF,0,5,0,{Btwo(32),Btwo(-1),16,1,
        Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(72),Btwo(0),0},
/*009*/ "Be more or less verbose","v",LEAF,0,6,0,{Btwo(56),Btwo(-1),8,
        1,Btwo(1),Btwo(0),Btwo(0),1,0,Btwo(74),Btwo(0),0}
};
