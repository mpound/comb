struct of_scan
{
	short ndat[896];
	short nszsc,nfsc,numpts,nstrt;
	short invrt,longtd,lattd,if1,map,iexist;
	int lnfil,nbknd,mbknd,ncur,inver,kontrl,numbch;
	short nbadch[8];
	float tsky,tsys,fmult;
	int ndfn;		/* Fortran index of last char in datfn
				 * == strlen(datfn) */
	char datfn[PATHLENGTH];
	short iobstp, ibad;	/* ndat(13) and ndat(34) replacements */
	int iobssec;		/* Time of start of scan (sec since 1/1/70) */
	float azimuth, elevation; /* Az set to < 0 for old scans */
        double TAmbient,DTAbs;  /* Ambient temperature and DTAbs with scan */
        double IFCenter;        /* IF Reference Frequency */
        double VAnt;            /* Antenna Velocity wrt LSR */
        int mmsb;               /* which sideband 1 for USB, -1 for LSB */
        double fbCntr;          /* channel offset of IF Freq center of scan */
} ;
extern struct of_scan scan_;

	/******************************/
	/* structure for scan numbers */
	/******************************/
/* N. B. these members don't have the same names as in F.h */

typedef struct
{
	char fil[4];			/* file name */
	short num;			/* scan number */
}
Scan;
extern Scan curscn_;

struct of_STOscan
{
	short ndat[896];
	short nszsc,nfsc,numpts,nstrt;
	short invrt,longtd,lattd,if1,map,iexist;
	int lnfil,nbknd,mbknd,ncur,inver,kontrl,numbch;
	short nbadch[8];
	float tsky,tsys,fmult;
	int ndfn;		/* Fortran index of last char in dirfn
				 * == strlen(dirfn) */
	char dirfn[PATHLENGTH];
	short iobstp, ibad;	/* ndat(13) and ndat(34) replacements */
	int iobssec;		/* Time of start of scan (sec since 1/1/70) */
	float azimuth, elevation; /* Az set to < 0 for old scans */
        double TAmbient,DTAbs;  /* Ambient temperature and DTAbs with scan */
        double IFCenter;        /* IF Reference Frequency */
        double VAnt;            /* Antenna Velocity wrt LSR */
        int mmsb;               /* which sideband 1 for USB, -1 for LSB */
        double fbCntr;          /* channel offset of IF Freq center of scan */
} ;
extern struct of_STOscan STOscan_;

	/**********************************/
	/* structure for STO scan numbers */
	/**********************************/

typedef struct
{
	char filname[7];			/* file name */
	int filnum;			/* file number */
}
STOScan;
extern STOScan curSTOscn;


#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* scan.c */
void opend_ P_((void));
int read_ P_((short int *iscan));
int get_ P_((void));
void pscanhdr_ P_((void));
void pscandat P_((void));
#if 0
HdrVar *GetScan2HdrVar P_((char *name));
#endif
double Scan2HdrNumVal P_((int n, int i));
char *Scan2HdrStrVal P_((int n));
void write_ P_((void));
void mrkbad P_((int sn, int care));
int ScanInFile P_((register int sn));
void putfn_ P_((void));
void prtsl_ P_((char *out, int length));
int scanStr P_((register char *cp, int len, int start));
void scnlmt_ P_((short int *sn1n, short int *sn2n, char *sn1f, char *sn2f, int step, int snf));
int islast_ P_((void));
int nxtscn_ P_((void));

#undef P_

#if 0
/* scan.c*/

extern void LastScan();
extern void opend_();
extern int read_(/*iscan*/);
extern int get_();
extern void pscanhdr_();
extern void pscandat();
#if 0
extern HdrVar *GetScan2HdrVar(/*name*/);
#endif /*0*/
extern double Scan2HdrNumVal(/*n, i*/);
extern char *Scan2HdrStrVal(/*n);
extern void write_();
extern void mrkbad(/*sn,care*/);
extern int ScanInFile(/*sn*/);
extern void putfn_();
extern void prtsl_();
extern int scanStr(/*cp, len, start*/);
extern void scnlmt_(/*sn1n,sn2n,sn1f,sn2f,step,snf*/);
extern int islast_();
extern int nxtscn_();
extern int OpenSDD();
extern int OpenPDFL();
extern int ReadNRAO();
extern int getNRAO();
extern void LastScanPDFL();
extern void LastScanSDD();
extern void PrintNRAOVals();
extern void PrintScan2MainHdr();
#endif /*0*/

/* definitions which may be used in nrao.c and scan.c
 * These match sci.fileType.
 */
#define OLDBL 0
#define NEWBL 1
#define PDFL  2
#define SDD   3

extern char ObjectName[16];
extern int subscanReq; /* requested subscan number */
extern int cSubscan;   /* current subscan #, default = 0 */
