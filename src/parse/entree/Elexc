#include <stdio.h>
#include <stdlib.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# ifndef YYLMAX 
# define YYLMAX BUFSIZ
# endif 
#ifndef __cplusplus
# define output(c) (void)putc(c,yyout)
#else
# define lex_output(c) (void)putc(c,yyout)
#endif

#if defined(__cplusplus) || defined(__STDC__)

#if defined(__cplusplus) && defined(__EXTERN_C__)
extern "C" {
#endif
	int yyback(int *, int);
	int yyinput(void);
	int yylook(void);
	void yyoutput(int);
	int yyracc(int);
	int yyreject(void);
	void yyunput(int);
	int yylex(void);
#ifdef YYLEX_E
	void yywoutput(wchar_t);
	wchar_t yywinput(void);
#endif
#ifndef yyless
	int yyless(int);
#endif
#ifndef yywrap
	int yywrap(void);
#endif
#ifdef LEXDEBUG
	void allprint(char);
	void sprint(char *);
#endif
#if defined(__cplusplus) && defined(__EXTERN_C__)
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void exit(int);
#ifdef __cplusplus
}
#endif

#endif
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
#ifndef __cplusplus
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#else
# define lex_input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#endif
#define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng;
#define YYISARRAY
char yytext[YYLMAX];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin , *yyout ;
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"
# include "y.tab.h"
	extern char *yylval;
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
#ifdef __cplusplus
/* to avoid CC and lint complaining yyfussy not being used ...*/
static int __lex_hack = 0;
if (__lex_hack) goto yyfussy;
#endif
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:

# line 11 "E.lex.x"
					{	/* parse */
	return(PARS_T);
}
break;
case 2:

# line 15 "E.lex.x"
					{	/* exclusive */
	return(EX_T);
}
break;
case 3:

# line 19 "E.lex.x"
					{	/* flag */
	return(FL_T);
}
break;
case 4:

# line 23 "E.lex.x"
					{	/* integer */
	return(INT_T);
}
break;
case 5:

# line 27 "E.lex.x"
					{	/* real */
	return(REAL_T);
}
break;
case 6:

# line 31 "E.lex.x"
					{	/* scan number */
	return(SCAN_T);
}
break;
case 7:

# line 35 "E.lex.x"
					{	/* string variable */
	return(STR_T);
}
break;
case 8:

# line 39 "E.lex.x"
					{	/* command list */
	return(CMD_T);
}
break;
case 9:

# line 43 "E.lex.x"
					{	/* expression */
	return(EXP_T);
}
break;
case 10:

# line 47 "E.lex.x"
					{	/* range */
	return(RNG_T);
}
break;
case 11:

# line 51 "E.lex.x"
					{	/* list */
	return(LST_T);
}
break;
case 12:

# line 55 "E.lex.x"
					{	/* automatic */
	return(AUTO_T);
}
break;
case 13:

# line 59 "E.lex.x"
					{	/* set */
	return(SET_T);
}
break;
case 14:

# line 63 "E.lex.x"
					{	/* old */
	return(OLD_T);
}
break;
case 15:

# line 67 "E.lex.x"
					{	/* flag default */
	return(FLAG_T);
}
break;
case 16:

# line 71 "E.lex.x"
				{	/* toggle default */
	return(TOGGLE_T);
}
break;
case 17:

# line 75 "E.lex.x"
				{	/* external variable */
	return(EXT_T);
}
break;
case 18:

# line 79 "E.lex.x"
					{	/* strings */
	char *tstrpos;			/* variable for constructing */
					/* temporary string */

	/* make room in temporary string */
	yylval = tstrpos = (char *)calloc(1000,sizeof(char));

	/* construct temporary string */
	while((*tstrpos = input()) != *yytext)
	{
		if(*tstrpos == '\n')
		{
			enterr("unbalanced quotation marks");
			unput('\n');
			break;
		}
		if(*tstrpos == '\\')
			*tstrpos = input();
		tstrpos++;
	}
	*tstrpos = 0;

	/* return string token */
	return(STRNG_T);
}
break;
case 19:

# line 105 "E.lex.x"
				{	/* characters to */
}
break;
case 20:

# line 108 "E.lex.x"
					{	/* new line */
	clin++;
}
break;
case 21:

