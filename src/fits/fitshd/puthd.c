/* combine an edited header with the original fits file to make an new file */

# include <stdio.h>
# include <ctype.h>
# include <fcntl.h>

# define FITSRECL 2880
char outName[64];

main(argc, argv)
int argc;
char *argv[];
{
	char buf[FITSRECL + 8];
	char hdrName[64];
	register char *op, *cp;
	register int n;
	int inHdr;
	FILE *hfp;
	int ifd, ofd;

	if(argc != 2) {
		printf("Usage: puthd fitsName  expects hdr in fitsName.hdr");
		printf("\n\twrites new file in fitsName.new\n");
		exit(1);
	}
	if( (ifd = open(argv[1],O_RDONLY)) < 0) {
		printf("Can't open %s\n", argv[1]);
		perror("");
		exit(1);
	}
	sprintf(hdrName, "%s.hdr", argv[1]);
	sprintf(outName, "%s.new", argv[1]);
	if( (hfp = fopen(hdrName,"r")) == NULL) {
		printf("Can't open %s\n",hdrName);
		perror("");
		exit(1);
	}
	if( (ofd = open(outName, O_WRONLY | O_CREAT, 0666)) < 0) {
		printf("Can't create %s\n",outName);
		perror("");
		exit(1);
	}
	for(;;) {
	    for(op = buf; op < buf + FITSRECL; op += 80) {
		if(fgets(op, 84, hfp) == NULL) {
			printf("Found EOF in hdr with no END record\n");
			cleanup();
		}
		op[83] = '\0';
		if( !isalpha(*op) ) {
			printf("First char of hdr line must be alpha\n%s\n",op);
			cleanup();
		}
		n = strlen(op);
		op[n--] = '\0';
		for(cp = op; *cp; cp++) {
			if( islower(*cp) )
				*cp = toupper(*cp);
		}
		if(n > 80) {
			printf("Hdr line %s\n is too long\n",op);
			cleanup();
		}
		while(n < 80)
			op[n++] = ' ';
		if( strncmp(op, "END ",4) == 0) {
			if(fgets(&op[81], 4, hfp) != NULL) {
				printf("'END' line before end of hdr file\n");
				cleanup();
			}
			op += 80;
			goto endhdr;
		}
	    }
	    if( write(ofd, buf, FITSRECL) != FITSRECL) {
		perror("Trouble writing header of new file");
		cleanup();
	    }
	}
endhdr:
	while(op < &buf[FITSRECL])
		*op++ = ' ';
	if( write(ofd, buf, FITSRECL) != FITSRECL) {
		perror("Trouble writing header of new file");
		cleanup();
	}
	fclose(hfp);
	inHdr = 1;
	cp = &buf[FITSRECL - 80];
	while( (n = read(ifd, buf, FITSRECL) ) == FITSRECL) {
		if(inHdr) {
			if(strncmp(cp, "END ", 4) == 0 ||
					strncmp(cp, "    ", 4) == 0)
				inHdr = 0;
		} else {
			if(write(ofd, buf, FITSRECL) != FITSRECL) {
				printf("Trouble writing data in %s\n", outName);
				perror("");
				cleanup();
			}
		}
	}
	if(n > 0)
		printf("Bad FITS file length, %d bytes in last record\n",n);
}

cleanup()
{
	printf("Unlinking %s\n",outName);
	unlink(outName);
	exit(1);
}
