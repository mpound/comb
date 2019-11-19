/*
 * hcsetup: set up $COMB/src/cmd/hc/hc.t and 
 *          $COMB/lib/ hc.lpr for confused users 
 *
 * History:
 * original version: Thu May 27 11:35:27 EDT 1993 MWP
 *
 * Added code for HP LaserJet printers. 
 * Only 2 Imagen, 2 LJ printers and 1 HPGL allowed despite array sizes=4. 
 * lj.driver.c can handle 4 LJ, but for now default is 4 PostScript,
 * 2 Imagen, 2LJ, and 1 HP-GL.
 *		- MWP  Thu Sep  2 14:42:03 EDT 1993
 *
 * Minor changes  to run on PC with LINUX and gcc  -  RWW Mar 1 1994
 *
 * Allowed 1 Tektronix printer, so I can get rid of #if KITTPEAK
 * dependencies in tek.driver.c Mon Mar 14 13:48:03 EST 1994
 *
 * Use OS name to choose between "nb" and "nobanner" for lp command.
 * MWP - Mon Jun  5 08:23:39 PDT 1995 
 *
 * Allow for user-named hardcopy file in hc.t 
 *  - MWP Tue Apr  2 15:43:44 PST 1996 
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/utsname.h>
#include "../src/main/machinedep.h"

/* defaults printer names for hc.lpr */
char postp[4][16] = {"unused","unused", "unused", "unused"}; 
char imp[4][16] = {"unused","unused", "unused", "unused"}; 
char ljp[4][16] = {"unused","unused", "unused", "unused"}; 
char hpglp[4][16] = {"unused","unused", "unused", "unused"}; 
char tekp[4][16] = {"unused","unused", "unused", "unused"}; 

/* default printer names for hc.t */
char hcpost[4][16]={"PostScript 1","PostScript 2","PostScript 3","PostScript 4"}; 
char hcimp[4][16] = {"Imagen 1","Imagen 2", "Imagen 3", "Imagen 4"}; 
char hclj[4][16] = {"LaserJet 1","LaserJet 2", "LaserJet 3", "LaserJet 4"}; 
char hchpgl[4][16] = {"HP-GL 1","HP-GL 2", "HG-GL 3", "HP-GL 4"}; 
char hctek[4][16]={"Tektronix 1","Tektronix 2","Tektronix 3","Tektronix 4"}; 

/* printer labels for hc.t */

char postlabel[4][4];
char imlabel[4][4];
char ljlabel[4][4];
char hpgllabel[4][4];
char teklabel[4][4];

char *hcfile="str(\"60\") auto(\"hcfile\") - \"hcfn\"";

#if SYSTEM_V
char *lpstring = "#lpr -P${PRINTER} -h -r -s ${FILE}\n\
#The default print command is:\n\
lp -o %s -d ${PRINTER} -c ${FILE}\nrm ${FILE}\n";

#else /*SYSTEM_V*/ 
char *lpstring = "#The default print command is:\n\
lpr -P${PRINTER} -h -r -s ${FILE}\n\
#lp -o nobanner -d ${PRINTER} -c ${FILE}\n#rm ${FILE}\n";
#endif /*SYSTEM_V*/

