
# line 2 "P.yacc.y"
# include <string.h>
# include "P.tree.h"
#include "../main/C.h"
#if COMB
#include "../scan/C.h"
#endif /*COMB*/
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.extern.h"
# include <stdio.h>
# include "../error/C.h"

# define PUSH *cda_stp++ = cda_type;cda_type = REAL | INTN | STRING;
# define PULL cda_type = *--cda_stp;
# define M_PI	3.14159265358979323846
static double degToRad = M_PI/180.;

int cda_stack[100];		/* stack for storing current data type */
int *cda_stp = cda_stack;	/* stack pointer */
# define END_T 257
# define NUM_T 258
# define STR_T 259
# define FILNM_T 260
# define NAND_T 261
# define SAND_T 262
# define LN_T 263
# define LOG_T 264
# define SIN_T 265
# define COS_T 266
# define TAN_T 267
# define ATAN_T 268
# define SUB_T 269
# define EXP_T 270
# define ABS_T 271
# define X_T 272
# define Y_T 273
# define CRSR_T 274
# define HDRVAR_T 275
# define STRHDRVAR_T 276
# define EQUEQU_T 277
# define EOARGS 1
# define LOW 279
# define COND 280
# define EQUEQU 281
# define HIGH 282
# define HIGHER 283

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef yyerror
	void yyerror(const char *);
#endif

#ifndef yylex
#ifdef __EXTERN_C__
	extern "C" { int yylex(void); }
#else
	int yylex(void);
#endif
#endif
	int yyparse(void);

#endif
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256
static const yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 97,
	69, 0,
	-2, 20,
-1, 98,
	62, 0,
	60, 0,
	-2, 22,
-1, 100,
	62, 0,
	60, 0,
	-2, 23,
-1, 103,
	62, 0,
	60, 0,
	-2, 27,
-1, 136,
	62, 0,
	60, 0,
	-2, 24,
-1, 137,
	62, 0,
	60, 0,
	-2, 25,
-1, 138,
	62, 0,
	60, 0,
	-2, 26,
-1, 139,
	62, 0,
	60, 0,
	-2, 28,
	};
