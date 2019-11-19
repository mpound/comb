#define MAXVALUE 1e20		/* maximum absolute value plotable */
#define DRAGON dragon_.d	/* value to stay away from in making graphs */
#define FDRAGON dragon_.f	/* these will be set in main.c */
#define iDRAGON (int)(0x80000000) /* indicates an unknown int */
#define FNaN dragon_.fNaN	/* Float quiet NaN */

struct of_dragon {
	float f;
	float fNaN;
	double d;
	double NaN;
};
extern struct of_dragon dragon_;

struct of_constants {
	float  AMU;  /* atomic mass unit  */
	double C;    /* speed o' light, cm/s. */
	double E;    /* natural log base      */
	float  G;    /* gravitational constant, cgs */
	float  K;    /* boltzmann constant, cgs */
	float  MSUN; /* 1 solar mass,   grams */
	double PC;   /* 1 parsec,       cm    */
/* changed from PI to Pi because of conflicts with math.h in 
 * some OS (slackware)
 */
	double Pi;  /* you know, pi */
};

extern struct of_constants constants_;
