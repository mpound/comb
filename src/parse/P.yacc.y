%{
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
%}

%token END_T 257

%token <real> NUM_T 258
%token <string> STR_T 259
%token <string> FILNM_T 260

%token NAND_T 261
%token SAND_T 262
%token LN_T 263
%token LOG_T 264
%token SIN_T 265
%token COS_T 266
%token TAN_T 267
%token ATAN_T 268
%token SUB_T 269
%token EXP_T 270
%token ABS_T 271
%token X_T 272
%token Y_T 273
%token CRSR_T 274
%token <string> HDRVAR_T 275
%token <string> STRHDRVAR_T 276
%token EQUEQU_T 277

%token EOARGS 1

%type <real> nexp nvar svar
%type <string> sexp

%right LOW
%left FILNM_T NAND_T SAND_T SUB_T
/* %left STR_T FILNM_T NAND_T SAND_T SUB_T */
%nonassoc COND '>' '<' EQUEQU
%right '='
%left STR_T
/* %left EQUEQU */
%left '+' '-'
%left '*' '/'
%right '^'
%left HIGH
%left HIGHER
%right ':'
%nonassoc 'E'

%%

	/*****************/
	/* argument list */
	/*****************/

args	:
	|	args endcode
	|	args number
	|	args string
	|	args scannum
	|	args EOARGS
		{
			YYACCEPT;
		}
	;

	/***************************************/
	/* all tokens that can end a parameter */
	/***************************************/

endcode	:	END_T
	|	';'
		{
			/* insure that lex will return an end */
			/* with next call so that yacc will */
			/* stop */
			unput(EOARGS);
		}
	;

	/***********/
	/* numbers */
	/***********/

number	:	nexp endcode
		{
			/* store value */
			st_num($1,$1);

			/* set default flags */
			tag(cnode);
		}

	|	nexp '_' nexp endcode
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
			st_num($1,$3);

			/* set default flags */
			tag(cnode);
		}
	;

nexp	:	NUM_T
	|	NAND_T
		{
			/* check that "&" may be used */
			if(!(USE_TYPE(cnode) & cda_type))
				nferr("old data type incompatable");

			/* get value */
			switch(USE_TYPE(cnode))
			{
				case REAL:
					if(spor)
						$$ = L_PARAM2(double,cnode);
					else
						$$ = L_PARAM(double,cnode);
					break;
				case INTN:
					if(spor)
						$$ = L_PARAM2(int,cnode);
					else
						$$ = L_PARAM(int,cnode);
					break;
#if COMB
				case SCAN:
					if(spor)
						$$ = L_PARAM2(Scan,cnode).num;
					else
						$$ = L_PARAM(Scan,cnode).num;
					break;
#endif /*COMB*/
			}

			/* limit data type to current data type */
			cda_type = USE_TYPE(cnode);
		}
	|	'(' nexp ')'
		{
			$$ = $2;
		}
	|	nexp '+' nexp
		{
			$$ = $1 + $3;
		}
	|	nexp '-' nexp
		{
			$$ = $1 - $3;
		}
	|	nexp '*' nexp
		{
			$$ = $1 * $3;
		}
	|	nexp '/' nexp
		{
			/* check for error */
			if($3 == 0)
				nferr("divide by zero");

			$$ = $1 / $3;
		}
	|	nexp '^' nexp
		{
			/* check for errors */
			if($3 != INT($3) && $1 < 0)
			{
				str -= yyleng;
				nferr("negative number to non-integral power");
			}
			if($3 < 0 && $1 == 0)
			{
				str -= yyleng;
				nferr("zero to negative power");
			}

			$$ = pow($1,$3);
		}
	|	nexp ':' nexp
		{
			/* check for errors */
			if(!(cda_type &= (REAL | INTN)))
			{
				str -= yyleng;
				nferr("hh:mm:ss only works for numbers");
			}
			if($3 < 0)
			{
				str -= yyleng;
				nferr("negative number in hh:mm:ss");
			}

			$$ = $1 + $3 / 60;
		}
	|	nexp 'E' nexp
		{
			$$ = $1 * pow(10.,$3);
		}
	|	'-' nexp			%prec HIGH
		{
			$$ = -$2;
		}
	|	nexp '>' nexp
		{
			$$ = ($1 > $3);
		}
	|	nexp '<' nexp
		{
			$$ = ($1 < $3);
		}
	|	nexp '>' '=' nexp			%prec COND
		{
			$$ = ($1 >= $4);
		}
/*	|	nexp '=' '>' nexp			%prec COND
		{
			$$ = ($1 >= $4);
		}
*/
	|	nexp '<' '=' nexp			%prec COND
		{
			$$ = ($1 <= $4);
		}
/*	|	nexp '=' '<' nexp			%prec COND
		{
			$$ = ($1 <= $4);
		}
*/
	|	nexp '<' '>' nexp
		{
			$$ = ($1 != $4);
		}
	|	nexp EQUEQU_T nexp			%prec COND 
		{
			$$ = ($1 == $3);
		}
