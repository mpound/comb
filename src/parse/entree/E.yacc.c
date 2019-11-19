
# line 2 "E.yacc.y"
# include <string.h>
# include <stdio.h>
# include "../P.tree.h"
# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"

	typedef char *YYSTYPE;
# define STRNG_T 257
# define EX_T 258
# define FL_T 259
# define EXT_T 260
# define LST_T 261
# define RNG_T 262
# define INT_T 263
# define REAL_T 264
# define SCAN_T 265
# define STR_T 266
# define CMD_T 267
# define EXP_T 268
# define AUTO_T 269
# define SET_T 270
# define OLD_T 271
# define FLAG_T 272
# define TOGGLE_T 273
# define PARS_T 274

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
#ifndef YYSTYPE
#define YYSTYPE int
#endif
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
yytabelem yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 43
# define YYLAST 235
yytabelem yyact[]={

    37,     3,    50,    21,    49,    98,    29,    30,    31,    26,
    27,    28,    29,    30,    31,    26,    27,    28,    46,    15,
    16,    86,     5,    87,     6,   107,   103,   102,   101,    82,
    81,    80,    79,    78,    77,    10,    76,    75,    74,    71,
    70,    69,    63,    56,    55,    54,    11,    20,    38,    13,
    12,    36,    32,    25,    24,    47,   104,    57,   106,   105,
    95,    94,    93,    92,    91,    90,    89,    88,    85,    84,
    83,    73,    72,    17,    97,    96,    67,    64,    62,    61,
    60,    59,    58,    53,    52,    51,    45,    34,    33,     8,
     2,    23,    22,    19,    18,    14,     4,     1,     0,     0,
     0,    66,    65,    66,    68,     0,     0,     0,    35,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     7,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    39,     0,     0,     0,    48,     0,
     0,     0,     0,     0,    40,    41,    42,    43,    44,    40,
    41,    42,    43,    44,     9 };
yytabelem yypact[]={

  -273,-10000000,  -101,    49,-10000000,-10000000,-10000000,   -23,  -211,    -9,
  -239,    32,-10000000,  -257,    -7,    48,    47,-10000000,    -8,   -45,
-10000000,    46,  -243,     8,   -43,  -260,    45,    44,    43,-10000000,
-10000000,-10000000,-10000000,  -212,  -213,  -101,-10000000,  -214,    12,    42,
    41,    40,    39,-10000000,    38,  -215,    37,  -251,    36,  -251,
-10000000,  -216,  -217,  -218,    31,    30,-10000000,  -219,  -220,  -221,
  -223,  -224,  -225,  -226,  -227,-10000000,-10000000,  -228,-10000000,    29,
    28,    27,  -238,  -235,-10000000,    26,    25,    24,    23,    22,
    21,    20,    19,-10000000,-10000000,-10000000,    35,    34,   -40,-10000000,
-10000000,-10000000,-10000000,  -251,-10000000,-10000000,  -229,  -230,  -231,    11,
-10000000,    18,    17,-10000000,  -232,-10000000,-10000000,-10000000 };
yytabelem yypgo[]={

     0,    97,    90,    96,    95,    94,    93,    48,    47,    92,
    91,    54,    53 };
yytabelem yyr1[]={

     0,     1,     1,     2,     2,     2,     2,     3,     3,     3,
     4,     4,     4,     5,     5,     5,     5,     5,     6,     6,
     8,     8,     8,     8,     8,     9,    12,    12,    12,    10,
    10,    11,    11,    11,    11,     7,     7,     7,     7,     7,
     7,     7,     7 };
yytabelem yyr2[]={

     0,     3,    11,     0,     4,     5,     5,     7,     9,    11,
     9,    17,    17,     0,     7,     9,    15,    17,     3,    13,
     2,     2,     3,    11,    11,     5,     3,     3,     3,     7,
     7,     2,     9,     9,     9,     3,     9,     9,     3,     9,
     3,     3,     9 };
