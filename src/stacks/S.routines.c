# include "S.struct.h"
# include "S.constant.h"
# include <stdio.h>

static int searched;

/* global variables (used instead of passing parameters for speed) */
static int record[FIELDS];
static int (*procedure)();
static int compField;		/* field to be compared with */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* S.routines.c */
static int SearchNode P_((register NODE *node, int *low, int *high, EDGEFLAG *use));
static int FieldCompare P_((int *data, register NODE *node));
static int FirstBitSet P_((register unsigned number));
#undef P_

/* Struct for passing index to search and store */
struct SRCHINDX {int v[FIELDS];};

#include "T.decl.h"

/**********************************/
/** EXTERNALLY CALLABLE ROUTINES **/
/**********************************/

/************************************************************/
/* Screat - create a search file and return a pointer to it */
/************************************************************/

SFILE Screat(filName,permission)
	char *filName;			/* name of search file */
	int permission;			/* permission used in creat */
{
	TFILE Tcreate();		/* routine for creating */
					/* tree (search) file */

	/* create file */
	return((SFILE)(Tcreate(filName,LH,permission)));
}

/******************************************************/
/* Sopen - open a search file for reading and writing */
/******************************************************/

SFILE Sopen(filName, readOnly)
	char *filName;			/* name of file to open */
	int readOnly;
{
	TFILE Topen();			/* routine for opening tree (search) */
					/* file */

	/* open file */
	return((SFILE)(Topen(filName, readOnly)));
}

/*******************************************************
 * Sclean - control automatic writing of sfile buffers *
 *******************************************************/

void Sclean(sfile, clean)
SFILE sfile;
int clean;
{
	/* clean < 0 for assuring that cleaning is turned on and cleaning
	up if cleaning was previously off */
	if(clean < 0 && ((TFILE)sfile)->clean)
		return;
	((TFILE)sfile)->clean = clean;
	if(clean)
		CleanUp( (TFILE)sfile );
}

/**************************************/
/* Sclose - close an open search file */
/**************************************/

void Sclose(sfile)
	SFILE sfile;			/* file to be closed */
{
	/* close file */
	Tclose(sfile);
}

/**************************************************/
/* Sstore - store a given record in a search file */
/**************************************************/

void Sstore(sfile,data)
	SFILE sfile;		/* file to store it in */
	struct SRCHINDX data; /* record to store */
{
	NODE nodeA;		/* node to be broken up */
	NODE nodeB;		/* node to contain new data */
	NODE nodeC;		/* node to contain rest of old data */
	int branchField;	/* field to break nodeA up in */
	int bitsIn;		/* bits into field section at which to make */
				/* break */
	int branchBit;		/* bit in branchField at which to make break */
	int length;		/* length of new field section */
	unsigned int field;	/* field being checked for break up */
	unsigned int test;	/* comparison between fieldSec and */
				/* nodeA.value */
	int firstBit;		/* first bit different in test */
	int newChild;		/* flag for whether new data goes in child1 */
	int bit;		/* bit for calling ibit */
	int child;		/* child to go to in search */
/*
fprintf(stderr,"Entering Sstore: lastNode = %d\n",sfile->head.lastNode);
fprintf(stderr,"v[] = %x, %x, %x, %x, %x\n",data.v[0], data.v[1], data.v[2],
	data.v[3], data.v[4] );
ThdrPrint(sfile);
*/

	if(!sfile->head.lastNode)
	{
		nodeA.branchField = -1;
		nodeA.bit = 0;
		nodeA.bits = 32;
		for(field = 0;field < FIELDS;field++)
			nodeA.value[field] = data.v[field];
		MakeRoot(sfile,&nodeA);
		return;
	}

	/* find node to store data in */

