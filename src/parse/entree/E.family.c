# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"

	/*******************************************************/
	/* family - update family information for a given node */
	/*******************************************************/

family(node)
	int node;		/* node to be updated */
{
	wtree[node].prn = stkpnt->parent;
	wtree[node].brn = stkpnt->branch;
	wtree[stkpnt->parent].chd[stkpnt->branch++] = cnode;
	wtree[node].lin = clin;
}
