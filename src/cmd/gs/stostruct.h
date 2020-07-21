#ifndef STOSTRUCT_H
#define STOSTRUCT_H

#define NBEAM 8                /* Number of STO beams */
#define NMXR_CARD 4				/* Number of STO 8-mixer boards */
#define MXR_CARD 2              /* Number of STO mixers per board */
#define NSP_COM 1               /* Number of Omnisys embedded computers */
#define NSP_CARD 4              /* Number of Omnisys cards per computer */
#define NSP_INP 2 /*not sure*/  /* Number of Omnisys inputs per card */
#define NFPGA (NSP_COM*NSP_CARD) /* Number of FPGA's in the spectrometer */
#define NFFT (NFPGA*NSP_INP)    /* Number of FFT's done in the spectrometer */
#define NBM_FFT 1 		/* Number of spectra (beams) per FFT */
#define NCHANFFT 1024           /* Number of channels per FFT */
#define NCHAN (NCHANFFT/NBM_FFT) /* No. of channels per STO spectrum */
#define SPEC_CENT  (NCHAN_SP/2.0)     /* Center channel of spectrum */

#define MAX_OTF_RATIO 1.2	/* Default max mRMS/tRMS for 2 OFFS in OTF */
#define BEAM_SPACING 120         /* Nominal beam spacing (arcsec) */
#define DEL_AZ_EL (BEAM_SPACING*M_SQRT1_2) /* Offset up, down, left, or right */

#define OMNI_CLOCK 1.1e9        /* ADC samples per second in Omnisys */

/* Omnisys frequency resolution (MHz) */
#define OMNI_FRES (1e-6*OMNI_CLOCK/(NCHANFFT)) 
#define VLIGHT (299792.5)	/* Velocity of light (km/s) */
#define NCHAN_STO 1024	       /* Number of STO spectral channels */

/* STO observation-related data struct */
struct stostruct {

  float dewarNII;           /* Dewar [N II] temperature */
  float dewarCII;           /* Dewar [C II] temperature */
  float dewar4K;            /* Dewar 4 K temperature */
  float IVCSTemp;	    /* IVCS temperature */
  float OVCSTemp;	    /* OVCS temperature */
  float DACTemp;	    /* DAC temperature */
  float MUXTemp;	    /* MUX temperature */
  float FPGATemp;	    /* FPGA temperature */
  float RFTemp;	    	    /* RF temperature */
  float IFTemp1;	    /* IF1 temperature */
  float IFTemp2;	    /* IF2 temperature */
  float temp1;		    /* Temperature 1 */
  float temp2;		    /* Temperature 2 */
  float temp3;		    /* Temperature 3 */
  float HeBoil;		    /* Helium boil off rate */
  float HeFrac;		    /* Helium level fraction */
  float HePres;		    /* Helium pressure */         
  float biasVolts[NBEAM];   /* Bias voltages */
  float biasCurr[NBEAM];    /* Bias currents */
  float totalPower[NBEAM];  /* Total power for each beam (counts) */
  double UDPtime;	    /* UDP time stamp (seconds since Epoch) */
  float jd0;		    /* UDP Julian date */
  float longitude;	    /* STO longitude (deg) */
  float latitude;	    /* STO latitude (deg) */
  float STOazimuth;	    /* STO raw azimuth (deg) (no pointing model corr) */
  float STOelevation;	    /* STO raw elevation (deg) */
  float elError;	    /* STO elevation error (arcsec) */
  float azError;	    /* STO elevation error (arcsec) */
  float STOra;		    /* STO raw RA */
  float STOdec;		    /* STO raw Dec */

  unsigned int specFPGA[NFPGA];     /* Spectrometer board FPGA temperatures */
  unsigned int specAmb[NFPGA];      /* Spectrometer board ambient temps */
  unsigned int specStatus[NFPGA];   /* Spectrometer board status */
  unsigned int beamIntCount[NBEAM]; /* Beam integration count */
  unsigned int beamOutRange[NBEAM]; /* Beam out-of-range count */

  char object[20];           /* Object name */
  double epocRA;             /* Object right ascension for epoch (deg) */
  double epocDec;            /* Object declination for epoch (deg) */
  double offRA;              /* Offset in RA (deg) from SDD's xsource-epocra */
  double offDec;             /* Offset in Dec (deg) from SDD's ysource-epocdec */
  double offAz;              /* Offset in az (deg) (Non-zero in 5-point pointings) */
  double offEl;              /* Offset in el (deg) (Non-zero in 5-point pointings) */
  double epoch;              /* The epoch year */
  double galLong;            /* Object galactic longitude (deg) */
  double galLat;             /* Object galactic latitude (deg) */
  double azim;               /* Object azimuth (deg) */
  double elev;               /* Object elevation (deg) */
  double parallactic;        /* Parallactic angle (deg) */
  double currRA;             /* Object current right ascension (deg) */
  double currDec;            /* Object current declination (deg) */
  char coordCd[12];          /* Coordinate code abbr (Coord. system used for 
                                object in catalog) */
  double velocity;           /* Object velocity */
  double antVel;             /* Antenna velocity */
  char velDef[12];           /* Velocity frame abbreviation */
  double velSpacing;	     /* Velocity channel spacing (km/s) */