# define YYNPROD 85
# define YYLAST 1461
static const yytabelem yyact[]={

     6,    37,    38,    51,    46,   104,    14,   106,   167,    43,
    41,    15,    42,   172,    44,    47,    66,    83,    82,    78,
    81,    66,    36,    35,   179,    46,   168,   101,   102,    16,
    46,   165,    87,    85,    37,    38,    47,    80,   178,    14,
    45,    47,   164,     5,    15,     4,     2,     3,     1,    33,
    28,     0,     0,     0,    37,    38,    39,    57,     8,    14,
     0,    45,    16,     0,    15,     0,    45,     0,     0,     0,
    51,     0,     0,    37,    38,     0,     0,     0,    14,   123,
     0,     0,    16,    15,     0,     0,     0,     0,     0,     0,
     0,    51,    37,    38,     0,     0,    51,    14,   121,     0,
     0,    16,    15,     0,     0,     0,     0,   108,     0,     0,
     0,    37,    38,     0,     0,     0,    14,    46,     0,     0,
    16,    15,     0,     0,     0,     0,     0,     0,    47,     0,
     0,    37,    38,     0,     0,   134,    14,    99,    93,    16,
     0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
    37,    38,     0,     0,     0,    14,    50,     0,     0,    16,
    15,     0,     0,     0,     0,     0,     0,     0,     0,    37,
    38,     0,     0,     0,    14,     0,     0,     0,    16,    15,
     0,    65,   161,    51,     0,     0,     0,     0,   162,     0,
    38,     0,    38,     0,    59,     0,    59,    16,     0,    67,
    68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
   169,    79,     0,    38,     0,     0,     0,    59,   111,     0,
     0,     0,     0,    50,    12,    30,    52,    13,    31,    17,
    18,    19,    20,    21,    22,    32,    23,    24,    25,    26,
    27,    29,    34,     0,    50,     0,     0,     0,     0,    50,
    86,    84,     0,     0,     0,     0,     7,    12,    30,    11,
    13,    31,    17,    18,    19,    20,    21,    22,    32,    23,
    24,    25,    26,    27,    29,    34,     0,    12,    30,   142,
    13,    31,    17,    18,    19,    20,    21,    22,    32,    23,
    24,    25,    26,    27,    29,    34,    12,    30,     0,    13,
    31,    17,    18,    19,    20,    21,    22,    32,    23,    24,
    25,    26,    27,    29,    34,    12,    30,     0,    13,    31,
    17,    18,    19,    20,    21,    22,    32,    23,    24,    25,
    26,    27,    29,    34,    12,    30,    50,    13,    31,    17,
    18,    19,    20,    21,    22,    32,    23,    24,    25,    26,
    27,    29,    34,     0,    12,    30,     0,    13,    31,    17,
    18,    19,    20,    21,    22,    32,    23,    24,    25,    26,
    27,    29,    34,    12,    30,    89,    13,    31,    17,    18,
    19,    20,    21,    22,    32,    23,    24,    25,    26,    27,
    29,    34,    12,    30,     0,    13,    31,    17,    18,    19,
    20,    21,    22,    32,    23,    24,    25,    26,    27,    29,
    34,    43,    41,    30,    42,    30,    44,    43,    41,     0,
    42,     0,    44,    32,     0,     0,     0,    46,     8,    49,
    34,    48,    34,    46,     8,    49,    30,    48,    47,    31,
    43,    41,     0,    42,    47,    44,    32,     0,     0,     0,
     0,     0,     0,    34,     0,     0,    46,     8,    49,     0,
    48,    38,     0,    45,   109,    59,     0,    47,     0,    45,
    40,     0,     0,     0,     0,     0,    38,   183,    43,    41,
    59,    42,     0,    44,     0,   182,    43,    41,     0,    42,
     0,    44,    45,    51,    46,     0,    49,     0,    48,    51,
    56,     0,    46,     0,    49,    47,    48,    38,   176,    43,
    41,    59,    42,    47,    44,     0,     0,     0,     0,     0,
     0,     0,    51,     0,     0,    46,     0,    49,     0,    48,
    45,   160,    43,    41,     0,    42,    47,    44,    45,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    46,   158,
    49,    43,    48,     0,     0,     0,    44,     0,     0,    47,
    51,    45,    38,     0,     0,     0,    59,    46,    51,     0,
     0,   159,    43,    41,     0,    42,     0,    44,    47,     0,
     0,     0,     0,     0,    45,     8,    56,     0,    46,     0,
    49,    51,    48,     0,   157,    43,    41,     0,    42,    47,
    44,    38,     0,    45,     0,    59,   111,     0,     0,     0,
     0,    46,     0,    49,    51,    48,     0,     0,     0,     0,
     0,     0,    47,     0,    45,    56,     7,     0,    52,    54,
     0,    53,     7,    51,    52,    54,     0,    53,     0,     0,
     0,     0,     0,     0,     0,     0,    50,    45,     0,     0,
     0,     0,    50,     0,    51,     7,     0,    52,    54,     0,
    53,     0,   156,    43,    41,     0,    42,     0,    44,     0,
     0,     0,   155,    43,    41,    50,    42,    51,    44,    46,
     0,    49,     0,    48,    30,     0,     0,    31,     0,    46,
    47,    49,     0,    48,    32,    52,    54,     0,    53,    30,
    47,    34,    31,    52,    54,     0,    53,     0,     0,    32,
     0,     0,     0,    50,     0,    45,    34,    55,   154,    43,
    41,    50,    42,     0,    44,    45,    52,    54,     0,    53,
    30,     0,     0,    31,     0,    46,     0,    49,     0,    48,
    32,     0,     0,     0,    50,    51,    47,    34,     0,    52,
    54,     0,    53,     0,     0,    51,   153,    43,    41,     0,
    42,     0,    44,     0,     0,     0,     0,    50,     0,     0,
     0,    45,     0,    46,     0,    49,     0,    48,     0,     0,
     0,     0,     0,     7,    47,    30,    50,     0,    31,    52,
    54,     0,    53,     0,     0,    32,     0,     0,     0,     0,
     0,    51,    34,    55,   152,    43,    41,    50,    42,    45,
    44,     0,    52,    54,     0,    53,   151,    43,    41,     0,
    42,    46,   150,    49,    30,    48,     0,    31,     0,     0,
    50,     0,    47,    46,    32,    49,     0,    48,     0,    51,
     0,    34,    55,     0,    47,     0,     0,   149,    43,    41,
     0,    42,     0,    44,     0,     0,     0,    45,    43,    41,
     0,    42,     0,    44,    46,     0,    49,     0,    48,    45,
     0,     0,     0,     0,    46,    47,    49,     0,    48,     0,
    52,    54,     0,    53,     0,    47,     0,    51,     0,     0,
    52,    54,     0,    53,     0,     0,     0,     0,    50,    51,
    45,     0,     0,   148,    43,    41,     0,    42,    50,    44,
    45,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    46,     0,    49,     0,    48,     0,     0,     0,     0,     0,
    51,    47,     0,     0,     0,     0,    52,    54,     0,    53,
   177,   147,    43,    41,     0,    42,     0,    44,     0,     0,
     0,     0,     0,     0,    50,     0,    45,     0,    46,     0,
    49,     0,    48,     0,     0,     0,     0,     0,     0,    47,
     0,     0,     0,     0,    52,    54,     0,    53,   146,    43,
    41,     0,    42,     0,    44,     0,    51,     0,     0,     0,
     0,     0,    50,     0,    45,    46,     0,    49,     0,    48,
     0,     0,     0,     0,     0,     0,    47,     0,     0,     0,
     0,   144,    43,    41,     0,    42,     0,    44,     0,     0,
     0,     0,    52,    54,    51,    53,     0,     0,    46,     0,
    49,    45,    48,     0,    52,    54,     0,    53,     0,    47,
    50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    50,   110,    43,    41,     0,    42,     0,    44,
     0,   145,     0,     0,    45,    52,    54,     0,    53,     0,
    46,     0,    49,     0,    48,    52,    54,     0,    53,     0,
     0,    47,     0,    50,     0,    43,    41,     0,    42,     0,
    44,     0,     0,    50,    51,     0,     0,     0,     0,    43,
    41,    46,    42,    49,    44,    48,    45,     0,     0,     0,
     0,     0,    47,     0,     0,    46,     0,    49,     0,    48,
     0,    52,    54,     0,    53,     0,    47,    61,     0,    10,
     0,     0,     0,     0,     0,     0,    51,    45,    58,    50,
     0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
     0,    45,     0,     0,     0,     0,     0,     0,     0,    52,
    54,     0,    53,     0,     0,     0,     0,   175,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
     0,   143,     0,   105,     0,     0,    58,   107,     0,    58,
     0,    58,     0,     0,     0,     0,    52,    54,     0,    53,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   129,
   130,   131,     0,     0,    50,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
    54,     0,    53,    58,   140,    58,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    58,    58,    58,
     0,     0,     0,     0,     0,     0,     0,     0,    58,     0,
     0,    52,    54,     0,    53,     0,     0,     0,     0,     0,
     9,     0,     0,     0,     0,     0,     0,     0,     0,    50,
     0,     0,    60,     0,     0,    62,    64,     0,     0,     0,
     0,     0,    52,    54,     0,    53,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    52,    54,     0,    53,
    50,    88,    90,    91,    92,    94,    95,    96,    97,    98,
   100,   103,     0,     0,    50,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   112,     0,   113,   114,
   115,   116,   117,   118,   119,   120,   122,   124,   125,   126,
   127,   128,     0,     0,     0,     0,   132,     0,   133,     0,
   135,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   136,     0,   137,   138,     0,   139,     0,     0,     0,     0,
   141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   163,     0,     0,     0,     0,     0,     0,
     0,   166,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   170,   171,     0,   173,   174,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
   181 };
