/* A program for starting a new Bell Labs Spectral Data File for OBS.  It can
 * use a new preamble and will prompt the user for values to put in the main
 * header.  It copies the preamble which it is given to the new file and
 * then adds the main header with all scans empty, but the address of the
 * first scan in place. */

#include <stdio.h>
#include <fcntl.h>
#define COMB 0
#include "scan2.h"
#include "../error/C.h"
#include "../main/dragon.h"

#define DEBUG 1

static char hdrChar[3] = {'M','S','A'};
struct of_dragon dragon_;

main(argc, argv)
int argc;
char *argv[];
{
    register struct val_description *vdp;
    int pfd, nfd;			/* Preamble, new file descriptors */
    int n;
    char keyword[10];

    if(argc != 3) {
	error("Usage: Preamble_path new_file_path\n");
    }
    if((pfd = open(argv[1], O_RDWR)) < 0) {
	printf("can't open %s", argv[1]);
	perror("");
	exit(1);
    }
    if(access(argv[2], 0) == 0) {
	error("'New' file %s already exists\n", argv[2]);
	exit(1);
    }
    if((nfd = open(argv[2], O_RDWR | O_CREAT, 0664)) < 0) {
	printf("can't create %s", argv[2]);
	perror("");
	exit(1);
    }
    /* positive quiet NaN independent of byte order */
    *(int *)&dragon_.NaN = 0x7fffffff;
    ((int *)&dragon_.NaN)[1] = 0xffffff7f;

    sci.fd = pfd;
    if(ReadPreamble())
	error("Didn't find the 'blsdf' line");
    CopyScan2Preamble(nfd);
    close(pfd);
    sci.fd = nfd;
    AllocateScan2MainHdr(1);	/* init. to 0, Set 1st scanLoc to dataStart */
    printf("You may set values for Main Header keywords now (END to stop)\n");
    for(;;) {
	printf("Keyword value >  ");
	scanf("%10s", keyword);
	if(!strcmp("END", keyword))
	    break;
	if(n = FindValDes(keyword)) {
	    double d;
	    int index, i;
	    char sbuf[128];

	    vdp = &valDes[n];
	    if(vdp->type == 'A') {
		if(scanf("%s", sbuf) >0)
		    PS(sbuf, n);
	    } else {
		for(index = 0; index < vdp->repeat; index++) {
		    if(vdp->type == 'D' || vdp->type == 'E') {
			if(scanf("%lf",&d) == 1)
			    PDI(d, n, index);
			else
			    error("No value for %s, index %d", keyword, index);
		    } else {
			if(scanf("%d",&i) == 1)
			    PII(i, n, index);
			else
			    error("No value for %s, index %d", keyword, index);
		    }
		}
	    }
	} else
	    printf("Unrecognized Keyword %s\n", keyword);
    }
    WriteScan2MainHdr();
    exit(0);
}

int FindValDes(kw)
char *kw;
{
    register struct val_description *vdp;

    for(vdp = valDes; vdp < &valDes[valDesSize]; vdp++ )
	if(strncmp(kw, vdp->keyword, 8) == 0)
	    return(vdp - valDes);
    return(0);
}

char *malloc(), *realloc();

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

#if 0
ChkRRead(fd, loc, buf, size, reason)
int fd, loc;
char *buf;
int size;
char *reason;
{
    if(lseek(fd, loc, 0) < 0)
	error("Error seeking in %s", reason);
    if(read(fd, buf, size) != size)
	error("Error reading %s", reason);
}
#endif

#include <stdarg.h>
/*VARARGS*/
error(char *s, ...)
{
	va_list ap;

	va_start(ap, s);
	vprintf(s, ap);
	va_end(ap);
	printf("\n");
	exit(1);
}

noint_() {}
yesint_() {}
