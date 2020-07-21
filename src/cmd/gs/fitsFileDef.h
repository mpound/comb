/* Definition of the FITS header keywords and binary table columns and
 * the struct fitsHeader & struct fitsBinaryTable used to read/write
 * them.  Also contains arrays of these structures with pointers to
 * the variables read or written when reading/writing the FITS file.

 * If the binary table is NOT to be contained in the Global Memory structure
 * before #including this file, #define SZC to be the address of this
 * structure enclosed in parentheses as in #define SCG (&myStructure).  If it
 * is not defined, it is assumed that they are contained in Global Memory, and
 * SCG set to a NULL pointer to this structure.  Programs that don't define SCG
 * must then increment the fitsBinaryTable.value elements by the base
 * address of the global memory structure as global memory addresses aren't
 * defined until they are mapped.  (Scooper does this.)
 */

#include "stostruct.h"

#define SSZ ((struct stostruct *)0)

/* FITS header definition */
/* Axis names */
char fitsCtype1[80] = {"VELO-LSR"};
char fitsCtype2[80] = {"RA---GLS"};	
char fitsCtype3[80] = {"DEC--GLS"};	
char fitsVelDef[80] = {"RADI-LSR"};

int fitsMaxis = 4;	        /* Number of MATRIX axes */
int fitsMaxis2 = 1;		/* MAXIS 2-4 set to 1 (degenerate axes) */
int fitsMaxis3 = 1;
int fitsMaxis4 = 1;

char fitsOrigin[80] = {"STO"};
char fitsExtName[80] = {"STO"};
int fitsExtVer = 1;		/* Supercam FITS version number */
int fitsZero = 0;		/* Dummy variable for CRPIX2 and CRPIX3 */

/* STO header values */
#define STO_IF   (1.65e9)	/* First IF */
#define CII_FREQ (1900.5469e9)	/* Rest frequency in Hz */
#define CII_FREQIM (CII_FREQ - 2.*STO_IF)
#define CII_NAME "[C II]"	/* Line name */
#define NII_FREQ (1461.132e9)	/* Rest frequency in Hz */
#define NII_FREQIM (NII_FREQ - 2.*STO_IF)
#define NII_NAME "[N II]"
#define CI_FREQ  (492.160700e9)	/* Rest frequency in Hz */
#define CI_FREQIM (CI_FREQ + 2.*STO_IF)
#define CI_NAME  "C I(1-0)"

/* STO array for Class TELESCOP parameter */
char *telescope[NBEAM] = {"STO-UP", "STO-DOWN", "STO-LEFT", "STO-RIGHT",
			    "STO-CENTER", "STO-DOWN", "STO-LEFT", "STO-RIGHT"};
/* STO arrays for Class LINE, RESTFRQ, IMAGFREQ, LAMOF, BETOF, VRES parameters */
char *lineName[NBEAM] = {CII_NAME, CII_NAME, CII_NAME, CII_NAME,
				CI_NAME, NII_NAME, NII_NAME, NII_NAME};
double restFreq[NBEAM] = {CII_FREQ, CII_FREQ, CII_FREQ, CII_FREQ,
				 CI_FREQ, NII_FREQ, NII_FREQ, NII_FREQ};
double imagFreq[NBEAM] = {CII_FREQIM, CII_FREQIM, CII_FREQIM, CII_FREQIM,
				 CI_FREQIM, NII_FREQIM, NII_FREQIM, NII_FREQIM};
double RAoff[NBEAM], DecOff[NBEAM], velSpa[NBEAM];

