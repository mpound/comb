# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"
# include "../P.tree.h"

	/*******************************/
	/* wrtre - write out .t.c file */
	/*******************************/

wrtre()
{
	int cnt = 0;			/* counter for fprnt */
	int ncnt;			/* node counter */
	int ccnt;			/* child counter */

	/* define global variable */
	fprnt(tfc,TFLL,&cnt,"","# include \"../../parse/P.tree.h\"\n");
	fprnt(tfc,TFLL,&cnt,"","\nNode %st_[] =\n",comnam);
	fprnt(tfc,TFLL,&cnt,"","{\n");

	/* print all nodes */
	for(ncnt = 0;ncnt < cnode;ncnt++)
	{

		/* initialize line */
		fprnt(tfc,TFLL,&cnt,"","/*%03d*/ ",ncnt);

		/* print correct format given node type */
		switch(wtree[ncnt].typ)
		{

			/* normal */
			case NORMAL:
				fprnt(tfc,TFLL,&cnt,TFST,"\"%s\",",
					wtree[ncnt].des);
				fprnt(tfc,TFLL,&cnt,TFST,"\"%s\",",
					wtree[ncnt].lbl);
				fprnt(tfc,TFLL,&cnt,TFST,"NORMAL,");
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].prn);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].brn);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",0);
				fprnt(tfc,TFLL,&cnt,TFST,"{");
				ccnt = 0;
				while(wtree[ncnt].chd[ccnt])
					fprnt(tfc,TFLL,&cnt,TFST,"%d,",
						wtree[ncnt].chd[ccnt++]);
				fprnt(tfc,TFLL,&cnt,TFST,"0");
				fprnt(tfc,TFLL,&cnt,TFST,"}");
				break;

			/* exclusive */
			case EXCLSV:
				fprnt(tfc,TFLL,&cnt,TFST,"\"%s\",",
					wtree[ncnt].des);
				fprnt(tfc,TFLL,&cnt,TFST,"\"\",");
				fprnt(tfc,TFLL,&cnt,TFST,"EXCLSV,");
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].prn);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].brn);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",0);
				fprnt(tfc,TFLL,&cnt,TFST,"{");
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].fvr ?
					var[SHORT][wtree[ncnt].var].loc :
					-1);
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].ffl ?
					var[SHORT][wtree[ncnt].flg].loc :
					-1);
				ccnt = 0;
				while(wtree[ncnt].chd[ccnt])
					fprnt(tfc,TFLL,&cnt,TFST,"%d,",
						wtree[ncnt].chd[ccnt++]);
				fprnt(tfc,TFLL,&cnt,TFST,"0");
				fprnt(tfc,TFLL,&cnt,TFST,"}");
				break;

			/* leaf */
			case LEAF:
				fprnt(tfc,TFLL,&cnt,TFST,"\"%s%s%s\",",
					wtree[ncnt].des,
					wtree[ncnt].ext ? "\\0" : "",
					wtree[ncnt].ext ?
					wtree[ncnt].eds :
					"");
				fprnt(tfc,TFLL,&cnt,TFST,"\"%s\",",
					wtree[ncnt].lbl);
				fprnt(tfc,TFLL,&cnt,TFST,"LEAF,");
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].prn);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].brn);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",0);
				fprnt(tfc,TFLL,&cnt,TFST,"{");
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].fvr ?
					var[da_ecf[wtree[ncnt].dtp]][wtree[ncnt].var].loc :
					-1);
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].ffl ?
					var[SHORT][wtree[ncnt].flg].loc :
					-1);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].dtp);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].rng + 1);
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].chl + 1);
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].lln);
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(0),");
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].dft);
				fprnt(tfc,TFLL,&cnt,TFST,"%d,",
					wtree[ncnt].ext);
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].fvr ?
					var[SHORT][wtree[ncnt].tfv].loc :
					0);
				fprnt(tfc,TFLL,&cnt,TFST,"Btwo(%d),",
					wtree[ncnt].lln ?
					var[SHORT][wtree[ncnt].efv].loc :
					0);
				fprnt(tfc,TFLL,&cnt,TFST,"0");
				fprnt(tfc,TFLL,&cnt,TFST,"}");
		}

		/* finish line */
		if(ncnt + 1 < cnode)
			fprnt(tfc,TFLL,&cnt,TFST,",");
		fprnt(tfc,TFLL,&cnt,"","\n");
	}

	/* finish definition */
	fprnt(tfc,TFLL,&cnt,"","};\n");
}
