# include "T.struct.h"
# include "C.h"
# include <stdio.h>
# include <fcntl.h>
# include "../error/C.h"
# include "../main/machinedep.h"

#if SYSTEM_V
#define bcopy(s, d, n) memmove(d, s, n)
#define bzero(s, n) memset(s, 0, n)
#endif /*SYSTEM_V*/

extern int curDir;
extern off_t lseek();

#include "T.decl.h"

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* T.routines.c */
static Ancestors P_((NODE *node, int change));
static NumDescend P_((NODE *node));
static WriteNode P_((TFILE tfile, int nodeNumber, NODE *node));
static void ReadNode P_((TFILE tfile, int nodeNumber, register NODE *node));
static InsertSpace P_((register TFILE tfile, int nodeNumber));
static RemoveSpace P_((register TFILE tfile, int nodeNumber));
static SplitBlock P_((register TFILE tfile));
static NODE *GetNodeIndex P_((register TFILE tfile, int nodeNumber));
static FillBlock P_((register TFILE tfile, register int cBlock, char *cBuf, char *nBuf));
static SortBlocks P_((register TFILE tfile, char *buf0, char *buf1));
static FlushBuf P_((TFILE tfile, int nbuf));
static ReadBlock P_((register TFILE tfile, char *to, int block));
static WriteBlock P_((register TFILE tfile, char *from, int block));

#undef P_

/**********************************/
/** EXTERNALLY CALLABLE ROUTINES **/
/**********************************/

/***********************************************************/
/* Tcreate - create a tree file and return a pointer to it */
/***********************************************************/

TFILE Tcreate(name,order,permission)
	char *name;		/* name of file */
	int order;		/* order in which file will be searched */
				/* (LH = low to high, HL = high to low) */
	int permission;		/* permission as required by creat() */
{
	char newhdr[HEADSIZE];	/*creat HEADER for new file*/
	int fc;			/* file code returned by open() */
	TFILE Topen();		/* routine to open tree file and fill in */
				/* TFILE structure */

	/* create file */
	fc = ChkOpen("New Search File", name, O_RDWR | O_CREAT, permission);

	/* make header */
	bzero(newhdr, HEADSIZE);
	newhdr[0] = order;
	write(fc, newhdr, sizeof(newhdr));
	close(fc);

	/* open with Topen() and return tree file pointer */

	/* For simplicity always open new file wead-write.  This might
	 * be an error in the future if someone changes permission from 
	 * 0666 as it is in comb.
	 */
	return(Topen(name, 0));
}

/*****************************************************************/
/* Topen - open an existing tree file and return a pointer to it */
/*****************************************************************/

TFILE Topen(name, readOnly)
	char *name;		/* name of file to be opened */
	int readOnly;
{
	register TFILE tfile;	/* tree file pointer */
	register int n;
	char *malloc();		/* memory allocator */

	/* allocate space for description of open file */
	tfile = (TFILE)ChkAlloc(sizeof(*tfile),"Tfile");

	/* open file */
	tfile->fc = ChkOpen("Search file", name, (readOnly)? O_RDONLY: O_RDWR,
		0666);
	tfile->readOnly = readOnly;
	tfile->headValid = INVALID;

	/* read in header */
	if( read(tfile->fc,(char *)&tfile->head,sizeof(tfile->head)) !=
			sizeof(tfile->head) )
		error("Can't read header of search file %s\n",name);
	tfile->headValid = VALID;

	/* initialize buffers to be invalid */
	for(n = 0; n < NUMBBUF; n++) {
		( tfile->bbp[n] = &(tfile->b[n]) )->valid = INVALID;
	}
	tfile->clean = 1;

/* ThdrPrint(tfile); */
	/* return pointer */
	return(tfile);
}

/******************************/
/* Tclose - close a tree file */
/******************************/

Tclose(tfile)
	register TFILE tfile;
{
	/* flush buffer and write out header */
	if( !tfile->readOnly) {
		tfile->clean = 1;
		CleanUp(tfile);
	}

	/* close file */
	close(tfile->fc);

	/* deallocate storage */
	ChkFree((char *)tfile);
}