static const yytabelem yypact[]={

-10000000,    -1,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   375,
   526,   134,-10000000,-10000000,   134,   134,   -19,   -19,   -19,   -19,
   -19,   -19,   -19,   -19,   -19,   -19,   -19,   -19,   -42,   -24,
-10000000,-10000000,   -20,   -43,   -44,    -7,    -8,-10000000,-10000000,-10000000,
   115,   134,   134,    96,   134,   134,   134,   134,    76,   -34,
   134,  -119,-10000000,-10000000,-10000000,   471,   -55,-10000000,   471,   471,
   369,   440,  1012,   565,    59,   134,-10000000,   134,   134,   134,
   134,   134,   134,   134,   134,    57,    38,   134,   134,   134,
   134,   471,   471,   471,-10000000,   134,-10000000,   134,   398,   134,
   509,   509,   -54,-10000000,   -54,   -54,    59,  -121,   -33,   134,
   -33,   134,   134,   -33,   134,   154,   471,   177,-10000000,    19,
-10000000,-10000000,  1057,   970,   937,   900,   862,   806,   775,   763,
   715,-10000000,   677,-10000000,   631,   621,   -33,   553,   -33,   425,
   156,   156,   530,   490,-10000000,   398,   -33,   -33,   -33,   -33,
   154,   398,   134,  -119,-10000000,  -119,-10000000,-10000000,-10000000,-10000000,
   134,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   -53,-10000000,-10000000,
-10000000,-10000000,-10000000,   398,   134,   134,   -28,   134,   134,-10000000,
  1043,   467,-10000000,   -33,   816,  -119,-10000000,  -119,   134,   134,
   444,   436,-10000000,-10000000 };
static const yytabelem yypgo[]={

     0,  1280,    50,    49,  1127,    48,    46,    47,    45,    43,
   181,    42,    38,    31,    26,    24,    23,    22 };
static const yytabelem yyr1[]={

     0,     5,     5,     5,     5,     5,     5,     6,     6,     7,
     7,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,    11,    12,     1,     1,     1,    13,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     8,     4,     4,     4,     4,    14,    15,     4,     4,     4,
     4,     4,     9,     9,     9,     9,    10,     2,     2,     2,
    16,     3,     3,     3,    17 };
static const yytabelem yyr2[]={

     0,     0,     4,     4,     4,     4,     5,     2,     3,     5,
     9,     2,     3,     7,     7,     7,     7,     7,     7,     7,
     7,     5,     7,     7,     9,     9,     9,     7,     9,     7,
     9,     1,     1,    21,     9,     9,     1,    15,     9,     9,
     9,    13,     9,     9,     9,     7,     7,     9,     9,     9,
     7,     3,     3,     9,     7,    13,     5,     5,     5,     7,
     5,     3,     3,     4,     7,     1,     1,    21,     7,     3,
     3,     7,     7,    11,    11,    13,     3,     5,     9,     3,
     3,     5,     9,     3,     3 };