/* allow "OR" for nexp. "AND" can be done with "*" - mwp 29sep97*/
	|	nexp '|' '|' nexp			%prec COND
		{
			if($4 == 1 || $1 == 1)
				$$ = 1;
			else
				$$ = 0;
		}
	/*RWW added 9/21/91 for comparing string values header variables in
	 * scan2.  The 'strcmp' test is difficult because Matt concatinates
	 * the strings as they come in.  In principle they should be kept in
	 * separate buffers until the place where two adjacent strings down
	 * under strings:.  All of the strings are kept in one static buffer
	 * rather than separately malloc'ed buffers.*/
	|	sexp EQUEQU_T sexp			%prec COND
		{
			if($3 - $1 != strlen($3))
				$$ = 0;
			else
				$$ = !strncmp($1, $3, $3 - $1);
		}
	/* MWP added 12/21/91 for "not equals" test with strings */
	|	sexp '<' '>' sexp			%prec COND
		{
			if($4 - $1 != strlen($4))
				$$ = 1;
			else
				$$ = strncmp($1, $4, $4 - $1);
		}
	|	'?' push nexp '|' {PULL PUSH} nexp '|' {PULL PUSH} nexp ')'
		{
			PULL
			if((int)($3))
				$$ = $6;
			else
				$$ = $9;
		}
	|	LN_T push nexp ')'
		{
			PULL;

			/* check for errors */
			if($3 < 0)
				nferr("natural log of a negative number");
			if($3 == 0)
				nferr("natural log of zero");

			$$ = log($3);
		}
	|	LOG_T push nexp ')'
		{
			PULL;

			/* check for errors */
			if($3 < 0)
				nferr("log base ten of a negative number");
			if($3 == 0)
				nferr("log base ten of zero");

			$$ = log10($3);
		}
	|	LOG_T push nexp '|' {PULL PUSH} nexp ')'
		{
			PULL;

			/* check for errors */
			if($3 < 0)
				nferr("log with a negative base");
			if($3 == 0)
				nferr("log base zero");
			if($6 < 0)
			{
				sprintf(errstr,
					"log base %g of a negative number",
					$3);
				nferr(errstr);
			}
			if($6 == 0)
			{
				sprintf(errstr,"log base %g of zero",$3);
				nferr(errstr);
			}

			$$ = log($6) / log($3);
		}
	|	SIN_T push nexp ')'
		{
			PULL;

			$$ = sin($3 * degToRad);
		}
	|	COS_T push nexp ')'
		{
			PULL;

			$$ = cos($3 * degToRad);
		}
	|	TAN_T push nexp ')'
		{
			PULL;

			/* check for errors */
			if($3 == 90)
				nferr("tangent of vertical line");

			$$ = sin($3 * degToRad) / cos($3 * degToRad);
		}
	|	ATAN_T push nexp '/' nexp ')'
		{
			PULL;

			$$ = atan2($3,$5) / degToRad;
		}
	|	ATAN_T push nexp ')'
		{
			PULL;

			$$ = atan2($3,1.) / degToRad;
		}
	|       EXP_T push nexp ')'
		{
			PULL;
			$$ = exp($3);
		}
	|       ABS_T push nexp ')'
		{
			PULL;
			$$ = fabs($3);
		}
	|	X_T push ')'
		{
			double txc,tyc;
			char tchar;

			PULL;

/*
			gcurs_(&txc,&tyc,&tchar);
*/
			$$ = txc;
		}
	|	Y_T push ')'
		{
			double txc,tyc;
			char tchar;

			PULL;

/*
			gcurs_(&txc,&tyc,&tchar);
*/
			$$ = tyc;
		}
	|	X_T push nexp ')'
		{
			PULL;

			/* check for error */
			if(INT($3) < 0 || INT($3) > 9)
				error_("Illegal cursor subscript");

			$$ = xc[INT($3)];
		}
	|	Y_T push nexp ')'
		{
			PULL;

			/* check for error */
			if(INT($3) < 0 || INT($3) > 9)
				error_("Illegal cursor subscript");

			$$ = yc[INT($3)];
		}
	|	CRSR_T push nexp ')'
		{
			PULL;

			/* check for error */
			if(INT($3) < 0 || INT($3) > 9)
				error_("Illegal cursor subscript");

			$$ = crsr[INT($3)];
		}
	|	nvar '=' nexp
		{
			$$ = (gg_[INT($1)] = $3);
		}
	|	nvar
		{
			$$ = gg_[INT($1)];
		}
	|	HDRVAR_T
		{
			$$ = hdrval((HdrVar *)($1),1);
		}
	|	HDRVAR_T push nexp ')'
		{
			PULL;
			$$ = hdrval((HdrVar *)($1),(int)($3));
		}
	|	HDRVAR_T '=' nexp
		{
			$$ = $3;
			asgnhdr((HdrVar *)($1),1,$3);
		}
	|	HDRVAR_T push nexp ')' '=' nexp
		{
			PULL;
			$$ = $6;
			asgnhdr((HdrVar *)($1),(int)($3),$6);
		}

		/* smart errors */

	|	nexp STR_T
		{
			nferr("string in numerical expression");
		}
	|	nexp SAND_T
		{
			nferr("string value in numerical expression");
		}
	|	nexp FILNM_T
		{
			nferr("file name in numerical expression");
		}
	|	nexp '*' '*'
		{
			nferr("characters undefined - use ^ for exponentiation");
		}
	;

	/***********/
	/* strings */
	/***********/

