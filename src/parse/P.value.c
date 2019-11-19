# include "P.tree.h"
#include "../main/C.h"
#if COMB
#include "../scan/C.h"
#endif /*COMB*/
# include "P.struct.h"
# include "P.macro.h"
# include "P.constant.h"
# include "P.extern.h"

	/************************************/
	/* st_num - store a number in cnode */
	/************************************/

st_num(num1,num2)
	double num1,num2;		/* numbers to be stored */
{

	/* check that cnode is a parameter */
	if(!L_IS_PARAM(cnode))
	{
		if(unspt[-1] != 1)str -= yyleng;
		nferr("trying to enter number in non-parameter");
	}

	/* store num1 and num2 in cnode */

	/* store as real */
	if(cda_type & REAL)
	{
		USE_TYPE(cnode) = REAL;
		L_PARAM(double,cnode) = num1;
		if(SEQ(cnode) == RANGE)
			L_PARAM2(double,cnode) = num2;
	}

	/* store as integer */
	else if(cda_type & INTN)
	{
		USE_TYPE(cnode) = INTN;
		L_PARAM(int,cnode) = INT(num1);
		if(SEQ(cnode) == RANGE)
			L_PARAM2(int,cnode) = INT(num2);
	}

#if COMB
	/* store as scan number */
	else if(cda_type & SCAN)
	{
		/* construct scan number */
		conscan((char *)0,num1);

		/* store it */
		L_PARAM(Scan,cnode) = curscn_;

		/* put num2 in if range */
		if(SEQ(cnode) == RANGE)
		{
			/* construct scan number */
			conscan((char *)0,num2);

			/* store it */
			L_PARAM2(Scan,cnode) = curscn_;
		}
	}
#endif /*COMB*/

	/* error if string */
	else
	{
		if(unspt[-1] != 1)str -= yyleng;
		nferr("trying to enter string in number");
	}
}

	/*************************/
	/* st_str - store string */
	/*************************/

st_str(strng)
	char *strng;			/* string to be stored in cnode */
{

	/* check that cnode is a parameter */
	if(!L_IS_PARAM(cnode))
	{
		if(unspt[-1] != 1)str -= yyleng;
		nferr("trying to enter string in non-parameter");
	}

	/* check that cnode is correct data type */
	if(!(cda_type & STRING))
	{
		if(unspt[-1] != 1)str -= yyleng;
		nferr("trying to enter string in non-string parameter");
	}

	/* check that string does not overflow parameter */
	if(strlen(strng) >= CLEN(cnode))
	{
		if(unspt[-1] != 1)str -= yyleng;
		sprintf(errstr,"string too long, may only be %d characters",
			CLEN(cnode) - 1);
		nferr(errstr);
	}

	/* store string */
	/* to account for lists of strings, CNT * CLEN is added to the */
	/* character pointer and CNT is subtracted (because L_PARAM */
	/* adds CNT * sizeof(char)) */
	strcpy(&L_PARAM(char,cnode) + CNT(cnode) * (CLEN(cnode) - 1),strng);

	/* save data type */
	USE_TYPE(cnode) = STRING;
}

#if COMB
	/****************************************/
	/* st_scan - store scan number in cnode */
	/****************************************/

st_scan(filnm1,num1,filnm2,num2)
	char *filnm1,*filnm2;		/* file names */
	double num1,num2;		/* scan numbers */
{

	/* check that cnode is a parameter */
	if(!L_IS_PARAM(cnode))
	{
		if(unspt[-1] != 1)str -= yyleng;
		nferr("trying to enter scan number in non-parameter");
	}

	/* check that cnode is a scan number */
	if(!(cda_type & SCAN))
	{
		if(unspt[-1] != 1)str -= yyleng;
		nferr("trying to enter scan number in non-scan number");
	}

	/* construct and store first scan number */
	conscan(filnm1,num1);
	L_PARAM(Scan,cnode) = curscn_;

	/* construct and store second scan number */
	if(SEQ(cnode) == RANGE)
	{
		conscan(filnm2,num2);
		L_PARAM2(Scan,cnode) = curscn_;
	}

	/* save data type */
	USE_TYPE(cnode) = SCAN;
}

	/*************************************/
	/* conscan - construct a scan number */
	/*************************************/

conscan(filnm,num)
	char *filnm;			/* partial file name */
	double num;			/* number */
{
	int fillen;			/* length of partial file name */

	/* if partial file name is given, place it in curscn_ */
	if(filnm)
	{
		fillen = strlen(filnm);
/*		strncpy(curscn_.fil + 3 - fillen,filnm,fillen); */
		strcpy(curscn_.fil + 3 - fillen,filnm);
	}

	/* place number in curscn_ */
	curscn_.num = INT(num);
}
#endif /*COMB*/