yytabelem yychk[]={

-10000000,    -1,    -2,   274,    -3,   123,   125,   257,    40,   257,
    58,   257,    59,    58,    -4,   258,   259,    41,    -5,    -6,
    -8,   260,    -9,   -10,   -11,   -12,   266,   267,   268,   263,
   264,   265,    59,    40,    40,    -2,    59,    45,    -7,   259,
   269,   270,   271,   272,   273,    40,   261,    47,   261,    47,
   262,    40,    40,    40,   257,   257,   257,    45,    40,    40,
    40,    40,    40,   257,    40,   -11,   -12,    40,   -11,   257,
   257,   257,    41,    41,   257,   257,   257,   257,   257,   257,
   257,   257,   257,    41,    41,    41,   259,   258,    41,    41,
    41,    41,    41,    41,    41,    41,    40,    40,    45,    -7,
    -8,   257,   257,   257,    45,    41,    41,   257 };
yytabelem yydef[]={

     3,    -2,     1,     0,     4,     5,     6,     0,     0,     0,
     0,     0,     7,    13,     0,     0,     0,     3,     0,     0,
    18,     0,    20,    21,    22,    31,     0,     0,     0,    26,
    27,    28,     8,     0,     0,     2,     9,     0,     0,     0,
    35,     0,    38,    40,    41,     0,     0,     0,     0,     0,
    25,     0,     0,     0,     0,     0,    14,     0,     0,     0,
     0,     0,     0,     0,     0,    30,    31,     0,    29,     0,
     0,     0,    10,     0,    15,     0,     0,     0,     0,     0,
     0,     0,     0,    32,    33,    34,     0,     0,     0,    36,
    37,    39,    42,     0,    23,    24,     0,     0,     0,     0,
    19,     0,     0,    16,     0,    12,    11,    17 };
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
	"STRNG_T",	257,
	"EX_T",	258,
	"FL_T",	259,
	"EXT_T",	260,
	"LST_T",	261,
	"RNG_T",	262,
	"INT_T",	263,
	"REAL_T",	264,
	"SCAN_T",	265,
	"STR_T",	266,
	"CMD_T",	267,
	"EXP_T",	268,
	"AUTO_T",	269,
	"SET_T",	270,
	"OLD_T",	271,
	"FLAG_T",	272,
	"TOGGLE_T",	273,
	"PARS_T",	274,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"tredes : tree",
	"tredes : PARS_T '(' STRNG_T ')' tree",
	"tree : /* empty */",
	"tree : tree node",
	"tree : tree '{'",
	"tree : tree '}'",
	"node : STRNG_T STRNG_T ';'",
	"node : STRNG_T ':' exops ';'",
	"node : STRNG_T STRNG_T ':' leafops ';'",
	"exops : EX_T '(' STRNG_T ')'",
	"exops : FL_T '(' STRNG_T ')' EX_T '(' STRNG_T ')'",
	"exops : EX_T '(' STRNG_T ')' FL_T '(' STRNG_T ')'",
	"leafops : /* empty */",
	"leafops : type '-' STRNG_T",
	"leafops : type deftype '-' STRNG_T",
	"leafops : type FL_T '(' STRNG_T ')' '-' STRNG_T",
	"leafops : type FL_T '(' STRNG_T ')' deftype '-' STRNG_T",
	"type : atype",
	"type : EXT_T '(' STRNG_T STRNG_T ')' atype",
	"atype : rngtype",
	"atype : multype",
	"atype : types",
	"atype : rngtype LST_T '(' STRNG_T ')'",
	"atype : types LST_T '(' STRNG_T ')'",
	"rngtype : ntype RNG_T",
	"ntype : INT_T",
	"ntype : REAL_T",
	"ntype : SCAN_T",
	"multype : types '/' types",
	"multype : multype '/' types",
	"types : ntype",
	"types : STR_T '(' STRNG_T ')'",
	"types : CMD_T '(' STRNG_T ')'",
	"types : EXP_T '(' STRNG_T ')'",
	"deftype : AUTO_T",
	"deftype : AUTO_T '(' STRNG_T ')'",
	"deftype : SET_T '(' STRNG_T ')'",
	"deftype : OLD_T",
	"deftype : OLD_T '(' STRNG_T ')'",
	"deftype : FLAG_T",
	"deftype : TOGGLE_T",
	"deftype : TOGGLE_T '(' STRNG_T ')'",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.12	93/06/07 SMI"

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
	(type *) memcpy(to, (char *) from, yynewmax * sizeof(type))
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
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; yypvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
	yypvt = 0;
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
				register int *yyxi = yyexca;

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
		
case 1:
# line 18 "E.yacc.y"
{
			stpars = "str";
		} break;