/***************************************
 * ThdrPrint - print out file hdr info *
 ***************************************/

ThdrPrint(tfile)
TFILE tfile;
{
	ListBuf(tfile);
	ListHead(tfile);
}

/****************************************/
/* MakeRoot - make the root of the tree */
/****************************************/

MakeRoot(tfile,node)
	register TFILE tfile;	/* file in which to make root */
	NODE *node;		/* node to be root */
{
	/* update header information */
	tfile->headValid = NEEDSWRITE;
	tfile->head.lastNode = 1;
	tfile->head.blocks->nodes = 1;
	tfile->head.blocks->next = 0;
	tfile->head.lastBlock = 1;

	/* construct linking information for node */
	node->l.nextChild = 0;

	/* copy node into file */
	WriteNode(tfile,0,node);
	CleanUp(tfile);
/*
fprintf(stderr,"MakeRoot made:\n");
PrintNode(node);
*/
}

/**************************************/
/* Root - get the root of a tree file */
/**************************************/

void Root(tfile,node)
	TFILE tfile;		/* tree file to get the root of */
	NODE *node;		/* place to put the root node */
{
	/* get root */
	ReadNode(tfile,0,node);
}

/****************************************************************/
/* MakeChildren - insert a new set of children for a given node */
/****************************************************************/

MakeChildren(node,heir,child1, child2)
	register NODE *node;		/* node to get new children */
	int heir;		/* number of child to inherit previous */
				/* children of node */
	NODE *child1, *child2;	/* new children */
{
	register TFILE tfile;		/* file containing node */

	/* get tfile */
	tfile = (TFILE)(node->l.d.tfile);

	/* update linking information of ancestors */
	Ancestors(node,2);

	/* construct linking information for children */
	if(heir) {
		child1->l.nextChild = 0;
		child2->l.nextChild = node->l.nextChild;
	} else {
		child2->l.nextChild = 0;
		child1->l.nextChild = node->l.nextChild;
	}

	/* construct new linking information for node */
	if(heir == tfile->head.order)
		node->l.nextChild = 1;
	else
		node->l.nextChild = NumDescend(node) + 1;

	/* store nodes */
	WriteNode(tfile,node->l.d.number,node);
	InsertSpace(tfile,node->l.d.number + 1);
	WriteNode(tfile,node->l.d.number + 1,
		(tfile->head.order)? child1: child2);
	InsertSpace(tfile,node->l.d.number + node->l.nextChild + 1);
	WriteNode(tfile,node->l.d.number + node->l.nextChild + 1,
		(tfile->head.order)? child2: child1);
	CleanUp(tfile);
}

/*********************************************/
/* Child - get one of the children of a node */
/*********************************************/

void Child(node,childNum,child)
	register NODE *node;	/* node to get child of */
	int childNum;		/* which child to get (0 or 1) */
	register NODE *child;	/* place to put the child */
{
	register TFILE tfile;	/* file that node is in */

	/* set tfile */
	tfile = (TFILE)(node->l.d.tfile);

	/* if the child is the same as the ordering, then the child is */
	/* stored in node #(node.l.d.number + node.l.nextChild + 1), */
	/* otherwise it is the next node in the file */
	if(childNum == tfile->head.order)
		ReadNode(tfile,node->l.d.number + node->l.nextChild +
			1,child);
	else
		ReadNode(tfile,node->l.d.number + 1,child);
}

/***************************************************************************/
/* RemoveChildren - remove the children of a node as long as one is a leaf */
/***************************************************************************/

