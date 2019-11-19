# include "P.tree.h"
#include "../main/C.h"
#if COMB
#include "../scan/C.h"
#endif /*COMB*/
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.extern.h"
#include <stdio.h>
#include <string.h>

	/******************************************************/
	/* doctree - print full description of tree structure */
	/******************************************************/

doctree()
{
	extern Node *tree;
	int paging;

	paging = PageStdout();
	pchildren(0,tree);
	if(paging)
		ResetStdout();
	error_(0);
}

	/****************************************************************/
	/* pchildren - print the description of a node and its children */
	/****************************************************************/

pchildren(indent,node)
	int indent;			/* number of spaces to indent */
	Node *node;			/* node to be printed */
{
	Node *child;			/* child being printed */
	int branch = 0;			/* branch of child being printed */

	/* print description of node */
	prnode(indent,node);
	printf("\n");

	/* if node is not a leaf, print its children */
	if(node->type != LEAF)
		while(child = CHILD(node,branch++))
			pchildren(indent + 2,child);
}

	/*****************************************/
	/* hhenter - enter tree in handhold mode */
	/*****************************************/

hhenter()
{
	hhstruct handhold();			/* routine for entering nodes */
	hhstruct humancmd;			/* humanized command */
	FILE *fpjunk;

	/* set handhold flag */
	hhmode = 1;

	/* enter tree */
	humancmd = handhold(tree,0);

	/* set defaults */
	defaults(tree,0);

	/* print humanized command */
	printf("\nYou could have typed:\n");

	/* account for the fact that the first character in the humanized */
	/* argument string is sometimes a space */
	if(*humancmd.cmd == ' ')
		printf("%s%s\n",tree->label,humancmd.cmd);
	else
		printf("%s %s\n",tree->label,humancmd.cmd);

#if HISTORY
	/* fix up $HOME/.history file if possible */
	fix_hist(tree->label,humancmd.cmd);
#endif /*HISTORY*/
}

	/************************************/
	/* handhold - execute handhold mode */
	/************************************/

