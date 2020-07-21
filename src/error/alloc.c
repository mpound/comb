# include <stdio.h>
/*# define DEBUG  		/* uncomment for debugging */
# define NUMCHUNKS 262144
struct of_memChunk {		/* structure to hold info about one */
	char *pntr;		/* Safely allocated chunk of memory */
#ifdef DEBUG
	char name[8];
#endif
};
static struct of_memChunk memChunk[NUMCHUNKS];	/* array containing all info */
static struct of_memChunk *last = memChunk;	/* no chunks beyond last */
char *malloc(),*realloc(),*strncpy();

/************************************************************
 * ChkAlloc - call malloc and check for no available memory *
 ************************************************************/

char *ChkAlloc(size,name)
int size;
char *name;
{
	register char *cp;

	if( !(cp = malloc((unsigned int)size)) )
		error("Can't allocate %d bytes for %s",size,name);
#ifdef DEBUG
	fprintf(stderr,"ChkAlloc %d bytes at %d for %s\n",size,cp,name);
#endif
	return(cp);
}

/**************************************************************
 * ChkRealloc - call malloc and check for no available memory *
 **************************************************************/

char *ChkRealloc(pntr,size,name)
char *pntr;
int size;
char *name;
{
	register char *cp;

	if( !(cp = realloc(pntr,(unsigned int)size)) )
		error("Can't reallocate block at %d to %d bytes for %s",
			pntr,size,name);
#ifdef DEBUG
	fprintf(stderr,"ChkRealloc block at %d to %d bytes at %d for %s\n",
		pntr,size,cp,name);
#endif
	return(cp);
}

/****************************************************
 * ChkFree - release memory obtained by Chk Alloc *
 ****************************************************/

void ChkFree(memory)
char *memory;
{
	free(memory);
#ifdef DEBUG
	fprintf(stderr,"ChkFree release block at %d\n",memory);
#endif
}

/*************************************************************
 * SafeAlloc - call malloc safely so memory will not be lost *
 *************************************************************/

char *SafeAlloc(size,name)
int size;
char *name;
{
	register char *cp;
	register struct of_memChunk *p,*end;

	if( !(cp = malloc((unsigned int)size)) )
		error("Can't allocate %d bytes for %s",size,name);

	/* find a free memChunk structure and put info in it */
	end = memChunk + NUMCHUNKS;
	for(p = memChunk;p < end;p++) {
		if( ! p->pntr) {
			p->pntr = cp;
#ifdef DEBUG
			strncpy(p->name,name,8);
			fprintf(stderr,
				"SafeAlloc %d bytes at %d for %s, chunk %d\n",
				size,cp,name, (p - memChunk));
#endif
			if(p >= last)
				last = p + 1;
			return(cp);
		}
	}
	fprintf(stderr,"Internal error:SafeAlloc buffer too small\n");
	return(cp);
}

/****************************************************
 * SafeFree - release memory obtained by Safe Alloc *
 ****************************************************/

void SafeFree(memory)
char *memory;
{
	register struct of_memChunk *p;
#ifdef DEBUG
	char tempname[9];
	tempname[8] = 0;
#endif

	free(memory);

	/* Empty the memChunk containing info on this allocation */
	for(p = last - 1;p >= memChunk && ! p->pntr;p--)
		last = p;
	for(;p >= memChunk;p--) {
		if(p->pntr == memory) {
#ifdef DEBUG
			strncpy(tempname,p->name,8);
			fprintf(stderr,"deallocating block for %s at %d\n",
				tempname,memory);
#endif
			p->pntr = (char *)0;
			return;
		}
	}
	fprintf(stderr,
		"Internal error: SafeFree releasing unknown memory at %d\n",
		memory);
}

/***************************************************************
 * ReleaseAllSafe - Release all memory obtained with SafeAlloc *
 ***************************************************************/

void ReleaseAllSafe()
{
	register struct of_memChunk *p;
#ifdef DEBUG
	char tempname[9];
	tempname[8] = 0;
#endif

	for(p = memChunk;p < last;p++) {
		if(p->pntr) {
			free(p->pntr);
#ifdef DEBUG
			strncpy(tempname,p->name,8);
			fprintf(stderr,
				"ReleaseAllSafe releasing memory for %s at %d\n"
			,tempname,p->pntr);
#endif
			p->pntr = (char *)0;
		}
	}
	last = memChunk;
}
