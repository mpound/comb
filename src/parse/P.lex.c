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

# line 2 "P.lex.x"
/* MWP needed to increase default space for lex after adding ABS_T node*/
# include <string.h>
#include "../main/C.h"
# include "P.tree.h"
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
#if COMB
# include "../scan/C.h"
#endif /*COMB*/
# include "P.extern.h"
# include "../error/C.h"
extern HdrVar *findhdr();
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

# line 29 "P.lex.x"
			{	/* commas */

	SPCHAR;

	/* set nnode to next parameter */
	nnode = param(next(nnode));

#ifdef DEBUG
	/* print report of comma for testing */
	printf("FOUND COMMA: next node = %s\n",nnode->des);
#endif

	return(END_T);
}
break;
case 2:

# line 44 "P.lex.x"
		{	/* labels */

	Node *node;			/* node described by label */

	SPCHAR;

	/* find node described by label */
	if(!(node = search(nnode,ex_label(yytext))))
		nferr("label not found");
	nnode = param(descend(node));

#ifdef DEBUG
	/* print message for testing */
	printf("LABEL %s FOUND: next node = %s\n",ex_label(yytext),nnode->des);
#endif

	return(END_T);
}
break;
case 3:

# line 63 "P.lex.x"
	{	/* label with [ */

	SPCHAR;

	/* check for error */
	if(rstkpnt == rootstack + 40)
		nferr("stack overflow - use fewer open brackets");

	/* store old root */
	*rstkpnt++ = root;

	/* find new root */
	if(!(root = search(nnode,ex_label(yytext))))
		nferr("label not found");

	/* set next node */
	nnode = param(descend(root));

#ifdef DEBUG
	/* print message for testing */
	printf("LABEL %s FOUND: root = %s, next node = %s\n",ex_label(yytext),
		 root->des,nnode->des);
	printf("PARENTS are %d, %d\n",root->parent,nnode->parent);
#endif

	return(END_T);
}
break;
case 4:

# line 91 "P.lex.x"
				{	/* [ */

	Node *tnode1,*tnode2;			/* temporary nodes for */
						/* finding new root */

	SPCHAR;

	/* check for error */
	if(root->type == LEAF)
		nferr("too many open brackets");
	if(rstkpnt == rootstack + 40)
		nferr("stack overflow - use fewer open brackets");

	/* save old root */
	*rstkpnt++ = root;

	/* find new root */
	tnode1 = tnode2 = nnode;
	while(tnode1 != root)
	{
		if(tnode1->type != EXCLSV)
			tnode2 = tnode1;
		tnode1 = PARENT(tnode1);
	}
	root = tnode2;

#ifdef DEBUG
	/* print message for testing */
	printf("ROOT DOWN: root = %s\n",root->des);
#endif

	/* if nothing else has moved the next node pointer */
	/* from the last parameter, move it */
	if(nnode == cnode)
	{
		nnode = param(next(nnode));
		return(END_T);
	}
}
break;
case 5:

# line 131 "P.lex.x"
			{	/* ] */

	Node *troot;				/* temporary root */

	SPCHAR;

	/* check for error */
	if(rstkpnt == rootstack)
		nferr("too many close brackets");

	/* recover root */
	troot = root;
	root = *--rstkpnt;

	/* set next node */
	nnode = param(descend(next(troot)));

#ifdef DEBUG
	/* print message for testing */
	printf("ROOT UP: root = %s, next node = %s\n",root->des,nnode->des);
#endif

	return(END_T);
}
break;
case 6:

# line 156 "P.lex.x"
					{	/* white space */

	SPCHAR;

	/* set nnode to next parameter */
	nnode = param(next(nnode));

#ifdef DEBUG
	/* print report of white space for testing */
	printf("FOUND WHITE SPACE: next node = %s\n",nnode->des);
#endif

	return(END_T);
}
break;
case 7:

# line 172 "P.lex.x"
					case 8:

# line 173 "P.lex.x"
				case 9:

# line 174 "P.lex.x"
				{

	START;

	/* find value */
	yylval.real = atof(yytext);

#ifdef DEBUG
	/* print message for testing */
	printf("FOUND NUMBER (%s): yylval = %g\n",yytext,yylval.real);
#endif

	/* return */
	return(NUM_T);
}
break;
case 10:

# line 190 "P.lex.x"
			{	/* header variables */

	START;

	/* find variable */
	if((yylval.string = (char *)findhdr(yytext + 1))==NULL)
		error("Variable %s not found",yytext+1);

#ifdef DEBUG
	/* print message for testing */
	printf("FOUND HEADER VARIABLE (%s)\n",yytext + 1);
#endif

	/* return */
	if( ((HdrVar *)(int)yylval.string)->tp == HVSTRING ||
		((HdrVar *)(int)yylval.string)->tp == SCAN2STR ||
		((HdrVar *)(int)yylval.string)->tp == UVSTR)
		return(STRHDRVAR_T);
	return(HDRVAR_T);
}
break;
case 11:

# line 211 "P.lex.x"
					{	/* logical */
	yylval.real = 1.;
	return(NUM_T);
}
break;
case 12:

# line 216 "P.lex.x"
					{
	yylval.real = 0.;
	return(NUM_T);
}
break;
case 13:

# line 221 "P.lex.x"
{
	return(EQUEQU_T);
}
break;
case 14:

# line 225 "P.lex.x"
					{	/* & */

	START;

	/* make sure cnode is a parameter */
	if(!L_IS_PARAM(cnode))
		nferr("trying to use value of non-parameter");

	/* make sure cnode has the correct default type */
	if(DF_TYPE(cnode) != OLD)
		nferr("trying to use previouse value of non-old parameter");

	/* choose between NAND_T and SAND_T */
	if(USE_TYPE(cnode) == STRING)
	{
#ifdef DEBUG
		/* message for testing */
		printf("FOUND SAND (%s)\n",yytext);
#endif

		return(SAND_T);
	}

#ifdef DEBUG
	/* message for testing */
	printf("FOUND NAND (%s)\n",yytext);
#endif

	return(NAND_T);
}
break;
case 15:

# line 256 "P.lex.x"
					case 16:

# line 257 "P.lex.x"
					{
	char *tpos;		/* position in temporary string */

	START;

	/* initialize pointers to temporary string */
	/* Matt's form changed by rww 4/25/89
	tpos = yylval.string = (char *)(calloc(1000,sizeof(char))); */
	tpos = yylval.string = (char *)(ChkAlloc(1000,"Parsed String"));

	/* write string into temporary string buffer */
	while(*tpos = *str++)
	{
		if(*tpos == *yytext)
		{
			if(tpos[-1] == '\\')
				*--tpos = *yytext;
			else
			{
				*tpos = 0;
	
#ifdef DEBUG
				/* message for testing */
				printf("FOUND STRING (%s): yylval = %s\n",
					yytext,yylval.string);
#endif

				return(STR_T);
			}
		}
		tpos++;
	}

	/* string does not end before end of command */
	nferr("unbalanced quotes");
}
break;
case 17:

# line 294 "P.lex.x"
					{
	int brkcnt;		/* number of outstanding brackets */
	char *tpos;		/* pointer to temporary string */

	START;

	/* make temporary string */
	/* tpos = yylval.string = (char *)(calloc(1000,sizeof(char))); */
	tpos = yylval.string = (char *)(ChkAlloc(1000,"Parsed String"));

	/* set number of outstanding brackets */
	brkcnt = 1;

	/* extract string */
	while(brkcnt)
	{

		/* check for end of line */
		if(!(*tpos = *str++))
			nferr("no closing bracket");

		/* count brackets */
		if(*tpos == '{')
			if(tpos[-1] == '\\')
				*--tpos = '{';
			else
				brkcnt++;
		if(*tpos == '}')
			if(tpos[-1] == '\\')
				*--tpos = '}';
			else
				brkcnt--;

		/* prepare to enter next character */
		tpos++;
	}

	/* end string */
	*--tpos = 0;

	return(STR_T);
}
break;
case 18:

# line 337 "P.lex.x"
case 19:

# line 338 "P.lex.x"
		case 20:

# line 339 "P.lex.x"
			{

	START;

	/* store file name */
	yylval.string = strpos;
	strcpy(strpos,yytext);
	strpos = strchr(strpos,0) + 1;

#ifdef DEBUG
	/* message for testing */
	printf("FOUND FILE NAME (%s)\n",yytext);
#endif

	/* return */
	return(FILNM_T);
}
break;
case 21:

# line 357 "P.lex.x"
					{	/* _ */

	START;

	/* set second part of range flag */
	spor = 1;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING UNDERSCORE\n");
#endif

	return('_');
}
break;
case 22:

# line 372 "P.lex.x"
					{	/* natural log */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING LN\n");
#endif

	return(LN_T);
}
break;
case 23:

# line 383 "P.lex.x"
					{	/* log to a base */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING LOG\n");
#endif

	return(LOG_T);
}
break;
case 24:

# line 394 "P.lex.x"
					{	/* sine */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING SIN\n");
#endif

	return(SIN_T);
}
break;
case 25:

# line 405 "P.lex.x"
					{	/* cosine */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING COS\n");
#endif

	return(COS_T);
}
break;
case 26:

# line 416 "P.lex.x"
					{	/* tangent */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING TAN\n");
#endif

	return(TAN_T);
}
break;
case 27:

# line 427 "P.lex.x"
					{	/* arc-tangent */
	START;

#ifdef DEBUG
	/* print message for testing */
	 printf("RETURNING ATAN\n");
#endif

	return(ATAN_T);
}
break;
case 28:

# line 437 "P.lex.x"
					{       /* exp() */
	START;
	return(EXP_T);
}
break;
case 29:

# line 442 "P.lex.x"
                                          {       /* abs() */
        START;
        return(ABS_T);
}
break;
case 30:

# line 448 "P.lex.x"
					{	/* substring */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING SUB\n");
#endif

	return(SUB_T);
}
break;
case 31:

# line 459 "P.lex.x"
					{	/* x cursor */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING X\n");
#endif

	return(X_T);
}
break;
case 32:

# line 470 "P.lex.x"
					{	/* y cursor */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING Y\n");
#endif

	return(Y_T);
}
break;
case 33:

# line 481 "P.lex.x"
					{	/* cursor flag */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING CRSR\n");
#endif

	return(CRSR_T);
}
break;
case 34:

