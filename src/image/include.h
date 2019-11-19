/*
 * Structure to keep the names of keywords we use and their position in the
 * imagehdr structure.
 */
#define LOGICAL 0
#define INT 1 
#define STR 2
#define DOUBLE 3
#define IHISTORY 4

struct of_fitskey {
	char keyword[9]; /* the keyword name */
	char type;  /* one of above, e.g. LOGICAL */
	short int index; /* numeric location of the keyword */
	short int sequence; /* order of the keyword */
	short int special; /* special processing instruction:
  	                    * 0  - no special processing needed
			    * 1 to 8 - NAXISn, CTYPEn
			    * 9  - the END card
			    * 10 - output only if non-zero
			    * 11 - used only for stacks
			    */
/* Eventually this should include a description string
 * (const char *) that is output after the trailing /
 */
};