static const yytabelem yychk[]={

-10000000,    -5,    -6,    -7,    -8,    -9,     1,   257,    59,    -1,
    -4,   260,   258,   261,    40,    45,    63,   263,   264,   265,
   266,   267,   268,   270,   271,   272,   273,   274,    -2,   275,
   259,   262,   269,    -3,   276,   -16,   -17,    35,    36,    -6,
    95,    43,    45,    42,    47,    94,    58,    69,    62,    60,
   277,   124,   259,   262,   260,   277,    60,    -6,    -4,    40,
    -1,    -4,    -1,    -4,    -1,   -10,    40,   -10,   -10,   -10,
   -10,   -10,   -10,   -10,   -10,   -10,   -10,   -10,    61,   -10,
    61,    40,    61,    61,   258,    40,   258,    40,    -1,   260,
    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    61,
    -1,    61,    62,    -1,   124,    -4,    62,    -4,    -6,    95,
    41,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    41,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -4,
    -4,    -4,    -1,    -1,    -6,    -1,    -1,    -1,    -1,    -1,
    -4,    -1,   260,   124,    41,   124,    41,    41,    41,    41,
    47,    41,    41,    41,    41,    41,    41,    41,   124,    41,
    41,    -6,    -6,    -1,   -11,   -13,    -1,    61,   -14,    -6,
    -1,    -1,    41,    -1,    -1,   124,    41,   124,   -12,   -15,
    -1,    -1,    41,    41 };