	/* search through nodes */
	Root(sfile,&nodeA);
/*fprintf(stderr,"Read root:\n"); */
	bit = nodeA.bit;
	bitsIn = -1;
	branchField = -1;
	while(bitsIn == -1)
	{
/*PrintNode(&nodeA);*/

		/* compare data with node */

		/* check all fields */
		for(field = 0;field < FIELDS;field++)
		{
			test = ((unsigned)data.v[field] >> nodeA.bit);
			test ^= nodeA.value[field];
			if(nodeA.bits < 32)
				test &= ~(-1 << nodeA.bits);
/* fprintf(stderr,"field %d, test = %x\n",field, test); */
			if(test)

				/* if different, see if difference is big */
				/* enough to warrent breaking in this field */
				if((firstBit = FirstBitSet(test)) > bitsIn)
				{
					bitsIn = firstBit;
					branchField = field;
				}
		}

		/* go on to next node if data in this one */
		if(bitsIn == -1)
		{

			/* if this node is a leaf, then the data is */
			/* already in file and Sstore should return */
			if(Leaf(nodeA)) {
/* fprintf(stderr,"Got to Leaf node\n"); */
				return;
			}

			/* otherwise, go on to next node */
			bit = nodeA.bit;
			child = ibit_(data.v + nodeA.branchField,&bit);
			Child(&nodeA,child,&nodeA);
/* fprintf(stderr,"Got new child:\n"); */
		}
	}

	/* make new nodes */
	length = nodeA.bits - bitsIn;
	branchBit = bitsIn + nodeA.bit;
/*
fprintf(stderr,"About to make new nodes: bitsIn = %d,length = %d, branchBit = %d\n",
bitsIn, length, branchBit);
*/

	/* make nodeC */
	nodeC = nodeA;
	nodeC.bits = bitsIn + 1;

	/* make nodeB */
	nodeB.branchField = -1;
	for(field = 0;field < FIELDS;field++)
		nodeB.value[field] = data.v[field];
	nodeB.bit = 0;
	nodeB.bits = branchBit + 1;

	/* make nodeA */
	nodeA.branchField = branchField;
	for(field = 0;field < FIELDS;field++)
		nodeA.value[field] >>= (bitsIn + 1);
	nodeA.bit = branchBit + 1;
	nodeA.bits = length - 1;
	bit = nodeA.bit;
	newChild = ibit_(data.v + nodeA.branchField,&bit);

	/* store nodes */
	if(newChild)
		MakeChildren(&nodeA,0,&nodeC,&nodeB);
	else
		MakeChildren(&nodeA,1,&nodeB,&nodeC);
}

/**********************************/
/* Ssearch - search a given range */
/**********************************/

void Ssearch(sfile,low,high,proc)
	SFILE sfile;		/* search file in which to search */
	int low[FIELDS];	/* low bounds */
	int high[FIELDS];	/* high bounds */
	int (*proc)();		/* procedure to call for each data point */
{
	EDGEFLAG use[FIELDS];	/* flags for whether or not to use bound */
	int field;		/* counter for setting use */
	NODE tempnode;

	/* return if file is empty */
	if(!sfile->head.lastNode)
		return;

	/* set use and zero record */
	for(field = 0;field < FIELDS;field++)
	{
		use[field].low = use[field].high = 1;
		record[field] = 0;
	}

	/* search */
	procedure = proc;
	searched = 0;
	Root(sfile,&tempnode);
	SearchNode(&tempnode,low,high,use);
}

/******************************************************/
/* Sremove - remove a given record from a search file */
/******************************************************/

void Sremove(sfile,data)
	SFILE sfile;		/* search file from which to remove it */
	struct SRCHINDX data; /* record to be removed */
{
	NODE doomed;		/* node doomed to be removed */
	NODE brother;		/* brother of doomed */
	NODE parent;		/* parent of doomed */
	int branchBit;		/* bit to branch on in finding doomed */
	int branch;		/* value of branchBit in data */
	int mask;		/* mask for splicing together parent */
				/* and brother */

	/* if file is empty, return */
	if(!sfile->head.lastNode)
		return;

	/* find doomed node */
	Root(sfile,&doomed);
	while(!Leaf(doomed))
	{
		for(compField = 0;compField < FIELDS;compField++)
			if(FieldCompare(data.v,&doomed) != EQ)
				return;
		branchBit = doomed.bit;
		branch = ibit_(data.v + doomed.branchField,&branchBit);
		parent = doomed;
		Child(&parent,branch,&doomed);
	}
	for(compField = 0;compField < FIELDS;compField++)
		if(FieldCompare(data.v,&doomed) != EQ)
			return;

