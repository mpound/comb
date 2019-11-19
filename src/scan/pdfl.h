/*
	Cactus file @(#)pdfl.h	1.3
		SID 1.3
	       Date 7/12/91
*/
/* PDFL.H -- Defined HEADER and HEADER2 structures for POPS I/O */

#define getheads(a) swaps(a)
#define MAX_SCANS	2048

/*-------------------ALTERED BY MARC POUND FOR COMB 1/92-----------------*/
/* I separated the HEADER structures into separate CLASSN structures -mwp
 * struct HEADER2   head;  SEE CACTUS FILE PDFL.H
 *  struct HEADER    *h; 
 */

/* Mon Feb  1 15:36:43 EST 1993 -- Rewritten to accomodate new pdfl structure */

struct CLASSPTRS {
  short headcls; 	/* Number of header classes */
  short one; 		/* Starting loc of class 1 */
  short two; 		/* Starting loc of class 2 */
  short three; 		/* Starting loc of class 3 */
  short four; 		/* Starting loc of class 4 */
  short five; 		/* Starting loc of class 5 */
  short six; 		/* Starting loc of class 6 */
  short seven; 		/* Starting loc of class 7 */
  short eight; 		/* Starting loc of class 8 */
  short nine; 		/* Starting loc of class 9 */
  short ten; 		/* Starting loc of class 10 */
  short eleven; 	/* Starting loc of class 11 */
  short twelve; 	/* Starting loc of class 12 */
  short thirteen; 	/* Starting loc of class 13 */
  short align1; 	/* makes class 1 align */
  short align2; 	/* makes class 1 align */
};

struct CLASS1 {
/* Class 1 */

  double headlen;	/*Length of header in bytes */
  double datalen;	/* Length of data in bytes */
  double scan;		/* scan number */
  char obsid[8];	/*Observer & Operator Initials */
  char observer[16];	/* Observer Name */
  char telescop[8];	/* Telescope */
  char projid[8];	/* Project ID */
  char object[16];	/* Source Name */
  char obsmode[8];	/* Type of data & observing mode */
  char frontend[8];	/* Front End */
  char backend[8];	/* Back End */
  char precis[8]; 	/* Data precision */

};
struct CLASS2 {
  double xpoint;	/* Az/RA Pointing Corr (arcsec) */
  double ypoint;	/* El/Dec Pointing Corr (arcsec) */
  double uxpnt;		/* User Az/RA Correction (arcsec) */
  double uypnt;		/* User El/Dec Correction(arcsec) */
  double ptcon[4];	/* Pointing constant - PVLS */
  double orient;	/* Rcvr Box or Sec Orient (deg)*/
  double focusr;	/* Radial Focus (m) */
  double focusv;	/* North-South Focus (m)*/
  double focush;	/* East-West Focus (m)*/
  char pt_model[8];     /* pointing model */
};
struct CLASS3 {
/* Class 3 */

  double utdate; 	/* Universal Time Date(YYYY.MMDD)  */
  double ut; 		/* Universal Time (hours) */
  double lst; 		/* LST (hours) */
  double norchan; 	/* Number of Receivers */
  double noswvar; 	/* Number of Switching Variables */
  double nophase; 	/* Number of Phases per Cycle */
  double cycllen; 	/* Length of cycle (sec) */
  double samprat; 	/* Sample Period (sec) */
  char cl11type[8];     /* class 11 type--whatever that means */
}; 	
 
struct CLASS4 {
/* Class 4 */
 
  double epoch; 	/* Epoch of observation */
  double xsource;	/* Cmmnd Source RA (deg) */
  double ysource;	/* Cmmnd Source Dec (deg) */
  double xref;		/* Cmmnd Reference RA (deg) */
  double yref;		/* Cmmnd Reference Dec (deg) */
  double epocra;	/* Cmmnd Epoch RA (deg)*/
  double epocdec;	/* Cmmnd Epoch Dec (deg) */
  double gallong;	/* Cmmnd Galactic Longitude (deg) */
  double gallat;	/* Cmmnd Galactic Latitude (deg) */
  double az;		/* Cmmnd Azimuth (deg) */
  double el;		/* Cmmnd Elevation (deg) */
/* indx and indy are in the CURRENT epoch. , e.g. 1992.5 */
  double indx;		/* Indicated X Position (deg) */
  double indy;		/* Indicated Y Position (deg) */
  double desorg[3];	/* Descriptive origin */
  char coordcd[8];	/* Coordinate system code */
};

struct CLASS5 {
/* Class 5 */

  double tamb;   /* Ambient Temp (deg C) */
  double pressure; /* pressure (cm-Hg) (possibly mm-Hg==Torr) */
  double humidity; /* % humdity */
  double refrac;   /* refraction correction (arcsec) */
  double dewpt;    /* dew point  (C) */
  double mmh2o;    /* mm water vapor (mm)*/
};
struct CLASS6 {
/* Class 6 */

  double scanang;	/* Map scanning angle (deg)  */
  double xzero;		/* X Pos at Map Ref Pos Zero(deg)  */
  double yzero;		/* Y Pos at Map Ref Pos Zero(deg)  */
  double deltax;	/* Delta X or X Rate (arcsec) */
  double deltay;	/* Delta Y or Y Rate (arcsec) */
  double nopts;		/* Number of Grid Points */
  double noxpts;	/* Number of X Grid Points */
  double noypts;	/* Number of Y Grid Points */
  double xcell0;	/* Starting X Cell Grid Number */
  double ycell0;	/* Starting Y Cell Grid Number */
  char frame[8];	/* XY Reference Frame Code */

};