static const yytabelem yydef[]={

     1,    -2,     2,     3,     4,     5,     6,     7,     8,     0,
     0,     0,    11,    12,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    51,    52,
    61,    62,     0,    69,    70,    79,    83,    80,    84,     9,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    56,    57,    58,     0,     0,    60,    63,     0,
     0,     0,     0,     0,    21,     0,    76,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    77,     0,    81,     0,     0,     0,
    14,    15,    16,    59,    17,    18,    19,    -2,    -2,     0,
    -2,     0,     0,    -2,     0,    29,     0,     0,    72,     0,
    13,    64,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    45,     0,    46,     0,     0,    50,     0,    54,     0,
    68,    71,     0,     0,    10,     0,    -2,    -2,    -2,    -2,
    30,     0,     0,    31,    34,    36,    35,    38,    39,    40,
     0,    42,    43,    44,    47,    48,    49,    53,    65,    78,
    82,    74,    73,     0,     0,     0,    17,     0,     0,    75,
     0,     0,    41,    55,     0,    32,    37,    66,     0,     0,
     0,     0,    33,    67 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"END_T",	257,
	"NUM_T",	258,
	"STR_T",	259,
	"FILNM_T",	260,
	"NAND_T",	261,
	"SAND_T",	262,
	"LN_T",	263,
	"LOG_T",	264,
	"SIN_T",	265,
	"COS_T",	266,
	"TAN_T",	267,
	"ATAN_T",	268,
	"SUB_T",	269,
	"EXP_T",	270,
	"ABS_T",	271,
	"X_T",	272,
	"Y_T",	273,
	"CRSR_T",	274,
	"HDRVAR_T",	275,
	"STRHDRVAR_T",	276,
	"EQUEQU_T",	277,
	"EOARGS",	1,
	"LOW",	279,
	"COND",	280,
	">",	62,
	"<",	60,
	"EQUEQU",	281,
	"=",	61,
	"+",	43,
	"-",	45,
	"*",	42,
	"/",	47,
	"^",	94,
	"HIGH",	282,
	"HIGHER",	283,
	":",	58,
	"E",	69,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"args : /* empty */",
	"args : args endcode",
	"args : args number",
	"args : args string",
	"args : args scannum",
	"args : args EOARGS",
	"endcode : END_T",
	"endcode : ';'",
	"number : nexp endcode",
	"number : nexp '_' nexp endcode",
	"nexp : NUM_T",
	"nexp : NAND_T",
	"nexp : '(' nexp ')'",
	"nexp : nexp '+' nexp",
	"nexp : nexp '-' nexp",
	"nexp : nexp '*' nexp",
	"nexp : nexp '/' nexp",
	"nexp : nexp '^' nexp",
	"nexp : nexp ':' nexp",
	"nexp : nexp 'E' nexp",
	"nexp : '-' nexp",
	"nexp : nexp '>' nexp",
	"nexp : nexp '<' nexp",
	"nexp : nexp '>' '=' nexp",
	"nexp : nexp '<' '=' nexp",
	"nexp : nexp '<' '>' nexp",
	"nexp : nexp EQUEQU_T nexp",
	"nexp : nexp '|' '|' nexp",
	"nexp : sexp EQUEQU_T sexp",
	"nexp : sexp '<' '>' sexp",
	"nexp : '?' push nexp '|'",
	"nexp : '?' push nexp '|' nexp '|'",
	"nexp : '?' push nexp '|' nexp '|' nexp ')'",
	"nexp : LN_T push nexp ')'",
	"nexp : LOG_T push nexp ')'",
	"nexp : LOG_T push nexp '|'",
	"nexp : LOG_T push nexp '|' nexp ')'",
	"nexp : SIN_T push nexp ')'",
	"nexp : COS_T push nexp ')'",
	"nexp : TAN_T push nexp ')'",
	"nexp : ATAN_T push nexp '/' nexp ')'",
	"nexp : ATAN_T push nexp ')'",
	"nexp : EXP_T push nexp ')'",
	"nexp : ABS_T push nexp ')'",
	"nexp : X_T push ')'",
	"nexp : Y_T push ')'",
	"nexp : X_T push nexp ')'",
	"nexp : Y_T push nexp ')'",
	"nexp : CRSR_T push nexp ')'",
	"nexp : nvar '=' nexp",
	"nexp : nvar",
	"nexp : HDRVAR_T",
	"nexp : HDRVAR_T push nexp ')'",
	"nexp : HDRVAR_T '=' nexp",
	"nexp : HDRVAR_T push nexp ')' '=' nexp",
	"nexp : nexp STR_T",
	"nexp : nexp SAND_T",
	"nexp : nexp FILNM_T",
	"nexp : nexp '*' '*'",
	"string : sexp endcode",
	"sexp : STR_T",
	"sexp : SAND_T",
	"sexp : sexp sexp",
	"sexp : '(' sexp ')'",
	"sexp : SUB_T '(' sexp '|'",
	"sexp : SUB_T '(' sexp '|' nexp '|'",
	"sexp : SUB_T '(' sexp '|' nexp '|' nexp ')'",
	"sexp : svar '=' sexp",
	"sexp : svar",
	"sexp : STRHDRVAR_T",
	"sexp : STRHDRVAR_T '=' sexp",
	"scannum : FILNM_T nexp endcode",
	"scannum : FILNM_T nexp '_' nexp endcode",
	"scannum : nexp '_' FILNM_T nexp endcode",
	"scannum : FILNM_T nexp '_' FILNM_T nexp endcode",
	"push : '('",
	"nvar : nvart NUM_T",
	"nvar : nvart '(' nexp ')'",
	"nvar : nvart",
	"nvart : '#'",
	"svar : svart NUM_T",
	"svar : svart '(' nexp ')'",
	"svar : svart",
	"svart : '$'",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.14	97/01/16 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yymaxdepth * sizeof (type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register const int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 6:
# line 80 "P.yacc.y"
{
			YYACCEPT;
		} break;
case 8:
# line 91 "P.yacc.y"
{
			/* insure that lex will return an end */
			/* with next call so that yacc will */
			/* stop */
			unput(EOARGS);
		} break;
case 9:
# line 104 "P.yacc.y"
{
			/* store value */
			st_num(yypvt[-1].real,yypvt[-1].real);

			/* set default flags */
			tag(cnode);
		} break;
case 10:
# line 113 "P.yacc.y"
{

			/* reset second part of range flag */
			spor = 0;

			/* check that cnode is a range */
			if(SEQ(cnode) != RANGE)
			{
				if(unspt[-1] != 1)str -= yyleng;
				nferr("trying to enter range in non-range");
			}

			/* store values */
			st_num(yypvt[-3].real,yypvt[-1].real);

			/* set default flags */
			tag(cnode);
		} break;
case 12:
# line 135 "P.yacc.y"
{
			/* check that "&" may be used */
			if(!(USE_TYPE(cnode) & cda_type))
				nferr("old data type incompatable");

			/* get value */
			switch(USE_TYPE(cnode))
			{
				case REAL:
					if(spor)
						yyval.real = L_PARAM2(double,cnode);
					else
						yyval.real = L_PARAM(double,cnode);
					break;
				case INTN:
					if(spor)
						yyval.real = L_PARAM2(int,cnode);
					else
						yyval.real = L_PARAM(int,cnode);
					break;
#if COMB
				case SCAN:
					if(spor)
						yyval.real = L_PARAM2(Scan,cnode).num;
					else
						yyval.real = L_PARAM(Scan,cnode).num;
					break;
#endif /*COMB*/
			}

			/* limit data type to current data type */
			cda_type = USE_TYPE(cnode);
		} break;
case 13:
# line 169 "P.yacc.y"
{
			yyval.real = yypvt[-1].real;
		} break;
case 14:
# line 173 "P.yacc.y"
{
			yyval.real = yypvt[-2].real + yypvt[-0].real;
		} break;
case 15:
# line 177 "P.yacc.y"
{
			yyval.real = yypvt[-2].real - yypvt[-0].real;
		} break;
case 16:
# line 181 "P.yacc.y"
{
			yyval.real = yypvt[-2].real * yypvt[-0].real;
		} break;
case 17:
# line 185 "P.yacc.y"
{
			/* check for error */
			if(yypvt[-0].real == 0)
				nferr("divide by zero");

			yyval.real = yypvt[-2].real / yypvt[-0].real;
		} break;
case 18:
# line 193 "P.yacc.y"
{
			/* check for errors */
			if(yypvt[-0].real != INT(yypvt[-0].real) && yypvt[-2].real < 0)
			{
				str -= yyleng;
				nferr("negative number to non-integral power");
			}
			if(yypvt[-0].real < 0 && yypvt[-2].real == 0)
			{
				str -= yyleng;
				nferr("zero to negative power");
			}

			yyval.real = pow(yypvt[-2].real,yypvt[-0].real);
		} break;
case 19:
# line 209 "P.yacc.y"
{
			/* check for errors */
			if(!(cda_type &= (REAL | INTN)))
			{
				str -= yyleng;
				nferr("hh:mm:ss only works for numbers");
			}
			if(yypvt[-0].real < 0)
			{
				str -= yyleng;
				nferr("negative number in hh:mm:ss");
			}

			yyval.real = yypvt[-2].real + yypvt[-0].real / 60;
		} break;
case 20:
# line 225 "P.yacc.y"
{
			yyval.real = yypvt[-2].real * pow(10.,yypvt[-0].real);
		} break;
case 21:
# line 229 "P.yacc.y"
{
			yyval.real = -yypvt[-0].real;
		} break;
case 22:
# line 233 "P.yacc.y"
{
			yyval.real = (yypvt[-2].real > yypvt[-0].real);
		} break;
case 23:
# line 237 "P.yacc.y"
{
			yyval.real = (yypvt[-2].real < yypvt[-0].real);
		} break;
case 24:
# line 241 "P.yacc.y"
{
			yyval.real = (yypvt[-3].real >= yypvt[-0].real);
		} break;
case 25:
# line 250 "P.yacc.y"
{
			yyval.real = (yypvt[-3].real <= yypvt[-0].real);
		} break;
case 26:
# line 259 "P.yacc.y"
{
			yyval.real = (yypvt[-3].real != yypvt[-0].real);
		} break;
case 27:
# line 263 "P.yacc.y"
{
			yyval.real = (yypvt[-2].real == yypvt[-0].real);
		} break;
case 28:
# line 268 "P.yacc.y"
{
			if(yypvt[-0].real == 1 || yypvt[-3].real == 1)
				yyval.real = 1;
			else
				yyval.real = 0;
		} break;
case 29:
# line 281 "P.yacc.y"
{
			if(yypvt[-0].string - yypvt[-2].string != strlen(yypvt[-0].string))
				yyval.real = 0;
			else
				yyval.real = !strncmp(yypvt[-2].string, yypvt[-0].string, yypvt[-0].string - yypvt[-2].string);
		} break;
case 30:
# line 289 "P.yacc.y"
{
			if(yypvt[-0].string - yypvt[-3].string != strlen(yypvt[-0].string))
				yyval.real = 1;
			else
				yyval.real = strncmp(yypvt[-3].string, yypvt[-0].string, yypvt[-0].string - yypvt[-3].string);
		} break;
case 31:
# line 295 "P.yacc.y"
{PULL PUSH} break;
case 32:
# line 295 "P.yacc.y"
{PULL PUSH} break;
case 33:
# line 296 "P.yacc.y"
{
			PULL
			if((int)(yypvt[-7].real))
				yyval.real = yypvt[-4].real;
			else
				yyval.real = yypvt[-1].real;
		} break;
case 34:
# line 304 "P.yacc.y"
{
			PULL;

			/* check for errors */
			if(yypvt[-1].real < 0)
				nferr("natural log of a negative number");
			if(yypvt[-1].real == 0)
				nferr("natural log of zero");

			yyval.real = log(yypvt[-1].real);
		} break;
case 35:
# line 316 "P.yacc.y"
{
			PULL;

			/* check for errors */
			if(yypvt[-1].real < 0)
				nferr("log base ten of a negative number");
			if(yypvt[-1].real == 0)
				nferr("log base ten of zero");

			yyval.real = log10(yypvt[-1].real);
		} break;
case 36:
# line 327 "P.yacc.y"
{PULL PUSH} break;
case 37:
# line 328 "P.yacc.y"
{
			PULL;

			/* check for errors */
			if(yypvt[-4].real < 0)
				nferr("log with a negative base");
			if(yypvt[-4].real == 0)
				nferr("log base zero");
			if(yypvt[-1].real < 0)
			{
				sprintf(errstr,
					"log base %g of a negative number",
					yypvt[-4].real);
				nferr(errstr);
			}
			if(yypvt[-1].real == 0)
			{
				sprintf(errstr,"log base %g of zero",yypvt[-4].real);
				nferr(errstr);
			}

			yyval.real = log(yypvt[-1].real) / log(yypvt[-4].real);
		} break;
case 38:
# line 352 "P.yacc.y"
{
			PULL;

			yyval.real = sin(yypvt[-1].real * degToRad);
		} break;
case 39:
# line 358 "P.yacc.y"
{
			PULL;

			yyval.real = cos(yypvt[-1].real * degToRad);
		} break;
case 40:
# line 364 "P.yacc.y"
{
			PULL;

			/* check for errors */
			if(yypvt[-1].real == 90)
				nferr("tangent of vertical line");

			yyval.real = sin(yypvt[-1].real * degToRad) / cos(yypvt[-1].real * degToRad);
		} break;
case 41:
# line 374 "P.yacc.y"
{
			PULL;

			yyval.real = atan2(yypvt[-3].real,yypvt[-1].real) / degToRad;
		} break;
case 42:
# line 380 "P.yacc.y"
{
			PULL;

			yyval.real = atan2(yypvt[-1].real,1.) / degToRad;
		} break;
case 43:
# line 386 "P.yacc.y"
{
			PULL;
			yyval.real = exp(yypvt[-1].real);
		} break;
case 44:
# line 391 "P.yacc.y"
{
			PULL;
			yyval.real = fabs(yypvt[-1].real);
		} break;
case 45:
# line 396 "P.yacc.y"
{
			double txc,tyc;
			char tchar;

			PULL;

/*
			gcurs_(&txc,&tyc,&tchar);
*/
			yyval.real = txc;
		} break;
case 46:
# line 408 "P.yacc.y"
{
			double txc,tyc;
			char tchar;

			PULL;

/*
			gcurs_(&txc,&tyc,&tchar);
*/
			yyval.real = tyc;
		} break;
case 47:
# line 420 "P.yacc.y"
{
			PULL;

			/* check for error */
			if(INT(yypvt[-1].real) < 0 || INT(yypvt[-1].real) > 9)
				error_("Illegal cursor subscript");

			yyval.real = xc[INT(yypvt[-1].real)];
		} break;
case 48:
# line 430 "P.yacc.y"
{
			PULL;

			/* check for error */
			if(INT(yypvt[-1].real) < 0 || INT(yypvt[-1].real) > 9)
				error_("Illegal cursor subscript");

			yyval.real = yc[INT(yypvt[-1].real)];
		} break;
case 49:
# line 440 "P.yacc.y"
{
			PULL;

			/* check for error */
			if(INT(yypvt[-1].real) < 0 || INT(yypvt[-1].real) > 9)
				error_("Illegal cursor subscript");

			yyval.real = crsr[INT(yypvt[-1].real)];
		} break;
case 50:
# line 450 "P.yacc.y"
{
			yyval.real = (gg_[INT(yypvt[-2].real)] = yypvt[-0].real);
		} break;
case 51:
# line 454 "P.yacc.y"
{
			yyval.real = gg_[INT(yypvt[-0].real)];
		} break;
case 52:
# line 458 "P.yacc.y"
{
			yyval.real = hdrval((HdrVar *)(yypvt[-0].string),1);
		} break;
case 53:
# line 462 "P.yacc.y"
{
			PULL;
			yyval.real = hdrval((HdrVar *)(yypvt[-3].string),(int)(yypvt[-1].real));
		} break;
case 54:
# line 467 "P.yacc.y"
{
			yyval.real = yypvt[-0].real;
			asgnhdr((HdrVar *)(yypvt[-2].string),1,yypvt[-0].real);
		} break;
case 55:
# line 472 "P.yacc.y"
{
			PULL;
			yyval.real = yypvt[-0].real;
			asgnhdr((HdrVar *)(yypvt[-5].string),(int)(yypvt[-3].real),yypvt[-0].real);
		} break;
case 56:
# line 481 "P.yacc.y"
{
			nferr("string in numerical expression");
		} break;
case 57:
# line 485 "P.yacc.y"
{
			nferr("string value in numerical expression");
		} break;
case 58:
# line 489 "P.yacc.y"
{
			nferr("file name in numerical expression");
		} break;
case 59:
# line 493 "P.yacc.y"
{
			nferr("characters undefined - use ^ for exponentiation");
		} break;
case 60:
# line 503 "P.yacc.y"
{
			/* store value */
			st_str(yypvt[-1].string);

			/* set default flags */
			tag(cnode);
		} break;
case 61:
# line 513 "P.yacc.y"
{
			/* save current string position */
			yyval.string = strpos;

			/* copy string into string buffer */
			strcpy(strpos,yypvt[-0].string);

			/* update strpos */
			strpos = strchr(strpos,0);

			/* erase string */
			ChkFree(yypvt[-0].string);
		} break;
case 62:
# line 527 "P.yacc.y"
{
			/* make assignment */
			yyval.string = strpos;
			strcpy(strpos,&L_PARAM(char,cnode));
			strpos = strchr(strpos,0);
		} break;
case 64:
# line 535 "P.yacc.y"
{
			yyval.string = yypvt[-1].string;
		} break;
case 65:
# line 538 "P.yacc.y"
{PUSH} break;
case 66:
# line 538 "P.yacc.y"
{PULL PUSH} break;
case 67:
# line 539 "P.yacc.y"
{
			PULL;

			/* compensate for string starting at 0 */
			yypvt[-4].real = yypvt[-4].real - 1;

			/* check for errors */
			if(yypvt[-4].real < 0)
				nferr("illegal starting location");
			if(yypvt[-4].real >= (int)strlen(yypvt[-7].string))
				nferr("starting location too high");
			if(yypvt[-1].real <= 0)
				nferr("illegal length");
			if(yypvt[-1].real > (int)strlen(yypvt[-7].string) - INT(yypvt[-4].real))
				nferr("length too long");

			/* make assignment */
/*			bcopy($3 + INT($6),$3, INT($9)); */
			memcpy(yypvt[-7].string, yypvt[-7].string + INT(yypvt[-4].real), INT(yypvt[-1].real));
			strpos = yypvt[-7].string + INT(yypvt[-1].real);
			*strpos = 0;

			/* set final value */
			yyval.string = yypvt[-7].string;
		} break;
case 68:
# line 565 "P.yacc.y"
{
			/* save beginning of string */
			yyval.string = yypvt[-0].string;

			/* make room in string variable */
			if(sg_[INT(yypvt[-2].real)])
			{
				ChkFree(sg_[INT(yypvt[-2].real)]);
			}
			sg_[INT(yypvt[-2].real)] = (ChkAlloc(strlen(yypvt[-0].string) + 1, "Parse String"));

			/* store string */
			strcpy(sg_[INT(yypvt[-2].real)],yypvt[-0].string);
		} break;
case 69:
# line 580 "P.yacc.y"
{
			/* save beginning of string */
			yyval.string = strpos;

			/* error if variable is not defined */
			if(sg_[INT(yypvt[-0].real)] == 0)
				nferr("Global string undefined");

			/* copy variable into string buffer */
			strcpy(strpos,sg_[INT(yypvt[-0].real)]);

			/* set strpos to end of string buffer */
			strpos = strchr(strpos,0);
			
		} break;
case 70:
# line 597 "P.yacc.y"
{
			yyval.string = strpos;

			/* error if variable is not defined */
			if(((HdrVar *)yypvt[-0].string)->lc == 0)
				nferr("User string variable not set");

                        strcpy(strpos, hdrstrval((HdrVar *)yypvt[-0].string));
			strpos = strchr(strpos,0);
		} break;
case 71:
# line 608 "P.yacc.y"
{
			yyval.string = yypvt[-0].string;
			asgnstrhdr((HdrVar *)(yypvt[-2].string), yypvt[-0].string);
		} break;
case 72:
# line 619 "P.yacc.y"
{
#if COMB
			/* store value */
			st_scan(yypvt[-2].string,yypvt[-1].real,yypvt[-2].string,yypvt[-1].real);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		} break;
case 73:
# line 631 "P.yacc.y"
{
#if COMB
			/* reset second part of range flag */
			spor = 0;

			/* check that cnode is range */
			if(SEQ(cnode) != RANGE)
			{
				if(unspt[-1] != 1)str -= yyleng;
				nferr("trying to enter range in non-range");
			}

			/* store values */
			st_scan(yypvt[-4].string,yypvt[-3].real,yypvt[-4].string,yypvt[-1].real);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		} break;
case 74:
# line 653 "P.yacc.y"
{
#if COMB
			/* reset second part of range flag */
			spor = 0;

			/* check that cnode is range */
			if(SEQ(cnode) != RANGE)
			{
				if(unspt[-1] != 1)str -= yyleng;
				nferr("trying to enter range in non-range");
			}

			/* store values */
			st_scan((char *)0,yypvt[-4].real,yypvt[-2].string,yypvt[-1].real);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		} break;
case 75:
# line 675 "P.yacc.y"
{
#if COMB
			/* reset second part of range flag */
			spor = 0;

			/* check that cnode is range */
			if(SEQ(cnode) != RANGE)
			{
				if(unspt[-1] != 1)str -= yyleng;
				nferr("trying to enter range in non-range");
			}

			/* store values */
			st_scan(yypvt[-5].string,yypvt[-4].real,yypvt[-2].string,yypvt[-1].real);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		} break;
case 76:
# line 703 "P.yacc.y"
{
			PUSH;
		} break;
case 77:
# line 713 "P.yacc.y"
{
			PULL;
			/* check for errors */
			if(yypvt[-0].real < 0 || yypvt[-0].real > 9)
				nferr("subscript out of bounds");

			/* get correct variable */
			yyval.real = yypvt[-0].real;
		} break;
case 78:
# line 723 "P.yacc.y"
{
			PULL;
			if(yypvt[-1].real < 0 || yypvt[-1].real > 9)
				nferr("subscript out of bounds");

			yyval.real = yypvt[-1].real;
		} break;
case 79:
# line 731 "P.yacc.y"
{
			PULL;
			yyval.real = 0;
		} break;
case 80:
# line 738 "P.yacc.y"
{
			PUSH;
		} break;
case 81:
# line 744 "P.yacc.y"
{
			PULL;
			/* check for error */
			if(yypvt[-0].real < 0 || yypvt[-0].real > 9)
				nferr("subscript out of bounds");

			/* get correct variable */
			yyval.real = yypvt[-0].real;
		} break;
case 82:
# line 754 "P.yacc.y"
{
			PULL;
			if(yypvt[-1].real < 0 || yypvt[-1].real > 9)
				nferr("subscript out of bounds");

			yyval.real = yypvt[-1].real;
		} break;
case 83:
# line 762 "P.yacc.y"
{
			PULL;
			yyval.real = 0;
		} break;
case 84:
# line 769 "P.yacc.y"
{
			PUSH;
		} break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

