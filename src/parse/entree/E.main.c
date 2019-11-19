# include <string.h>
# include "E.constant.h"
# include "E.struct.h"
# include "../P.tree.h"
# include "E.global.h"

extern FILE *yyin;		/* lex input file code */

main(ac,av)
	int ac;
	char **av;
{
	char file[100];				/* filename */
	static char *(ends[2][2]) =		/* endings for include files */
	{
		"dc.h",
		"df.h",
		"ec.h",
		"ef.h"
	};
	static char usage[] = "Usage: entree <-C> xx.t\n";


	/* check for -C option */
	if(ac != 2 && ac != 3) {
		fprintf(stderr, usage);
		exit(2);
	}
	if(!strcmp(av[1],"-C"))
	{
		if(ac != 3) {
			fprintf(stderr, usage);
			exit(2);
		}
		strcpy(sfile,av[2]);
		strcpy(file,sfile);
		fortrn = 0;
	}
	else
	{
		strcpy(sfile,av[1]);
		strcpy(file,sfile);
		fortrn = 1;
	}

	/* construct file names */
	strrchr(file,'.')[1] = 0;
	strcpy(efile,file);
	strcat(efile,ends[1][fortrn]);
	strcpy(dfile,file);
	strcat(dfile,ends[0][fortrn]);
	if(fortrn)
	{
		strcpy(ifile,file);
		strcat(ifile,"if.h");
	}

	/* construct common name */
	strchr(file,0)[-1] = 0;
	strcpy(comnam,file);

	/* .c file name which defines the Node structure */
	strcpy(tfile,file);
	strcat(tfile,"t_.c");

	/* open files */
	yyin = sfc = fopen(sfile,"r");
	if(sfc == NULL) {
		fprintf(stderr, "Entree: Can't open %s\n",sfile);
		exit(4);
	}
	efc = fopen(efile,"w");
	dfc = fopen(dfile,"w");
	if(fortrn)
		ifc = fopen(ifile,"w");
	tfc = fopen(tfile,"w");

	/* parse source */
	stkpnt = stack;
	cnode = 0;
	clin = 1;
	yyparse();

	/* compute locations of variables */
	lastloc = flocs();
	if(!lastloc)
		lastloc = 1;

	/* write out .d*.h file */
	wrdef();

	/* write out .e*.h file */
	wrex();

	/* write out .t.c file */
	wrtre();
	exit(0);
}

/*
 * Dummy for Lex
 */
yywrap()
{
	return(1);
}
