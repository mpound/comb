/*
 * A program for writing a dummy scan in a Bell Labs Spectral Data File.
 */

#include <stdio.h>
#include <fcntl.h>
#include "scan2.h"
#include "../error/C.h"
#include "../main/dragon.h"

#define DEBUG 1
#define MAXARRAY 3

static char hdrChar[3] = {'M','S','A'};
struct of_dragon dragon_;

main(argc, argv)
int argc;
char *argv[];
{
    register struct val_description *vdp;
    int na, nc[MAXARRAY];
    int n, i;
    char keyword[10];

    if(argc != 2) {
	error("Usage: ws2 file_name\n");
    }
    if((sci.fd = open(argv[1], O_RDWR, 0664)) < 0) {
	printf("can't open %s", argv[2]);
	perror("");
	exit(1);
    }
    /* positive quiet NaN independent of byte order */
    *(int *)&dragon_.NaN = 0x7fffffff;
    ((int *)&dragon_.NaN)[1] = 0xffffff7f;

    OpenScan2();

    printf("Chans/array (end with 0)");
    for(na = 0;na < MAXARRAY; na++) {
	scanf("%d", &nc[na]);
	if(nc[na]<= 0)
	    break;
    }
    AllocateScan2Buf(na, nc);
    for(n = 0; n < valDesSize && n < ArrayLocN; n++)
	PrintScan2ValDes(n);
    PrintScan2ValDes(ArrayLocN);
    for(i = 0; i < na; i++) {
	SetScan2Array(i);
	printf("\nArray %d at %d\n", i, GII(ArrayLocN, i));
	for(n = ArrayLocN + 1; n < valDesSize; n++) {
	    PrintScan2ValDes(n);
	}
    }
#if 0
    while((n = scanf("%10s", keyword)) == 1 && strcmp("END", keyword)) {
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
	    error("Unrecognized Keyword %s\n", keyword);
    }
    WriteScan2MainHdr();
#endif 0
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
