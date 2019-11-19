/**************************************/
/* Routines for handling default file */
/**************************************/

# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>
# include <fcntl.h>
# include <string.h>

# include "../stacks/C.h"
# include "../coordsys/C.h"
# include "include.h"

static FILE *defFile[2] = {0,0};	/* default file pointers */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* default.c */
static int FindName P_((char *name));
#undef P_

/*****************************************/
/* CreateDefault - create a default file */
/*****************************************/

void CreateDefault()
{
	char defName[MAXDNLEN + 8];	/* name of default file */

	/* construct name */
	sprintf(defName,"%s/default",dirName[curDir]);

	/* create file if it doesn't exist */
	if(access(defName,0))
		close(creat(defName,0666));
}

/***************************************/
/* OpenDefault - open the default file */
/***************************************/

void OpenDefault()
{
	char defName[MAXDNLEN + 8];	/* name of defualt file */
	char errStr[MAXDNLEN + 26];	/* error string */
	FILE *FChkOpen();		/* routine for opening file */

	/* construct name */
	sprintf(defName,"%s/default",dirName[curDir]);

	/* close old file if open */
	if(defFile[curDir])
		fclose(defFile[curDir]);

	/* open new file */
	sprintf(errStr,"default file for stack directory %s",dirName[curDir]);
	if(access(defName, W_OK) ) {
		defFile[curDir] = FChkOpen(errStr,defName,"r");
		stkDefReadOnly[curDir] = 1;
		fprintf(stderr, "Warning: Stack defaults file is read only\n");
	} else {
		defFile[curDir] = FChkOpen(errStr,defName,"r+");
		stkDefReadOnly[curDir] = 0;
	}
}

/*****************************************************/
/* CloseDefault - Close the default file -- mwp 5/92 */
/* removed offending sprintf - mwp 5/25/93          */
/*****************************************************/

void CloseDefault()
{
        /* close old file */
        fclose(defFile[curDir]);
}
/***********************************************************/
/* GtStkDflt - get a default value out of the default file */
/***********************************************************/

int GtStkDflt(name,format,address,bomb)
	char *name;			/* name of default value */
	char *format;			/* format of default value */
	char *address;			/* location in which to put it */
	int bomb;			/* 1 if ok to warn, 2 if to error,
					   3 if either allowed */
{
	char *dot;			/* location of dot in name */
	char errStr[MAXDNLEN + MAXNMLEN + 24]; /* error string */

	/* check that a directory is open */
	DirErr("No open stack directory to give defaults");

	/* find default value */
	if(FindName(name))
	{
		if(dot = strchr(name,'.'))
			name = dot + 1;
		sprintf(errStr,"No default %s in directory %s",
		    name,dirName[curDir]);
		if(bomb & 2)
			error_(errStr);
		if(bomb & 1)
			warn_(errStr);
		return(0);
	}

	/* get default value */
	fscanf(defFile[curDir],format,address);
	return(1);
}

/*************************************************/
/* WrStkDflt - write a value to the default file */
/*************************************************/

/*VARARGS*/
void WrStkDflt(char *name, char *format, ...)
{
	va_list ap;

	/* check that a directory is open */
	DirErr("No open stack directory for storing defaults");
	if(stkDefReadOnly[curDir])
		return;

	/* remove old value if there */
	noint_();
	va_start(ap, format);
	RmDefault(name);

	/* write out new value */
	if(format)
	{
		fseek(defFile[curDir],0,2);
		fprintf(defFile[curDir],"%s=",name);
		vfprintf(defFile[curDir], format, ap);
		va_end(ap);
		fprintf(defFile[curDir],"\n");
		fflush(defFile[curDir]);
	}
	yesint_();
}

/*******************************************************************/
/* FindName - find the name of a default value in the default file */
/*******************************************************************/

static int FindName(name)		/* returns 0 if found, 1 if not */
	char *name;			/* name of default value */
{
	char tname[MAXNMLEN];		/* name in file being checked */
	register FILE *file = defFile[curDir];
	register int c;

	/* start at top of file */
	rewind(file);

	/* find name */
	while(fscanf(file,"%[^=]",tname) > 0)
	{
		if(strcmp(name,tname))
			while((c = getc(file)) != '\n' && c != EOF)
				;
		else
		{
			getc(file);
			return(0);
		}
	}

	/* name wasn't found */
	return(1);
}

/**********************************************/
/* RmDefault - remove a default from the file */
/**********************************************/

void RmDefault(name)
	char *name;		/* name of defualt to be removed */
{
	char buf[101];		/* buffer for moving data in file */
	int moveToLoc;		/* location to move data to */
	int moveBack;		/* location to move back to for reading */
				/* data */

	/* find name in file */
	if(FindName(name))
		return;

	/* find extent of default value */
	moveToLoc = ftell(defFile[curDir]) - strlen(name) - 1;
	while(getc(defFile[curDir]) != '\n')
		;

	/* move data */
	noint_();
	/*RWW while(fscanf(defFile[curDir],"%100[^]",buf) != EOF) */
	while(fgets(buf, 100, defFile[curDir]))
	{
		moveBack = ftell(defFile[curDir]);
		fseek(defFile[curDir],moveToLoc,0);
		fprintf(defFile[curDir],"%s",buf);
		moveToLoc += strlen(buf);
		fseek(defFile[curDir],moveBack,0);
	}

	/* truncate file */
	ftruncate(fileno(defFile[curDir]), moveToLoc);
	fflush(defFile[curDir]);
	yesint_();
}