	/* if the root is the only node, remove it */
	if(sfile->head.lastNode == 1)
		sfile->head.lastNode = 0;

	/* otherwise, get rid of doomed */
	else
	{
		Child(&parent,1 - branch,&brother);
		parent.branchField = brother.branchField;
		parent.bit = brother.bit;
		parent.bits += brother.bits;
		mask = ~(0xffffffff << brother.bits);
		for(compField = 0;compField < FIELDS;compField++)
			parent.value[compField] = (parent.value[compField] <<
					brother.bits) |
					(brother.value[compField] & mask);
		RemoveChildren(&parent);
	}
}

/********************/
/** LOCAL ROUTINES **/
/********************/

/*****************************************************************/
/* SearchNode - search a given node for records in a given range */
/*****************************************************************/

static int SearchNode(node,low,high,use)
	register NODE *node;	/* node to search */
	int *low;		/* low bounds */
	int *high;		/* high bounds */
	EDGEFLAG *use;		/* whether or not to use fields */
				/* in low and high */
{
	register int comparison; /* comparison between range and node */
	int branchBit;		/* bit into data on which branch is made */
	register int mask;	/* mask for constructing field */
	EDGEFLAG newUse[FIELDS];/* flags for recursive call */
	register EDGEFLAG *useP; /* pointer to use */
	register EDGEFLAG *newUseP; /* pointer to new use */
	NODE child;

	searched++;
	/* make sure node is in range */
	useP = use;
	newUseP = newUse;
	for(compField = 0;compField < FIELDS;compField++)
	{

		/* check low */
		if(useP->low)
		{
			comparison = FieldCompare(low,node);
			if(comparison == GT)
				return(0);
			if(comparison == LT)
				newUseP->low = 0;
			else
				newUseP->low = 1;
		}
		else
			newUseP->low = 0;

		/* check high */
		if(useP->high)
		{
			comparison = FieldCompare(high,node);
			if(comparison == LT)
				return(0);
			if(comparison == GT)
				newUseP->high = 0;
			else
				newUseP->high = 1;
		}
		else
			newUseP->high = 0;

		/* increment pointers */
		useP++;
		newUseP++;

		/* construct compField */
		mask = (~(-1 << node->bits)) << node->bit;
		record[compField] = (record[compField] & ~mask) |
				((node->value[compField] << node->bit) & mask);
	}

	/* if record completely found (ie. a leaf node), call procedure */
	if(!node->l.nextChild)
		return((*procedure)(record));

	/* otherwise, search sub nodes where appropriate */
	branchBit = node->bit + (node->branchField << 5);
	newUseP = newUse + node->branchField;
	if((!newUseP->low) || (!ibit_(low,&branchBit))) {
		Child(node,0,&child);
		if(SearchNode(&child,low,high,newUse))
			return(1);
	}
	if((!newUseP->high) || (ibit_(high,&branchBit))) {
		Child(node,1,&child);
		if(SearchNode(&child,low,high,newUse))
			return(1);
	}

	/* return */
	return(0);
}

/*****************************************************************/
/* FieldCompare - compare a give piece of data with a given node */
/*****************************************************************/

static int FieldCompare(data,node)
	int *data;		/* data to be compared */
	register NODE *node;	/* node to compare it with */
{
	int fieldSec;		/* field section from data */
	register int nodeFieldSec; /* field section from node */

	/* extract field sections */
	if(node->bits == 32)
		fieldSec = ((unsigned)data[compField]) >> node->bit;
	else
		fieldSec = (((unsigned)data[compField]) >> node->bit) &
			~(-1 << node->bits);
	nodeFieldSec = node->value[compField] & ~(0xffffffff << node->bits);

	/* do comparison */
	if(fieldSec < nodeFieldSec)
		return(LT);
	if(fieldSec > nodeFieldSec)
		return(GT);
	return(EQ);
}

/****************************************************/
/* FirstBitSet - find the highest set bit in a word */
/****************************************************/

static int FirstBitSet(number)
	register unsigned number;	/* word to find set bit in */
{
	register int bit;		/* counter for bit set */

	/* find bit */
	bit = 0;
	number >>= 1;
	while(number)
	{
		bit++;
		number >>= 1;
	}
	return(bit);
}