struct fitsHeader {
  char *keyword;                /* Name of Keyword */
  void *value;                  /* Pointer to keyword value */
  int type;                     /* Keyword (FITS) datatype */
  int inDb;						/* 1 if in database, 0 if not */
  char *comment;                /* Keyword comment */
} fitsHdr[] = {
  {"EXTNAME",  fitsExtName,       TSTRING, 0, NULL},
  {"EXTVER",   &fitsExtVer,       TINT,    0, NULL},
  {"MAXIS",    &fitsMaxis,        TINT,    0, NULL}, /* Class reads these */
  {"MAXIS1",   &SSZ->nChan,       TINT,    0, NULL}, /* No. of spectral channels */
  {"MAXIS2",   &fitsMaxis2,       TINT,    0, NULL},
  {"MAXIS3",   &fitsMaxis3,       TINT,    0, NULL},
  {"MAXIS4",   &fitsMaxis4,       TINT,    0, NULL},

  {"CTYPE1",   fitsCtype1,        TSTRING, 0, "Axis 1: Velocity & frame"},
  {"CRVAL1",   &SSZ->refVel,      TDOUBLE, 0, "Object LSR velocity (m/s)"},
  {"CRPIX1",   &SSZ->centerChan,  TDOUBLE, 0, "Center channel of spectrum"},
  {"CDELT1",   &SSZ->refVelSpac,  TDOUBLE, 0, "Spacing between velocity channels (m/s)"},

  {"CTYPE2",   fitsCtype2,        TSTRING, 0, "Axis 2: Longitude & projection"},
  {"CRVAL2",   &SSZ->epocRA,      TDOUBLE, 0, "RA of Object in catalog (deg)"},
  {"CRPIX2",   &fitsZero,         TINT,    0, "Should be 0 for Class"},
  {"CDELT2",   &SSZ->RASpac,      TDOUBLE, 0, "Nominal beam spacing in RA (deg)"},
  {"CTYPE3",   fitsCtype3,        TSTRING, 0, "Axis 3: Latitude & projection"},
  {"CRVAL3",   &SSZ->epocDec,     TDOUBLE, 0, "Dec of Object in catalog (deg)"},
  {"CRPIX3",   &fitsZero,         TINT,    0, "Should be 0 for Class"},
  {"CDELT3",   &SSZ->decSpac,     TDOUBLE, 0, "Nominal beam spacing in Dec (deg)"},

  {"OBJECT",   &SSZ->object,      TSTRING, 1, "Object name"},
  {"OFFRA",    &SSZ->offRA,       TDOUBLE, 0, "RA offset of Fid. pixel wrt Object"},
  {"OFFDEC",   &SSZ->offDec,      TDOUBLE, 0, "Dec offset of Fid. pixel wrt Object"},
  {"VELFRAME", &SSZ->velDef,      TSTRING, 0, "Velocity frame abbreviation"},
  {"EQUINOX",  &SSZ->epoch,       TFLOAT,  0, "Epoch of RA & Dec"},
  {"AZIMUTH",  &SSZ->STOazimuth,  TFLOAT,  0, "Azimuth (deg)"},  /* for CLASS */
  {"ELEVATIO", &SSZ->STOelevation,TFLOAT,  0, "Elevation (deg)"},/* for CLASS */
  {"RA",       &SSZ->STOra,       TFLOAT,  0, "Raw right ascension"},
  {"DEC",      &SSZ->STOdec,      TFLOAT,  0, "Raw declination"},
  {"PARLACTC", &SSZ->parallactic, TDOUBLE, 0, "Parallactic angle (deg)"},
  {"SCAN",     &SSZ->obsid,       TINT,    1, "Scan number"},    /* for CLASS */
  {"OBSID",    &SSZ->obsid,       TINT,    1, "Scan number"},
  {"TYPE",     &SSZ->obsType,	  TSTRING, 1, "Scan type"},
  {"DATE-OBS", &SSZ->FITStime,    TSTRING, 1, "Time stamp for integration end (UT)"},
  {"OBSTIME",  &SSZ->obsTime,     TDOUBLE, 1, "Mean integration time (sec)"},
  {"TCHOP",    &SSZ->Tchop,       TDOUBLE, 0, "Calibration vane temperature (K)"},
  {"TCOLD",    &SSZ->Tcold,       TDOUBLE, 0, "Cold load temperature (K)"},
  {"TCAL",     &SSZ->TcalSDD,     TDOUBLE, 0, "Spectral line calibration temp. (K)"},
  {"AXFOCPOS", &SSZ->focusAx,     TDOUBLE, 0, "Axial focus (mm)"},
  {"VTFOCPOS", &SSZ->focusVert,   TDOUBLE, 0, "Vertical focus (mm)"},
  {"NIITEMP",  &SSZ->dewarNII,    TFLOAT,  0, "Dewar [N II] temperature"},
  {"CIITEMP",  &SSZ->dewarCII,    TFLOAT,  0, "Dewar [C II] temperature"},
  {"4KTEMP",   &SSZ->dewar4K,     TFLOAT,  0, "Dewar 4 K stage temperature"},
  {"IVCSTEMP", &SSZ->IVCSTemp,    TFLOAT,  0, "IVCS temperature"},	  
  {"OVCSTEMP", &SSZ->OVCSTemp,    TFLOAT,  0, "OVCS temperature"},	  
  {"DACTEMP",  &SSZ->DACTemp,     TFLOAT,  0, "DAC temperature"},	  
  {"MUXTEMP",  &SSZ->MUXTemp,     TFLOAT,  0, "MUX temperature"},	  
  {"FPGATEMP", &SSZ->FPGATemp,    TFLOAT,  0, "FPGA temperature"},	  
  {"RFTEMP",   &SSZ->RFTemp,      TFLOAT,  0, "RF temperature"},	  
  {"IFTEMP1",  &SSZ->IFTemp1,     TFLOAT,  0, "IF1 temperature"},	  
  {"IFTEMP2",  &SSZ->IFTemp2,     TFLOAT,  0, "IF2 temperature"},	  
  {"TEMP1",    &SSZ->temp1,       TFLOAT,  0, "Temperature 1"},	  
  {"TEMP2",    &SSZ->temp2,       TFLOAT,  0, "Temperature 2"},	  
  {"TEMP3",    &SSZ->temp3,       TFLOAT,  0, "Temperature 3"},	  
  {"HEBOIL",   &SSZ->HeBoil,      TFLOAT,  0, "Helium boil off rate"},  
  {"HEFRAC",   &SSZ->HeFrac,      TFLOAT,  0, "Helium level fraction"}, 
  {"HEPRES",   &SSZ->HePres,      TFLOAT,  0, "Helium pressure"},         
  {"UDPTIME",  &SSZ->UDPtime,     TDOUBLE, 0, "UDP header time stamp"},
  {"UDPJD",    &SSZ->jd0,         TFLOAT,  0, "UDP Julian day"},
};

