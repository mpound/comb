/* MWP needed to increase default space for lex after adding ABS_T node*/
%n 3000     /* number transitions --  default is 500  */
%a 3000     /* number packed transitions --  default is 500  */
%{
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
%}

num		[0-9]
alph		[A-Za-z]
l_alph		[a-z]
alph_num	[0-9A-Za-z]
l_alph_num	[0-9a-z]
filnum		({num}+|"&")
white		[	 ]*

%%

{white}","{white}				{	/* commas */

	SPCHAR;

	/* set nnode to next parameter */
	nnode = param(next(nnode));

#ifdef DEBUG
	/* print report of comma for testing */
	printf("FOUND COMMA: next node = %s\n",nnode->des);
#endif

	return(END_T);
}

{white}{alph}+":"{white}			{	/* labels */

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

{white}{alph}+":"{white}"["{white}		{	/* label with [ */

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

"["{white}					{	/* [ */

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

{white}"]"{white}				{	/* ] */

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

{white}						{	/* white space */

	SPCHAR;

	/* set nnode to next parameter */
	nnode = param(next(nnode));

#ifdef DEBUG
	/* print report of white space for testing */
	printf("FOUND WHITE SPACE: next node = %s\n",nnode->des);
#endif

	return(END_T);
}


{num}+						|	/* numbers */
{num}+"."{num}*					|
"."{num}+					{

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

"."{alph}{alph_num}*				{	/* header variables */

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

"yes"						{	/* logical */
	yylval.real = 1.;
	return(NUM_T);
}

"no"						{
	yylval.real = 0.;
	return(NUM_T);
}

"=="	{
	return(EQUEQU_T);
}

"&"						{	/* & */

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

"\""						|	/* strings */
"'"						{
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

"{"						{
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

{l_alph_num}{l_alph_num}{l_alph}/{filnum}	|	/* file names */
{l_alph_num}{l_alph}/{filnum}			|
{l_alph}/{filnum}				{

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

"_"						{	/* _ */

	START;

	/* set second part of range flag */
	spor = 1;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING UNDERSCORE\n");
#endif

	return('_');
}

"ln"						{	/* natural log */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING LN\n");
#endif

	return(LN_T);
}

"log"						{	/* log to a base */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING LOG\n");
#endif

	return(LOG_T);
}

"sin"						{	/* sine */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING SIN\n");
#endif

	return(SIN_T);
}

"cos"						{	/* cosine */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING COS\n");
#endif

	return(COS_T);
}

"tan"						{	/* tangent */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING TAN\n");
#endif

	return(TAN_T);
}

"atan"						{	/* arc-tangent */
	START;

#ifdef DEBUG
	/* print message for testing */
	 printf("RETURNING ATAN\n");
#endif

	return(ATAN_T);
}
"exp"						{       /* exp() */
	START;
	return(EXP_T);
}

"abs"                                           {       /* abs() */
        START;
        return(ABS_T);
}


"sub"						{	/* substring */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING SUB\n");
#endif

	return(SUB_T);
}

"x"						{	/* x cursor */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING X\n");
#endif

	return(X_T);
}

"y"						{	/* y cursor */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING Y\n");
#endif

	return(Y_T);
}

"crsr"						{	/* cursor flag */
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING CRSR\n");
#endif

	return(CRSR_T);
}

{l_alph_num}+					{	/* smart error */
							/* detection */

	SPCHAR;

	nferr("characters not recognized - labels end with \":\"");
}

.						{
	START;

#ifdef DEBUG
	/* print message for testing */
	printf("RETURNING CHARACTER: yytext = %c\n",*yytext);
#endif

	return(*yytext);
}