  char lineName[NBEAM][20];         /* Molecular line name */
  double restFreq[NBEAM];           /* Rest frequency (MHz) */
  double skyFreq;            /* Sky frequency (MHz) */
  int sideband;		     /* Sideband code: 1=LSB, 2=USB */

  unsigned long int obsid;   /* Observation scan number */
  char obsType[5];	     /* observing sequence type: SRC,REF,HOT,COLD,OTF */
  char observer[20];         /* Observer name */
  char obsMode[12];          /* Observation mode abbreviation */
  double utDate;             /* UT date as a floating point number yyyy.mmdd */
  double ut;                 /* UTC (hours) */
  double lmst;               /* LMST (hours) */
  double Tamb;               /* Ambient temperature (C) */
  double pressure;           /* Ambient pressure in torr */
  double humidity;           /* Relative humidity */
  double tipHumidity;        /* Relative humidity from tipper opacity */
  double refrac;             /* Refraction at 45 degrees elevation */
  double dewPt;              /* Dewpoint temperature (C) */
  double mmH2O;              /* Precipitible water (mm) */
  double TatmS;              /* Atmospheric temperature in signal sideband (K) */
  double TatmI;              /* Atmospheric temperature in image sideband (K) */
  double tauS;               /* Opacity in signal sideband (nepers) */
  double tauI;               /* Opacity in image sideband (nepers) */
  double Tchop;              /* Calibration vane temperature (C) */
  double Tcold;              /* Cold load temperature (K) */
  double TcalSDD;            /* Calibration temperature (K) (Tcal for 
                                spectral line intensity) */

  double pBeamThrow;         /* Subreflector throw for +Beam (arcsec) */
  double mBeamThrow;         /* Subreflector throw for -Beam (arcsec) */
  double focusAx;            /* Axial focus (mm) */
  double focusVert;          /* Vertical focus (mm) */
  double mixerAtten[NBEAM];  /* Mixer attenuators */
  double beamFac[NBEAM];     /* Factors for beam intensity scale */
  double Tsys[NBEAM];        /* Average beam system temperature */
  double Trx[NBEAM];         /* Average beam receiver temperature */
  double Tcal[NBEAM];        /* Tcal temperature for each beam */
  int numOff[NBEAM];         /* Number of OTF OFF's used for each beam */
  int beamMap[NBEAM];        /* Beam number for part+board+input+hilo */
  int specMap[NBEAM];        /* part+board+input+hilo for beam number */
  double beamOffAz[NBEAM];   /* Beam offset in azimuth */
  double beamOffEl[NBEAM];   /* Beam offset in elevation */

  int badBeam[NBEAM+1];      /* Beam numbers to ignore. -1 marks end of list */
  int badSpec[NBEAM+1];	     /* Spectrum nos. to ignore. -1 marks end of list */
  double RAoff[NBEAM];	     /* Beam RA offset (includes beamOff's) */
  double DecOff[NBEAM];	     /* Beam Dec offset (includes beamOff's) */
  double intTime[NBEAM];     /* Spectrometer integration time in seconds */

  int boardID[NSP_COM];      /* Board ID for Omnisys start commands (zero out
                                bits in 4-bit bit mask of bad/unused boards) */
 
  double otfRatio;	     /* Threshold for ratio btw OFF RMS's for 2 OFFS */

  /* Misc FITS variables used to create the FITS Header & Binary Table */

  double fidRotate;	      /* CROTA1 & CROTA2: Rotation corresponding to 
                                 Equatorial or Galactic North (deg) */
  double azSpac;	      /* Nominal beam spacing in azimuth */
  double elSpac;	      /* Nominal beam spacing in elevation */
  double RASpac;	      /* CDELT1: beam spacing in RA after rotation */
  double decSpac;	      /* CDELT2: beam spacing in Dec after rotation */
  double refVelSpac;          /* CDELT3: Velocity spacing in m/sec */
  double fidRA;		      /* CRVAL1: Fiducial pixel RA */
  double fidDec;	      /* CRVAL2: Fiducial pixel Dec */
  double refVel;	      /* CRVAL3: Reference velocity in m/sec */
  double centerChan;	      /* CRPIX3: Center channel of the spactrum */ 
  char FITStime[32]; 	      /* Time stamp in FITS format */
  double obsTime;             /* Mean integration time for all FPGA's (sec) */
  int nChan;                  /* Number of channels per spectrum */
  int deleteOmnisys;	      /* Non-zero if deleting raw files when read */
  } *SSC; /*pointer to the stostruct*/

#endif                  /* End #ifndef STOSTRUCT_H */