# line 492 "P.lex.x"
				{	/* smart error */
							/* detection */

	SPCHAR;

	nferr("characters not recognized - labels end with \":\"");
}
break;
case 35:

# line 500 "P.lex.x"
					{
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING CHARACTER: yytext = %c\n",*yytext);
#endif

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

6,
0, 

6,
0, 

35,
0, 

6,
35,
0, 

15,
35,
0, 

14,
35,
0, 

16,
35,
0, 

1,
35,
0, 

35,
0, 

7,
34,
35,
0, 

35,
0, 

35,
0, 

4,
35,
0, 

5,
35,
0, 

21,
35,
0, 

34,
35,
-20,
0, 

34,
35,
-20,
0, 

34,
35,
-20,
0, 

34,
35,
-20,
0, 

34,
35,
-20,
0, 

34,
35,
-20,
0, 

34,
35,
-20,
0, 

34,
35,
-20,
0, 

31,
34,
35,
-20,
0, 

32,
34,
35,
-20,
0, 

17,
35,
0, 

6,
0, 

1,
0, 

5,
0, 

9,
0, 

10,
0, 

8,
0, 

7,
34,
0, 

34,
-19,
0, 

13,
0, 

2,
0, 

4,
0, 

20,
0, 

20,
34,
0, 

34,
-19,
0, 

34,
-19,
0, 

34,
-19,
0, 

34,
-19,
0, 

34,
-19,
0, 

34,
-19,
0, 

22,
34,
-19,
0, 

34,
-19,
0, 

12,
34,
-19,
0, 

34,
-19,
0, 

34,
-19,
0, 

34,
-19,
0, 

34,
-19,
0, 

7,
34,
0, 

34,
-18,
0, 

19,
0, 

19,
34,
0, 

3,
0, 

20,
34,
0, 

34,
-18,
0, 

29,
34,
-18,
0, 

34,
-18,
0, 

25,
34,
-18,
0, 

34,
-18,
0, 

28,
34,
-18,
0, 

23,
34,
-18,
0, 

24,
34,
-18,
0, 

30,
34,
-18,
0, 

26,
34,
-18,
0, 

11,
34,
-18,
0, 

34,
0, 

18,
0, 

18,
34,
0, 

34,
0, 

27,
34,
0, 

33,
34,
0, 
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	8,28,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,5,	
13,38,	14,30,	58,58,	1,6,	
1,7,	0,0,	0,0,	0,0,	
2,0,	1,8,	37,37,	1,9,	
0,0,	1,10,	8,28,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	13,38,	
14,30,	58,58,	1,11,	0,0,	
0,0,	0,0,	1,12,	0,0,	
2,5,	37,37,	0,0,	0,0,	
2,6,	2,7,	0,0,	0,0,	
0,0,	0,0,	2,8,	0,0,	
2,9,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
11,36,	0,0,	0,0,	0,0,	
1,13,	0,0,	1,14,	2,11,	
1,15,	0,0,	1,16,	1,17,	
1,18,	1,17,	1,19,	1,17,	
1,17,	1,17,	1,17,	1,17,	
1,17,	1,20,	1,17,	1,21,	
1,17,	1,17,	1,17,	1,17,	
1,22,	1,23,	1,17,	1,17,	
1,17,	1,24,	1,25,	1,17,	
1,26,	2,13,	0,0,	2,14,	
37,58,	2,15,	0,0,	0,0,	
2,17,	2,18,	2,17,	2,19,	
2,17,	2,17,	2,17,	2,17,	
2,17,	2,17,	2,20,	2,17,	
2,21,	2,17,	2,17,	2,17,	
2,17,	2,22,	2,23,	2,17,	
2,17,	2,17,	2,24,	2,25,	
2,17,	2,26,	4,27,	31,31,	
31,31,	31,31,	31,31,	31,31,	
31,31,	31,31,	31,31,	31,31,	
31,31,	32,32,	32,32,	32,32,	
32,32,	32,32,	32,32,	32,32,	
32,32,	32,32,	32,32,	0,0,	
0,0,	4,27,	33,33,	33,33,	
33,33,	33,33,	33,33,	33,33,	
33,33,	33,33,	33,33,	33,33,	
0,0,	4,28,	59,59,	59,59,	
59,59,	59,59,	59,59,	59,59,	
59,59,	59,59,	59,59,	59,59,	
71,71,	71,71,	71,71,	71,71,	
71,71,	71,71,	71,71,	71,71,	
71,71,	71,71,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
0,0,	0,0,	4,30,	0,0,	
0,0,	0,0,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
4,29,	4,29,	4,29,	4,29,	
9,31,	9,31,	9,31,	9,31,	
9,31,	9,31,	9,31,	9,31,	
9,31,	9,31,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	9,32,	
9,32,	9,32,	9,32,	10,33,	
0,0,	10,34,	10,34,	10,34,	
10,34,	10,34,	10,34,	10,34,	
10,34,	10,34,	10,34,	73,73,	
73,73,	73,73,	73,73,	73,73,	
73,73,	73,73,	73,73,	73,73,	
73,73,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	10,35,	10,35,	
10,35,	10,35,	10,35,	10,35,	
10,35,	10,35,	10,35,	10,35,	
10,35,	10,35,	10,35,	10,35,	
10,35,	10,35,	10,35,	10,35,	
10,35,	10,35,	10,35,	10,35,	
10,35,	10,35,	10,35,	10,35,	
12,37,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	12,29,	12,29,	12,29,	
12,29,	16,39,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	16,40,	
16,40,	16,40,	16,40,	16,40,	
16,40,	16,40,	16,40,	16,40,	
16,40,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	17,39,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	17,40,	17,40,	17,40,	
17,40,	17,40,	17,40,	17,40,	
17,40,	17,40,	17,40,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
16,41,	16,42,	16,41,	16,41,	
16,41,	16,41,	16,41,	16,41,	
16,41,	16,41,	16,41,	16,41,	
16,41,	16,41,	16,41,	16,41,	
16,41,	16,41,	16,41,	16,43,	
16,41,	16,41,	16,41,	16,41,	
16,41,	16,41,	17,41,	17,41,	
17,41,	17,41,	17,41,	17,41,	
17,41,	17,41,	17,41,	17,41,	
17,41,	17,41,	17,41,	17,41,	
17,41,	17,41,	17,41,	17,41,	
17,41,	17,41,	17,41,	17,41,	
17,41,	17,41,	17,41,	17,41,	
18,39,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	18,40,	18,40,	
18,40,	18,40,	18,40,	18,40,	
18,40,	18,40,	18,40,	18,40,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	19,39,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
19,40,	19,40,	19,40,	19,40,	
19,40,	19,40,	19,40,	19,40,	
19,40,	19,40,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	18,41,	
18,41,	18,41,	18,41,	18,41,	
18,41,	18,41,	18,41,	18,41,	
18,41,	18,41,	18,41,	18,41,	
18,41,	18,44,	18,41,	18,41,	
18,45,	18,41,	18,41,	18,41,	
18,41,	18,41,	18,41,	18,41,	
18,41,	19,41,	19,41,	19,41,	
19,41,	19,41,	19,41,	19,41,	
19,41,	19,41,	19,41,	19,41,	
19,41,	19,41,	19,41,	19,41,	
19,41,	19,41,	19,41,	19,41,	
19,41,	19,41,	19,41,	19,41,	
19,46,	19,41,	19,41,	20,39,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	20,40,	20,40,	20,40,	
20,40,	20,40,	20,40,	20,40,	
20,40,	20,40,	20,40,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	21,39,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	21,40,	
21,40,	21,40,	21,40,	21,40,	
21,40,	21,40,	21,40,	21,40,	
21,40,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	20,41,	20,41,	
20,41,	20,41,	20,41,	20,41,	
20,41,	20,41,	20,41,	20,41,	
20,41,	20,41,	20,41,	20,47,	
20,48,	20,41,	20,41,	20,41,	
20,41,	20,41,	20,41,	20,41,	
20,41,	20,41,	20,41,	20,41,	
21,41,	21,41,	21,41,	21,41,	
21,41,	21,41,	21,41,	21,41,	
21,41,	21,41,	21,41,	21,41,	
21,41,	21,41,	21,49,	21,41,	
21,41,	21,41,	21,41,	21,41,	
21,41,	21,41,	21,41,	21,41,	
21,41,	21,41,	22,39,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
22,40,	22,40,	22,40,	22,40,	
22,40,	22,40,	22,40,	22,40,	
22,40,	22,40,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
23,39,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	23,40,	23,40,	
23,40,	23,40,	23,40,	23,40,	
23,40,	23,40,	23,40,	23,40,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	22,41,	22,41,	22,41,	
22,41,	22,41,	22,41,	22,41,	
22,41,	22,50,	22,41,	22,41,	
22,41,	22,41,	22,41,	22,41,	
22,41,	22,41,	22,41,	22,41,	
22,41,	22,51,	22,41,	22,41,	
22,41,	22,41,	22,41,	23,52,	
23,41,	23,41,	23,41,	23,41,	
23,41,	23,41,	23,41,	23,41,	
23,41,	23,41,	23,41,	23,41,	
23,41,	23,41,	23,41,	23,41,	
23,41,	23,41,	23,41,	23,41,	
23,41,	23,41,	23,41,	23,41,	
23,41,	24,39,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	24,40,	
24,40,	24,40,	24,40,	24,40,	
24,40,	24,40,	24,40,	24,40,	
24,40,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	25,39,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	25,40,	25,40,	25,40,	
25,40,	25,40,	25,40,	25,40,	
25,40,	25,40,	25,40,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
24,41,	24,41,	24,41,	24,41,	
24,41,	24,41,	24,41,	24,41,	
24,41,	24,41,	24,41,	24,41,	
24,41,	24,41,	24,41,	24,41,	
24,41,	24,41,	24,41,	24,41,	
24,41,	24,41,	24,41,	24,41,	
24,41,	24,41,	25,41,	25,41,	
25,41,	25,41,	25,53,	25,41,	
25,41,	25,41,	25,41,	25,41,	
25,41,	25,41,	25,41,	25,41,	
25,41,	25,41,	25,41,	25,41,	
25,41,	25,41,	25,41,	25,41,	
25,41,	25,41,	25,41,	25,41,	
34,54,	34,54,	34,54,	34,54,	
34,54,	34,54,	34,54,	34,54,	
34,54,	34,54,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
35,56,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	35,57,	35,57,	
35,57,	35,57,	35,57,	35,57,	
35,57,	35,57,	35,57,	35,57,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	34,55,	34,55,	34,55,	
34,55,	34,55,	34,55,	34,55,	
34,55,	34,55,	34,55,	34,55,	
34,55,	34,55,	34,55,	34,55,	
34,55,	34,55,	34,55,	34,55,	
34,55,	34,55,	34,55,	34,55,	
34,55,	34,55,	34,55,	35,55,	
35,55,	35,55,	35,55,	35,55,	
35,55,	35,55,	35,55,	35,55,	
35,55,	35,55,	35,55,	35,55,	
35,55,	35,55,	35,55,	35,55,	
35,55,	35,55,	35,55,	35,55,	
35,55,	35,55,	35,55,	35,55,	
35,55,	40,59,	40,59,	40,59,	
40,59,	40,59,	40,59,	40,59,	
40,59,	40,59,	40,59,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	41,56,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	41,57,	
41,57,	41,57,	41,57,	41,57,	
41,57,	41,57,	41,57,	41,57,	
41,57,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	40,55,	40,55,	
40,55,	40,55,	40,55,	40,55,	
40,55,	40,55,	40,55,	40,55,	
40,55,	40,55,	40,55,	40,55,	
40,55,	40,55,	40,55,	40,55,	
40,55,	40,55,	40,55,	40,55,	
40,55,	40,55,	40,55,	40,55,	
41,60,	41,60,	41,60,	41,60,	
41,60,	41,60,	41,60,	41,60,	
41,60,	41,60,	41,60,	41,60,	
41,60,	41,60,	41,60,	41,60,	
41,60,	41,60,	41,60,	41,60,	
41,60,	41,60,	41,60,	41,60,	
41,60,	41,60,	42,56,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
42,57,	42,57,	42,57,	42,57,	
42,57,	42,57,	42,57,	42,57,	
42,57,	42,57,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
43,56,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	43,57,	43,57,	
43,57,	43,57,	43,57,	43,57,	
43,57,	43,57,	43,57,	43,57,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	42,60,	42,60,	42,60,	
42,60,	42,60,	42,60,	42,60,	
42,60,	42,60,	42,60,	42,60,	
42,60,	42,60,	42,60,	42,60,	
42,60,	42,60,	42,60,	42,61,	
42,60,	42,60,	42,60,	42,60,	
42,60,	42,60,	42,60,	43,62,	
43,60,	43,60,	43,60,	43,60,	
43,60,	43,60,	43,60,	43,60,	
43,60,	43,60,	43,60,	43,60,	
43,60,	43,60,	43,60,	43,60,	
43,60,	43,60,	43,60,	43,60,	
43,60,	43,60,	43,60,	43,60,	
43,60,	44,56,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	44,57,	
44,57,	44,57,	44,57,	44,57,	
44,57,	44,57,	44,57,	44,57,	
44,57,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	45,56,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	45,57,	45,57,	45,57,	
45,57,	45,57,	45,57,	45,57,	
45,57,	45,57,	45,57,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
44,60,	44,60,	44,60,	44,60,	
44,60,	44,60,	44,60,	44,60,	
44,60,	44,60,	44,60,	44,60,	
44,60,	44,60,	44,60,	44,60,	
44,60,	44,60,	44,63,	44,60,	
44,60,	44,60,	44,60,	44,60,	
44,60,	44,60,	45,60,	45,60,	
45,60,	45,60,	45,60,	45,60,	
45,60,	45,60,	45,60,	45,60,	
45,60,	45,60,	45,60,	45,60,	
45,60,	45,60,	45,60,	45,60,	
45,64,	45,60,	45,60,	45,60,	
45,60,	45,60,	45,60,	45,60,	
46,56,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	46,57,	46,57,	
46,57,	46,57,	46,57,	46,57,	
46,57,	46,57,	46,57,	46,57,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	47,56,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
47,57,	47,57,	47,57,	47,57,	
47,57,	47,57,	47,57,	47,57,	
47,57,	47,57,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	46,60,	
46,60,	46,60,	46,60,	46,60,	
46,60,	46,60,	46,60,	46,60,	
46,60,	46,60,	46,60,	46,60,	
46,60,	46,60,	46,65,	46,60,	
46,60,	46,60,	46,60,	46,60,	
46,60,	46,60,	46,60,	46,60,	
46,60,	47,60,	47,60,	47,60,	
47,60,	47,60,	47,60,	47,60,	
47,60,	47,60,	47,60,	47,60,	
47,60,	47,60,	47,60,	47,60,	
47,60,	47,60,	47,60,	47,60,	
47,60,	47,60,	47,60,	47,60,	
47,60,	47,60,	47,60,	48,56,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	48,57,	48,57,	48,57,	
48,57,	48,57,	48,57,	48,57,	
48,57,	48,57,	48,57,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	49,56,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	49,57,	
49,57,	49,57,	49,57,	49,57,	
49,57,	49,57,	49,57,	49,57,	
49,57,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	48,60,	48,60,	
48,60,	48,60,	48,60,	48,60,	
48,66,	48,60,	48,60,	48,60,	
48,60,	48,60,	48,60,	48,60,	
48,60,	48,60,	48,60,	48,60,	
48,60,	48,60,	48,60,	48,60,	
48,60,	48,60,	48,60,	48,60,	
49,60,	49,60,	49,60,	49,60,	
49,60,	49,60,	49,60,	49,60,	
49,60,	49,60,	49,60,	49,60,	
49,60,	49,60,	49,60,	49,60,	
49,60,	49,60,	49,60,	49,60,	
49,60,	49,60,	49,60,	49,60,	
49,60,	49,60,	50,56,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
50,57,	50,57,	50,57,	50,57,	
50,57,	50,57,	50,57,	50,57,	
50,57,	50,57,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
51,56,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	51,57,	51,57,	
51,57,	51,57,	51,57,	51,57,	
51,57,	51,57,	51,57,	51,57,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	50,60,	50,60,	50,60,	
50,60,	50,60,	50,60,	50,60,	
50,60,	50,60,	50,60,	50,60,	
50,60,	50,60,	50,67,	50,60,	
50,60,	50,60,	50,60,	50,60,	
50,60,	50,60,	50,60,	50,60,	
50,60,	50,60,	50,60,	51,60,	
51,68,	51,60,	51,60,	51,60,	
51,60,	51,60,	51,60,	51,60,	
51,60,	51,60,	51,60,	51,60,	
51,60,	51,60,	51,60,	51,60,	
51,60,	51,60,	51,60,	51,60,	
51,60,	51,60,	51,60,	51,60,	
51,60,	52,56,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	52,57,	
52,57,	52,57,	52,57,	52,57,	
52,57,	52,57,	52,57,	52,57,	
52,57,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	53,56,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	53,57,	53,57,	53,57,	
53,57,	53,57,	53,57,	53,57,	
53,57,	53,57,	53,57,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
52,60,	52,60,	52,60,	52,60,	
52,60,	52,60,	52,60,	52,60,	
52,60,	52,60,	52,60,	52,60,	
52,60,	52,69,	52,60,	52,60,	
52,60,	52,60,	52,60,	52,60,	
52,60,	52,60,	52,60,	52,60,	
52,60,	52,60,	53,60,	53,60,	
53,60,	53,60,	53,60,	53,60,	
53,60,	53,60,	53,60,	53,60,	
53,60,	53,60,	53,60,	53,60,	
53,60,	53,60,	53,60,	53,60,	
53,70,	53,60,	53,60,	53,60,	
53,60,	53,60,	53,60,	53,60,	
54,54,	54,54,	54,54,	54,54,	
54,54,	54,54,	54,54,	54,54,	
54,54,	54,54,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
55,72,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	55,73,	55,73,	
55,73,	55,73,	55,73,	55,73,	
55,73,	55,73,	55,73,	55,73,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	54,71,	54,71,	54,71,	
54,71,	54,71,	54,71,	54,71,	
54,71,	54,71,	54,71,	54,71,	
54,71,	54,71,	54,71,	54,71,	
54,71,	54,71,	54,71,	54,71,	
54,71,	54,71,	54,71,	54,71,	
54,71,	54,71,	54,71,	55,71,	
55,71,	55,71,	55,71,	55,71,	
55,71,	55,71,	55,71,	55,71,	
55,71,	55,71,	55,71,	55,71,	
55,71,	55,71,	55,71,	55,71,	
55,71,	55,71,	55,71,	55,71,	
55,71,	55,71,	55,71,	55,71,	
55,71,	57,57,	57,57,	57,57,	
57,57,	57,57,	57,57,	57,57,	
57,57,	57,57,	57,57,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	60,72,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	60,73,	
60,73,	60,73,	60,73,	60,73,	
60,73,	60,73,	60,73,	60,73,	
60,73,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	57,71,	57,71,	
57,71,	57,71,	57,71,	57,71,	
57,71,	57,71,	57,71,	57,71,	
57,71,	57,71,	57,71,	57,71,	
57,71,	57,71,	57,71,	57,71,	
57,71,	57,71,	57,71,	57,71,	
57,71,	57,71,	57,71,	57,71,	
60,74,	60,74,	60,74,	60,74,	
60,74,	60,74,	60,74,	60,74,	
60,74,	60,74,	60,74,	60,74,	
60,74,	60,74,	60,74,	60,74,	
60,74,	60,74,	60,74,	60,74,	
60,74,	60,74,	60,74,	60,74,	
60,74,	60,74,	61,72,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
61,73,	61,73,	61,73,	61,73,	
61,73,	61,73,	61,73,	61,73,	
61,73,	61,73,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
62,72,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	62,73,	62,73,	
62,73,	62,73,	62,73,	62,73,	
62,73,	62,73,	62,73,	62,73,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	61,74,	61,74,	61,74,	
61,74,	61,74,	61,74,	61,74,	
61,74,	61,74,	61,74,	61,74,	
61,74,	61,74,	61,74,	61,74,	
61,74,	61,74,	61,74,	61,74,	
61,74,	61,74,	61,74,	61,74,	
61,74,	61,74,	61,74,	62,74,	
62,74,	62,74,	62,74,	62,74,	
62,74,	62,74,	62,74,	62,74,	
62,74,	62,74,	62,74,	62,74,	
62,75,	62,74,	62,74,	62,74,	
62,74,	62,74,	62,74,	62,74,	
62,74,	62,74,	62,74,	62,74,	
62,74,	63,72,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	63,73,	
63,73,	63,73,	63,73,	63,73,	
63,73,	63,73,	63,73,	63,73,	
63,73,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	64,72,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	64,73,	64,73,	64,73,	
64,73,	64,73,	64,73,	64,73,	
64,73,	64,73,	64,73,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
63,74,	63,74,	63,74,	63,74,	
63,74,	63,74,	63,74,	63,74,	
63,74,	63,74,	63,74,	63,74,	
63,74,	63,74,	63,74,	63,74,	
63,74,	63,74,	63,74,	63,74,	
63,74,	63,74,	63,74,	63,74,	
63,74,	63,74,	64,74,	64,74,	
64,74,	64,74,	64,74,	64,74,	
64,74,	64,74,	64,74,	64,74,	
64,74,	64,74,	64,74,	64,74,	
64,74,	64,74,	64,74,	64,76,	
64,74,	64,74,	64,74,	64,74,	
64,74,	64,74,	64,74,	64,74,	
65,72,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	65,73,	65,73,	
65,73,	65,73,	65,73,	65,73,	
65,73,	65,73,	65,73,	65,73,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	66,72,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
66,73,	66,73,	66,73,	66,73,	
66,73,	66,73,	66,73,	66,73,	
66,73,	66,73,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	65,74,	
65,74,	65,74,	65,74,	65,74,	
65,74,	65,74,	65,74,	65,74,	
65,74,	65,74,	65,74,	65,74,	
65,74,	65,74,	65,74,	65,74,	
65,74,	65,74,	65,74,	65,74,	
65,74,	65,74,	65,74,	65,74,	
65,74,	66,74,	66,74,	66,74,	
66,74,	66,74,	66,74,	66,74,	
66,74,	66,74,	66,74,	66,74,	
66,74,	66,74,	66,74,	66,74,	
66,74,	66,74,	66,74,	66,74,	
66,74,	66,74,	66,74,	66,74,	
66,74,	66,74,	66,74,	67,72,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	67,73,	67,73,	67,73,	
67,73,	67,73,	67,73,	67,73,	
67,73,	67,73,	67,73,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	68,72,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	68,73,	
68,73,	68,73,	68,73,	68,73,	
68,73,	68,73,	68,73,	68,73,	
68,73,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	67,74,	67,74,	
67,74,	67,74,	67,74,	67,74,	
67,74,	67,74,	67,74,	67,74,	
67,74,	67,74,	67,74,	67,74,	
67,74,	67,74,	67,74,	67,74,	
67,74,	67,74,	67,74,	67,74,	
67,74,	67,74,	67,74,	67,74,	
68,74,	68,74,	68,74,	68,74,	
68,74,	68,74,	68,74,	68,74,	
68,74,	68,74,	68,74,	68,74,	
68,74,	68,74,	68,74,	68,74,	
68,74,	68,74,	68,74,	68,74,	
68,74,	68,74,	68,74,	68,74,	
68,74,	68,74,	69,72,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
69,73,	69,73,	69,73,	69,73,	
69,73,	69,73,	69,73,	69,73,	
69,73,	69,73,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
70,72,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	70,73,	70,73,	
70,73,	70,73,	70,73,	70,73,	
70,73,	70,73,	70,73,	70,73,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	69,74,	69,74,	69,74,	
69,74,	69,74,	69,74,	69,74,	
69,74,	69,74,	69,74,	69,74,	
69,74,	69,74,	69,74,	69,74,	
69,74,	69,74,	69,74,	69,74,	
69,74,	69,74,	69,74,	69,74,	
69,74,	69,74,	69,74,	70,74,	
70,74,	70,74,	70,74,	70,74,	
70,74,	70,74,	70,74,	70,74,	
70,74,	70,74,	70,74,	70,74,	
70,74,	70,74,	70,74,	70,74,	
70,74,	70,74,	70,74,	70,74,	
70,74,	70,74,	70,74,	70,74,	
70,74,	74,71,	74,71,	74,71,	
74,71,	74,71,	74,71,	74,71,	
74,71,	74,71,	74,71,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	75,71,	
75,71,	75,71,	75,71,	75,71,	
75,71,	75,71,	75,71,	75,71,	
75,71,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	74,74,	74,74,	
74,74,	74,74,	74,74,	74,74,	
74,74,	74,74,	74,74,	74,74,	
74,74,	74,74,	74,74,	74,74,	
74,74,	74,74,	74,74,	74,74,	
74,74,	74,74,	74,74,	74,74,	
74,74,	74,74,	74,74,	74,74,	
75,74,	75,74,	75,74,	75,74,	
75,74,	75,74,	75,74,	75,74,	
75,74,	75,74,	75,74,	75,74,	
75,74,	75,74,	75,74,	75,74,	
75,74,	75,74,	75,74,	75,74,	
75,74,	75,74,	75,74,	75,74,	
75,74,	75,74,	76,71,	76,71,	
76,71,	76,71,	76,71,	76,71,	
76,71,	76,71,	76,71,	76,71,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	76,74,	
76,74,	76,74,	76,74,	76,74,	
76,74,	76,74,	76,74,	76,74,	
76,74,	76,74,	76,74,	76,74,	
76,74,	76,74,	76,74,	76,74,	
76,74,	76,74,	76,74,	76,74,	
76,74,	76,74,	76,74,	76,74,	
76,74,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		yyvstop+1,
yycrank+-34,	yysvec+1,	yyvstop+3,
yycrank+0,	0,		yyvstop+5,
yycrank+149,	0,		yyvstop+7,
yycrank+0,	0,		yyvstop+10,
yycrank+0,	0,		yyvstop+13,
yycrank+0,	0,		yyvstop+16,
yycrank+18,	0,		yyvstop+19,
yycrank+224,	0,		yyvstop+22,
yycrank+301,	0,		yyvstop+24,
yycrank+27,	0,		yyvstop+28,
yycrank+366,	0,		yyvstop+30,
yycrank+27,	0,		yyvstop+32,
yycrank+28,	0,		yyvstop+35,
yycrank+0,	0,		yyvstop+38,
yycrank+451,	yysvec+12,	yyvstop+41,
yycrank+477,	yysvec+12,	yyvstop+45,
yycrank+562,	yysvec+12,	yyvstop+49,
yycrank+588,	yysvec+12,	yyvstop+53,
yycrank+673,	yysvec+12,	yyvstop+57,
yycrank+699,	yysvec+12,	yyvstop+61,
yycrank+784,	yysvec+12,	yyvstop+65,
yycrank+810,	yysvec+12,	yyvstop+69,
yycrank+895,	yysvec+12,	yyvstop+73,
yycrank+921,	yysvec+12,	yyvstop+78,
yycrank+0,	0,		yyvstop+83,
yycrank+0,	yysvec+4,	yyvstop+86,
yycrank+0,	yysvec+8,	yyvstop+88,
yycrank+0,	yysvec+12,	0,	
yycrank+0,	yysvec+14,	yyvstop+90,
yycrank+111,	0,		yyvstop+92,
yycrank+121,	yysvec+9,	yyvstop+94,
yycrank+134,	0,		yyvstop+96,
yycrank+996,	yysvec+10,	yyvstop+98,
yycrank+1022,	0,		yyvstop+101,
yycrank+0,	0,		yyvstop+104,
yycrank+37,	0,		yyvstop+106,
yycrank+0,	yysvec+13,	yyvstop+108,
yycrank+0,	0,		yyvstop+110,
yycrank+1097,	0,		yyvstop+112,
yycrank+1123,	yysvec+12,	yyvstop+115,
yycrank+1208,	yysvec+12,	yyvstop+118,
yycrank+1234,	yysvec+12,	yyvstop+121,
yycrank+1319,	yysvec+12,	yyvstop+124,
yycrank+1345,	yysvec+12,	yyvstop+127,
yycrank+1430,	yysvec+12,	yyvstop+130,
yycrank+1456,	yysvec+12,	yyvstop+133,
yycrank+1541,	yysvec+12,	yyvstop+137,
yycrank+1567,	yysvec+12,	yyvstop+140,
yycrank+1652,	yysvec+12,	yyvstop+144,
yycrank+1678,	yysvec+12,	yyvstop+147,
yycrank+1763,	yysvec+12,	yyvstop+150,
yycrank+1789,	yysvec+12,	yyvstop+153,
yycrank+1864,	yysvec+10,	yyvstop+156,
yycrank+1890,	0,		yyvstop+159,
yycrank+0,	0,		yyvstop+162,
yycrank+1965,	0,		yyvstop+164,
yycrank+29,	0,		yyvstop+167,
yycrank+146,	yysvec+57,	yyvstop+169,
yycrank+1991,	yysvec+12,	yyvstop+172,
yycrank+2076,	yysvec+12,	yyvstop+175,
yycrank+2102,	yysvec+12,	yyvstop+179,
yycrank+2187,	yysvec+12,	yyvstop+182,
yycrank+2213,	yysvec+12,	yyvstop+186,
yycrank+2298,	yysvec+12,	yyvstop+189,
yycrank+2324,	yysvec+12,	yyvstop+193,
yycrank+2409,	yysvec+12,	yyvstop+197,
yycrank+2435,	yysvec+12,	yyvstop+201,
yycrank+2520,	yysvec+12,	yyvstop+205,
yycrank+2546,	yysvec+12,	yyvstop+209,
yycrank+156,	yysvec+57,	yyvstop+213,
yycrank+0,	0,		yyvstop+215,
yycrank+311,	yysvec+57,	yyvstop+217,
yycrank+2621,	yysvec+12,	yyvstop+220,
yycrank+2647,	yysvec+12,	yyvstop+222,
yycrank+2722,	yysvec+12,	yyvstop+225,
0,	0,	0};
struct yywork *yytop = yycrank+2844;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
  0,   1,   1,   1,   1,   1,   1,   1, 
  1,   9,  10,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
  9,   1,   1,   1,   1,   1,   1,   1, 
  1,   1,   1,   1,   1,   1,   1,   1, 
 48,  48,  48,  48,  48,  48,  48,  48, 
 48,  48,   1,   1,   1,   1,   1,   1, 
  1,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,  65,  65,  65,  65,  65, 
 65,  65,  65,   1,   1,   1,   1,   1, 
  1,  97,  97,  97,  97,  97,  97,  97, 
 97,  97,  97,  97,  97,  97,  97,  97, 
 97,  97,  97,  97,  97,  97,  97,  97, 
 97,  97,  97,   1,   1,   1,   1,   1, 
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
0,0,1,1,1,0,0,0,
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
