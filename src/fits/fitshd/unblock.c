# include <stdio.h>
# include <ctype.h>
# include <sys/file.h>

# define FITSRECL 2880

main(argc, argv)
int argc;
char *argv[];
{
    char buf[80];
    register char *cp;
    int n;
    for(;;) {
	if((n = read(0, buf, 80)) != 80) {
	    if(n == 0)
		exit(0);
	    fprintf(stderr, "Unblock: Trouble reading input");
	    exit(1);
	}
	for(cp = &buf[79]; *cp == ' ' && cp > buf; cp--)
	    ;
	if(cp > buf) {
	    cp[1] = '\n';
	    if(write(1, buf, n = cp + 2 - buf) != n) {
		fprintf(stderr, "Unblock: Trouble writing output");
		exit(2);
	    }
	    if(strncmp(buf, "END", 3) == 0)
		exit(0);
	}
    }
}