/* !! N.B. THis routine changes some of the info in node !! */
RemoveChildren(node)
	register NODE *node;	/* node to remove children from */
{
	register TFILE tfile;	/* file containing node */
	NODE child[2];		/* children of node */
	int cleanSave;

	/* get tfile */
	tfile = (TFILE)(node->l.d.tfile);

	/* get children */
	Child(node,0,&child[0]);
	Child(node,1,&child[1]);

	/* if neither child is a leaf, return an error */
	if(!Leaf(child[0]) && !Leaf(child[1]))
		return(-1);

	/* update information of ancestors */
	Ancestors(node,-2);

	/* give node correct linking */
	if(Leaf(child[0]))
		node->l.nextChild = child[1].l.nextChild;
	else
		node->l.nextChild = child[0].l.nextChild;
	WriteNode(tfile,node->l.d.number,node);

	/* remove children */
	RemoveSpace(tfile,child[tfile->head.order].l.d.number);
	RemoveSpace(tfile,child[1 - tfile->head.order].l.d.number);
	if(tfile->bbp[0]->nodes < 5) {
		cleanSave = tfile->clean;
		GarbCollect(tfile);
		tfile->clean = cleanSave;
	} else
		CleanUp(tfile);

	/* return no error */
	return(0);
}

/************************************/
/* GarbCollect - garbage collection */
/************************************/

void GarbCollect(tfile)
register TFILE tfile;		/* file to be sanitized */
{
	register int cBlock;	/* current block */
	char buf0[BUFLEN];	/* buffers for holding blocks */
	char buf1[BUFLEN];
	register char *cBuf;	/* pointer to buffer for current block */
	register char *nBuf;	/* pointer to buffer for next block */
	register char *tBuf;	/* temporary pointer for exchanging blocks */
	int nbuf;		/* counter for stepping through the buffers */

	if(tfile->readOnly)
		return;

	/* make sure file being cleaned up is up to date */
	tfile->clean = 1;
	CleanUp(tfile);

	/* find first non-full block */
	cBlock = 0;
	while(tfile->head.blocks[cBlock].nodes == INBLOCK)
		if(!(cBlock = tfile->head.blocks[cBlock].next))
			return;
	if(!tfile->head.blocks[cBlock].next)
		return;
	ReadBlock(tfile, buf0, cBlock);

	/* Warn user of garbage collection */
	fprintf(stderr,"Collecting garbage in search file\n");

	/* fill blocks */
	cBuf = buf0;
	nBuf = buf1;
	while(cBlock != -1)
	{
		cBlock = FillBlock(tfile,cBlock,cBuf,nBuf);
		tBuf = cBuf;
		cBuf = nBuf;
		nBuf = tBuf;
	}

	/* sort blocks */
	SortBlocks(tfile,buf0,buf1);

	/* indicate that buffers are invalid */
	for(nbuf = 0; nbuf < NUMBBUF; nbuf++) {
		tfile->b[nbuf].valid = INVALID;
	}
	tfile->clean = 1;
	CleanUp(tfile);
	ftruncate(tfile->fc, (off_t)(HEADSIZE +
		tfile->head.lastBlock * BLOCKSIZE));

}

/********************/
/** LOCAL ROUTINES **/
/********************/

/**********************************************************************/
/* Ancestors - change nextChild for all the ancestors of a given node */
/**********************************************************************/

static Ancestors(node,change)
	NODE *node;		/* node whos ancestors are to be changed */
	int change;		/* amount to add to nextChild */
{
	NODE ancestor;		/* ancestor being changed */

	/* start with root */
	Root( (TFILE)node->l.d.tfile,&ancestor);

	/* loop through all ancestors */
	while(ancestor.l.d.number != node->l.d.number)
	{
		if(node->l.d.number >=
		   ancestor.l.d.number + ancestor.l.nextChild + 1)
			ReadNode( (TFILE)node->l.d.tfile, ancestor.l.d.number +
				ancestor.l.nextChild + 1,&ancestor);
		else
		{
			ancestor.l.nextChild += change;
			WriteNode( (TFILE)node->l.d.tfile,ancestor.l.d.number,
				&ancestor);
			ReadNode( (TFILE)node->l.d.tfile, ancestor.l.d.number +
				 1,&ancestor);
		}
	}
}

/**************************************************************/
/* NumDescend - count the number of descendants of a given node */
/**************************************************************/