main()
{
	FILE *fp;
	struct utsname *uNp, Xname;
	char comb[64], hclpr[128], hct[128], *cp;
	char banner[16];
	int np, ni, nlj, nhpgl, ntek, i; /* counters */
 
	memset(banner,0,16);
	uNp = &Xname; /* concession to make it run on HP-UX */
        /* get the main directory path for comb */
        if((cp = getenv("COMB")) != NULL)
                strcpy(comb,cp);
        else {
                fprintf(stderr, "Please define COMB in your environment \
before running hcsetup\n");
                exit(1);
        }

	sprintf(hclpr,"%s/lib/hc.lpr",comb);
	sprintf(hct,"%s/src/cmd/hc/hc.t",comb);
	printf("\nThis program will set up the printer files for comb.\n");

/* PostScript */
	printf("\nHow many PostScript printers do you want set up (0 - 4)? ");
	scanf("%d",&np);
	if(np>4)
	   np=4;
	for(i=0; i < np; i++) {
		printf("Enter the name of PostScript printer #%1d: ",i+1);
		scanf("%16s",postp[i]);
		printf("Enter a label for PostScript printer #%1d\
\n(usually the first letter of the printer name): ",i+1);
		scanf("%3s",postlabel[i]);
	}

/* Imagen */
	printf("How many Imagen printers do you want set up (0 - 2)? ");
	scanf("%d",&ni);
	if(ni>2)
	   ni=2;
	for(i=0; i < ni; i++) {
		printf("Enter the name of Imagen printer #%1d: ",i+1);
		scanf("%15s",imp[i]);
		printf("Enter a label for Imagen printer #%1d\
\n(usually the first letter of the printer name): ",i+1);
		scanf("%3s",imlabel[i]);
	}

/* HP LaserJet */
        printf("How many LaserJet III (or later) printers do you want set up (0 - 2)? ");
        scanf("%d",&nlj);   
	if(nlj>2)
	   nlj=2;
        for(i=0; i < nlj; i++) {
                printf("Enter the name of LaserJet printer #%1d: ",i+1);
                scanf("%15s",ljp[i]);
                printf("Enter a label for LaserJet printer #%1d\
\n(usually the first letter of the printer name): ",i+1);
                scanf("%3s",ljlabel[i]);
        }

/* HP-GL */
        printf("How many HP-GL plotters do you want set up (0 - 1)? ");
        scanf("%d",&nhpgl);
	if(nhpgl>1)
	   nhpgl=1;
        for(i=0; i < nhpgl; i++) {
                printf("Enter the name of HP-GL printer #%1d: ",i+1);
                scanf("%15s",hpglp[i]);
                printf("Enter a label for HP-GL printer #%1d\
\n(usually the first letter of the printer name): ",i+1);
                scanf("%3s",hpgllabel[i]);
        }

/*Tektronix  */
        printf("How many Tektronix printers do you want set up (0 - 1)? ");
        scanf("%d",&ntek);
	if(ntek>1)
	   ntek=1;
        for(i=0; i < ntek; i++) {
                printf("Enter the name of Tektronix printer #%1d: ",i+1);
                scanf("%15s",tekp[i]);
                printf("Enter a label for Tektronix printer #%1d\
\n(usually the first letter of the printer name): ",i+1);
                scanf("%3s",teklabel[i]);
        }

/*************************************/
/* now write out the files 	     */
/* hc.lpr is first		     */
/*************************************/

	if((fp = fopen(hclpr,"w")) == NULL) {
		printf("Can't open %s\n",hclpr);
		exit(1);
	}
	fprintf(stderr,"\nWriting %s...",hclpr);
	fprintf(fp,"#!/bin/sh\n\
#\n# This file was generated by %s/bin/hcsetup.\n#\n\
# hc.lpr : print file (arg2) on (arg1) device\n\
#\n\
# Called from comb printer drivers in %s/graphics \n\
# laser.driver.c, im.driver.c, lj.driver.c, etc.\n#\n\
# This holds up to 4 PostScript, 2 Imagen,  2 LaserJet, \n\
# 1 HP-GL, and 1 Tektronix printers.\n#\n\
# Substitute your printer name(s) for astro, bima, neptune , unused, etc.\n\
# If you don't have this many printers, just change as many as you\n\
# have. Change the others to 'unused' to indicate that they aren't used.\n\
# Edit the default print command at the end of this file to match your\n\
# system.\n\
#\n\
# To build this file and %s/src/hc/hc.t automatically, run \n\
# %s/bin/hcsetup\n\
#\n\
\n\nDEVICE=$1\nFILE=$2\nPRINTER=\n\ncase ${DEVICE} in\n",comb,comb,comb,comb);

	for(i=0;i<4;i++) {
	   fprintf(fp,"\tpost%1d)\n\t\tPRINTER=%s\n\t\t;;\n",i+1,postp[i]);
	}

	for(i=0;i<2;i++) {
	   fprintf(fp,"\timagen%1d)\n\t\tPRINTER=%s\n\t\t;;\n",i+1,imp[i]);
	}
	for(i=0;i<2;i++) {
	   fprintf(fp,"\tlj%1d)\n\t\tPRINTER=%s\n\t\t;;\n",i+1,ljp[i]);
	}
	for(i=0;i<1;i++) { 
        /* if we ever support more than 1 HP-GL....
	 * fprintf(fp,"\thpglplot%1d)\n\t\tPRINTER=%s\n\t\t;;\n",i+1,hpglp[i]);
	 */
	   fprintf(fp,"\thpglplot)\n\t\tPRINTER=%s\n\t\t;;\n",hpglp[i]);
	}
	for(i=0;i<1;i++) { 
        /* if we ever support more than 1 Tektronix....
	 * fprintf(fp,"\ttekplot%1d)\n\t\tPRINTER=%s\n\t\t;;\n",i+1,tekp[i]);
	 */
	   fprintf(fp,"\ttekplot)\n\t\tPRINTER=%s\n\t\t;;\n",tekp[i]);
	}

	fprintf(fp,"\t*)\n\t\techo \"$0 : bad device \\\"${DEVICE}\\\"\" 1>&2\n\
\t\trm ${FILE}\n\t\t;;\n\nesac\n");

#if SYSTEM_V
	/* take care of the different banner flags for HP/Sun */
        i = uname(uNp);
        /*fprintf(stderr,"%s ",uNp->sysname);*/
        if(strcmp(uNp->sysname,"HP-UX"))
                strncpy(banner,"nobanner",8);
        else
                strncpy(banner,"nb -oraw",8);
	fprintf(fp,lpstring,banner);
#else /* SYSTEM_V */
        fprintf(fp,"%s",lpstring);
#endif /* SYSTEM_V */
	fclose(fp);

/********************************************/
/*					    */
/*  now write out $COMB/src/cmd/hc/hc.t     */
/*                                          */
/********************************************/

	if((fp = fopen(hct,"w")) == NULL) {
		printf("Can't open %s\n",hct);
		exit(1);
	}
	fprintf(stderr,"\nWriting %s...",hct);
	fprintf(fp,"\
parse (\"cmnd_.gstr\")\n\n\
\"Make a Hard copy of the current screen\" \"hc\";\n{\n\
\t\"Type of device\":ex(\"typ\");\n\t{\n");

	
	for(i=0;i<np;i++) {
		if(islower(postp[i][0]))
		   postp[i][0] = postp[i][0] + 'A' - 'a';
		fprintf(fp,"\t\t\"%s\" \"%s\":;\n",postp[i],postlabel[i]);
	}
	for(i=np;i<4;i++)
		fprintf(fp,"\t\t\"%s\" \"unused\":;\n",hcpost[i]);

	for(i=0;i<ni;i++) {
		if(islower(imp[i][0]))
		   imp[i][0] = imp[i][0] + 'A' - 'a';
		fprintf(fp,"\t\t\"%s\" \"%s\":;\n",imp[i],imlabel[i]);
	}
	for(i=ni;i<2;i++)
		fprintf(fp,"\t\t\"%s\" \"unused\":;\n",hcimp[i]);

        for(i=0;i<nlj;i++) {
		if(islower(ljp[i][0]))
                   ljp[i][0] = ljp[i][0] + 'A' - 'a';
                fprintf(fp,"\t\t\"%s\" \"%s\":;\n",ljp[i],ljlabel[i]);
        }
        for(i=nlj;i<2;i++)
		fprintf(fp,"\t\t\"%s\" \"unused\":;\n",hclj[i]);

        for(i=0;i<nhpgl;i++) {
                if(islower(hpglp[i][0]))
                   hpglp[i][0] = hpglp[i][0] + 'A' - 'a';
                fprintf(fp,"\t\t\"%s\" \"%s\":;\n",hpglp[i],hpgllabel[i]);
        }
        for(i=nhpgl;i<1;i++)
                fprintf(fp,"\t\t\"%s\" \"unused\":;\n",hchpgl[i]);

        for(i=0;i<ntek;i++) {
                if(islower(tekp[i][0]))
                   tekp[i][0] = tekp[i][0] + 'A' - 'a';
                fprintf(fp,"\t\t\"%s\" \"%s\":;\n",tekp[i],teklabel[i]);
        }
        for(i=ntek;i<1;i++)
                fprintf(fp,"\t\t\"%s\" \"unused\":;\n",hctek[i]);


/*	fprintf(fp,"\t\t\"PostScript file\" \"pf\":;\n\t\t\"Pen Plotter\" \"pp\":;\
\n\t}\n}\n"); */
	fprintf(fp,"\t\t\"PostScript file\" \"pf\": %s;\n\
\t\t\"LaserJet file\" \"lf\": %s;\n\
\t\t\"Tektronix file\" \"tf\": %s;\n\
\t}\n",hcfile,hcfile,hcfile);

	fprintf(fp,"\t\"Page Orientation\":ex(\"ori\");\n\t{\n\
\t\t\"Landscape\" \"land\":;\n\t\t\"Portrait\" \"port\":;\n\t}\n}\n");

	
	fprintf(stderr,"done\n");
	fclose(fp);

	fprintf(stderr,"\nIf you are not running `makecomb install' \
subsequent to\nsetting up these printer files, you must do the following:\n\n\
\t\tcd %s/bin\n\
\t\tremake_printers\n\nHave a nice day.\n", comb);
	exit(0);
}
