# include "E.constant.h"
# include "E.struct.h"
# include "../P.tree.h"
# include "E.extern.h"

wrex()
{
	int node;
	int cnt = 0;
	for(node = 0;node < cnode;node++)
	{
		if(wtree[node].typ == LEAF && wtree[node].fvr)
		{
		switch(wtree[node].dft)
		{
			case OLD:
			case TOGGLE:
				if(!fortrn)
				{
				if(wtree[node].ext)
					rext(da_ecf[wtree[node].sdt],
					     wtree[node].var,
					     wtree[node].evr,
					     &cnt);
				else
					rval(da_ecf[wtree[node].sdt],
					     wtree[node].var,
					     &cnt);
				if(wtree[node].sdt != wtree[node].dtp)
					rof(wtree[node].tfv,&cnt);
				else
					roc(wtree[node].tfv,wtree[node].sdt,&cnt);
				if(wtree[node].lln)
					rof(wtree[node].efv,&cnt);
				}
				else
				if(wtree[node].ext)
					rext(da_ecf[wtree[node].sdt],
					     wtree[node].var,
					     wtree[node].evr,
					     &cnt);
				break;
			case SET:
				if(da_ecf[wtree[node].sdt] == CHRPT)
				{
					char tstr[100];
					sprintf(tstr,"\"%s\"",wtree[node].dfv);
					strcpy(wtree[node].dfv,tstr);
				}
			case AUTOFUN:
				ival(da_ecf[wtree[node].sdt],wtree[node].var,
				     wtree[node].dfv,&cnt);
				roc(wtree[node].tfv,wtree[node].sdt,&cnt);
				if(wtree[node].lln)
					roc(wtree[node].efv,1,&cnt);
				break;
			case AUTO:
			case FLAG:
			case ILLEGAL:
				roc(wtree[node].tfv,wtree[node].sdt,&cnt);
				if(wtree[node].lln)
					roc(wtree[node].efv,1,&cnt);
		}
		}
		if(wtree[node].typ == EXCLSV)
		{
			roc(wtree[node].var,0,&cnt);
		}
	}
	wcall(&cnt);
	for(node = 0;node < cnode;node++)
	{
		if(!fortrn && wtree[node].typ == LEAF && wtree[node].fvr &&
		   (wtree[node].dft == OLD || wtree[node].dft == TOGGLE))
		{
			if(!wtree[node].ext)
				lval(da_ecf[wtree[node].sdt],
				     wtree[node].var,
				     &cnt);
			if(wtree[node].sdt != wtree[node].dtp)
				lof(wtree[node].tfv,&cnt);
			if(wtree[node].lln)
				lof(wtree[node].efv,&cnt);
		}
		if(wtree[node].typ == LEAF &&
		   wtree[node].fvr && wtree[node].ext)
		{
			lext(da_ecf[wtree[node].sdt],
			     wtree[node].var,
			     wtree[node].evr,
			     &cnt);
		}
	}
}

wcall(cnt)
	int *cnt;
{
	if(!fortrn)
	{
		fprnt(efc,EFLLC,cnt,"","	parse_(&%s,%st_,%sb.c);\n",
		      stpars,
		      comnam,
		      comnam);
	}
	else
	{
		fprnt(efc,EFLLC,cnt,"","	call parse(%s,%sd,%sb)\n",
		      stpars,
		      comnam,
		      comnam);
	}
}

rext(typ,vrble,exv,cnt)
	int typ,vrble;
	char *exv;
	int *cnt;
{
	if(var[typ][vrble].rng)
	{
		asvrv(typ,vrble,exv,"1","",cnt);
		asvrv(typ,vrble,exv,"2","",cnt);
	}
	else
		asvrv(typ,vrble,exv,"","",cnt);
}

lext(typ,vrble,exv,cnt)
	int typ,vrble;
	char *exv;
	int *cnt;
{
	if(var[typ][vrble].rng)
	{
		rsvrv(typ,vrble,exv,"1","",cnt);
		rsvrv(typ,vrble,exv,"2","",cnt);
	}
	else
		rsvrv(typ,vrble,exv,"","",cnt);
}


