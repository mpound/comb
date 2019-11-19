/**************************************/
/* Routines for handling files safely */
/**************************************/

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

# include "C.h"

# define MAXOPEN 30		/* maximum number of open files */
# define NORMAL 0		/* normal file -- opened with open() */
# define STREAM 1		/* stream -- opened with fopen() */
# define OPEN 0			/* software trying to open a file */
# define ACCESS 1		/* software checking access of a file */

extern int errno;		/* system error number */
static struct of_openFile	/* list of open files */
{
	char type;			/* type of file (NORMAL or STREAM) */
	union				/* file descriptor */
	{
		int normal;			/* normal file descriptor */
		FILE *stream;			/* stream file descriptor */
	}
	des;
}
openFile[MAXOPEN];
static int numOpen = 0;		/* number of open files */
static char empty[] = "";


#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* file.c */
static int MakeError P_((char *file, int perm, int act, int warn, char *mssg));
#undef P_

/*********************************************/
/* ChkAccess - check accessability of a file */
/*********************************************/

/* returns 1 if can't access, else 0 */

/*VARARGS4*/
int ChkAccess(file,path,mode,warn,mssg)
	char *file;		/* what to call file in error messages */
	char *path;		/* pathname of file */
	int mode;		/* mode of access checking */
	int warn;		/* flag for giving a warning instead of */
				/* an error if file can't be accessed */
	char *mssg;		/* additional message for warning */
				/* (only used if warn = 1) */
{
	char errStr[100];	/* error string */

	/* if access mode is illegal, error out */
	if(mode > 7 || mode < 0)
	{
		sprintf(errStr,"Can't access %s\nbecause ",file);
		strcat(errStr,"the access routine is called with an illegal mode\n(this is an internal error -- consult a programmer)");
		error_(errStr);
	}

	/* check access of file */
	if(access(path,mode) == -1)
	{
		switch(mode)
		{
			case 0:
			case 4:
				MakeError(file,O_RDONLY,ACCESS,warn,mssg);
				break;
			case 2:
				MakeError(file,O_WRONLY,ACCESS,warn,mssg);
				break;
			case 1:
			case 3:
			case 5:
			case 6:
			case 7:
				MakeError(file,O_RDWR,ACCESS,warn,mssg);
				break;
		}
		return(1);
	}
	else
		return(0);
}

/*******************************************/
/* SafeOpen - open a file safely with open */
/*******************************************/

/* returns file descriptor */

/*VARARGS3*/
int SafeOpen(file,path,opType,mode)
	char *file;		/* what file should be called in errors */
	char *path;		/* pathname of file */
	int opType;		/* type of open (read only, create, etc.) */
	int mode;		/* mode for creating file */
{
	/* add it to the list of files to be closed */
	openFile[numOpen].type = NORMAL;
	noint_();
	openFile[numOpen].des.normal = ChkOpen(file,path,opType,mode);
	numOpen++;
	yesint_();
	return(openFile[numOpen - 1].des.normal);
}

/*********************************************/
/* FSafeOpen - open a file safely with fopen */
/*********************************************/

/* returns file pointer */

FILE *FSafeOpen(file,path,opStr)
	char *file;		/* what file should be called in errors */
	char *path;		/* pathname of file */
	char *opStr;		/* string describing way to open */
{
	FILE *FChkOpen();

	/* add it to the list of files to be closed */
	openFile[numOpen].type = STREAM;
	noint_();
	openFile[numOpen].des.stream = FChkOpen(file,path,opStr);
	numOpen++;
	yesint_();
	return(openFile[numOpen - 1].des.stream);
}

/********************************************************/
/* ChkOpen - open a file with open and check for errors */
/********************************************************/

int ChkOpen(file,path,opType,mode)
	char *file;		/* what file should be called in errors */
	char *path;		/* pathname of file */
	int opType;		/* type of open (read only, create, etc.) */
	int mode;		/* mode for creating file */
{
	int filDes;		/* file descriptor */

	/* open file and check for error */
	if((filDes = open(path,opType,mode)) == -1)
		MakeError(file,opType,OPEN,0, empty);
	return(filDes);
}

/**********************************************************/
/* FChkOpen - open a file with fopen and check for errors */
/**********************************************************/

FILE *FChkOpen(file,path,opStr)
	char *file;		/* what file should be called in errors */
	char *path;		/* pathname of file */
	char *opStr;		/* string describing way file should be */
				/* opened */
{
	char errStr[120];	/* error string */
	FILE *filP;		/* file pointer */
	FILE *fopen();

	/* open file and check for error */
	if((filP = fopen(path,opStr)) == NULL)
		if(opStr[1] == '+' && opStr[2] == 0)
			MakeError(file,O_RDWR,OPEN,0, empty);
		else if(opStr[1] == 0)
			switch(*opStr)
			{
				case 'r':
					MakeError(file,O_RDONLY,OPEN,0, empty);
				case 'w':
					MakeError(file,O_WRONLY,OPEN,0, empty);
				case 'a':
					MakeError(file,O_APPEND,OPEN,0, empty);
				default:
					sprintf(errStr,
					    "Can't open %s\nbecause ",file);
					strcat(errStr,
					    "the routine to open it was called with a bad option string (internal error)");
					error_(errStr);
			}
		else
		{
			sprintf(errStr,"Can't open %s\nbecause ",file);
			strcat(errStr,"the routine to open it was called with a bad option string\n(this is an internal error -- consult a programmer)");
			error_(errStr);
		}
	return(filP);
}