static NumDescend(node)
	NODE *node;		/* node whose descendants are to be counted */
{
	NODE descend;		/* highest descendant of node */

	/* find descend */
	descend = *node;
	while(!Leaf(descend))
		ReadNode( (TFILE)node->l.d.tfile,descend.l.d.number +
			descend.l.nextChild + 1,&descend);

	/* return number of nodes */
	return(descend.l.d.number - node->l.d.number);
}

/****************************************/
/* WriteNode - write a node into buffer */
/****************************************/

static WriteNode(tfile,nodeNumber,node)
	TFILE tfile;		/* file to be written in */
	int nodeNumber;		/* number of node to write it in */
	NODE *node;		/* node to write */
{
	NODE *GetNodeIndex();	/* routine for putting a node in the buffer */
				/* and returning a pointer to it */

	/* copy node (except DINFO) into tnode */
	bcopy((char *)node,(char *)GetNodeIndex(tfile,nodeNumber),FNSIZE);

	/* set write flag */
	tfile->bbp[0]->valid = NEEDSWRITE;
}

/************************************/
/* ReadNode - read a node from file */
/************************************/

static void ReadNode(tfile,nodeNumber,node)
	TFILE tfile;		/* file to read from */
	int nodeNumber;		/* number of node to read */
	register NODE *node;	/* address of node to be read out of file */
{
	NODE *GetNodeIndex();	/* routine for getting node out of file */

	bcopy((char *)GetNodeIndex(tfile,nodeNumber),(char *)node,FNSIZE);

	/* compute derived information */
	node->l.d.tfile = (char *)(tfile);
	node->l.d.number = nodeNumber;
}

/********************************************/
/* InsertSpace - make room to insert a node */
/********************************************/

static InsertSpace(tfile,nodeNumber)
	register TFILE tfile;	/* tree file in which to insert space */
	int nodeNumber;		/* node at which to insert space */
{
	register BLOCKBUF *bbp;	/* Active block buffer */
	char *index;		/* index into buffer of node */
	int bytes;		/* number of bytes to be moved to make */
				/* space */
	register int n;

	NODE *GetNodeIndex();	/* routine to pull in right block */

	/* if adding a new node on the end of the file, don't do much */
	/* fancy stuff */
	if(nodeNumber >= tfile->head.lastNode)
	{

		/* get appropriate block */
		GetNodeIndex(tfile,tfile->head.lastNode - 1);

		/* if block is full, split it */
		if( (bbp = tfile->bbp[0])->nodes == INBLOCK)
		{
			SplitBlock(tfile);
			(void)GetNodeIndex(tfile,tfile->head.lastNode - 1);
			bbp = tfile->bbp[0];
		}
	}
	else
	{

		/* get appropriate block */
		index = (char *)(GetNodeIndex(tfile,nodeNumber));
	
		/* if this block is full, split it into two blocks */
		if( (bbp = tfile->bbp[0])->nodes == INBLOCK)
		{
			SplitBlock(tfile);
			index = (char *)(GetNodeIndex(tfile,nodeNumber));
			bbp = tfile->bbp[0];
		}
	
		/* move enough nodes to make space */
		bytes = (bbp->lowNode + bbp->nodes - nodeNumber) *
			FNSIZE;
		if(bytes < 0) {
			fprintf(stderr, "InsertSpace: neg bytes at node %d\n",
				nodeNumber);
			fprintf(stderr, "lowNode = %d, nodes = %d\n",
				bbp->lowNode, bbp->nodes);
			ListHead(tfile);
			ListBuf(tfile);
			fflush(stderr);
			error("Neg move length scheduled");
		}
		bcopy((char *)index,(char *)index + FNSIZE,bytes);
		bbp->valid = NEEDSWRITE;
	}

	/* update rest of information */
	bbp->nodes = ++(tfile->head.blocks[bbp->block].nodes);
	tfile->head.lastNode++;
	tfile->headValid = NEEDSWRITE;

	/* update lowNode in each block buffer with higher node numbers */
	for(n = 1; n < NUMBBUF && (bbp = tfile->bbp[n])->valid; n++) {
		if(bbp->lowNode > nodeNumber)
			bbp->lowNode++;
	}
/*
	if(++count >= 200) {
		count = 0;
		Printf("InsertSpace\n");
		ListHead(tfile);
		ListBuf(tfile);
	}
*/
}

