# include "E.constant.h"
# include "E.struct.h"
# include "E.extern.h"

	/***********************************/
	/* flocs - find variable locations */
	/***********************************/

flocs()
{
	int type;		/* type of variable being found */
	int sz;			/* size of variable type */
	int loc = 0;		/* current location */
	int cnt;		/* counter for variable */

	/* loop through all variable types */
	for(type = 0;type < TYPES;type++)
	{

		/* find the location of each variable of that type */
		for(cnt = 0;cnt < vars[type];cnt++)
		{
			if(var[type][cnt].sec == -1)
			{
				var[type][cnt].loc = loc;
				sz = siz[type];
				if(sz < var[type][cnt].chl + 1)
					sz = var[type][cnt].chl + 1;
				if(var[type][cnt].lln > 0)
					sz *= var[type][cnt].lln;
				if(var[type][cnt].rng)
					sz *= 2;
				if(sz / siz[type] != (float)(sz) / siz[type])
					sz = siz[type] * ((int)(sz / siz[type])
					     + 1);
				loc += sz;
			}
		}
	}

	/* loop through all variable types again and find the locations */
	/* of variables that are secondary data types */
	for(type = 0;type < TYPES;type++)
	{
		for(cnt = 0;cnt < vars[type];cnt++)
		{
			if(var[type][cnt].sec != -1)
			{
				var[type][cnt].loc =
				   var[var[type][cnt].ptp][var[type][cnt].sec].loc;
			}
		}
	}

	/* return number of bytes needed in buffer */
	return(loc);
}