/**********************************************************/
/* SafeClose - close a file that was opened with SafeOpen */
/**********************************************************/

void SafeClose(filDes)
	int filDes;		/* file descriptor */
{
	struct of_openFile *chkFile; /* file being checked to see if it */
				/* is filDes */
	struct of_openFile *lstFile; /* pointer to last file in openFile */

	/* find file to be closed */
	lstFile = openFile + (numOpen - 1);
	for(chkFile = openFile; chkFile <= lstFile; chkFile++)
		if(chkFile->type == NORMAL && chkFile->des.normal == filDes)
			break;

	/* error out if not found */
	if(chkFile->type != NORMAL || chkFile->des.normal != filDes)
		error_("Trying to close a file that wasn't opened properly");

	/* close file and remove from list */
	noint_();
	close(filDes);
	*chkFile = *lstFile;
	numOpen--;
	yesint_();
}

/************************************************************/
/* FSafeClose - close a file that was opened with FSafeOpen */
/************************************************************/

void FSafeClose(filP)
	FILE *filP;		/* pointer to file to be closed */
{
	struct of_openFile *chkFile; /* file being checked to see if it */
				/* is filP */
	struct of_openFile *lstFile; /* last file in openFile */

	/* find file to be closed */
	lstFile = openFile + (numOpen - 1);
	for(chkFile = openFile; chkFile <= lstFile; chkFile++)
		if(chkFile->type == STREAM && chkFile->des.stream == filP)
			break;

	/* error out if not found */
	if(chkFile->type != STREAM || chkFile->des.stream != filP)
		error_("Trying to close a file that wasn't opened properly");

	/* close file and remove from openFile */
	noint_();
	fclose(filP);
	*chkFile = *lstFile;
	numOpen--;
	yesint_();
}

/*****************************************************************/
/* CloseAll - close all files opened with SafeOpen and FSafeOpen */
/*****************************************************************/

void CloseAll()
{
	struct of_openFile *cFile; /* file being closed */
	struct of_openFile *stop; /* place to stop closing files */

	/* close files */
	stop = openFile + numOpen;
	for(cFile = openFile; cFile < stop; cFile++)
		switch(cFile->type)
		{
			case NORMAL:
				printf("CLOSING %d\n",cFile->des.normal);
				close(cFile->des.normal);
				break;
			case STREAM:
				printf("FCLOSING %d\n",cFile->des.stream);
				fclose(cFile->des.stream);
				break;
			default:
				error_("Bad file in open file list");
		}
	numOpen = 0;
}

/***********************************************************/
/* MakeError - make and give the appropriate error message */
/***********************************************************/

/*VARARGS4*/
static int MakeError(file,perm,act,warn,mssg)
	char *file;		/* what to call the file */
	int perm;		/* permission requested */
	int act;		/* action that program was trying to */
				/* perform (OPEN or ACCESS) */
	int warn;		/* flag for making warning instead of */
				/* error */
	char *mssg;		/* additional message for warning */
				/* (only used if warn = 1) */
{
	char errStr[200];	/* error string */

	switch(act)
	{
		case OPEN:
			sprintf(errStr,"Can't open %s\nbecause ",file);
			break;
		case ACCESS:
			sprintf(errStr,"Can't access %s\nbecause ",file);
			break;
	}
	switch(errno)
	{
		case ENOTDIR:
			strcat(errStr,"a directory in the path doesn't exist");
			break;
		case ENOENT:
			strcat(errStr,"it does not exist");
			break;
		case EPERM:
			strcat(errStr,
			    "there is an illegal character in its name");
			break;
		case ELOOP:
			strcat(errStr,
			    "there are too many symbolic links in its pathname");
			break;
		case EROFS:
			strcat(errStr,
			    "it is on a file system that cannot be written to");
			break;
		case ETXTBSY:
			strcat(errStr,
			    "it is busy (being executed)");
			break;
		case EACCES:
			switch(perm)
			{
				case O_RDONLY:
					strcat(errStr,
					    "permission to read it is denied");
					break;
				case O_RDWR:
					strcat(errStr,
					    "permission to read or write is denied");
					break;
				case O_WRONLY:
				case O_APPEND:
				case O_CREAT:
				case O_TRUNC:
					strcat(errStr,
					    "permission to change it is denied");
					break;
				default:
					strcat(errStr,
					    "some kind of permission is denied");
					break;
			}
			break;
		case EFAULT:
			strcat(errStr,
			    "its pathname has been incorrectly specified\n(this is an internal error -- consult a programmer)");
			warn = 0;
			break;
		case EEXIST:
			strcat(errStr,
			    "it is to be created, but already exists");
			break;
		case EISDIR:
			strcat(errStr,
			    "it is a directory, and directories cannot be written to");
			break;
		case EMFILE:
			strcat(errStr,
			    "there are too many files open already");
			break;
		case ENXIO:
			strcat(errStr,
			    "either it is a special file whose device does not exist\nor it is a modem with no carrier");
			break;
		default:
			strcat(errStr,
			    "of some unknown reason");
	}
	if(warn)
	{
		strcat(errStr,"\n\n");
		strcat(errStr,mssg);
		warn_(errStr);
	}
	else
		error_(errStr);
}