/***********************************************/
/* RemoveSpace - remove a given node from file */
/***********************************************/

static RemoveSpace(tfile,nodeNumber)
	register TFILE tfile;	/* tree file to remove it from */
	int nodeNumber;		/* number of node to remove */
{
	register BLOCKBUF *bbp;	/* Active block buffer */
	register int bytes;	/* number of bytes to move in removeing it */
	char *index;		/* index of node in buffer */
	register int n;
	NODE *GetNodeIndex();	/* routine for getting index */

	/* get index */
	index = (char *)(GetNodeIndex(tfile,nodeNumber));
	bbp = tfile->bbp[0];

	/* move other nodes to cover the one being removed */
	bytes = (bbp->lowNode + bbp->nodes - nodeNumber - 1) *
		FNSIZE;
	if(bytes)
	{
		bcopy((char *)index + FNSIZE,(char *)index,bytes);
		bbp->valid = NEEDSWRITE;
	}

	/* update header information */
	bbp->nodes = --(tfile->head.blocks[bbp->block].nodes);
	tfile->head.lastNode--;
	tfile->headValid = NEEDSWRITE;

	/* update lowNode in each block buffer with higher node numbers */
	for(n = 1; n < NUMBBUF && (bbp = tfile->bbp[n])->valid; n++) {
		if(bbp->lowNode > nodeNumber)
			bbp->lowNode--;
	}
}

/*****************************************************/
/* SplitBlock - break the block in the buffer in two */
/*****************************************************/

static SplitBlock(tfile)
	register TFILE tfile;	/* file in which to split block */
{
	register BLOCKBUF *bbp = tfile->bbp[0];	/* Active block buffer */
	register BLOCKBUF *bbp2;/* Block buffer for new block */
	int bytes;		/* number of bytes to put in second block */
	register int n;

	/* Find an empty buffer or free the least recently used buffer
	 * for the new block */
	for(n = 1; n < NUMBBUF; n++) {
		if( !(bbp2 = tfile->bbp[n])->valid)
			goto foundEmpty;
	}
	FlushBuf(tfile, NUMBBUF - 1);
foundEmpty:
	/* at this point bbp2 points to a free buffer */

	/* compute number of nodes in each block */
	bbp->nodes = tfile->head.blocks[bbp->block].nodes = INBLOCK / 2;
	bbp2->nodes = tfile->head.blocks[tfile->head.lastBlock].nodes =
		INBLOCK - bbp->nodes;

	/* move nodes to new block */
	bytes = bbp2->nodes * FNSIZE;
	bcopy(bbp->buf + (INBLOCK * FNSIZE) - bytes, bbp2->buf, bytes);
	bzero(bbp2->buf + bytes, BLOCKSIZE - bytes);

	/* fill in info in new block buffer */
	bbp2->lowNode = bbp->lowNode + INBLOCK / 2;
	bbp2->block = tfile->head.lastBlock;
	bbp2->valid = NEEDSWRITE;
	bbp->valid = NEEDSWRITE;

	/* update linking information */
	tfile->head.blocks[tfile->head.lastBlock].next =
		tfile->head.blocks[bbp->block].next;
	tfile->head.blocks[bbp->block].next = tfile->head.lastBlock;
	tfile->head.lastBlock++;
	tfile->headValid = NEEDSWRITE;
}

/************************************************************************/
/* GetNodeIndex - get a node out of the file and return a pointer to it */
/************************************************************************/

