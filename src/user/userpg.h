/*************/
/* Constants */
/*************/

# define MAXSTKNUM 262143	/* maximum stack number */
# define MAXSCNUM 32767		/* maximum scan number */
# define MAXCHN 16224 		/* Max number of channels in a stack */
# define NOTFS -9999		/* flag for refch to indicate that
				the observation was not freq switched */
# define MAXNSTK_ 32		/* size of nstk array in stacks */
# define MAXNSTKX 128		/* size of nstk array in stacks */

/*******************/
/* Data Structures */
/*******************/

struct of_stk			/* structure of stacks in core */
{
	short numpt;			/* number of channels */
	short nver;			/* version number of header */
	short nprc;			/* data type */
	short nplc;			/* preferred plotting coordinates */
	short onstk;			/* old short stack number */
	short nwts;			/* number of weights */
	short locwt[15];		/* locations of weights */
	short ibsln;			/* order of baseline removed */
	char kuse[64];			/* use array */
	short nstk[MAXNSTK_];		/* list of scan numbers */
	short nlist;			/* number of scan numbers in list */
	short indx;			/* last location in nstk */
	short isdum[38];		/* not used */
	int numst;			/* stack number */
	float fwid;			/* filter width */
	float expch;			/* expected channel */
	float refch;			/* expected line center */
	float vlsr;			/* LSR velocity of expch */
	float exwid;			/* expected line width */
	float wght[15];			/* weights */
	float factt;			/* fudge factor */
	float time;			/* integration time */
	float pwrav;			/* average pwer per channel */
	float pwrer;			/* rms error in pwrav */
	float rms;			/* rms of channels in use array */
	float tmax;			/* peak temperature */
	float arerr;			/* error in the area */
	float area;			/* total area under the line */
	float vcent;			/* Velocity of the centroid */
	float fwhm;			/* FWHM or FWZP */
	float fwerr;			/* fitting error in fwhm */
	float fpeak;			/* velocity of peak of curve from */
					/* gaussian fit */
	float fperr;			/* fitting error in fpeak */
	float ftmax;			/* temperature at peak of gaussian */
					/* fit */
	float fterr;			/* fitting error in ftmax */
	float rmval;			/* temp subtracted from center chan */
	float ra;			/* ra of map center */
	float dec;			/* declination of map center */
	float l;			/* longitude of map center */
	float b;			/* latitude of map center */
	float dra;			/* ra offset */
	float ddec;			/* dec offset */
	float dl;			/* latitude offset */
	float db;			/* longitude offset */
	float dx;			/* horizontal user offset */
	float dy;			/* vertical user offset */
	float odra;			/* ra observing offset */
	float oddec;			/* dec observing offset */
	float epoch;			/* year of ra/dec coordinate system */
	float vpeak;			/* velocity of tmax in in */
	float rsdm[12];			/* not used */
	double freq;			/* assumed line frequency */
	float stak[MAXCHN];		/* values in stack */
	short nstkx[MAXNSTKX];		/* extension of scan number list */
};

/********************/
/* Global Variables */
/********************/

extern struct of_stk stk_;	/* stack in core */