struct CLASS7 {
/* Class 7 */

  double bfwhm;		/* Beam Fwdth at Half Max(arcsec) */
			/* Despite the name, NRAO definition of this
			 * is half-width at half max. DUMB! We multiply
  			 * by 2 in pdfl.c/sdd.c */
  double offscan;	/* Off-Scan number */
  double badchv;	/* Bad channel value */
  double rvsys;		/* Velocity Correction (m/s?) */
  double velocity;	/* Velocity wrt Reference (km/s) */
  char veldef[8];	/* Velocity Def and Ref */
  char typecal[8];	/* Type of Calibration  */
};
struct CLASS8 {
/* Class 8 */

  double appeff;	/* Antenna Aperture Efficiency, f(freq) */
  double beameff;  	/* main beam efficiency	       */
  double antgain;       /* antenna gain 	       */
  double etal;		/* rear spillover and scattering efficiency */
  double etafss;	/* Forward Spillover and scattering 	    */
		        /* (Ta*->Tr* conversion)		    */
};

struct CLASS9 {
/* Class 9 - Kitt Peak */

  double synfreq;	/* Synthesizer Frequency (Hz) */
  double lofact;	/* LO Factor (Hz) */
  double harmonic;	/* Harmonic */
  double loif;		/* LO IF (Hz) */
  double firstif;	/* First IF (Hz) */
  double razoff;	/* Ref Az Offset (arcsec) */
  double reloff;	/* Ref El Offset (arcsec) */
  double bmthrow;	/* Beam Throw (arcsec) */
  double bmorent;	/* Beam Orientation (deg) */
  double baseoff;	/* Baseline Offset (deg) */
  double obstol;	/* Observing Tolerance (arcsec) */
  double sideband;	/* Sideband */
  double wl;		/* Wavelength (focus) */
  double gains;		/* Gains scan number */
  double pbeam[2];	/* Plus beam offset */
  double mbeam[2];	/* Minus beam offset */
  double sroff[4];	/* RA/Dec offsets */
  double foffsig;       /* sig for freq. switching */
  double foffref1;      /* ref1 for freq. switching */
  double foffref2;      /* ref2 for freq. switching */
};

struct CLASS10 {
 /* Class 10 */
  double openpar[10];  /* Observer generated HISTORY */

};

struct CLASS11 {  
/* class 11: switching and phase patterns. Ignore. */
  double noswvarf;
  double numcyc;
  double numcycf;
  double nophasef;
  double cycllenf;
  double samptimf;
  double varval01;
  double vardes01;
  char   phastb01[32];
  double varval02;
  double vardes02;
  char   phastb02[32];
  double varval03;
  double vardes03;
  char   phastb03[32];
  double varval04;
  double vardes04;
  char   phastb04[32];
  double varval05;
  double vardes05;
  char   phastb05[32];
};

struct CLASS12 {
/* Class 12 */
  double obsfreq;	/* Observed Frequency (Hz)*/
  double restfreq;	/* Rst Freq or TP Cal (Hz) */
  double freqres;	/* Freq Res or SP Cal (Hz) */
  double bw;		/* Band Width (Hz) */
  double trx;		/* Receiver Temp (K)*/
  double tcal;		/* Calibration Temp (K)*/
  double stsys;		/* Signal System Temp (K)*/
  double rtsys;		/* Ref System Temp (K)*/
  double tsource;	/* Source Temp (K)*/
  double trms;		/* RMS of Mean*/
  double refpt;		/* Ref Point Number*/
  double x0;		/* Velocity at Ref Point (m/s)*/
  double deltax12;	/* Delta X*/
  double inttime;	/* Total Integration Time (sec)*/
  double noint;		/* # Integrations or channels*/
  double spn;		/* Starting point number*/
  double tauh2o;	/* H2O Opacity */
  double th2o;		/* H2O Temp (K) */
  double tauo2;		/* O2 Opacity*/
  double to2;		/* O2 Temp (K) */
  char polariz[8];	/* Polarization  */
  double effint;	/* ? */
  char rx_info[16];	/* receiver info */

};

struct CLASS13 {
/* Class 13 */
  double nostac;	/* Number of scans stacked  */
  double fscan;		/* First scan in stack */
  double lscan;		/* Last scan in stack */
  double lamp;		/* Line amplitude */
  double lwid;		/* Line width */
  double ili;		/* Integrated line intensity */
  double rms;		/* RMS noise */
  double align3[4];
};

struct CLASS_STRINGS {/* kluge to fix non-null terminated FORTRAN strings */
  char obsid[16];        /* Observer & Operator Initials */
  char observer[32];     /* Observer Name */
  char telescop[16];     /* Telescope */
  char projid[16];       /* Project ID */
  char object[32];       /* Source Name */
  char obsmode[16];      /* Type of data & observing mode */
  char frontend[16];     /* Front End */
  char backend[16];      /* Back End */
  char precis[16];       /* Data precision */
  char pt_model[16];	 /* pointing model */
  char cl11type[16];     /* class 11 type */
  char coordcd[16];      /* Coordinate system code */
  char frame[16];        /* XY Reference Frame Code */
  char veldef[16];       /* Velocity Def and Ref */
  char typecal[16];      /* Type of Calibration  */
  char phastN[64];	 /* ? from class 11 */
  char polariz[16];      /* Polarization  */
  char rx_info[32];	 /* reciever info */
  char openpar[128];     /* User history */
};

