%{
# include <string.h>
# include <stdio.h>
# include "../P.tree.h"
# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"

	typedef char *YYSTYPE;
%}

%token STRNG_T EX_T FL_T EXT_T LST_T RNG_T INT_T REAL_T SCAN_T STR_T CMD_T EXP_T
%token AUTO_T SET_T OLD_T FLAG_T TOGGLE_T PARS_T

%%

tredes	:	tree
		{
			stpars = "str";
		}
	|	PARS_T '(' STRNG_T ')' tree
		{
			stpars = (char *)($3);
		}
	;

tree	:	
	|	tree node
	|	tree '{'
		{
			/* start children */
			stkpnt++;
			stkpnt->branch = 0;
			stkpnt->parent = cnode - 1;
		}
	|	tree '}'
		{
			/* end children */
			stkpnt--;
		}
	;

node	:	STRNG_T STRNG_T ';'
		{
			wtree[cnode].typ = NORMAL;
			strcpy(wtree[cnode].des,(char *)$1);
			strcpy(wtree[cnode].lbl,(char *)$2);
			family(cnode);
			cnode++;
			free((void *)$1);
			free((void *)$2);
		}
	|	STRNG_T ':' exops ';'
		{
			wtree[cnode].typ = EXCLSV;
			strcpy(wtree[cnode].des,(char *)$1);
			family(cnode);
			cnode++;
			free((void *)$1);
		}
	|	STRNG_T STRNG_T ':' leafops ';'
		{
			wtree[cnode].typ = LEAF;
			strcpy(wtree[cnode].des,(char *)$1);
			strcpy(wtree[cnode].lbl,(char *)$2);
			family(cnode);
			cnode++;
			free((void *)$1);
			free((void *)$2);
		}
	;

exops	:	EX_T '(' STRNG_T ')'
		{
			wtree[cnode].fvr = 1;
			wtree[cnode].var = variable($3,SHORT);
			free((void *)$3);
		}
	|	FL_T '(' STRNG_T ')' EX_T '(' STRNG_T ')'
		{
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable($3,SHORT);
			wtree[cnode].fvr = 1;
			wtree[cnode].var = variable($7,SHORT);
			free((void *)$3);
			free((void *)$7);
		}
	|	EX_T '(' STRNG_T ')' FL_T '(' STRNG_T ')'
		{
			wtree[cnode].fvr = 1;
			wtree[cnode].var = variable($3,SHORT);
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable($7,SHORT);
			free((void *)$3);
			free((void *)$7);
		}
	;

leafops	:	
	|	type '-' STRNG_T
		{
			char *tch;
			wtree[cnode].dft = ILLEGAL;
			wtree[cnode].fvr = 1;
			avars(cnode,$3);
			var[(int)($1)][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)$3,0)) = 't';
			wtree[cnode].tfv = variable($3,SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable($3,SHORT);
			}
			free((void *)$3);
		}
	|	type deftype '-' STRNG_T
		{
			char *tch;
			wtree[cnode].fvr = 1;
			avars(cnode,$4);
			var[(int)($1)][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)$4,0)) = 't';
			wtree[cnode].tfv = variable($4,SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable($4,SHORT);
			}
			free((void *)$4);
		}
	|	type FL_T '(' STRNG_T ')' '-' STRNG_T
		{
			char *tch;
			wtree[cnode].dft = ILLEGAL;
			wtree[cnode].fvr = 1;
			avars(cnode,$7);
			var[(int)($1)][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)$7,0)) = 't';
			wtree[cnode].tfv = variable($7,SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable($7,SHORT);
			}
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable($4,SHORT);
			free((void *)$4);
			free((void *)$7);
		}
	|	type FL_T '(' STRNG_T ')' deftype '-' STRNG_T
		{
			char *tch;
			wtree[cnode].fvr = 1;
			avars(cnode,$8);
			var[(int)($1)][wtree[cnode].var].rng = wtree[cnode].rng;
			*(tch = strchr((char *)$8,0)) = 't';
			wtree[cnode].tfv = variable($8,SHORT);
			if(wtree[cnode].lln)
			{
				*tch = 'e';
				wtree[cnode].efv = variable($8,SHORT);
			}
			wtree[cnode].ffl = 1;
			wtree[cnode].flg = variable($4,SHORT);
			free((void *)$4);
			free((void *)$8);
		}
	;

