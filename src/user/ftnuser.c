/*
 * Main program to communicate with comb and call a user subroutine in fortran
 * to process a stack
 */

#include <stdio.h>
#include <string.h>
#include "userpg.h"

struct of_stk stk_;

main(argc, argv)
int argc;
char *argv[];
{
	int ifd, ofd;
	int n, nread;

	if(argc != 3 || sscanf(argv[1], "%d", &ifd) != 1 ||
			sscanf(argv[2], "%d", &ofd) != 1 ) {
	    fprintf(stderr, "User program: Bad command line\n");
	    exit(100);
	}
	for(nread = 0; nread < sizeof(stk_); nread += n) {
	    if((n = read(ifd, nread + (char *)&stk_, sizeof(stk_) - nread))
		<= 0) {
		    fprintf(stderr, "User program: Didn't get full stack\n");
		    exit(101);
		}
	}


	/* Call a fortran subroutine to do the real work */
	ftnsub_();

	if((n = write(ofd, &stk_, sizeof(stk_))) != sizeof(stk_)) {
	    fprintf(stderr, "User program: Only wrote %d bytes back\n", n);
	    exit(102);
	}
	exit(0);
}
