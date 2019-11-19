# include "T.struct.h"

/*************/
/* bit field */
/*************/

typedef struct
{
	int bit;			/* first bit */
	int numBits;			/* number of bits */
}
BITFIELD;

/*****************************/
/* flags for edges in search */
/*****************************/

typedef struct
{
	int high;			/* whether high bound is meaningful */
	int low;			/* whether low bound is meaningful */
}
EDGEFLAG;

/***********************/
/* search file pointer */
/***********************/

typedef TFILE SFILE;

/* S.routines */

SFILE Screat(/*filName,permission*/);
SFILE Sopen(/*filName*/);
void Sclean(/*sfile, clean*/);
void Sclose(/*sfile*/);
void Sstore(/*sfile,data*/);
void Ssearch(/*sfile,low,high,proc*/);
void Sremove(/*sfile,data*/);
