# define VTST printf(". . . VALUE OF FIRST LEAF IN TREE: %g\n",L_PARAM(double,param(descend(root))));
# define VTPR printf("\nparam = %x, descend = %x, root = %x   CNT(mt1.n) = %d ,SEQ(mt1.n) =%d ",param(descend(root)),descend(root),root,CNT(mt1.n),SEQ(mt1.n));
# define VTPC printf("\n*param = %d, *descend = %d, *root = %d    ",*param(descend(root)),*descend(root),*root);
# define PRTNODE printf("\n Printout of values for node:\n\tdes = %s, label = %s, type = %d, parent = %d, branch = %d, info[1]= %d\n",node->des,node->label,node->type,node->parent,node->branch,node->info[1]);

# include "P.tree.h"
# include "../main/C.h"

#if COMB
#include "../scan/C.h"
#endif /*COMB*/

# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.extern.h"
#include <stdio.h>

	/***********************************************************/
	/* setup - initialize a node and its children for defaults */
	/***********************************************************/

setup(node)
	Node *node;			/* node to be initialized */
{
	Node *child;			/* child being initialized */
	int branch = 0;			/* branch of child */

	/* initialize on the basis of node type */
	switch(node->type)
	{

		/* exclusive nodes - set flag to 1 and initialize children */
		case EXCLSV:
			if(E_IS_FLAG(node))
				E_FLAG(node) = 1;

		/* normal - initialize children */
		case NORMAL:
			while(child = CHILD(node,branch++))
				setup(child);
			break;

		/* leaf - if it is a parameter, and the default type is */
		/*        flag, set it to 0.  set the defaulted flag to */
		/*	  1 (defaulted so far).  set CNT to 0 (no elements) */
		case LEAF:
			if(L_IS_PARAM(node))
			{
				if(DF_TYPE(node) == FLAG)
					L_PARAM(int,node) = 0;
#if COMB
				if(USE_TYPE(node) == SCAN &&
				   (DF_TYPE(node) == OLD ||
				   DF_TYPE(node) == AUTO))
					L_PARAM(Scan,node) = curscn_;
#endif /*COMB*/
			}
			DFLTD(node) = 1;
			CNT(node) = 0;
	}
}

	/************************************************/
	/* defaults - find default values after parsing */
	/************************************************/

defaults(node,illflag)
	Node *node;			/* node for which to find defaults */
	int illflag;			/* flag for whether or not ILLEGAL */
					/* default types may not default */
{
	Node *child;			/* child of node for which defaults */
					/* are being found */
	int branch = 0;			/* branch of child */

	/* find default on the basis of node type */
	switch(node->type)
	{

		/* exclusive - find defaults of all children, letting */
		/*	       ILLEGALs default except under branch */
		/*	       used */
		case EXCLSV:
			while(child = CHILD(node,branch))
				if(branch++ == E_PARAM(node))
					defaults(child,illflag);
				else
					defaults(child,0);
			break;

		/* normal - find defaults of all children */
		case NORMAL:
			while(child = CHILD(node,branch++))
				defaults(child,illflag);
			break;

		/* leaf */
		case LEAF:

			/* the following only pertains to parameters */
			/* (it is not indented because that would make */
			/* the terminal overflow) */
			if(L_IS_PARAM(node))
			{

			/* if leaf defaulted, default type is ILLEGAL, */
			/* and ILLEGALs may not default, then print */
			/* error */
			if(DFLTD(node) && DF_TYPE(node) == ILLEGAL && illflag)
			{
				fprintf(stderr,"Error: ");
				prnode(0,node);
				error_(" may not default");
			}

			/* if leaf is a list, and (it did not default, */
			/* or it is not an old default type) set number */
			/* of elements to CNT+1 */
			if(LEN(node) && (!DFLTD(node)
			   || DF_TYPE(node) != OLD))
				ELMNTS(node) = CNT(node);

			/* the following pertains to all leaves */
			}

			/* if the leaf has a flag, set it to DFLTD */
			if(L_IS_FLAG(node))
				L_FLAG(node) = DFLTD(node);
	}
}

	/***************************************************/
	/* tag - set default flags when parameter is given */
	/***************************************************/

tag(node)
	Node *node;			/* node for which parameter was given */
{
	/* set correct branch numbers of exclusive ancestors */
	tagex(node);

	/* reset defaulted flag */
	DFLTD(node) = 0;

	/* if node is a list, increment the count */
	if(LEN(node) && CNT(node) != LEN(node))
		(CNT(node))++;
}

	/*************************************************************/
	/* tagex - set correct branch numbers of exclusive ancestors */
	/*************************************************************/

tagex(node)
	Node *node;			/* node to set ancestors of */
{
	int branch;			/* branch from which node is */
					/* descended */

	/* find ancestors */
	do
	{
		branch = node->branch;
		node = tree + node->parent;

		/* if ancestor is exclusive, set branch number */
		if(node->type == EXCLSV)
		{
			E_PARAM(node) = branch;
			/* if node has a flag, reset it */
			if(E_IS_FLAG(node))
				E_FLAG(node) = 0;
		}
	}
	while(node != tree);
}