hhstruct handhold(node,indent)
	Node *node;				/* node to be entered */
	int indent;				/* number of spaces to indent */
{
	Node *child;				/* child being entered */
	int branch = 0;				/* branch of child */
	char answer[100];			/* buffer for answers */
	int cnt;				/* counter for indenting */
	hhstruct handhold();			/* routine itself */
/* this might possibly cause a problem on hpux on return from longjmp.  The
 * other automatic variables aren't expected to retain anything. */
	hhstruct ret;				/* humanized command return */
	hhstruct chldret;			/* structure returned by child */
	int dfltd;				/* whether or not a leaf */
						/* defaulted */
	int tmp;				/* old value of leaf */
	char ts[200];				/* temporary string */

	/* initialize ret */
	*ret.cmd = 0;

	/* choose correct execution based on node type */
	switch(node->type)
	{

		/* normal node */
		case NORMAL:
		{

			hhstruct cret[20];	/* record of entries for */
						/* children */
			int children[20];	/* list of children entered */
			int entrd;		/* number of children entered */
			int nentrd;		/* number of children not */
						/* entered in a row */


			/* print node and enter all children in */
			/* handhold mode */
			printf("\n");
			prnode(indent,node);
			printf("\n");
			nentrd = 0;
			entrd = 0;
			children[entrd] = 0;
			while(child = CHILD(node,branch++))
			{
				chldret = handhold(child,indent + 2);

				/* record entry for construction of humanized */
				/* command */
				if(chldret.entered)
				{
					/* count number of nodes entered */
					entrd++;

					/* store entry */
					cret[entrd] = chldret;
					children[entrd] = branch - 1;
				}
			}

			/* construct humanized command */

			/* decide whether node will be labeled and entered */
			if(entrd)
			{
				ret.labeled = 1;
				ret.entered = 1;
			}
			else
			{
				ret.labeled = 0;
				ret.entered = 0;
			}

			/* if more than one leaf was entered, do full */
			/* entry format */
			if(entrd > 1)
			{

				/* put all entries inside square brackets */
				/* if this node is not the root */
				if(node != tree)
					sprintf(ret.cmd," %s:[",node->label);

				/* write entries into ret.cmd */
				for(branch = 1;branch <= entrd;branch++)
				{

					/* if child is not labeled, get to it */
					/* somehow */
					if(!cret[branch].labeled)
					{

					/* if few enough children were left */
					/* out, get to the next child entered */
					/* by commas */
					if((nentrd = children[branch] -
					    children[branch - 1]) <= 3 &&
					    allleaves(children[branch - 1],
						      children[branch],node))
						while(nentrd--)
							strcat(ret.cmd,",");

					/* otherwise, get there by label */
					else
					{
						strcat(ret.cmd," ");
						strcat(ret.cmd,CHILD(node,
						       children[branch])->label
						       );
						strcat(ret.cmd,":");
					}
					}

					/* write entry into ret.cmd */
					strcat(ret.cmd,cret[branch].cmd);
				}

				/* if node is not root, close square */
				/* bracket */
				if(node != tree)
					strcat(ret.cmd,"]");
			}

			/* if only one child entered, just give label */
			else if(entrd)
			{

				/* if node is not root, give node's label */
				if(node != tree)
				{
					if(!cret[1].labeled)
						sprintf(ret.cmd," %s:%s:%s",
							node->label,
							CHILD(node,children[1])->label,
							cret[1].cmd);
					else
						sprintf(ret.cmd," %s:%s",
							node->label,
							cret[1].cmd);
				}

				/* if node is root, don't give node's label */
				else
				{
					if(!cret[1].labeled)
						sprintf(ret.cmd," %s:%s",
							CHILD(node,children[1])->label,
							cret[1].cmd);
					else
						strcpy(ret.cmd,cret[1].cmd);
				}
			}

			printf("\n");
			break;
		}

		/* exclusive node */
		case EXCLSV:

			/* print node */
			printf("\n");
			prnode(indent,node);
			printf("\n");

			/* tell the user to select one option */
			for(cnt = 0;cnt < indent;cnt++)
				printf(" ");
			printf(" select one of the following;\n");

			/* print out the options */
			while(child = CHILD(node,branch++))
				choicepr(indent + 2,child);

			/* print prompt */
			sterr = answer;
			setjmp(hherr);
			for(cnt = 0;cnt < indent;cnt++)
				printf(" ");
			printf(" Enter label of selection: ");

			/* enter label */
			fflush(stdout);
			if(fgets(answer,100,stdin) == NULL)
			{
				str = strchr(answer,'\n');
				nferr("string too long");
			}
			*strchr(answer,'\n') = 0;

			/* allow the user to type ":" at the end */
			if(strchr(answer,0)[-1] == ':')
				strchr(answer,0)[-1] = 0;

			/* find the label */
			if(!(child = checknode(node,answer)))
			{
				str = strchr(answer,0);
				nferr("not a valid choice");
			}
			tagex(child);

			/* enter child in handhold mode */
			chldret = handhold(child,indent + 2);
			printf("\n");

			/* construct humanized command */
			if(chldret.labeled)
				sprintf(ret.cmd,"%s",chldret.cmd);
			else
				sprintf(ret.cmd," %s:%s",answer,chldret.cmd);
			ret.entered = 1;
			ret.labeled = 1;
			break;

		/* leaf */
		case LEAF:

			/* don't do anything if leaf is not a parameter */
			if(!L_IS_PARAM(node))
			{
				/* set up humanized command */
				ret.entered = 0;
				ret.labeled = 0;
				break;
			}

			/* print leaf as a prompt */
			sterr = answer;
			setjmp(hherr);
			for(cnt = 0;cnt < indent;cnt++)
				printf(" ");
			printf("Enter ");
			prnode(0,node);
			CNT(node) = 0;
			printf(": ");

			/* enter value */
			if(DF_TYPE(node) == TOGGLE)
				tmp = L_PARAM(int,node);
			fflush(stdout);
			if(fgets(answer,100,stdin) == NULL)
			{
				str = strchr(answer,'\n');
				nferr("value too long");
			}
			*strchr(answer,'\n') = 0;

			/* check to see if answer is default */
			str = answer;
			while(*str == ' ' || *str == '	' || *str == ',')
				str++;
			if(!*str)
			{
				if(DF_TYPE(node) == ILLEGAL)
					nferr("may not let parameter default");
				dfltd = 1;
			}
			else
				dfltd = 0;

			/* parse entered value */
			root = node;
			cnode = nnode = node;
			unspt = unstk;
			errflag = 1;
			rstkpnt = rootstack;
			yyparse();

			/* construct humanized command */
			if(dfltd)
			{
				ret.entered = 0;
				ret.labeled = 0;
			}
			else
			{

				/* different outputs for different default */
				/* types */
				switch(DF_TYPE(node))
				{
					/* toggles, give label if changed */
					case TOGGLE:
						if((L_PARAM(int,node) && !tmp) ||
						   (!L_PARAM(int,node) && tmp))
						{
							sprintf(ret.cmd,
								" %s:",
								node->label);
							ret.entered = 1;
							ret.labeled = 1;
						}
						else
						{
							ret.entered = 0;
							ret.labeled = 0;
						}
						break;

					/* flags, give label if yes */
					case FLAG:
						if(L_PARAM(int,node))
						{
							sprintf(ret.cmd,
								" %s:",
								node->label);
							ret.entered = 1;
							ret.labeled = 1;
						}
						else
						{
							ret.entered = 0;
							ret.labeled = 0;
						}
						break;

					/* everything else, use answer */
					default:
						strcpy(ret.cmd,answer);
						ret.entered = 1;
						ret.labeled = 0;
				}

				/* if leaf is a list, enclose it in brackets */
				if(LEN(node) && CNT(node) > 1)
				{
					strcpy(ts,ret.cmd);
					sprintf(ret.cmd,"%s:[%s]",node->label,ts);
					ret.labeled = 1;
				}
			}
	}

	/* return humanized command */
	return(ret);
}

	/*****************************************************/
	/* allleaves - check that all of the children of one */
	/*	       node between two given children are   */
	/*	       leaves.				     */
	/*****************************************************/

