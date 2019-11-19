# include <string.h>
# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"
# include "../P.tree.h"

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

static void lvarF P_((int wnode, char *app, int *cnt));
static void lvalF P_((int wnode, int *cnt));

#undef P_

void pvarsC();
	/**************************************/
	/* wrdef - write out definitions file */
	/**************************************/

wrdef()
{
	int cnt;		/* counter for fprnt */
	int typ;		/* type counter */
	int psflag;		/* flag for whether or not "static short" */
				/* has been printed */
	int ncnt;		/* node counter */
	char val[300];

	/* tree description and parameter buffer */
	if(!fortrn)
	{
		fprnt(dfc,DFLLC,&cnt,"","extern Node %st_[];\n",comnam);
		fprintf(dfc,"union {\n\tchar c[%d];\n\tshort int s[1];\n\
	int l[1];\n\tdouble d[1];\n} %sb;\n",lastloc, comnam);
	}
	else
	{
		fprnt(dfc,DFLLF,&cnt,"","	common /%st/%sd(8)\n",
			comnam,comnam);
		fprnt(dfc,DFLLF,&cnt,"","	integer %sd\n",comnam);
		fprnt(dfc,DFLLF,&cnt,"","	character %sb(%d)\n",comnam,
		      (lastloc > 1)? lastloc - 1: 1);
	}

	/* variables */
	if(!fortrn)
	{
		for(typ = 0;typ < TYPES;typ++)
		{
			if(vars[typ])
			if(typ == SCNNM)
			{
				pvarsC(CHRPT,"f",0,typ);
				pvarsC(SHORT,"n",4,typ);
			}
			else
			{
				pvarsC(typ,"",0,typ);
			}
		}
	}
	else
	{
		for(typ = 0;typ < TYPES;typ++)
		{
			if(vars[typ])
			if(typ == SCNNM)
			{
				fprnt(dfc,DFLLF,&cnt,"","	character*3 ");
				pvarsF("f",SCNNM,&cnt);
				fprnt(dfc,DFLLF,&cnt,"","	integer*2 ");
				pvarsF("n",SCNNM,&cnt);
			}
			else if(typ == CHRPT)
			{
				int vcnt;
				for(vcnt = 0;vcnt < vars[typ];vcnt++)
				{
					fprnt(dfc,DFLLF,&cnt,"","	character*%d %s",
					      var[typ][vcnt].chl,
					      var[typ][vcnt].name);
					if(var[typ][vcnt].lln > 0)
						fprnt(dfc,DFLLF,&cnt,"","(%d)",
						      var[typ][vcnt].lln);
					fprnt(dfc,DFLLF,&cnt,"","\n");
				}
			}
			else
			{
				fprnt(dfc,DFLLF,&cnt,"","	%s ",ftnam[typ]);
				pvarsF("",typ,&cnt);
			}
		}
		for(typ = 0;typ < TYPES;typ++)
		{
			if(vars[typ])
			{
			fprnt(dfc,DFLLF,&cnt,"","\n	equivalence ");
			if(typ == SCNNM)
			{
				equF("f",0,SCNNM,&cnt);
				fprnt(dfc,DFLLF,&cnt,"     *  ",",");
				equF("n",4,SCNNM,&cnt);
			}
			else
				equF("",0,typ,&cnt);
			}
		}
		fprnt(dfc,DFLLF,&cnt,"","\n");
	}

	/* old values of typeflag and elements */
	if(!fortrn)
	{
		psflag = 0;
		for(ncnt = 0;ncnt < cnode;ncnt++)
		{
			if(wtree[ncnt].typ == LEAF && wtree[ncnt].fvr &&
			   wtree[ncnt].dtp != wtree[ncnt].sdt &&
			   wtree[ncnt].dft == OLD)
			{
				if(!psflag)
				{
					fprnt(dfc,DFLLC,&cnt,"","static short ");
					psflag = 2;
				}
				if(psflag != 2)
					fprnt(dfc,DFLLC,&cnt,DFSTC,",");
				psflag = 1;
				fprnt(dfc,DFLLC,&cnt,DFSTC,"%s_o = %d",
					var[SHORT][wtree[ncnt].tfv].name,
					wtree[ncnt].sdt);
			}
			if(wtree[ncnt].lln > 0)
			{
				if(!psflag)
				{
					fprnt(dfc,DFLLC,&cnt,"","static short ");
					psflag = 2;
				}
				if(psflag != 2)
					fprnt(dfc,DFLLC,&cnt,DFSTC,",");
				psflag = 1;
				fprnt(dfc,DFLLC,&cnt,DFSTC,"%s_o = 1",
					var[SHORT][wtree[ncnt].efv].name);
			}
		}
		if(psflag)
			fprnt(dfc,DFLLC,&cnt,DFSTC,";\n");
	}
	else
	{
		psflag = 0;
		for(ncnt = 0;ncnt < cnode;ncnt++)
		{
			if(wtree[ncnt].typ == LEAF && wtree[ncnt].fvr &&
			   (wtree[ncnt].dft == OLD || wtree[ncnt].dft == TOGGLE)
			   && wtree[ncnt].dtp != SCAN)
			{
				if(!psflag)
				{
					fprnt(ifc,DFLLF,&cnt,"","	data ");
					psflag = 2;
				}
				if(psflag != 2)
					fprnt(ifc,DFLLF,&cnt,"     *  ",",");
				psflag = 1;
				fprnt(ifc,DFLLF,&cnt,"     *  ","%s",
				      var[SHORT][wtree[ncnt].tfv].name);
				if(wtree[ncnt].lln > 0)
				{
					fprnt(ifc,DFLLF,&cnt,"      *  ",",%s",
						var[SHORT][wtree[ncnt].efv].name);
					lvarF(ncnt,"(1)",&cnt);
				}
				else
					lvarF(ncnt,"",&cnt);
			}
		}
		if(psflag)
		{
		fprnt(ifc,DFLLF,&cnt,"     *  ","/");
		psflag = 2;
		for(ncnt = 0;ncnt < cnode;ncnt++)
		{
			if(wtree[ncnt].typ == LEAF && wtree[ncnt].fvr &&
			   (wtree[ncnt].dft == OLD || wtree[ncnt].dft == TOGGLE)
			   && wtree[ncnt].sdt != SCAN)
			{
				if(psflag != 2)
					fprnt(ifc,DFLLF,&cnt,"     *  ",",");
				psflag = 1;
				fprnt(ifc,DFLLF,&cnt,"     *  ","%d",
				      wtree[ncnt].sdt);
				if(wtree[ncnt].lln > 0)
				{
					fprnt(ifc,DFLLF,&cnt,"     *  ",",1");
				}
				lvalF(ncnt,&cnt);
			}
		}
		fprnt(ifc,DFLLF,&cnt,"     *  ","/\n");
		}
	}

	/* old values for old default types */
	if(!fortrn)
	{
		for(ncnt = 0;ncnt < cnode;ncnt++)
		{
			if(wtree[ncnt].typ == LEAF &&
			   wtree[ncnt].fvr &&
			   !wtree[ncnt].ext &&
			   (wtree[ncnt].dft == OLD ||
			   wtree[ncnt].dft == TOGGLE) &&
			   wtree[ncnt].sdt != SCAN)
			{
				if(da_ecf[wtree[ncnt].sdt] == CHRPT)
				{
					fprnt(dfc,DFLLC,&cnt,"","static char %s_o[%d] = \"%s\";\n",
					var[CHRPT][wtree[ncnt].var].name,
					wtree[ncnt].chl + 1,
					wtree[ncnt].dfv);
				}
				else
					if(!var[da_ecf[wtree[ncnt].dtp]][wtree[ncnt].var].rng)
					fprnt(dfc,DFLLC,&cnt,"","static %s %s_o = %s;\n",
					ctnam[da_ecf[wtree[ncnt].dtp]],
					var[da_ecf[wtree[ncnt].dtp]][wtree[ncnt].var].name,
					wtree[ncnt].dfv);
					else
					{
					strcpy(val,wtree[ncnt].dfv);
					*strchr(val,'_') = 0;
					fprnt(dfc,DFLLC,&cnt,"","static %s %s1_o = %s,",
					ctnam[da_ecf[wtree[ncnt].dtp]],
					var[da_ecf[wtree[ncnt].dtp]][wtree[ncnt].var].name,
					val);
					fprnt(dfc,DFLLC,&cnt,"","%s2_o = %s;\n",
					var[da_ecf[wtree[ncnt].dtp]][wtree[ncnt].var].name,
					strchr(val,0) + 1);
					}
			}
		}
	}
}

	/**********************************************/
	/* pvarsC - print variable definitions in 'C' */
	/**********************************************/