rof(vrbl,cnt)
	int vrbl,*cnt;
{
	if(!fortrn)
		fprnt(efc,EFLLC,cnt,"","	*%s = %s_o;\n",
		      var[SHORT][vrbl].name,
		      var[SHORT][vrbl].name);
}

lof(vrbl,cnt)
	int vrbl,*cnt;
{
	if(!fortrn)
		fprnt(efc,EFLLC,cnt,"","	%s_o = *%s;\n",
		      var[SHORT][vrbl].name,
		      var[SHORT][vrbl].name);
}

roc(vrbl,val,cnt)
	int vrbl,val,*cnt;
{
	if(!fortrn)
		fprnt(efc,EFLLC,cnt,"","	*%s = %d;\n",
		      var[SHORT][vrbl].name,
		      val);
	else
		fprnt(efc,EFLLC,cnt,"","	%s = %d\n",
		      var[SHORT][vrbl].name,
		      val);
}


ival(typ,vrbl,val,cnt)
	int typ,vrbl;
	char *val;
	int *cnt;
{
	char tval[200],*strchr();

	if(var[typ][vrbl].rng)
	{
		strcpy(tval,val);
		*strchr(tval,'_') = 0;
		ascnv(typ,vrbl,"1",tval,cnt);
		ascnv(typ,vrbl,"2",strchr(tval,0) + 1,cnt);
	}
	else
		ascnv(typ,vrbl,"",val,cnt);
}

rval(typ,vrbl,cnt)
	int typ,vrbl,*cnt;
{
	if(var[typ][vrbl].rng)
	{
		asvrv(typ,vrbl,var[typ][vrbl].name,"1","_o",cnt);
		asvrv(typ,vrbl,var[typ][vrbl].name,"2","_o",cnt);
	}
	else
		asvrv(typ,vrbl,var[typ][vrbl].name,"","_o",cnt);
}

lval(typ,vrbl,cnt)
	int typ,vrbl,*cnt;
{
	if(var[typ][vrbl].rng)
	{
		rsvrv(typ,vrbl,var[typ][vrbl].name,"1","_o",cnt);
		rsvrv(typ,vrbl,var[typ][vrbl].name,"2","_o",cnt);
	}
	else
		rsvrv(typ,vrbl,var[typ][vrbl].name,"","_o",cnt);
}

ascnv(typ,vrbl,app,assgn,cnt)
	int typ,vrbl;
	char *app,*assgn;
	int *cnt;
{
	if(!fortrn)
	{
		if(typ == SCNNM)
		{
			fprnt(efc,EFLLC,cnt,"","	*%s%sn = %s;\n",
			      var[typ][vrbl].name,
			      app,
			      assgn + 3);
			fprnt(efc,EFLLC,cnt,"","	strncpy(%s%sf,\"%3.3s\",3);\n",
			      var[typ][vrbl].name,
			      app,
			      assgn);
		}
		else if(typ == CHRPT)
		{
			fprnt(efc,EFLLC,cnt,"","	strcpy(%s%s,%s);\n",
			      var[typ][vrbl].name,
			      app,
			      assgn);
		}
		else
		{
			fprnt(efc,EFLLC,cnt,"","	*%s%s = %s;\n",
			      var[typ][vrbl].name,
			      app,
			      assgn);
		}
	}
	else
	{
		if(typ == SCNNM)
		{
			fprnt(efc,EFLLC,cnt,"","	%s%sn = %s\n",
			      var[typ][vrbl].name,
			      app,
			      assgn + 3);
			fprnt(efc,EFLLC,cnt,"","	%s%sf = \"%3.3s\"\n",
			      var[typ][vrbl].name,
			      app,
			      assgn);
		}
		else if(typ == CHRPT)
		{
			fprnt(efc,EFLLC,cnt,"","	%s%s = %s\n",
			      var[typ][vrbl].name,
			      app,
			      assgn);
		}
		else
		{
			fprnt(efc,EFLLC,cnt,"","	%s%s = %s\n",
			      var[typ][vrbl].name,
			      app,
			      assgn);
		}
	}
}

