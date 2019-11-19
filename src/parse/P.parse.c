# include "P.tree.h"
#include "../main/C.h"
# include "../main/dragon.h"
#if COMB
# include "../scan/C.h"
#endif /*COMB*/
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.global.h"
#include <stdio.h>

	/*********************************/
	/* parse_ - main parsing routine */
	/*********************************/

void parse_(str_arg,tree_arg,pbuf_arg)
	char **str_arg;			/* string to be parsed */
	Node *tree_arg;			/* tree by which to parse it */
	char *pbuf_arg;			/* location of parameter buffer */
{
	int quote_fl;			/* flag set between quotes */
	char quote;			/* type of quote */
	int brackcnt;			/* squiggly bracket counter */
	char a_strbuf[NTTYLEN+1];	/* actual string buffer */
	char docfile[100];		/* documentation file name */

	/* set global pointers to arguments */
	str = *str_arg;
	tree = tree_arg;
	pbuf = pbuf_arg;

	/* point str_arg to end of command */
	quote_fl = 0;
	brackcnt = 0;
	while((**str_arg != ';' || quote_fl) && **str_arg)
	{

		/* check for quotation marks */
		if((**str_arg == '"' || **str_arg == '\'')
		   && (*str_arg)[-1] != '\\' && !brackcnt)
		{

			/* quote_fl set between quotes */
			if(quote_fl)
			{
				if(**str_arg == quote)
					quote_fl = 0;
			}
			else
			{
				quote_fl = 1;
				quote = **str_arg;
			}
		}

		/* check for brackets */
		if(**str_arg == '{' && (*str_arg)[-1] != '\\' && quote_fl != 1)
		{
			brackcnt++;
			quote_fl = 2;
		}
		if(**str_arg == '}' && (*str_arg)[-1] != '\\' && quote_fl != 1)
		{
			if(!--brackcnt)
				quote_fl = 0;
		}

		/* go on to next character */
		(*str_arg)++;
	}
	if(**str_arg == ';')
		(*str_arg)++;

	/* pass leading spaces or tabs and first comma */
	while(*str == ' ' || *str == '\t')str++;
	if(*str == ',')
	{
		str++;
		while(*str == ' ' || *str == '\t')str++;
	}

	/* reset handhold flag */
	hhmode = 0;

	/* initialize error pointer */
	sterr = str;
	errflag = 1;

	/* initialize string buffer */
	strbuf = a_strbuf;

	/* set default flags */
	setup(tree);

	/* check for question mark option */
	if(*str == '?' && str[1] != '(')
	{

		/* ?? */
		if(*++str == '?')
			doctree();

		/* ?d */
		if(*str == 'd')
		{
			printf("%s - %s",tree->label,tree->des);
			sprintf(docfile,"%s/doc/online/%s.doc"
				,main_.comb,tree->label);
			if(access(docfile,4))
				printf(" *");
			printf("\n");
			error_(0);
		}

		/* ?! */
		if(*str == '!')
		{
			sprintf(docfile,"%s/doc/online/%s.doc"
				,main_.comb,tree->label);
			if(access(docfile,4))
				error_("No documentation");
			sprintf(docfile,"more %s/doc/online/%s.doc"
				,main_.comb,tree->label);
			eshell_(docfile);
			error_(0);
		}

		/* ? */
		else
		{
			while(*str == ' ' || *str == '	')str++;
			if(*str != ';' && *str)
				error_("'?' not understood");
			hhenter();
		}
	}

	/* if not question mark, then parse */
	else
	{

		/* if tree has no leaves and input is given, generate */
		/* error, if input isn't given, stop parsing */
		if(!CHILD(tree,0))
		{
			while(*str == ' ' || *str == '	')str++;
			if(*str != ';' && *str)
				error_("command takes no arguments");
			return;
		}

		/* initialize variables for current situation */
		root = tree;
		nnode = cnode = param(descend(root));
		spor = 0;

		/* initialize root stack */
		rstkpnt = rootstack;

		/* initialize lex */
		unspt = unstk;

		/* parse */
		yyparse();

		/* evaluate defaults */
		defaults(tree,1);
	}
}

	/**********************************/
	/* yywrap - dummy routine for lex */
	/**********************************/

yywrap()
{
	return(1);
}
