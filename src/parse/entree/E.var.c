# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"

	/**********************************************/
	/* variable - find a variable of a given type */
	/**********************************************/

variable(name,typ)
	char *name;		/* name of variable */
	int typ;		/* type of variable */
{
	int type;		/* type being checked */
	int varble;		/* variable being checked */

	/* make sure varaible name is legal */
	if(strlen(name) >= MAXLEN)
		enterr("variable name too long");

	/* check all variable types */
	for(type = 0;type < TYPES;type++)
	{

		/* check all variables of each type */
		for(varble = 0;varble < vars[type];varble++)
		{

			/* make comparison */
			if(!strcmp(name,var[type][varble].name))
			{

				/* variable varble of type type has the */
				/* same name as the one we are looking for */

				/* if this is the right type, return varble */
				if(typ == type)
					return(varble);

				/* if it isn't, give error message */
				else
					enterr("duplicate variable name");
			}
		}
	}

	/* none of the variables match */

	/* create new variable */
	strcpy(var[typ][vars[typ]].name,name);
	var[typ][vars[typ]].chl = -1;
	var[typ][vars[typ]].lln = -1;
	var[typ][vars[typ]].sec = -1;

	/* return it */
	return(vars[typ]++);
}

avars(node,varnm)
	int node;
	char *varnm;
{
	int tcnt;
	int tv;
	int bound,bvar;
	char tvarnm[10];

	bound = da_ecf[wtree[node].dtp];
	for(tcnt = 0;tcnt < TYPES;tcnt++)
	{
		if(da_fce[tcnt] & wtree[node].dtp)
		{
			if(da_fce[tcnt] & wtree[node].sdt)
			{
				tv = wtree[node].var = variable(varnm,tcnt);
			}
			else
			{
				sprintf(tvarnm,"%s%c",varnm,tsub[tcnt]);
				tv = variable(tvarnm,tcnt);
			}
			if(tcnt == bound)
				bvar = tv;
		}
	}
	for(tcnt = 0;tcnt < TYPES;tcnt++)
	{
		if(da_fce[tcnt] & wtree[node].dtp)
		{
			if(da_fce[tcnt] & wtree[node].sdt)
				strcpy(tvarnm,varnm);
			else
				sprintf(tvarnm,"%s%c",varnm,tsub[tcnt]);
			tv = variable(tvarnm,tcnt);
			if(tcnt != bound)
			{
				var[tcnt][tv].sec = bvar;
				var[tcnt][tv].ptp = bound;
			}
			var[tcnt][tv].lln = wtree[node].lln;
			var[tcnt][tv].chl = wtree[node].chl;
		}
	}
}
