#include <stdio.h>
#include "C.h"
#define INDXSIZE 1024
static struct of_indx			/* index buffer */
{
        unsigned int loc;                       /* location of stack in file */
        unsigned short length;                  /* length of stack */
						/* if length is 0, then this */
						/* index does not represent */
						/* any space that has been */
						/* allocated */
	char full;				/* 1 if stack is full */
	char unem;				/* 1 if stack recoverable */
} indx[INDXSIZE], *ip;
struct of_sortIndx
{
	struct of_indx indx;			/* copy of the index */
	int num;				/* stack number */
};
int maxStk;			/* last stack number in file */
int numStk;			/* number of stacks in sortIndx */
int fd = -1;			/* file descriptor of index file */
int numInBuf;			/* remaining number of indices in indx */

extern char *malloc();
extern int compare();

main(argc,argv)
int argc;
char *argv[];
{
	register int i,j;
	struct of_sortIndx *sortIndx;
	register struct of_sortIndx *si;

	if(argc < 2) {
		printf("Usage: sfck stacks_directory_name name2 ...\n");
		printf("%d",sizeof(struct of_stk));
		exit(1);
	}
	while(--argc > 0) {
	    if(!OpenIndex((++argv)[0]))
		continue;
	    sortIndx = (struct of_sortIndx *)malloc((unsigned)maxStk *
		sizeof(struct of_sortIndx));

	    si = sortIndx;
	    for(i = 1; i <= maxStk; i++) {
		if(numInBuf <= 0)
			FillBuf(i);
		if(ip->length > 0) {
			si->num = i;
			si->indx = *ip;
			si++;
			numStk++;
		}
		ip++;
		numInBuf--;
	    }
	    printf("about to sort\n");
	    qsort(sortIndx,numStk,sizeof(struct of_sortIndx),compare);
	    printf("about to check sorted list\n");
	    si = sortIndx;
	    for(i = 1;i < numStk;i++) {
		j = si->indx.loc + si->indx.length - (si + 1)->indx.loc;
		if(j > 0) {
			printf("Overlap: ");
			PrintIndx(si);
			PrintIndx(si + 1);
			putchar('\n');
		} else if(j < 0) {
			printf("Hole: ");
			PrintIndx(si);
			PrintIndx(si + 1);
			putchar('\n');
		}
		si++;
	    }
	    free( (char *)sortIndx);
	}
}
compare(si1,si2)
struct of_sortIndx *si1, *si2;
{
	return(si1->indx.loc - si2->indx.loc);
}
OpenIndex(fn)
char *fn;
{
	char tfn[128];
	putchar('\n');
	if(fd >= 0)
		close(fd);
	sprintf(tfn,"%s/index",fn);
	if((fd = open(tfn,0,0)) < 0) {
		printf("Can't open index file %s\n",tfn);
		return(0);
	}
	numStk = 0;
	maxStk = lseek(fd,0L,2) / sizeof(struct of_indx);
	if(maxStk < 4) {
		printf("Index file in %s too short\n",fn);
		return(0);
	}
	printf("Max stack num in %s is %d\n",fn,maxStk);
	lseek(fd,0L,0);
	numInBuf = 0;
	return(1);
}
FillBuf(curStk)
int curStk;
{
	numInBuf = (maxStk - curStk + 1);
	if(numInBuf > INDXSIZE)
		numInBuf = INDXSIZE;
	read(fd,indx,numInBuf * sizeof(struct of_indx));
	ip = &indx[0];
}
PrintIndx(si)
register struct of_sortIndx *si;
{
	printf("%6d %8d %5d %2d   ",si->num,si->indx.loc,si->indx.length,
		si->indx.full);
}
