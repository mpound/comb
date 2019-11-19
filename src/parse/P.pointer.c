# include "P.tree.h"
#include "../main/C.h"
#if COMB
#include "../scan/C.h"
#endif /*COMB*/
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.extern.h"

	/****************************************/
	/* ex_label - extract label from string */
	/****************************************/

char *ex_label(string)
	char *string;			/* string from which to extract label */
{
	static char label[50];		/* label buffer */
	char *lbl_pnt;			/* pointer to label buffer */

	/* extract label */
	lbl_pnt = label;
	while(*string == ' ' || *string == '	')string++;
	while(*string != ':')*lbl_pnt++ = *string++;
	*lbl_pnt = 0;
	return(label);
}

	/*******************************************/
	/* search - find a node with a given label */
	/*******************************************/

Node *search(node,label)
	Node *node;			/* node from which to start search */
	char *label;			/* label to search for */
{
	Node *child;			/* node being checked */
	int branch;			/* branch of child */

	/* search until node == root */
	while(node != root)
	{

		/* set node to closest normal ancestor */
		/* this is done because all exclusive nodes */
		/* are expanded */
		do
			node = PARENT(node);
		while(node->type == EXCLSV);

		/* search children of node */
		branch = 0;
		while(child = CHILD(node,branch++))
			if(child = checknode(child,label))
				return(child);
	}

	/* if not found before node == root, check root */
	if(checknode(root,label))
		return(root);

	/* if the root isn't it, then return error */
	return(0);
}


	/*******************************************************************/
	/* checknode - check node against label, expanding exclusive nodes */
	/*******************************************************************/

Node *checknode(node,label)
	Node *node;			/* node to be checked */
	char *label;			/* label to check it against */
{
	Node *child;			/* child of exclusive node being */
					/* checked */
	int branch = 0;			/* branch of child */


	/* if node is exclusive, check its children */
	if(node->type == EXCLSV)
	{
		while(child = CHILD(node,branch++))
			if(child = checknode(child,label))
				return(child);

		/* return error if not found */
		return(0);
	}

	/* otherwise, check node against label */
	if(!strcmp(node->label,label))
	{
		/* set exclusive branches */
		tagex(node);

		/* if node is a leaf, and it is either a FLAG or */
		/* TOGGLE default type, set its default value */
		if(node->type == LEAF)
		{
			if(DF_TYPE(node) == FLAG && L_IS_PARAM(node))
				L_PARAM(int,node) = 1;
			if(DF_TYPE(node) == TOGGLE && L_IS_PARAM(node))
				L_PARAM(int,node) ^= 1;
		}

		/* return node */
		return(node);
	}

	/* if not label, return error */
	return(0);
}

	/***************************************/
	/* next - return node one to the right */
	/***************************************/

Node *next(node)
	Node *node;			/* node one to the left of return */
{
	Node *tnode;			/* tentative brother of node */
	int branch;			/* branch of node */

	/* climb up tree until node has a brother to the right */
	do
	{

		/* if node is the root, then this */
		/* means that the origional argument is */
		/* the right most leaf on the tree. */
		/* set the tentative brother to the root */
		if(node == root)
		{
			tnode = root;

			/* and get out of loop */
			/* (when descend is taken of tnode, */
			/* it will return the left most leaf */
			/* on the tree) */
			break;
		}

		/* skip over exclusive nodes */
		do
		{
			branch = node->branch;
			node = PARENT(node);
		}
		while(node->type == EXCLSV);

	}
	while(!(tnode = CHILD(node,branch + 1)));

	/* return left most descendent of right brother of node */
	return(descend(tnode));
}

	/*************************************************************/
	/* param - return next leaf with a parameter if there is one */
	/*************************************************************/

Node *param(node)
	Node *node;			/* node to start from */
{
	int flag = 0;			/* flag for whether or not the */
					/* whole tree has been searched */
					/* at least once */
	Node *left;			/* pointer to the left most leaf */
	Node *search_node;		/* leaf being searched */

	/* set left and search for parameter */
	left = descend(root);
	search_node = node;
	while(!L_IS_PARAM(search_node))
	{
		search_node = next(search_node);

		/* if search_node is equal to left, then we have */
		/* searched the tree from node to the right */
		/* most leaf. */
		if(search_node == left)
		{

			/* if flag is not set, then set it */
			if(!flag)
				flag = 1;

			/* otherwise, we have been here before, */
			/* and must have searched the whole tree. */
			/* return original value of node */
			else
				return(node);
		}
	}

	/* found one - return it */
	return(search_node);
}

	/*******************************************************/
	/* descend - return the left most descendent of a node */
	/*******************************************************/

Node *descend(node)
	Node *node;			/* node to find descendent of */
{
	/* find descendent */
	while(node->type != LEAF)
		node = CHILD(node,0);
	return(node);
}
