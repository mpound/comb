/*************/
/* Constants */
/*************/

# ifndef M_PI
#   define M_PI 3.14159265358979323846
# endif

# define SPHERERAD 536870912		/* radius of celestial sphere (2^29) */
# define EPOCH 1950.0			/* Standard epoch for XYZPOINTs */
# define RADEC 0			/* right ascension, declination */
# define LB 1				/* galactic coordinates */
# define DXDY 2				/* user defined coordinates */
		/* The following types of units should have consecutive values.
		Coord.c has an array of conversion factors indexed by them */
# define DEGREES 0x0			/* degrees */
# define RADIANS 0x1			/* radians */
# define HOURS 0x2			/* hours */
# define ARCMINUTES 0x3			/* arc minutes */
# define ARCSECONDS 0x4			/* arc seconds*/
# define NUMUNITS 5			/* used for error checking in coords */
# define SCALED 0x10			/* horizontal offset is scaled */
					/* (to be ored with DEGREES, */
					/* RADIANS, HOURS, or ARCMINUTES) */
/* additional units are defined in image/C.h */

# define CLIP 0x7fffffff		/* clipped projection */
# define CSYS 0				/* project into current coordinate */
					/* system */
# define ORTHO 1			/* orthographic projection */
# define MERCATOR 2			/* mercator projection */
# define GNOMONIC 3			/* project onto tangent plane and make
					   fits tape with both axes reversed */
# define POLAR 4			/* polar projection */
# define EQAREA 5			/* equal area projection */
# define CSNLEN 23			/* coordinate system name length */

/**************/
/* Data types */
/**************/

typedef struct				/* two dimensional point */
{
	double x;				/* horizontal */
	double y;				/* vertical */
}
XYPOINT;

typedef struct				/* three dimensional point */
{
	double x;				/* horizontal */
	double y;				/* depth */
	double z;				/* vertical */
}
XYZPOINT;

/* # include "include.h" */

struct for_csys				/* Current relative coordinate system */
{
	double mat[9];			/* Rotation matrix */
	double x,y;			/* Lon,lat coords of center (radians) */
	float epoch;			/* epoch for rel coords if RADEC */
	char rotate;			/* Non zero if rotation is needed */
	char type;			/* Type of rel coords */
	char xun,yun;			/* Units for rel coordinates */
};

typedef struct				/* Structure to define and store a
					rel coord sys in */
{
	char name[CSNLEN + 1];		/* Name of rel coord sys */
	double x,y;			/* Lon,lat type coords of center */
	float cepoch,oepoch;		/* Center,offset epoch if RADEC */
	double rot;			/* Position angle of the y axis */
	char ctp,otp;			/* Type of center, offset coord */
	char xun,yun;			/* Units to be used for x,y offsets */
	char dmy[4];			/* Fill the struct to 8 byte boundary */
}
RelCoordSys;

struct for_cproj		/* Current projection */
{
	double mat[9];		/* Matrix to rotate to center of csys */
	int type;		/* Type of proj ie. ORTHO,MERCATOR, etc */
};

extern struct for_csys csys;
extern RelCoordSys rcsys;
extern struct for_cproj cproj;
extern double minRcX, maxRcX;		/* Controls adding +/- 2*PI to x */
extern char unitNames[][8],horizCoordNames[][4],vertCoordNames[][4];
extern char projNames[][6];

/* end of old include.h */

/* half of the space occupied by the celestial sphere (2^30) */
# define HALFSPACE 1073741824

extern double cunits[];		/* Array to hold units conversion factors */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* coord.c */
XYZPOINT CsysToXyz P_((XYPOINT p));
XYPOINT CsysToAbs P_((XYPOINT p));
XYPOINT XyzToCsys P_((XYZPOINT p));
void SetRcXLimits P_((double x1, double x2));
double XyDistance P_((XYPOINT p1, XYPOINT p2));
XYZPOINT RaDecToXyz P_((double ra, double dec, double epoch));
void XyzToRaDec P_((XYZPOINT *p1, double epoch, double *ra, double *dec));
XYZPOINT LbToXyz P_((double l, double b));
void XyzToLb P_((XYZPOINT *p1, double *l, double *b));
void rdtlb P_((double ra, double dec, double epoch, double *l, double *b));
void lbtrd P_((double l, double b, double epoch, double *ra, double *dec));
void MkCsys P_((RelCoordSys rc));
double RotFromPoint P_((RelCoordSys rc, XYPOINT p));
XYPOINT Proj P_((XYZPOINT point));
XYZPOINT UnProj P_((XYPOINT pr));
XYZPOINT *UnProjArray P_((XYPOINT low, XYPOINT high, int nx, int ny));
XYZPOINT *CsysToXyzArray P_((XYPOINT low, XYPOINT high, int nx, int ny));
XYZPOINT *UnProjLine P_((XYPOINT p1, XYPOINT p2, int n));
int SetProj P_((int projtype));
void PointtoSphere P_((register XYZPOINT *po, register double *lon, double *lat));
void SpheretoPoint P_((register double lon, register double lat, register XYZPOINT *po));
void precess P_((double ep1, XYZPOINT *po1, double ep2, XYZPOINT *po2));
void rotate P_((register XYZPOINT *p1, register XYZPOINT *p2, register double *m));
void rotmat P_((double *mat, char *s, ...));

#undef P_