static NODE *GetNodeIndex(tfile,nodeNumber)
	register TFILE tfile;	/* file to get it out of */
	int nodeNumber;		/* number of node to get */
{
	register int n;		/* buffer pointer number */
	register BLOCKBUF *bbp;	/* points to current BLOCKBUFF */
	int index;		/* byte location of node in buffer */

	/* is node in a buffer? */
	for(n = 0; n < NUMBBUF; n++) {
		if( !(bbp = tfile->bbp[n])->valid)
			goto foundEmpty;
		if(nodeNumber >= bbp->lowNode &&
			nodeNumber < bbp->lowNode + bbp->nodes)
			goto inmem;
	}

	/* If we get here bbp points to the least recently referenced
	 * block buffer and n == NUMBBUF */
	FlushBuf(tfile, --n);

	/* If we get here bbp points to the first empty buffer or a newly
	 * free buffer */
foundEmpty:

	/* Search blocks for the one with nodeNumber in it.  If nodeNumber
	 * is smaller than those in this block, start from the beginning */
	if( !bbp->valid || bbp->lowNode > nodeNumber)
	{
		bbp->block = 0;
		bbp->lowNode = 0;
		bbp->nodes = tfile->head.blocks->nodes;
	}
	while(bbp->lowNode + bbp->nodes <= nodeNumber)
	{
		bbp->lowNode += bbp->nodes;
		bbp->block = tfile->head.blocks[bbp->block].next;
		bbp->nodes = tfile->head.blocks[bbp->block].nodes;
	}


/*
fprintf(stderr,"GetNodeIndex: Reading block %d with nodes %d to %d\n",bbp->block, bbp->lowNode, bbp->lowNode + bbp->nodes - 1);
*/

	ReadBlock(tfile, bbp->buf, bbp->block);
	bbp->valid = VALID;

inmem:
	/* put this BLOCKBUF pointer at the head of the list */
	if(n > 0) {
		bcopy((char *)tfile->bbp, (char *)(tfile->bbp + 1),
			n * sizeof(BLOCKBUF *) );
		tfile->bbp[0] = bbp;
	}

	/* return pointer to node */
	index = (nodeNumber - bbp->lowNode) * FNSIZE;
	return((NODE *)(bbp->buf + index));
}

/***************************************************/
/* FillBlock - fill a block for garbage collection */
/***************************************************/

static FillBlock(tfile,cBlock,cBuf,nBuf)
	register TFILE tfile;	/* file containing block */
	register int cBlock;	/* number of block */
	char *cBuf;		/* buffer containing block */
	char *nBuf;		/* buffer for next block */
{
	int cBytes;		/* number of bytes in block */
	int nBytes;		/* number of bytes in next block */
	int bytes;		/* number of bytes to be moved */
	int nBlock;		/* number of next block */
	char *from;		/* place to move data from */
	char *to;		/* place to move data to */

	/* find cBytes */
	cBytes = tfile->head.blocks[cBlock].nodes * FNSIZE;

	/* loop through blocks after cBlock, filling it up */
	nBlock = cBlock;
	tfile->headValid = NEEDSWRITE;
	while(tfile->head.blocks[cBlock].nodes != INBLOCK)
	{

		/* get next block */
		tfile->head.blocks[cBlock].next =
			tfile->head.blocks[nBlock].next;
		if(!(nBlock = tfile->head.blocks[cBlock].next))
		{
			tfile->head.blocks[cBlock].nodes = cBytes / FNSIZE;
			tfile->head.blocks[cBlock].next = 0;
			WriteBlock(tfile, cBuf, cBlock);
			return(-1);
		}
		ReadBlock(tfile, nBuf, nBlock);
		nBytes = tfile->head.blocks[nBlock].nodes * FNSIZE;

		/* move into block */
		to = cBuf + cBytes;
		from = nBuf;
		bytes = INBLOCK * FNSIZE - cBytes;
		if(bytes > nBytes)
			bytes = nBytes;
		bcopy(from,to,bytes);

		/* update information for next iteration */
		cBytes += bytes;
		tfile->head.blocks[cBlock].nodes += bytes / FNSIZE;
	}

	/* update next block */
	to = from;
	from += bytes;
	bytes = nBytes - bytes;
	if(bytes)
		bcopy(from,to,bytes);
	tfile->head.blocks[nBlock].nodes = bytes / FNSIZE;

	/* write out changes */
	WriteBlock(tfile, cBuf, cBlock);

	/* return */
	return(nBlock);
}