type	:	atype
		{
			$$ = da_ecf[(int)($1)];
		}
	|	EXT_T '(' STRNG_T STRNG_T ')' atype
		{
			$$ = da_ecf[(int)($6)];
			wtree[cnode].ext = 1;
			strcpy(wtree[cnode].evr,(char *)$3);
			strcpy(wtree[cnode].eds,(char *)$4);
			free((void *)$3);
			free((void *)$4);
		}
	;

atype	:	rngtype
	|	multype
	|	types
		{
			wtree[cnode].dtp = $1;
		}
	|	rngtype LST_T '(' STRNG_T ')'
		{
			wtree[cnode].lln = atoi((char *)$4);
			free((void *)$4);
		}
	|	types LST_T '(' STRNG_T ')'
		{
			wtree[cnode].dtp = $1;
			wtree[cnode].lln = atoi((char *)$4);
			free((void *)$4);
		}
	;

rngtype	:	ntype RNG_T
		{
			wtree[cnode].dtp = (int)($1);
			wtree[cnode].rng = 1;
		}
	;

ntype	:	INT_T
		{
			$$ = INTN;
			wtree[cnode].sdt = INTN;
		}
	|	REAL_T
		{
			$$ = REAL;
			wtree[cnode].sdt = REAL;
		}
	|	SCAN_T
		{
			$$ = SCAN;
			wtree[cnode].sdt = SCAN;
		}
	;

multype	:	types '/' types
		{
			wtree[cnode].dtp = (int)($1) | (int)($3);
		}
	|	multype '/' types
		{
			wtree[cnode].dtp |= (int)($3);
		}
	;

types	:	ntype
	|	STR_T '(' STRNG_T ')'
		{
			$$ = STRING;
			wtree[cnode].sdt = STRING;
			wtree[cnode].chl = atoi((char *)$3);
			free((void *)$3);
		}
	|	CMD_T '(' STRNG_T ')'
		{
			$$ = CMND;
			wtree[cnode].sdt = CMND;
			wtree[cnode].chl = atoi((char *)$3);
			free((void *)$3);
		}
	|	EXP_T '(' STRNG_T ')'
		{
			$$ = EXPR;
			wtree[cnode].sdt = EXPR;
			wtree[cnode].chl = atoi((char *)$3);
			free((void *)$3);
		}
	;

deftype	:	AUTO_T
		{
			wtree[cnode].dft = AUTO;
		}
	|	AUTO_T '(' STRNG_T ')'
		{
			wtree[cnode].dft = AUTOFUN;
			strcpy(wtree[cnode].dfv,(char *)$3);
			free((void *)$3);
		}
	|	SET_T '(' STRNG_T ')'
		{
			wtree[cnode].dft = SET;
			strcpy(wtree[cnode].dfv,(char *)$3);
			free((void *)$3);
		}
	|	OLD_T
		{
			if(!wtree[cnode].ext && wtree[cnode].sdt != SCAN)
				enterr("no initial value");
			wtree[cnode].dft = OLD;
		}
	|	OLD_T '(' STRNG_T ')'
		{
			if(wtree[cnode].ext)
				enterr("initial value for external variable");
			if(wtree[cnode].sdt == SCAN)
				enterr("initial value for scan number");
			wtree[cnode].dft = OLD;
			strcpy(wtree[cnode].dfv,(char *)$3);
			free((void *)$3);
		}
	|	FLAG_T
		{
			if(wtree[cnode].dtp != INTN)
				enterr("flag default type for non-integer");
			wtree[cnode].dft = FLAG;
		}
	|	TOGGLE_T
		{
			if(!wtree[cnode].ext)
				enterr("no starting value for toggle");
			if(wtree[cnode].dtp != INTN)
				enterr("toggle default type for non-integer");
			wtree[cnode].dft = TOGGLE;
		}
	|	TOGGLE_T '(' STRNG_T ')'
		{
			if(wtree[cnode].ext)
				enterr("starting value for external");
			if(wtree[cnode].dtp != INTN)
				enterr("toggle default type for non-integer");
			wtree[cnode].dft = TOGGLE;
			if(strcmp((char *)$3,"yes"))
			{
				if(strcmp((char *)$3,"no"))
					enterr("non-boolean value for toggle");
				strcpy(wtree[cnode].dfv,"0");
			}
			else
				strcpy(wtree[cnode].dfv,"1");
		}
	;
