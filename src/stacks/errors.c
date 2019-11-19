/************************************************/
/* Routines for handling common kinds of errors */
/************************************************/

# include "../coordsys/C.h"
# include "../stacks/C.h"
# include "include.h"
# include <stdio.h>

/*****************************************************/
/* DirErr - check that appropriate directory is open */
/*****************************************************/
void DirErr(message)
	char *message;			/* message to be printed if not open */
{
	if(notOpen[curDir])
		error_(message);
}

/***************************************************************************
/* CDirErr - check that appropriate directory is open and give std message *
/**************************************************************************/

void CDirErr()
{
	if(notOpen[curDir])
		error("%s directory not open",(curDir)?
			"Background":"Foreground");
}

/********************************************************/
/* StkNumErr - check that a given stack number is legal */
/********************************************************/

void StkNumErr(stkNum,message)
	int stkNum;			/* stack number being checked */
	char *message;			/* error message */
{
	char errStr[100];		/* error string */

	/* check stkNum */
	if(stkNum < 1 || stkNum > MAXSTKNUM)
	{
		sprintf(errStr,"%s illegal stack number %d",message,stkNum);
		error_(errStr);
	}
}

/************************************************/
/* StkDskErr - check if stack is on or off disk */
/************************************************/

void StkDskErr(stkNum,message,flag)
	int stkNum;			/* stack number being checked */
	char *message;			/* error message */
	int flag;			/* flag for whether on disk or */
					/* off disk causes error: */
					/* 0 = stkNum off disk causes error */
					/* 1 = stkNum on disk causes error */
{
	/* check stkNum */
	if(stkNum < FRSTONDISK)
	{
		if(!flag)
			error_(message);
	}
	else
		if(flag)
			error_(message);
}

/************************************************/
/* StkDskWrn - check if stack is on or off disk */
/************************************************/

void StkDskWrn(stkNum,message,flag)
	int stkNum;
	char *message;
	int flag;
{
	if(stkNum < FRSTONDISK)
	{
		if(!flag)
			warn_(message);
	}
	else
		if(flag)
			warn_(message);
}
