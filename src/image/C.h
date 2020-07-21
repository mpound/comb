#define MAXIMAGES 10
#define MAXAXES 4

/* values for use in axisname.type */
#define UNDEF 0
#define SPATIAL 1
#define VELOCITY 2
#define FREQUENCY 3
#define WAVELENGTH 4
#define TIME 5
#define LATTYPE 0x40
#define OFFSETTYPE 0x80

/* additional units defined (have to mesh with ../coordsys/C.h) */
# define NONE 5
# define SECONDS 6
# define MEGAHERTZ 7
# define MICROMETERS 8
# define KMPERSEC 9

/* logical values in a FITS header */
#define SIMPLE 0
#define simple l[SIMPLE]
/* END doesn't really have a value, so set aside no storage for it */
#define END 1

/* calibration applied for ISSA IRAS maps? */
/* this may mean the color correction has already been applied? */
#define APPCAL 2 
#define appcal l[APPCAL]
#define NUMFITSLOG 2

/* integer values in an FITS header */
#define BITPIX 0
#define bitpix i[BITPIX]
#define NAXIS 1
#define naxis i[NAXIS]
#define NAXIS1 2
#define naxis1 i[NAXIS1]
#define NAXIS2 3
#define naxis2 i[NAXIS2]
#define NAXIS3 4
#define naxis3 i[NAXIS3]
#define NAXIS4 5
#define naxis4 i[NAXIS3]
#define BLANK 6
#define blank i[BLANK]
#define BASELN 7
#define baseln i[BASELN]
#define NUMFITSINT 8

/* string values in an FITS header */
#define OBJECT 0
#define object s[OBJECT]
#define TELESCOP 1
#define telescop s[TELESCOP]
/* we distinguish between DATE and DATE-OBS
 * they really do have 2 different meanings...
 */
#define DATE 2  /* the date the file was created*/
#define date s[DATE]
#define DATEOBS  3  /* the date the observation was taken */
#define dateobs s[DATEOBS]
#define BUNIT 4
#define bunit s[BUNIT]
#define CTYPE1 5
#define ctype1 s[CTYPE1]
#define CTYPE2 6
#define ctype2 s[CTYPE2]
#define CTYPE3 7
#define ctype3 s[CTYPE3]
#define CTYPE4 8
#define ctype4 s[CTYPE3]
#define PROJTYPE 9
#define projtype s[PROJTYPE]
#define ORIGIN   10
#define origin s[ORIGIN]
#define CREATOR  11
#define creator s[CREATOR]
#define NUMFITSSTR 12

/* Long strings from HISTORY COMB lines in an FITS header */
#define TITLE 0
#define title h[TITLE]
#define PARAM 1
#define param h[PARAM]
#define IMAGEOP 2
#define imageop h[IMAGEOP]
#define USERHIST 3
#define userhist h[USERHIST]
#define NUMFITSHIST 4

/* double values in an FITS header */
#define CRVAL1 0
#define crval1 d[CRVAL1]
#define CDELT1 1
#define cdelt1 d[CDELT1]
#define CRPIX1 2
#define crpix1 d[CRPIX1]
#define CROTA1 3
#define crota1 d[CROTA1]
#define CRVAL2 4
#define crval2 d[CRVAL2]
#define CDELT2 5
#define cdelt2 d[CDELT2]
#define CRPIX2 6
#define crpix2 d[CRPIX2]
#define CROTA2 7
#define crota2 d[CROTA2]
#define CRVAL3 8
#define crval3 d[CRVAL3]
#define CDELT3 9
#define cdelt3 d[CDELT3]
#define CRPIX3 10
#define crpix3 d[CRPIX3]
#define CROTA3 11
#define crota3 d[CROTA3]
#define CRVAL4 12
#define crval4 d[CRVAL3]
#define CDELT4 13
#define cdelt4 d[CDELT3]
#define CRPIX4 14
#define crpix4 d[CRPIX3]
#define CROTA4 15
#define crota4 d[CROTA3]
#define EQUINOX 16
#define Fequinox d[EQUINOX]
#define BSCALE 17
#define bscale d[BSCALE]
#define BZERO 18
#define fbzero d[BZERO]
#define DATAMAX 19
#define datamax d[DATAMAX]
#define DATAMIN 20
#define datamin d[DATAMIN]
#define RESTFREQ 21
#define Frestfreq d[RESTFREQ]
#define VELO_LSR 22
#define Fvlsr d[VELO_LSR]
/* VELO is a CLASS FITS keyword that may be present if VLSR isn't - mwp 11/91 */
/* VELO-LSR is the 'correct' replacement for VLSR - RWW 5/95 */
#define Fvelo d[VELO_LSR]
#define Fvelo_lsr d[VELO_LSR]
#define DELTAV 23
#define Fdeltav d[DELTAV]
#define TSYS 24
#define Ftsys d[24]
#define OBSTIME 25
#define Fobstime d[OBSTIME]
#define SCAN_NUM 26
#define scan_num d[SCAN_NUM]
#define TELALT 27
#define tel_elevation d[TELALT]
#define TELAZ 28
#define tel_azimuth d[TELAZ]
#define TELTAMB 29
#define tel_Tamb d[TELTAMB]
#define TELTATM 30
#define tel_Tatm d[TELTATM]
#define TSKY 31
#define Ftsky d[TSKY]
#define RCRPIX1 32
#define Rcrpix1 d[RCRPIX1]
#define TRMS 33
#define Ftrms d[TRMS]
#define TELVANT 34
#define tel_VAnt d[TELVANT]
#define NUMFITSDOUBLE 35

/* file types */
#define TMP 0
#define EXTERNAL 1
#define TAPE 2
#define FITS 4
#define STACK 8