#define NKEYWORD (sizeof(fitsHdr)/sizeof(fitsHdr[0])) /* Size of fitsHdr array */

struct fitsHeader fitsHdr0[] = {		      /* Null image HDU */
  {"ORIGIN", fitsOrigin,    TSTRING, 0, NULL}
};						\

#define NKEYWORD0 (sizeof(fitsHdr0)/sizeof(fitsHdr0[0])) /* fitsHdr0 arr size */



/* Binary Table extension column definition */
struct fitsBinaryTable {
  char *name;                   /* Column name */
  void *value;                  /* Pointer to array of values in the column */
  int type;                     /* Value's (FITS) datatype code */
  int inDb;			/* 1 if in database, 0 if not */
  char *tform;                  /* Column format code */
  char *tunit;                  /* Units of value */
} fitsTbl[] = {
  {"CDELT1",   &velSpa,            TDOUBLE, 0, "1D", "m/s"},
  {"CDELT2",   &RAoff,             TDOUBLE, 0, "1D", "deg"},
  {"CDELT3",   &DecOff,            TDOUBLE, 0, "1D", "deg"},
  {"BiasVolt", &SSZ->biasVolts,    TDOUBLE, 0, "1D", "mV"},
  {"BiasCurr", &SSZ->biasCurr,     TDOUBLE, 0, "1D", "uA"},
  {"TotPower", &SSZ->totalPower,   TDOUBLE, 0, "1D", "counts"},
  {"MxrAtten", &SSZ->mixerAtten,   TDOUBLE, 0, "1D", "dB"},
  {"BeamFac",  &SSZ->beamFac,      TDOUBLE, 0, "1D", NULL},
  {"TSYS",     &SSZ->Tsys,         TDOUBLE, 0, "1D", "K"},
  {"Trx",      &SSZ->Trx,          TDOUBLE, 0, "1D", "K"},
  {"RAoff",    &SSZ->RAoff,        TDOUBLE, 0, "1D", "deg"},
  {"DecOff",   &SSZ->DecOff,       TDOUBLE, 0, "1D", "deg"},
  {"OutRange", &SSZ->beamOutRange, TINT,    0, "1J", "counts"},
  {"IntTime",  &SSZ->intTime,      TDOUBLE, 0, "1D", "sec"},
  {"NumOff",   &SSZ->numOff,       TINT,    0, "1J", NULL},
  {"TELESCOP", &telescope,         TSTRING, 0, "12A",NULL},
  {"LINE",     &lineName,          TSTRING, 0, "12A",NULL}, /* "LINE" for CLASS */
  {"RESTFREQ", &restFreq,          TDOUBLE, 0, "1D", "Hz"},
  {"IMAGFREQ", &imagFreq,          TDOUBLE, 0, "1D", "Hz"},
  {"DATA",     NULL,               TFLOAT,  0, "1024E", "K"}
};

#define NFITS_COL (sizeof(fitsTbl)/sizeof(fitsTbl[0]))