/******************************************/
/* SortBlocks - sort the blocks in a file */
/******************************************/

static SortBlocks(tfile,buf0,buf1)
	register TFILE tfile;	/* file in which to sort them */
	char *buf0;		/* buffers to use while sorting (saves */
	char *buf1;		/* memory to use buffers used elsewhere) */
{
	int newLoc[MAXBLOCKS + 1]; /* new locations of blocks */
	register int cLoc;	/* location of current block */
	register int mBlock;	/* block being moved */
	register int dBlock;	/* block being displaced */
	register int tBlock;	/* temporary block number */
	BLOCKDES mDes;		/* description of block being moved */
	BLOCKDES dDes;		/* description of block being displaced */
	char *mBuf;		/* buffer for block being moved */
	char *dBuf;		/* buffer for block being displaced */
	char *tBuf;		/* temporary pointer for switching buffers */

	/* initialize newLoc */
	for(mBlock = 0;mBlock < (int)tfile->head.lastBlock;mBlock++)
		newLoc[mBlock] = -1;
	newLoc[mBlock] = 0;

	/* set new locations */
	mBlock = 0;
	cLoc = 0;
	do
	{
		if(mBlock != cLoc)
			newLoc[mBlock] = cLoc;
		cLoc++;
		mBlock = tfile->head.blocks[mBlock].next;
	}
	while(mBlock);

	/* sort blocks */

	/* set pointers to buffers */
	mBuf = buf0;
	dBuf = buf1;

	/* find first block that must be moved */
	mBlock = 0;
	while(newLoc[++mBlock] == -1);

	/* loop through blocks */
	while(mBlock != tfile->head.lastBlock)
	{

		/* get description of block being moved */
		mDes = tfile->head.blocks[mBlock];

		/* get block being moved */
		ReadBlock(tfile, mBuf, mBlock);

		/* find place to move it to */
		dBlock = newLoc[mBlock];

		/* record the fact that this block doesn't need to */
		/* be moved again */
		newLoc[mBlock] = -1;

		/* move all displaced blocks */
		while(newLoc[dBlock] != -1)
		{

			/* get description of displaced block */
			dDes = tfile->head.blocks[dBlock];

			/* get displaced block */
			ReadBlock(tfile, dBuf, dBlock);

			/* store description of block being moved */
			tfile->head.blocks[dBlock] = mDes;

			/* store block being moved */
			WriteBlock(tfile, mBuf, dBlock);

			/* switch buffers to make block being displaced */
			/* into block being moved */
			tBuf = mBuf;
			mBuf = dBuf;
			dBuf = tBuf;

			/* make block being displaced into block being */
			/* moved */
			mDes = dDes;

			/* get new block to be displaced */
			tBlock = newLoc[dBlock];
			newLoc[dBlock] = -1;
			dBlock = tBlock;
		}

		/* store description of block being moved */
		tfile->head.blocks[dBlock] = mDes;

		/* store block being moved */
		WriteBlock(tfile, mBuf, dBlock);

		/* find next block that must be moved */
		while(newLoc[++mBlock] == -1);
	}

	/* update header information */
	tfile->head.lastBlock = cLoc;
	mBlock = 0;
	while(mBlock < cLoc - 1)
	{
		tfile->head.blocks[mBlock].next = mBlock + 1;
		mBlock++;
	}
	tfile->head.blocks[mBlock].next = 0;
	tfile->headValid = NEEDSWRITE;
}

/***********************************************/
/* CleanUp - flush buffer and write out header */
/***********************************************/

void CleanUp(tfile)
	register TFILE tfile;
{
	register int n;

/*
fprintf(stderr,"Enter CleanUp: tfile->clean = %d\n", tfile->clean);
*/
	/* Should we really do it? */
	if( !tfile->clean)
		return;

	/* flush buffers */
	for(n = 0; n < NUMBBUF && tfile->bbp[n]->valid; n++) {
		FlushBuf(tfile, n);
	}

	/* write out header */


	if(tfile->headValid == NEEDSWRITE) {
/*
fprintf(stderr,"CleanUp: Writing out header\n");
*/
	    if( lseek(tfile->fc,0L,0) < 0)
		error("Error seeking for writing search file header");
	    if( write(tfile->fc, (char *)&tfile->head,HEADSIZE) != HEADSIZE )
		error("Error writing search file header");
	    tfile->headValid = VALID;
	}
}