void pvarsC(trueType,app,add,typ)
	int trueType;			/* Actual type in case of scannum */
	char *app;			/* string to be appended to variable */
	int add;			/* number to be added to location */
	int typ;			/* type of variable to print */
{
	int vr;				/* variable counter */
	static char memNam[] = "dl sc";	/* Names of members of comnam struct */
	static char lengths[] = {	/* Lengths of basic types */
		sizeof(double),
		sizeof(int),
		0,
		sizeof(short),
		sizeof(char)
	};

	/* loop through all variables of type typ */
	for(vr = 0;vr < vars[typ];vr++)
	{

		/* if variable is range, print both 1 & 2 */
		if(var[typ][vr].rng)
		{
			fprintf(dfc,"#define %s1%s (%sb.%c + %d)\n",
				var[typ][vr].name,
				app,
				comnam,
				memNam[trueType],
				(var[typ][vr].loc + add) / lengths[trueType]);
			fprintf(dfc,"#define %s2%s (%sb.%c + %d)\n",
				var[typ][vr].name,
				app,
				comnam,
				memNam[trueType],
				(var[typ][vr].loc +siz[typ]+ add) /
					lengths[trueType]);
		}

		/* otherwise, just print one variable */
		else
		{
			fprintf(dfc,"#define %s%s (%sb.%c + %d)\n",
				var[typ][vr].name,
				app,
				comnam,
				memNam[trueType],
				(var[typ][vr].loc + add) / lengths[trueType]);
		}
	}
}