# line 112 "E.lex.x"
					{	/* anything else */
	return(*yytext);
}
break;
case -1:
break;
default:
(void)fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */
int yyvstop[] = {
0,

21,
0, 

19,
21,
0, 

20,
0, 

18,
21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

21,
0, 

2,
0, 

3,
0, 

8,
0, 

9,
0, 

4,
0, 

14,
0, 

13,
0, 

7,
0, 

12,
0, 

15,
0, 

11,
0, 

5,
0, 

6,
0, 

1,
0, 

10,
0, 

17,
0, 

16,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,6,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	2,6,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,7,	14,25,	
1,8,	15,26,	1,9,	1,10,	
19,33,	15,27,	1,11,	12,23,	
21,36,	1,12,	8,19,	10,21,	
1,13,	1,14,	11,22,	1,15,	
1,16,	1,17,	2,7,	7,18,	
2,8,	9,20,	2,9,	2,10,	
13,24,	16,28,	2,11,	16,29,	
17,31,	2,12,	18,32,	22,37,	
2,13,	2,14,	20,34,	2,15,	
2,16,	2,17,	20,35,	23,38,	
24,39,	25,40,	16,30,	26,41,	
27,42,	28,43,	29,44,	30,45,	
31,46,	32,47,	35,48,	36,49,	
38,50,	40,51,	41,52,	42,53,	
43,54,	46,55,	48,56,	51,57,	
52,58,	55,59,	56,60,	59,61,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-21,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+0,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+0,	0,		yyvstop+8,
yycrank+2,	0,		yyvstop+11,
yycrank+1,	0,		yyvstop+13,
yycrank+1,	0,		yyvstop+15,
yycrank+3,	0,		yyvstop+17,
yycrank+4,	0,		yyvstop+19,
yycrank+2,	0,		yyvstop+21,
yycrank+16,	0,		yyvstop+23,
yycrank+2,	0,		yyvstop+25,
yycrank+4,	0,		yyvstop+27,
yycrank+26,	0,		yyvstop+29,
yycrank+17,	0,		yyvstop+31,
yycrank+14,	0,		0,	
yycrank+4,	0,		0,	
yycrank+22,	0,		yyvstop+33,
yycrank+11,	0,		yyvstop+35,
yycrank+15,	0,		0,	
yycrank+24,	0,		0,	
yycrank+40,	0,		0,	
yycrank+27,	0,		0,	
yycrank+33,	0,		0,	
yycrank+47,	0,		0,	
yycrank+48,	0,		0,	
yycrank+30,	0,		0,	
yycrank+33,	0,		0,	
yycrank+45,	0,		0,	
yycrank+38,	0,		0,	
yycrank+0,	0,		yyvstop+37,
yycrank+0,	0,		yyvstop+39,
yycrank+49,	0,		0,	
yycrank+48,	0,		0,	
yycrank+0,	0,		yyvstop+41,
yycrank+36,	0,		0,	
yycrank+0,	0,		yyvstop+43,
yycrank+38,	0,		0,	
yycrank+51,	0,		0,	
yycrank+47,	0,		0,	
yycrank+46,	0,		0,	
yycrank+0,	0,		yyvstop+45,
yycrank+0,	0,		yyvstop+47,
yycrank+54,	0,		0,	
yycrank+0,	0,		yyvstop+49,
yycrank+44,	0,		0,	
yycrank+0,	0,		yyvstop+51,
yycrank+0,	0,		yyvstop+53,
yycrank+58,	0,		0,	
yycrank+59,	0,		0,	
yycrank+0,	0,		yyvstop+55,
yycrank+0,	0,		yyvstop+57,
yycrank+53,	0,		0,	
yycrank+52,	0,		0,	
yycrank+0,	0,		yyvstop+59,
yycrank+0,	0,		yyvstop+61,
yycrank+62,	0,		0,	
yycrank+0,	0,		yyvstop+63,
yycrank+0,	0,		yyvstop+65,
0,	0,	0};
struct yywork *yytop = yycrank+163;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   9,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	Copyright (c) 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#pragma ident	"@(#)ncform	6.11	97/01/06 SMI"

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
#if defined(__cplusplus) || defined(__STDC__)
int yylook(void)
#else
yylook()
#endif
{
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
#ifndef __cplusplus
			*yylastch++ = yych = input();
#else
			*yylastch++ = yych = lex_input();
#endif
#ifdef YYISARRAY
			if(yylastch > &yytext[YYLMAX]) {
				fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
				exit(1);
			}
#else
			if (yylastch >= &yytext[ yytextsz ]) {
				int	x = yylastch - yytext;

				yytextsz += YYTEXTSZINC;
				if (yytext == yy_tbuf) {
				    yytext = (char *) malloc(yytextsz);
				    memcpy(yytext, yy_tbuf, sizeof (yy_tbuf));
				}
				else
				    yytext = (char *) realloc(yytext, yytextsz);
				if (!yytext) {
				    fprintf(yyout,
					"Cannot realloc yytext\n");
				    exit(1);
				}
				yylastch = yytext + x;
			}
#endif
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					if(lsp > &yylstate[YYLMAX]) {
						fprintf(yyout,"Input string too long, limit %d\n",YYLMAX);
						exit(1);
					}
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
#ifndef __cplusplus
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
#else
		yyprevious = yytext[0] = lex_input();
		if (yyprevious>0)
			lex_output(yyprevious);
#endif
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
#if defined(__cplusplus) || defined(__STDC__)
int yyback(int *p, int m)
#else
yyback(p, m)
	int *p;
#endif
{
	if (p==0) return(0);
	while (*p) {
		if (*p++ == m)
			return(1);
	}
	return(0);
}
	/* the following are only used in the lex library */
#if defined(__cplusplus) || defined(__STDC__)
int yyinput(void)
#else
yyinput()
#endif
{
#ifndef __cplusplus
	return(input());
#else
	return(lex_input());
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyoutput(int c)
#else
yyoutput(c)
  int c; 
#endif
{
#ifndef __cplusplus
	output(c);
#else
	lex_output(c);
#endif
	}
#if defined(__cplusplus) || defined(__STDC__)
void yyunput(int c)
#else
yyunput(c)
   int c; 
#endif
{
	unput(c);
	}