/***************************/
/* FlushBuf - flush buffer */
/***************************/

static FlushBuf(tfile, nbuf)
	TFILE tfile;
	int nbuf;		/* the pointer number of the buffer to flush */
{
	register BLOCKBUF *bbp = tfile->bbp[nbuf];

	/* only write out buffer if it has been changed */
	if(bbp->valid == NEEDSWRITE)
	{

/*
fprintf(stderr,"FlushBuf: Writing out block %d\n",bbp->block);
*/

		WriteBlock(tfile, bbp->buf, bbp->block);
		bbp->valid = VALID;
	}
}

/******************************************
 * ReadBlock - Read a block into a buffer *
 ******************************************/

static ReadBlock(tfile, to, block)
register TFILE tfile;
char *to;
int block;
{
	static char myname[] = "ReadBlock";

	if(lseek(tfile->fc, (off_t)BLOCKLOC(block), 0) < 0) {
		perror(myname);
		error("Seeking for read in the search file in %s directory",
			(curDir == FOREGROUND)? "foreground": "background");	
	}
	if(read(tfile->fc, to, BLOCKSIZE) < 0) {
		perror(myname);
		error("Reading search file in %s directory",
			(curDir == FOREGROUND)? "foreground": "background");	
	}
}

/******************************************
 * WriteBlock - Write a block from a buffer *
 ******************************************/

static WriteBlock(tfile, from, block)
register TFILE tfile;
char *from;
int block;
{
	static char myname[] = "WriteBlock";

	if(lseek(tfile->fc, (off_t)BLOCKLOC(block), 0) < 0) {
		perror(myname);
		error("Seeking for write in the search file in %s directory",
			(curDir == FOREGROUND)? "foreground": "background");	
	}
	if(write(tfile->fc, from, BLOCKSIZE) < 0) {
		perror(myname);
		error("Writing the search file in %s directory",
			(curDir == FOREGROUND)? "foreground": "background");	
	}
}

/*************************************************************
 * ListBuf - List the contents of the memory resident blocks *
 *************************************************************/

ListBuf(tfile)
register TFILE tfile;
{
	register int n;
	register BLOCKBUF *bbp;

	for(n = 0; n < NUMBBUF; n++) {
		bbp = tfile->bbp[n];
		if(bbp->valid)
		    fprintf(stderr, "Block %d with nodes %d to %d v = %d\n",
			bbp->block, bbp->lowNode, bbp->lowNode + bbp->nodes - 1,
			bbp->valid);
	}
	fflush(stderr);
}

/****************************************************
 * ListHead - List the contents of the tfile header *
 ****************************************************/

 ListHead(tfile)
 register TFILE tfile;
 {
	register int block, lowNode, nodes;

	fprintf(stderr, "LastNode = %d, LastBlock = %d\n",tfile->head.lastNode,
		tfile->head.lastBlock);
	fprintf(stderr, "Block	f node	l node\n");
	block = 0;
	lowNode = 0;
	nodes = tfile->head.blocks[0].nodes;
	do {
		fprintf(stderr, "%d\t%d\t%d\n",block, lowNode, lowNode + nodes
			 - 1);
		lowNode += nodes;
		block = tfile->head.blocks[block].next;
		nodes = tfile->head.blocks[block].nodes;
	} while(block);
 }

 /* PrintNode(node)
 register NODE *node;
 {
	fprintf(stderr,"Node %d br field %d, b %d, %d bits\n",node->l.d.number,
		node->branchField, node->bit, node->bits);
	fprintf(stderr,"Values %x, %x, %x, %x, %x,   nextChild %d\n",
		node->value[0], node->value[1], node->value[2], node->value[3],
		node->value[4], node->l.nextChild);
} */