case 2:
# line 22 "E.yacc.y"
{
			stpars = (char *)(yypvt[-2]);
		} break;
case 5:
# line 30 "E.yacc.y"
{
			/* start children */
			stkpnt++;
			stkpnt->branch = 0;
			stkpnt->parent = cnode - 1;
		} break;
case 6:
# line 37 "E.yacc.y"
{
			/* end children */
			stkpnt--;
		} break;
case 7:
# line 44 "E.yacc.y"
{
			wtree[cnode].typ = NORMAL;
			strcpy(wtree[cnode].des,(char *)yypvt[-2]);
			strcpy(wtree[cnode].lbl,(char *)yypvt[-1]);
			family(cnode);
			cnode++;
			free((void *)yypvt[-2]);
			free((void *)yypvt[-1]);
		} break;
case 8:
# line 54 "E.yacc.y"
{
			wtree[cnode].typ = EXCLSV;
			strcpy(wtree[cnode].des,(char *)yypvt[-3]);
			family(cnode);
			cnode++;
			free((void *)yypvt[-3]);
		} break;
case 9:
# line 62 "E.yacc.y"
{
			wtree[cnode].typ = LEAF;
			strcpy(wtree[cnode].des,(char *)yypvt[-4]);
			strcpy(wtree[cnode].lbl,(char *)yypvt[-3]);
			family(cnode);
			cnode++;
			free((void *)yypvt[-4]);
			free((void *)yypvt[-3]);
		} break;
case 10:
# line 74 "E.yacc.y"
{
			wtree[cnode].fvr = 1;
			wtree[cnode].var = variable(yypvt[-1],SHORT);
			free((void *)yypvt[-1]);
		} break;
case 11:
# line 80 "E.yacc.y"
{
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable(yypvt[-5],SHORT);
			wtree[cnode].fvr = 1;
			wtree[cnode].var = variable(yypvt[-1],SHORT);
			free((void *)yypvt[-5]);
			free((void *)yypvt[-1]);
		} break;
case 12:
# line 89 "E.yacc.y"
{
			wtree[cnode].fvr = 1;
			wtree[cnode].var = variable(yypvt[-5],SHORT);
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable(yypvt[-1],SHORT);
			free((void *)yypvt[-5]);
			free((void *)yypvt[-1]);
		} break;
case 14:
# line 101 "E.yacc.y"
{
			char *tch;
			wtree[cnode].dft = ILLEGAL;
			wtree[cnode].fvr = 1;
			avars(cnode,yypvt[-0]);
			var[(int)(yypvt[-2])][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)yypvt[-0],0)) = 't';
			wtree[cnode].tfv = variable(yypvt[-0],SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable(yypvt[-0],SHORT);
			}
			free((void *)yypvt[-0]);
		} break;
case 15:
# line 117 "E.yacc.y"
{
			char *tch;
			wtree[cnode].fvr = 1;
			avars(cnode,yypvt[-0]);
			var[(int)(yypvt[-3])][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)yypvt[-0],0)) = 't';
			wtree[cnode].tfv = variable(yypvt[-0],SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable(yypvt[-0],SHORT);
			}
			free((void *)yypvt[-0]);
		} break;
case 16:
# line 132 "E.yacc.y"
{
			char *tch;
			wtree[cnode].dft = ILLEGAL;
			wtree[cnode].fvr = 1;
			avars(cnode,yypvt[-0]);
			var[(int)(yypvt[-6])][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)yypvt[-0],0)) = 't';
			wtree[cnode].tfv = variable(yypvt[-0],SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable(yypvt[-0],SHORT);
			}
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable(yypvt[-3],SHORT);
			free((void *)yypvt[-3]);
			free((void *)yypvt[-0]);
		} break;
case 17:
# line 151 "E.yacc.y"
{
			char *tch;
			wtree[cnode].fvr = 1;
			avars(cnode,yypvt[-0]);
			var[(int)(yypvt[-7])][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)yypvt[-0],0)) = 't';
			wtree[cnode].tfv = variable(yypvt[-0],SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable(yypvt[-0],SHORT);
			}
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable(yypvt[-4],SHORT);
			free((void *)yypvt[-4]);
			free((void *)yypvt[-0]);
		} break;
case 18:
# line 171 "E.yacc.y"
{
			yyval = da_ecf[(int)(yypvt[-0])];
		} break;
