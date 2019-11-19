	/**************************************/
	/* macros for accessing related nodes */
	/**************************************/

/* pointer to parent of node x */
# define PARENT(x) ((mt1.n = (x)) == root ? ferr(parent_err)->n : tree + mt1.n->parent)

/* pointer to child y of node x */
# define CHILD(x,y) ((mt1.n = (x))->type == LEAF ?\
		    ferr(chld_err)->n :\
		    (mt1.n->type == NORMAL ?\
		    ((mt2.i = mt1.n->info[y]) == 0 ? 0 : tree + mt2.i) :\
		    ((mt2.i = mt1.n->info[(y) + 4]) == 0 ? 0 : tree + mt2.i)))

	/*******************************************************/
	/* macros for accessing data unique to exclusive nodes */
	/*******************************************************/

/* parameter for exclusive node x */
# define E_PARAM(x) *((mt1.n = (x))->type == EXCLSV ?\
		    (short *)(pbuf + *(short *)(mt1.n->info)):\
		    ferr(exc_err)->shp)

/* flag for exclusive node x */
# define E_FLAG(x) *((mt1.n = (x))->type == EXCLSV ?\
		   (short *)(pbuf + *(short *)(mt1.n->info + 2)):\
		   ferr(exc_err)->shp)

/* parameter flag for exclusive node x (1 if x has a parameter, 0 if not) */
# define E_IS_PARAM(x) ((mt1.n = (x))->type == EXCLSV ?\
		     (*(short *)(mt1.n->info) == -1 ? 0:1):\
		     ferr(exc_err)->st)

/* flag flag for exclusive node x (1 if x has a flag, 0 if not) */
# define E_IS_FLAG(x) ((mt1.n = (x))->type == EXCLSV ?\
		      (*(short *)(mt1.n->info + 2) == -1 ? 0:1):\
		      ferr(exc_err)->i)

	/**********************************************/
	/* macros for accessing data unique to leaves */
	/**********************************************/

/* parameter of type y for leaf x */
# if 0
/* This is Mat's original code */
# define L_PARAM(y,x) (*(y *)(pbuf +\
		      *(short *)((mt1.n = (x))->info) + \
			(SEQ(x) == RANGE ?\
			CNT(mt1.n) * sizeof(y) * 2 :\
			CNT(mt1.n) * sizeof(y))))
# endif /*0*/
# define L_PARAM(y,x) (*(y *)(pbuf +\
		      *(short *)( (x)->info) + \
			(SEQ(x) == RANGE ?\
			CNT((x)) * sizeof(y) * 2 :\
			CNT((x)) * sizeof(y))))

/* second parameter of type y for leaf x (must be a range) */
# define L_PARAM2(y,x) (*((mt1.n = (x))->type == LEAF ?\
		       (SEQ(mt1.n) == RANGE ?\
		       (y *)(pbuf + *(short *)(mt1.n->info) +\
			sizeof(y) * CNT(x) * 2 + sizeof(y)):\
		       (y *)(ferr(rnge_err))) :\
		       (y *)(ferr(leaf_err))))

/* flag for leaf x */
# define L_FLAG(x) *((mt1.n = (x))->type == LEAF ?\
		   (short *)(pbuf + *(short *)(mt1.n->info + 2)):\
		   ferr(leaf_err)->shp)

/* data type of leaf x */
# define DA_TYPE(x) *((mt1.n = (x))->type == LEAF ?\
		    mt1.n->info + 4:\
		    (unsigned char *)ferr(leaf_err)->st)

/* sequence type of leaf x */
# define SEQ(x) *((mt1.n = (x))->type == LEAF ?\
		mt1.n->info + 5:\
		(unsigned char *)ferr(leaf_err)->st)

/* number of characters in string */
# define CLEN(x) *((mt1.n = (x))->type == LEAF ?\
		 (short *)(mt1.n->info + 6):\
		 ferr(leaf_err)->shp)

/* length of leaf x (# of characters in string or # of elements in list) */
# define LEN(x) *((mt1.n = (x))->type == LEAF ?\
		(short *)(mt1.n->info + 8):\
		ferr(leaf_err)->shp)

/* counter for number of elements in list */
# define CNT(z) *(short *)((z)->info + 10)

/* default type of leaf x */
# define DF_TYPE(x) *((mt1.n = (x))->type == LEAF ?\
		    mt1.n->info + 12:\
		    (unsigned char *)ferr(leaf_err)->st)

/* parameter flag of leaf x (1 if x is a parameter, 0 if not) */
# define L_IS_PARAM(x) ((mt1.n = (x))->type == LEAF ?\
		     (*(short *)(mt1.n->info) == -1 ? 0:1):\
		     ferr(leaf_err)->i)

/* flag flag for leaf x (1 if x has a flag, 0 if not) */
# define L_IS_FLAG(x) ((mt1.n = (x))->type == LEAF ?\
		      (*(short *)(mt1.n->info + 2) == -1 ? 0:1):\
		      ferr(leaf_err)->i)

/* external flag of leaf x (1 if x is external, 0 if not) */
# define IS_EXTERN(x) *((mt1.n = (x))->type == LEAF ?\
		   mt1.n->info + 13:\
		   (unsigned char *)ferr(leaf_err)->st)

/* external name of leaf x */
# define EXTERN(x) ((mt1.n = (x))->type == LEAF ?\
		     strchr(mt1.n->des,0) + 1:\
		     ferr(leaf_err)->st)

/* data type actually used */
# define USE_TYPE(x) *(short *)(pbuf + *(short *)((mt1.n = (x))->info + 14))

/* number of elements used in list */
# define ELMNTS(x) *((mt1.n = (x))->type == LEAF ?\
		   (short *)(pbuf + *(short *)(mt1.n->info + 16)):\
		   ferr(leaf_err)->shp)

/* whether or not the leaf has had a value entered for it */
# define DFLTD(x) *((mt1.n = (x))->type == LEAF ?\
		  mt1.n->info + 18:\
		  (unsigned char *)ferr(leaf_err)->st)

	/***********************************************************/
	/* macros for maintaining account of the current situation */
	/***********************************************************/

/* begin a new parameter */
# define START if(errflag){cnode = nnode;\
		cda_type = DA_TYPE(cnode);\
		sterr = str - (yyleng + (unspt - unstk));\
		strpos = strbuf;\
		errflag = 0;}

/* begin a new special character */
# define SPCHAR if(errflag){sterr = str - (yyleng + (unspt - unstk));\
		cnode = nnode;}\
		else errflag = 1;

	/**************/
	/* lex macros */
	/**************/

/* input a character from str */
# define input() (unspt > unstk ?\
		 (*--unspt) :\
		 (*str++ == 0 ? ';' : str[-1]))

/* push a character on the unput buffer */
# define unput(c)  *unspt++ = (c);

	/*********/
	/* misc. */
	/*********/

/* round a double */
# if 0
/* Matt's original version */
# define INT(x) (mt2.d = (x),irondd_(&mt2.d))
# endif /*0*/
# define INT(x) round(x)

/* convert the location of a header variable to an appropriate type */
# define A(x) (int)(&x)