/* image arithmatic operations */
#define ADD 0
#define SUB 1
#define DIV 2
#define MUL 3
#define TEMP 4
#define TAU  5
#define CC   6
#define TRUE 1
#define FALSE 0

#define FITSSTRLEN   68		        /* largest possible length of a FITS string. 
					 * (column 11=', column 80=')
					 * Strings in excess of this are truncated.
				         */
struct of_imagehdr {
	char fname[64];			/* name of image file */
	int fd;				/* file descriptor */
	int ftype;			/* file type (tmp, external, ...) */
	int dataStart;			/* byte offset of first pixel in file */
	int buflen;			/* data buffer len in lines */
	int bufline;			/* offset (in lines) of buffer from
					 * bottom line in image */
	int bufplane;			/* for data cubes */
	float *buf;			/* points to data buffer */
	char s[NUMFITSSTR][FITSSTRLEN];	/* strings from the FITS header */
	char h[NUMFITSHIST][60];	/* long strings from HISTORY COMB */
	char l[NUMFITSLOG];		/* logical values from the header */
	int i[NUMFITSINT];		/* integers from FITS header */
	double d[NUMFITSDOUBLE];	/* floats and doubles from header */
};
extern struct of_imagehdr *imageHdr[MAXIMAGES]; /* images 0 to MAXIMAGES can be used,
					   but most people don't know about image 0 */

typedef struct {
	float x,y;
} VERTEX;

/* An area is defined by the verticies of one or more polygons.  The end of a
 * polygon is indicated by a vertex with x == FDRAGON.  It may be followed by
 * the vertices of another polygon.
 */
struct of_area {
	int l;				/* Index of last vertex of last polygon
					 * That vertex will have x == FDRAGON */
	double area;			/* in square 'Arc */
	double perimeter;		/* in 'Arc */
	VERTEX v[129];
};
#define NUMAREAS 10
extern struct of_area *areas[NUMAREAS];	/* areas 0 to NUMAREAS can be used, 
					   but most people don't know about area 0 */

typedef struct {
	int n;
	float x[129];
} XLST;
extern XLST XList();

struct AXISNAMES {
	float factor;		/* multiply header values by factor */
	char unit;		/* e.g. ARCMINUTES preferred for display */
	unsigned char type;	/* whether spatial, longitude type, etc. */
	char fname[6];		/* Name to recognize in CTYPEn */
	char len;		/* number of chars to check in fname */
	char lname[13];		/* name to use in axis labels */
};

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* image.c */
void ChkImage P_((int imnumber));
void OpenFits P_((register int imnumber, char *fname));
void StartStdImage P_((int imnumber));
void NewImage P_((register int imnumber));
void LinkImage P_((register imSrc, register imDest));
void ReleaseImageBuff P_((register int imnumber));
void CloseImage P_((register int imnumber));
void ImageSimilar P_((int im1, int im2));
float *GetImageLine P_((int imnumber, int plane, int y, int nlines, int ydir));
void ImageXGrid P_((int imnumber, int *lowx, int *highx, double *deltax, int *xunit));
void ImageYGrid P_((int imnumber, int *lowy, int *highy, double *deltay, int *yunit));
void ImNumErr P_((int imnumber));
void WriteFits P_((int imnumber, char *fname, char *obj, char *telescope));
void SetDataScale P_((int imnumber));
void OpenFitsOut P_((char *fname));
void PadFitsFile P_((void));
void CloseFitsOut P_((void));
void WriteEOF P_((int fd));
void WriteFitsHdr P_((int imnumber));
void WriteFitsDataLine P_((int imnumber, float *data, int line, int plane));
void FFitsCard P_((char *name, double value));
void DFitsCard P_((char *name, double value));
void IFitsCard P_((char *name, int value));
void SFitsCard P_((char *name, char *value));
void HFitsCard P_((char *name, char *value));
void LFitsCard P_((char *name, int value));
void FitsCard P_((char *card));
char *FitsAxisLabel P_((int axis));
struct AXISNAMES FitsAxisUnits P_((register char *ctype));
void ImageOp P_((int ima, int imb, int imDest, int opr, double mina, double minb));
void RescaleImage P_((int imx, double rsfact, double addend));
void PrintHdr P_((int imnumber));
void CopyHdr P_((int imfrom, int imto));
void Subimage P_((int imSrc, int ImDest, double xmin, double xmax, double ymin, double ymax, double minpix));
void WriteFitsSpectrum P_((struct of_stk *stk, char *fn, char *obj, char *telescope));
void ReadFitsSpectrum P_((struct of_stk *stk, char *fn));

/* area.c */
void AreaNumChk P_((int anumber));
int ChkArea P_((int anumber));
struct of_area *AllocArea P_((register int anumber));
void AreaArea P_((int anumber));
XLST XList P_((int anumber, double y));
int AreaLimits P_((int anumber, register double *xmin, register double *xmax, register double *ymin, register double *ymax));


/* taut.c */
void taut P_((int sim, int lim, double power, int oper));
int r2temp P_((double pic1, double pic2, double factor, int band, float *rpic));
int cc P_((double pic1, double pic2, double mfactor, int band, float *rpic));
int taum P_((double pic1, double pic2, int band, float *rpic));

#undef P_


#if 0
/* taut.c & image.c */
void taut P_((int sim, int lim, double power, int oper));
int r2temp P_((double pic1, double pic2, double factor, int band, float *rpic));
int cc P_((double pic1, double pic2, double mfactor, int band, float *rpic));
int taum P_((double pic1, double pic2, int band, float *rpic));
int locate P_((double x, float *table, int n));
float interp P_((int i, double x, float array1, float array2));
int GetBand P_((struct of_imagehdr *ihp));
#endif