allleaves(from,to,node)
	int from,to;		/* children to be checked */
	Node *node;		/* node to check them in */
{
	int branch;		/* branch of child being checked */

	/* check children */
	for(branch = from;branch < to;branch++)
		if(CHILD(node,branch)->type != LEAF)
			return(0);

	/* if all clear, return 1 */
	return(1);
}

	/*************************************************/
	/* choicepr - print choices under exclusive node */
	/*************************************************/

choicepr(indent,node)
	int indent;				/* number of spaces to indent */
	Node *node;				/* choice to be printed */
{
	Node *child;				/* child of exclusive node */
	int branch = 0;				/* branch of child */

	/* if node is exclusive, print its children */
	if(node->type == EXCLSV)
	{
		prnode(indent,node);
		printf("\n");
		while(child = CHILD(node,branch++))
			choicepr(indent + 2,node);
	}

	/* otherwise, print node as choice */
	else
	{
		while(indent--)
			printf(" ");
		printf("(%s) %s\n",node->label,node->des);
	}
}

	/***********************************/
	/* prnode - print node description */
	/***********************************/

prnode(indent,node)
	int indent;			/* number of spaces to indent */
	Node *node;			/* node to be printed */
{
	/* indent correct number of spaces */
	while(indent--)
		printf(" ");

	/* print programmer's description of node */
	printf("%s ",node->des);

	/* if node is external, print message to that effect */
	/* with programmer's description of external variable */
	if(node->type == LEAF)
		if(IS_EXTERN(node))
			printf("(external: %s) ",EXTERN(node));

	/* print label if node is not exclusive, "(exclusive)" if */
	/* it is */
	if(node->type != EXCLSV)
		printf("\"%s\"",node->label);
	else
		printf("(exclusive)");

	/* if node is leaf, print leaf options that pertain */
	if(node->type == LEAF)

		/* there is only usefull information about a leaf */
		/* if it is a parameter, therefore, print information */
		/* only if leaf is a parameter */
		if(L_IS_PARAM(node))
		{

			/* print data type(s) */
			printf(" - ");
			if(DF_TYPE(node) != FLAG && DF_TYPE(node) != TOGGLE)
				printf("%s",da_type[DA_TYPE(node)]);

			/* print sequence if not single */
			if(SEQ(node) != SINGLE)
				printf(" %s",seq[SEQ(node)]);

			/* print list if node is a list */
			if(LEN(node))
				printf(" LIST");

			/* print default if not illegal */
			if(DF_TYPE(node) != ILLEGAL)
			{
				if(DF_TYPE(node) != FLAG &&
				   DF_TYPE(node) != TOGGLE)
					printf(", ");
				printf("%s",df_type[DF_TYPE(node)]);
			}

			/* print value if appropriate given the */
			/* default type */
			switch(DF_TYPE(node))
			{

				/* old, set, and autofunction defaults */
				case OLD:
				case SET:
				case AUTOFUN:
					printf("(");
					prval(node);
					printf(")");
					break;

				/* flag and toggle defaults */
				case FLAG:
				case TOGGLE:
					if(L_PARAM(int,node))
						printf("(yes)");
					else
						printf("(no)");
			}
		}
}

	/*********************************************/
	/* prval - print the current value of a node */
	/*********************************************/

prval(node)
	Node *node;			/* node whose value is to be printed */
{
	/* print all values */
	do
	{

		/* select correct data type */
		switch(USE_TYPE(node))
		{
	
			/* character string */
			/* CNT * CLEN is added to account for */
			/* string lists, CNT is subtracted because */
			/* L_PARAM adds CNT * sizeof(char) */
			case CMND:
			case EXPR:
			case STRING:
				printf("\"%s\"",&L_PARAM(char,node) +
					CNT(node) * (CLEN(node) - 1));
				break;
	
#if COMB
			/* scan number */
			case SCAN:
				printf("%.3s%d",L_PARAM(Scan,node).fil,
				       L_PARAM(Scan,node).num);
				if(SEQ(node) == RANGE)
					printf("_%.3s%d",L_PARAM2(Scan,node).fil,
					       L_PARAM2(Scan,node).num);
				break;
#endif /*COMB*/
	
			/* integer */
			case INTN:
				printf("%d",L_PARAM(int,node));
				if(SEQ(node) == RANGE)
					printf("_%d",L_PARAM2(int,node));
				break;
	
			/* real */
			case REAL:
				printf("%g",L_PARAM(double,node));
				if(SEQ(node) == RANGE)
					printf("_%g",L_PARAM2(double,node));
		}

		/* continue only if node is list and has OLD */
		/* defualt type */
		if(!LEN(node) || DF_TYPE(node) != OLD)
			break;

		/* print comma and continue if all node's values */
		/* have not been printed */
		if(++CNT(node) < ELMNTS(node))
			printf(", ");
		else
			break;
	}
	while(1);
}
