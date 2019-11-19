#define ScanSizeN 0
#define FrstScanN 1
#define LastScanN 2
#define HMIFreqN 3
#define IFCenterN 4
#define LongitudN 5
#define LatitudeN 6
#define HeightN 7
#define TelescopN 8
#define ObsPrgN 9
#define SpecTypeN 10
#define ProjIDN 11
#define ObserverN 12
#define ScanLocN 13
#define ScanNumN 14
#define SubScanN 15
#define ObsTypeN 16
#define J2SecondN 17
#define IntTimeN 18
#define ObjectN 19
#define PosTypeN 20
#define CentRaN 21
#define CentDecN 22
#define CentLN 23
#define CentBN 24
#define RaMapN 25
#define DecMapN 26
#define LMapN 27
#define BMapN 28
#define RaRefN 29
#define DecRefN 30
#define LRefN 31
#define BRefN 32
#define EpochN 33
#define AzimuthN 34
#define ElevatioN 35
#define RefAzOffN 36
#define RefElOffN 37
#define AzDeltaN 38
#define ElDeltaN 39
#define VRefSysN 40
#define LineFreqN 41
#define SigIFN 42
#define RefIFN 43
#define RefIF2N 44
#define VSourceN 45
#define VAntN 46
#define BZeroN 47
#define BScaleN 48
#define RecTempN 49
#define CalTempN 50
#define TypeCalN 51
#define SysTempN 52
#define HotSpillN 53
#define SbRatioN 54
#define TAmbientN 55
#define TSkyN 56
#define TSkyElevN 57
#define NRefN 58
#define BadN 59
#define MMHNN 60
#define MMSBN 61
#define HMHNN 62
#define HMSBN 63
#define SidebandN 64
#define BeamFWHMN 65
#define EtaFSSN 66
#define AppEffN 67
#define EtaLN 68
#define BeamEffN 69
#define PressureN 70
#define HumidityN 71
#define RefracN 72
#define DewPtN 73
#define MmH2ON 74
#define PolarizN 75
#define RxInfoN 76
#define HistoryN 77
#define DTAbsN 78
#define NumArrayN 79
#define ArrayLocN 80
#define NAxis1N 81
#define CType1N 82
#define DataTypeN 83
#define CRVal1N 84
#define CDelt1N 85
#define CRPix1N 86
#define BandwidN 87
#define DataN 88
#define NUMDEFINED 89
/* scan2_defs.h will be put in above here to make scan2.h */
/* number of extra entries in valDes for unknown scan header values */
#if COMB
#    define NUMEXTRAS 32
#else
#    define NUMEXTRAS 0
#endif /* COMB */

/* Define mnemonics for the header numbers */
#define MAIN_HDR 0
#define SCAN_HDR 1
#define ARRAY_HDR 2
#define UNDEFINED -1

#define GDI(N, I) scan2_getd(N, I)
#define GD0(N) scan2_getd(N, 0)
#define GII(N, I) scan2_geti(N, I)
#define GI0(N) scan2_geti(N, 0)
#define GS(N) scan2_gets(N)
#define PDI(val, N, I) scan2_putd(val, N, I)
#define PD0(val, N) scan2_putd(val, N, 0)
#define PII(val, N, I) scan2_puti(val, N, I)
#define PI0(val, N) scan2_puti(val, N, 0)
#define PS(val, N) scan2_puts(val, N)
#define HEXISTS(N) (valDes[N].whichHdr >= 0)
/* Note that WHICHHDR will not match if the header description wasn't found */
#define WHICHHDR(N) (valDes[N].whichHdr)

/* Structure for describing each header entry. */
struct val_description {
	char keyword[8];	/* The keyword expected in the main hdr */
	char whichHdr;		/* Values are MAIN_HDR, SCAN_HDR, & ARRAY_HDR */
	char type;		/* format char from header - 'I', 'E', etc. */
	short repeat;
	int offset;
};
extern struct val_description valDes[];
extern int valDesSize;

/* Structure for holding public scan2 file info */
struct scan_info {
	int fd;
	int fileType;		/* 0 old Bell Labs scan file, 
			         * 1 new Bell Labs,  2 for NRAO PDFL 
				 * 3 for NRAO SDD. See also ../scan/C.h */
	int nscan;		/* current scan number (<0 for invalid scan) */
	int firstScan;		/* Scan Number of first scan in file */
	int lastScan;		/* Scan Number of last sacn currently in file */
	int cNumArrays;		/* Number of arrays in the current scan */
	int cArray;		/* Number of the array currently set up */
	int cNumChans;		/* Number of channels in current array */
};
extern struct scan_info sci;

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* scan2.c */
int ReadPreamble P_((void));
double scan2_getd P_((int hdrN, int ind));
int scan2_geti P_((int hdrN, int ind));
char *scan2_gets P_((int hdrN));
int scan2_putd P_((double val, int hdrN, int ind));
int scan2_puti P_((int val, int hdrN, int ind));
int scan2_puts P_((char *str, int hdrN));
int ReadScan2 P_((int sn));
int SetScan2Array P_((int array));
int OpenScan2 P_((void));
void AllocateScan2MainHdr P_((int setup));
void PrintScan2ScanHdr P_((void));
int GetHdrByName P_((char *name));
void PrintScan2Arrays P_((void));
void LastScan2 P_((void));
void CopyScan2Preamble P_((int nfd));
void StartNextFile P_((int nfd));
void WriteScan2MainHdr P_((void));
void AllocateScan2Buf P_((int nArray, register int nChans[]));
int WriteScan2 P_((void));
void ReplaceScan2 P_((void));
void ChangeNextScan2 P_((register int nextScan));
void UpdateMainHdr P_((void));
int ChkRRead P_((int fd, int loc, char *buf, int size, char *reason));
int ChkRWrite P_((int fd, int loc, char *buf, int size, char *reason));

#undef P_