pvarsF(app,typ,cnt)
	char *app;
	int typ;
	int *cnt;
{
	int vrcnt;
	for(vrcnt = 0;vrcnt < vars[typ];vrcnt++)
	{
		if(vrcnt)
			fprnt(dfc,DFLLF,cnt,"     *  ",",");
		if(var[typ][vrcnt].rng)
		{
			fprnt(dfc,DFLLF,cnt,"     *  ","%s1%s",
			      var[typ][vrcnt].name,
			      app);
			if(var[typ][vrcnt].lln > 0)
			{
				fprnt(dfc,DFLLF,cnt,"     *  ","(%d)",
				      typ == SCNNM ?
				      (*app == 'f' ?
					var[typ][vrcnt].lln * 4 :
					var[typ][vrcnt].lln * 6) :
				      var[typ][vrcnt].lln * 2);
			}
			fprnt(dfc,DFLLF,cnt,"     *  ",",%s2%s",
			      var[typ][vrcnt].name,
			      app);
			if(var[typ][vrcnt].lln > 0)
			{
				fprnt(dfc,DFLLF,cnt,"     *  ","(%d)",
				      typ == SCNNM ?
				      (*app == 'f' ?
					var[typ][vrcnt].lln * 4 :
					var[typ][vrcnt].lln * 6) :
				      var[typ][vrcnt].lln * 2);
			}
		}
		else
		{
			fprnt(dfc,DFLLF,cnt,"     *  ","%s%s",
			      var[typ][vrcnt].name,
			      app);
			if(var[typ][vrcnt].lln > 0)
			{
				fprnt(dfc,DFLLF,cnt,"     *  ","(%d)",
				      typ == SCNNM ?
				      (*app == 'f' ?
					var[typ][vrcnt].lln * 2 :
					var[typ][vrcnt].lln * 3) :
				      var[typ][vrcnt].lln);
			}
		}
	}
	fprnt(dfc,DFLLF,cnt,"","\n");
}

