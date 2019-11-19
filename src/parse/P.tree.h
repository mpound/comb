/* node types */

# define EXCLSV 1			/* exclusive */
# define LEAF 2				/* leaf */
# define NORMAL 3			/* normal */

/* data types */

# define CMND 1				/* command string */
# define EXPR 2				/* expression string */
# define STRING 3			/* character string */
# define SCAN 4				/* scan number */
# define INTN 8				/* integer */
# define REAL 16				/* real number */

/* sequence types */

# define SINGLE 1			/* single element */
# define RANGE 2			/* range */

/* default types */

# define OLD 1				/* default to old value */
# define TOGGLE 2			/* toggle value (1,0) with label */
# define SET 3				/* preset default value */
# define AUTO 4				/* default automatically */
# define AUTOFUN 5			/* default preset automatically */
# define FLAG 6				/* default to 1 with label */
# define ILLEGAL 7			/* may not default */

/* byte conversions */
#include "../main/machinedep.h"
	/* short integer to two bytes */
# if BYTEREVERSED
# define Btwo(x) (x) & 255, 255 & ((x) >> 8)
# else
# define Btwo(x) ((x) >> 8) & 255, (x) & 255
# endif /*BYTEREVERSED*/

#ifndef NODEDEFINED
/* tree structure */

/* This is also in ../main/C.h, but it is needed in all commands
 * and this is the easiest way to get it in and not include extras */
typedef struct
{
	char *des;			/* description of node */
	char *label;			/* label of node */
	char type;			/* type of node */
	char parent;			/* parent of node */
	char branch;			/* branch number of node */
	char extra;			/* so info will start on an even byte */
	unsigned char info[32];		/* different for different node types:
						normal - list of children
							 single byte for each
							 child
						exclusive - location of var-
							    iable, location of
							    flag, list of
							    children.  Loc-
							    ations are 2 bytes
							    long.
						leaf - location of parameter,
						       location of flag,
						       data type (1 byte),
						       charlength (2 bytes),
						       sequence (1 byte),
						       listlength (2 bytes),
						       counter (2 bytes),
						       default type (1 byte),
						       external flag (1 byte),
						       location of type flag,
						       location of elements,
						       defaulted flag.
					*/
} Node;
#define NODEDEFINED
#endif /*NODEDEFINED*/