case 19:
# line 175 "E.yacc.y"
{
			yyval = da_ecf[(int)(yypvt[-0])];
			wtree[cnode].ext = 1;
			strcpy(wtree[cnode].evr,(char *)yypvt[-3]);
			strcpy(wtree[cnode].eds,(char *)yypvt[-2]);
			free((void *)yypvt[-3]);
			free((void *)yypvt[-2]);
		} break;
case 22:
# line 188 "E.yacc.y"
{
			wtree[cnode].dtp = yypvt[-0];
		} break;
case 23:
# line 192 "E.yacc.y"
{
			wtree[cnode].lln = atoi((char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 24:
# line 197 "E.yacc.y"
{
			wtree[cnode].dtp = yypvt[-4];
			wtree[cnode].lln = atoi((char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 25:
# line 205 "E.yacc.y"
{
			wtree[cnode].dtp = (int)(yypvt[-1]);
			wtree[cnode].rng = 1;
		} break;
case 26:
# line 212 "E.yacc.y"
{
			yyval = INTN;
			wtree[cnode].sdt = INTN;
		} break;
case 27:
# line 217 "E.yacc.y"
{
			yyval = REAL;
			wtree[cnode].sdt = REAL;
		} break;
case 28:
# line 222 "E.yacc.y"
{
			yyval = SCAN;
			wtree[cnode].sdt = SCAN;
		} break;
case 29:
# line 229 "E.yacc.y"
{
			wtree[cnode].dtp = (int)(yypvt[-2]) | (int)(yypvt[-0]);
		} break;
case 30:
# line 233 "E.yacc.y"
{
			wtree[cnode].dtp |= (int)(yypvt[-0]);
		} break;
case 32:
# line 240 "E.yacc.y"
{
			yyval = STRING;
			wtree[cnode].sdt = STRING;
			wtree[cnode].chl = atoi((char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 33:
# line 247 "E.yacc.y"
{
			yyval = CMND;
			wtree[cnode].sdt = CMND;
			wtree[cnode].chl = atoi((char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 34:
# line 254 "E.yacc.y"
{
			yyval = EXPR;
			wtree[cnode].sdt = EXPR;
			wtree[cnode].chl = atoi((char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 35:
# line 263 "E.yacc.y"
{
			wtree[cnode].dft = AUTO;
		} break;
case 36:
# line 267 "E.yacc.y"
{
			wtree[cnode].dft = AUTOFUN;
			strcpy(wtree[cnode].dfv,(char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 37:
# line 273 "E.yacc.y"
{
			wtree[cnode].dft = SET;
			strcpy(wtree[cnode].dfv,(char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 38:
# line 279 "E.yacc.y"
{
			if(!wtree[cnode].ext && wtree[cnode].sdt != SCAN)
				enterr("no initial value");
			wtree[cnode].dft = OLD;
		} break;
case 39:
# line 285 "E.yacc.y"
{
			if(wtree[cnode].ext)
				enterr("initial value for external variable");
			if(wtree[cnode].sdt == SCAN)
				enterr("initial value for scan number");
			wtree[cnode].dft = OLD;
			strcpy(wtree[cnode].dfv,(char *)yypvt[-1]);
			free((void *)yypvt[-1]);
		} break;
case 40:
# line 295 "E.yacc.y"
{
			if(wtree[cnode].dtp != INTN)
				enterr("flag default type for non-integer");
			wtree[cnode].dft = FLAG;
		} break;
case 41:
# line 301 "E.yacc.y"
{
			if(!wtree[cnode].ext)
				enterr("no starting value for toggle");
			if(wtree[cnode].dtp != INTN)
				enterr("toggle default type for non-integer");
			wtree[cnode].dft = TOGGLE;
		} break;
case 42:
# line 309 "E.yacc.y"
{
			if(wtree[cnode].ext)
				enterr("starting value for external");
			if(wtree[cnode].dtp != INTN)
				enterr("toggle default type for non-integer");
			wtree[cnode].dft = TOGGLE;
			if(strcmp((char *)yypvt[-1],"yes"))
			{
				if(strcmp((char *)yypvt[-1],"no"))
					enterr("non-boolean value for toggle");
				strcpy(wtree[cnode].dfv,"0");
			}
			else
				strcpy(wtree[cnode].dfv,"1");
		} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