equF(app,add,typ,cnt)
	char *app;
	int add,typ,*cnt;
{
	int vrcnt;
	for(vrcnt = 0;vrcnt < vars[typ];vrcnt++)
	{
		if(vrcnt)
			fprnt(dfc,DFLLF,cnt,"     *  ",",");
		if(var[typ][vrcnt].rng)
		{
			fprnt(dfc,DFLLF,cnt,"     *  ","(%s1%s,%sb(%d)),",
			      var[typ][vrcnt].name,
			      app,
			      comnam,
			      var[typ][vrcnt].loc + add + 1);
			fprnt(dfc,DFLLF,cnt,"     *  ","(%s2%s,%sb(%d))",
			      var[typ][vrcnt].name,
			      app,
			      comnam,
			      var[typ][vrcnt].loc + siz[typ] + add + 1);
		}
		else
			fprnt(dfc,DFLLF,cnt,"     *  ","(%s%s,%sb(%d))",
			      var[typ][vrcnt].name,
			      app,
			      comnam,
			      var[typ][vrcnt].loc + add + 1);
	}
}

static void lvarF(wnode,app,cnt)
	int wnode;
	char *app;
	int *cnt;
{
	if(wtree[wnode].ext)
		return;
	fprnt(ifc,DFLLF,cnt,"     *  ",",");
	if(wtree[wnode].rng)
	{
		if(wtree[wnode].sdt == SCAN)
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","%s1f%s,",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
			fprnt(ifc,DFLLF,cnt,"     *  ","%s1n%s,",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
			fprnt(ifc,DFLLF,cnt,"     *  ","%s2f%s,",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
			fprnt(ifc,DFLLF,cnt,"     *  ","%s2n%s",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
		}
		else
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","%s1%s,",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
			fprnt(ifc,DFLLF,cnt,"     *  ","%s2%s",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
		}
	}
	else
	{
		if(wtree[wnode].sdt == SCAN)
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","%sf%s,",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
			fprnt(ifc,DFLLF,cnt,"     *  ","%sn%s",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
		}
		else
			fprnt(ifc,DFLLF,cnt,"     *  ","%s%s",
			      var[da_ecf[wtree[wnode].sdt]][wtree[wnode].var].name,
			      app);
	}
}

static void lvalF(wnode,cnt)
	int wnode,*cnt;
{
	char tval[200];
	if(wtree[wnode].ext)
		return;
	fprnt(ifc,DFLLF,cnt,"     *  ",",");
	strcpy(tval,wtree[wnode].dfv);
	if(wtree[wnode].rng)
	{
		*strchr(tval,'_') = 0;
		if(wtree[wnode].sdt == SCAN)
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","\"%3.3s\",%s",
			      tval,
			      tval + 3);
			fprnt(ifc,DFLLF,cnt,"     *  ",",\"%3.3s\",%s",
			      strchr(tval,0) + 1,
			      strchr(tval,0) + 4);
		}
		else
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","%s,%s",
			      tval,
			      strchr(tval,0) + 1);
		}
	}
	else
	{
		if(wtree[wnode].sdt == SCAN)
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","\"%3.3s\",%s",
			      tval,
			      tval + 3);
		}
		else if(wtree[wnode].sdt & STRING)
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","\"%s\"",
			      tval);
		}
		else
		{
			fprnt(ifc,DFLLF,cnt,"     *  ","%s",
			      tval);
		}
	}
}