string	:	sexp endcode
		{
			/* store value */
			st_str($1);

			/* set default flags */
			tag(cnode);
		}
	;

sexp	:	STR_T
		{
			/* save current string position */
			$$ = strpos;

			/* copy string into string buffer */
			strcpy(strpos,$1);

			/* update strpos */
			strpos = strchr(strpos,0);

			/* erase string */
			ChkFree($1);
		}
	|	SAND_T
		{
			/* make assignment */
			$$ = strpos;
			strcpy(strpos,&L_PARAM(char,cnode));
			strpos = strchr(strpos,0);
		}
	|	sexp sexp			%prec LOW
	|	'(' sexp ')'
		{
			$$ = $2;
		}
	|	SUB_T '(' sexp '|' {PUSH} nexp '|' {PULL PUSH} nexp ')'
		{
			PULL;

			/* compensate for string starting at 0 */
			$6 = $6 - 1;

			/* check for errors */
			if($6 < 0)
				nferr("illegal starting location");
			if($6 >= (int)strlen($3))
				nferr("starting location too high");
			if($9 <= 0)
				nferr("illegal length");
			if($9 > (int)strlen($3) - INT($6))
				nferr("length too long");

			/* make assignment */
/*			bcopy($3 + INT($6),$3, INT($9)); */
			memcpy($3, $3 + INT($6), INT($9));
			strpos = $3 + INT($9);
			*strpos = 0;

			/* set final value */
			$$ = $3;
		}
	|	svar '=' sexp
		{
			/* save beginning of string */
			$$ = $3;

			/* make room in string variable */
			if(sg_[INT($1)])
			{
				ChkFree(sg_[INT($1)]);
			}
			sg_[INT($1)] = (ChkAlloc(strlen($3) + 1, "Parse String"));

			/* store string */
			strcpy(sg_[INT($1)],$3);
		}
	|	svar
		{
			/* save beginning of string */
			$$ = strpos;

			/* error if variable is not defined */
			if(sg_[INT($1)] == 0)
				nferr("Global string undefined");

			/* copy variable into string buffer */
			strcpy(strpos,sg_[INT($1)]);

			/* set strpos to end of string buffer */
			strpos = strchr(strpos,0);
			
		}
	/*RWW 9/21/91 added to read scan2 header variables */
	|	STRHDRVAR_T
		{
			$$ = strpos;

			/* error if variable is not defined */
			if(((HdrVar *)$1)->lc == 0)
				nferr("User string variable not set");

                        strcpy(strpos, hdrstrval((HdrVar *)$1));
			strpos = strchr(strpos,0);
		}
	|	STRHDRVAR_T '=' sexp
		{
			$$ = $3;
			asgnstrhdr((HdrVar *)($1), $3);
		}
	;

	/****************/
	/* scan numbers */
	/****************/

scannum	:	FILNM_T nexp endcode
		{
#if COMB
			/* store value */
			st_scan($1,$2,$1,$2);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		}
	|	FILNM_T nexp '_' nexp endcode
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
			st_scan($1,$2,$1,$4);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		}
	|	nexp '_' FILNM_T nexp endcode
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
			st_scan((char *)0,$1,$3,$4);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		}
	|	FILNM_T nexp '_' FILNM_T nexp endcode
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
			st_scan($1,$2,$4,$5);

			/* set default flags */
			tag(cnode);
#else /*COMB*/
			nferr("No 'scan numbers' in obs");
#endif /*COMB*/
		}
	;

	/**************************/
	/* open paren with a push */
	/**************************/

push	:	'('
		{
			PUSH;
		}
	;

	/******************/
	/* user variables */
	/******************/

nvar	:	nvart  NUM_T			%prec HIGHER
		{
			PULL;
			/* check for errors */
			if($2 < 0 || $2 > 9)
				nferr("subscript out of bounds");

			/* get correct variable */
			$$ = $2;
		}
	|	nvart '(' nexp ')'		%prec HIGHER
		{
			PULL;
			if($3 < 0 || $3 > 9)
				nferr("subscript out of bounds");

			$$ = $3;
		}
	|	nvart				%prec HIGHER
		{
			PULL;
			$$ = 0;
		}
	;

nvart	:	'#'
		{
			PUSH;
		}
	;

svar	:	svart NUM_T			%prec HIGHER
		{
			PULL;
			/* check for error */
			if($2 < 0 || $2 > 9)
				nferr("subscript out of bounds");

			/* get correct variable */
			$$ = $2;
		}
	|	svart '(' nexp ')'		%prec HIGHER
		{
			PULL;
			if($3 < 0 || $3 > 9)
				nferr("subscript out of bounds");

			$$ = $3;
		}
	|	svart				%prec HIGHER
		{
			PULL;
			$$ = 0;
		}
	;

svart	:	'$'
		{
			PUSH;
		}
	;
