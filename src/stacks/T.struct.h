# define MAXLEAVES 262144
# define BLOCKSIZE 4096
# define BUFLEN 4096
# define INBLOCK (BUFLEN / (sizeof(NODE) - sizeof(DINFO)))
# define MAXBLOCKS (ROUNDUP((2 * MAXLEAVES - 1),INBLOCK))
# define LH 1
# define HL 0
# define HEADSIZE ((ROUNDUP(sizeof(HEADER),BLOCKSIZE)) * BLOCKSIZE)
# define ROUNDUP(x,y) ((x + y - 1) / y)
# define BLOCKLOC(x) (x * BLOCKSIZE + HEADSIZE)
# define FNSIZE (sizeof(NODE) - sizeof(DINFO))
# define Leaf(x) (!(x).l.nextChild)
#define NUMBBUF 20
#define INVALID 0
#define VALID 1
#define NEEDSWRITE 2


/************************************/
/* information derived outside file */
/************************************/

typedef struct
{
	char *tfile;			/* tree file pointer of file */
					/* containing node (should be */
					/* TFILE type but the compiler */
					/* doesn't like that) */
	int number;			/* number of node */
}
DINFO;

/*********************************/
/* links for describing children */
/*********************************/

typedef struct
{
	int nextChild;			/* offset of child described by */
					/* order */
	DINFO d;			/* derived information (tfile, and */
					/* number) */
}
LINKS;

# include "node_type"

/**********************/
/* block descriptions */
/**********************/

typedef struct
{
	unsigned char nodes;		/* number of nodes in block */
	unsigned short next;			/* next block */
}
BLOCKDES;

/***********************/
/* header in tree file */
/***********************/

typedef struct
{
	char order;			/* order in which file will be */
					/* searched (LH or HL) */
	unsigned short lastBlock;	/* number of last block used in */
					/* file */
	int lastNode;			/* last node in file */
	BLOCKDES blocks[MAXBLOCKS];	/* descriptions of blocks */
}
HEADER;
/*************************************************
 * in memory copy of a block and its description *
 *************************************************/

 typedef struct {
	int lowNode;			/* low node in block */
	int nodes;			/* number of nodes in block */
	int block;			/* the block number of this data */
	int valid;			/* 0 if not valid, 1 if valid, and not
					 * changed; 2 if needs to be written */
	char buf[BLOCKSIZE];		/* Contents of block.  Not defined
					 * as type NODE as the derived
					 * information (DINFO) is not in the
					 * file */
} BLOCKBUF;

/*************************/
/* tree file description */
/*************************/

typedef struct
{
	int fc;				/* file code of open file */
	int clean;			/* control the automatic cleanups */
	short readOnly;			/* true for read only file */
	short headValid;		/* should be VALID or NEEDSWRITE */
	HEADER head;			/* header of tree file */
	BLOCKBUF *bbp[NUMBBUF];		/* pointers to the block buffers sorted
					 * with most recently accessed first
					 * The first is normally the current
					 * block */
	BLOCKBUF b[NUMBBUF];		/* the block buffers */
}
*TFILE,FILDES;
