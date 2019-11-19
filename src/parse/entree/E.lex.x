%{
# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"
# include "y.tab.h"
	extern char *yylval;
%}

%%

"parse"						{	/* parse */
	return(PARS_T);
}

"ex"						{	/* exclusive */
	return(EX_T);
}

"fl"						{	/* flag */
	return(FL_T);
}

"int"						{	/* integer */
	return(INT_T);
}

"real"						{	/* real */
	return(REAL_T);
}

"scan"						{	/* scan number */
	return(SCAN_T);
}

"str"						{	/* string variable */
	return(STR_T);
}

"cmd"						{	/* command list */
	return(CMD_T);
}

"exp"						{	/* expression */
	return(EXP_T);
}

"range"						{	/* range */
	return(RNG_T);
}

"list"						{	/* list */
	return(LST_T);
}

"auto"						{	/* automatic */
	return(AUTO_T);
}

"set"						{	/* set */
	return(SET_T);
}

"old"						{	/* old */
	return(OLD_T);
}

"flag"						{	/* flag default */
	return(FLAG_T);
}

"toggle"					{	/* toggle default */
	return(TOGGLE_T);
}

"extern"					{	/* external variable */
	return(EXT_T);
}

"\""						{	/* strings */
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

[ 	,]					{	/* characters to */
}							/* be ignored */

\n						{	/* new line */
	clin++;
}

.						{	/* anything else */
	return(*yytext);
}
