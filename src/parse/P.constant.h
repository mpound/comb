	/**********/
	/* tokens */
	/**********/

# define END_T 257
# define NUM_T 258
# define STR_T 259
# define FILNM_T 260
# define NAND_T 261
# define SAND_T 262
# define LN_T 263
# define LOG_T 264
# define SIN_T 265
# define COS_T 266
# define TAN_T 267
# define ATAN_T 268
# define SUB_T 269
# define EXP_T 270
# define ABS_T 271
# define X_T 272
# define Y_T 273
# define CRSR_T 274
# define HDRVAR_T 275
# define STRHDRVAR_T 276
# define EQUEQU_T 277

	/***********************************/
	/* data types for header variables */
	/***********************************/

# define DOUBLE 0
# define FLOAT 1
# define INTEGER 2
# define SHORT 3
# define CHAR 4
# define BIT 5
# define HVSTRING 6
# define SCAN2NUM 7
# define SCAN2STR 8
# define UVNUM 9
# define UVSTR 10
/*
 * Types of memory locations for header variables
 */
#define MAINMEM 0
#define SHMEM 1
