#include <stdio.h>
#include <fcntl.h>
#define COMB 0
#include "scan2.h"
#include "../main/dragon.h"

#define DEBUG 1

static char hdrChar[3] = {'M','S','A'};
struct of_dragon dragon_;
void PrintScan2ValDes(/*n*/);

main(argc, argv)
int argc;
char *argv[];
{
	int n;
	struct val_description *vdp;

	if(argc < 2) {
		printf("no file given\n");
		exit(1);
	}
	if((sci.fd = open(argv[1], O_RDWR)) < 0) {
		printf("can't open %s", argv[1]);
		perror("");
		exit(1);
	}
	/* positive quiet NaN independent of byte order */
	*(int *)&dragon_.NaN = 0x7fffffff;
	((int *)&dragon_.NaN)[1] = 0xffffff7f;

	if(OpenScan2())
		printf("Didn't find the 'blsdf' line\n");
	else for(n = 0; n < valDesSize; n++)
	    PrintScan2ValDes(n);
#if 0
	else for(vdp = valDes; vdp < &valDes[valDesSize]; vdp++) {
	    if(vdp->whichHdr == MAIN_HDR)
		printf("%-8.8s, hdr %c, type %c, rpt %2d, offset %3d  %g\n",
			vdp->keyword, hdrChar[vdp->whichHdr], vdp->type,
			vdp->repeat, vdp->offset, GD(vdp - valDes));
	    else if(vdp->whichHdr > 0)
		printf("%-8.8s, hdr %c, type %c, rpt %2d, offset %3d\n",
			vdp->keyword, hdrChar[vdp->whichHdr], vdp->type,
			vdp->repeat, vdp->offset);
	    else
		printf("%-8.8s is undefined\n", vdp->keyword);
	}
#endif 0
	exit(0);
}

void PrintScan2ValDes(n)
int n;
{
    register struct val_description *vdp = &valDes[n];
	
    if(vdp->whichHdr >= MAIN_HDR) {
	printf("%-8.8s, hdr %c, type %c, rpt %2d, offset %3d",
		vdp->keyword, hdrChar[vdp->whichHdr], vdp->type,
		vdp->repeat, vdp->offset);
	if(vdp->whichHdr == MAIN_HDR)
		PrintScan2Val(n);
	putchar('\n');
    } else
	printf("%-8.8s is undefined\n", vdp->keyword);
}

PrintScan2Val(n)
int n;
{
	register struct val_description *vdp = &valDes[n];
	int index;

	if(vdp->type == 'A') {
		printf(" %s", GS(n));
	} else {
	    for(index = 0; index < vdp->repeat; index++) {
		if(vdp->repeat > 3 && (index %10) == 0)
		    printf("\n %d   ", index);
		if(vdp->type == 'D' || vdp->type == 'E')
		    printf(" %g", GDI(n, index));
		else
		    printf(" %d", GII(n, index));
	    }
	}
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
#endif 0

#include <stdarg.h>
/*VARARGS*/
error(va_alist)
va_dcl
{
	register char *s;
	va_list ap;

	va_start(ap);
	s = va_arg(ap, char*);
	vprintf(s, ap);
	va_end(ap);
	printf("\n");
	exit(1);
}

noint_() {}
yesint_() {}