asvrv(typ,vrbl,assgn,app1,app2,cnt)
	int typ,vrbl;
	char *assgn;
	char *app1,*app2;
	int *cnt;
{
	if(!fortrn)
	{
		if(typ == SCNNM)
		{
			fprnt(efc,EFLLC,cnt,"","	*%s%sn = %s%sn%s;\n",
			      var[typ][vrbl].name,
			      app1,
			      assgn,
			      app1,
			      app2);
			fprnt(efc,EFLLC,cnt,"","	strncpy(%s%sf,%s%sf%s,3);\n",
			      var[typ][vrbl].name,
			      app1,
			      assgn,
			      app1,
			      app2);
		}
		else if(typ == CHRPT)
		{
			fprnt(efc,EFLLC,cnt,"","	strcpy(%s%s,%s%s%s);\n",
			      var[typ][vrbl].name,
			      app1,
			      assgn,
			      app1,
			      app2);
		}
		else
		{
			fprnt(efc,EFLLC,cnt,"","	*%s%s = %s%s%s;\n",
			      var[typ][vrbl].name,
			      app1,
			      assgn,
			      app1,
			      app2);
		}
	}
	else
	{
		if(typ == SCNNM)
		{
			fprnt(efc,EFLLC,cnt,"","	%s%sn = %s%sn%s\n",
			      var[typ][vrbl].name,
			      app1,
			      assgn,
			      app1,
			      app2);
			fprnt(efc,EFLLC,cnt,"","	%s%sf = %s%sf%s\n",
			      var[typ][vrbl].name,
			      app1,
			      assgn,
			      app1,
			      app2);
		}
		else
		{
			fprnt(efc,EFLLC,cnt,"","	%s%s = %s%s%s\n",
			      var[typ][vrbl].name,
			      app1,
			      assgn,
			      app1,
			      app2);
		}
	}
}

rsvrv(typ,vrbl,assgn,app1,app2,cnt)
	int typ,vrbl;
	char *assgn;
	char *app1,*app2;
	int *cnt;
{
	if(!fortrn)
	{
		if(typ == SCNNM)
		{
			fprnt(efc,EFLLC,cnt,"","	%s%sn%s = *%s%sn;\n",
			      assgn,
			      app1,
			      app2,
			      var[typ][vrbl].name,
			      app1);
			fprnt(efc,EFLLC,cnt,"","	strncpy(%s%sf%s,%s%sf,3);\n",
			      assgn,
			      app1,
			      app2,
			      var[typ][vrbl].name,
			      app1);
		}
		else if(typ == CHRPT)
		{
			fprnt(efc,EFLLC,cnt,"","	strcpy(%s%s%s,%s%s);\n",
			      assgn,
			      app1,
			      app2,
			      var[typ][vrbl].name,
			      app1);
		}
		else
		{
			fprnt(efc,EFLLC,cnt,"","	%s%s%s = *%s%s;\n",
			      assgn,
			      app1,
			      app2,
			      var[typ][vrbl].name,
			      app1);
		}
	}
	else
	{
		if(typ == SCNNM)
		{
			fprnt(efc,EFLLC,cnt,"","	%s%sn%s = %s%sn\n",
			      assgn,
			      app1,
			      app2,
			      var[typ][vrbl].name,
			      app1);
			fprnt(efc,EFLLC,cnt,"","	%s%sf%s = %s%sf\n",
			      assgn,
			      app1,
			      app2,
			      var[typ][vrbl].name,
			      app1);
		}
		else
		{
			fprnt(efc,EFLLC,cnt,"","	%s%s%s = %s%s\n",
			      assgn,
			      app1,
			      app2,
			      var[typ][vrbl].name,
			      app1);
		}
	}
}
