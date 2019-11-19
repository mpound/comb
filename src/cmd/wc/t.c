#define __const const
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
/* #include <stdio.h> */

char *valloc();
char jnk[8192];
const char name[] = "t.file";
main()
{
	void *paddr;
	int fd, i;
	int size = getpagesize();
	char *bp = valloc(8192);

printf("bp = %d\n", bp);
	if((fd = open(name, O_RDWR|O_CREAT, 0644)) < 0) {
	    printf("Can't create %s\n", name);
	    exit(1);
	}
	if(lseek(fd, 81930, 0) < 0) {
	    perror("Seek failed");
	    exit(1);
	}
	if(write(fd, jnk, 1) != 1) {
	    perror("Write failed");
	    exit(1);
	}
	printf("fd = %d, size = %d\n", fd, size);
	if((int)(paddr = mmap((caddr_t)0, size, PROT_WRITE | PROT_READ,
		MAP_PRIVATE, fd, (off_t)0)) == -1) {
/*		MAP_SHARED, fd, (off_t)0)) == -1) { */
	    perror("mmap failed");
	    exit(1);
	}
	printf("paddr = %d\n", paddr);
	for(i = 0; i < 1024; i++) {
	    ((int *)paddr)[i] = i;
	}
}
