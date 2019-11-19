/* Routines to deal with NRAO SDD format scan files, make them 
 * understandable to gt and changes in gt transparent to user.
 * SDD is the replacement for PDFL.  We should be able to 
 * understand Green Bank scans now.  I'll have Gerrit check
 * this out.
 *
 * HISTORY: beta version: Tue Dec 14 08:54:59 EST 1993 -- MWP
 *          the klugy way--make everything parallel to pdfl.c.
 *          ultimately I think I can do some sort of combination
 *          that is more elegant
 *
 *  bug fixes (objectname) and move SDD keywords to scan2hdr.
 *  MWP Wed Apr 26 11:24:49 PDT 1995
 *
 * fixed "y2K" problem in NRAOdateStr - Fri Apr  9 12:41:04 EDT 1999 MWP
 * now conforms to FITS DATE standard 'CCYY-MM-DDThh:mm:ss'
 * 
 * On BYTEREVERSED machines we have to swap the bytes of sdd (but not
 * of pdfl!)  
 * Use Bob's routines (decfloat, vaxfloat) rather than NRAO ones (ieeeflt.c). 
 * NRAO ones apparently assume vax-like numbers (16-bit?), so don't
 * work on SDD. (although they should work on PDFL).
 *  27 APR 1999 - mwp
 *
 * NB: SDD strings are NOT null-terminated! 
 *
 *     sdd_dire.obsmode == 256*m + mode[n].id   (see modes.h)
 *                         m=2 --> LINE
 *                         m=1 --> CONT
 *			   n=0..NUMMODES
 *
 * Fri Sep  8 15:48:45 EDT 2000
 * mwp - removed forced warning of unknown obs types. and cross fingers.
 * Tue Oct 17 15:00:11 EDT 2000
 * mwp - set .FrstScan  and .LastScan from sci struct.
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h> 
#include <time.h> 
#include "../main/C.h"
#include "../stacks/C.h"
#include "../coordsys/C.h"
#include "../scan2/scan2.h"
#include "../error/C.h"
#include "pops.h"
#include "pdfl.h"
#include "sdd.h"
#include "modes.h"
#include "include.h"
#include "C.h"
#include "../main/machinedep.h"

#if SYSTEM_V
#define bcopy(s, d, n) memmove(d, s, n)
#define bzero(s, n) memset(s, 0, n)
#endif /*SYSTEM_V*/

/*-----------------------------------------*/ 
/* Some abbreviations...                   */
/*-----------------------------------------*/ 
#define TRUE 1
#define FALSE 0
#define DEBUG FALSE
#define BAD -1
#define GOOD 0
#define DD sizeof(double)
#define CC sizeof(char)
#define FF sizeof(float)
#define JJ sizeof(int)
#define II sizeof(short)
#define VWS (vdp->whichHdr = SCAN_HDR)
#define VWM (vdp->whichHdr = MAIN_HDR)
#define VWA (vdp->whichHdr = ARRAY_HDR)
#define VTD (vdp->type = 'D')
#define VTA (vdp->type = 'A')
#define VTJ (vdp->type = 'J')
#define VTI (vdp->type = 'I')
#define VSO (vdp->offset = Soffset)
#define VMO (vdp->offset = Moffset)
#define VAO (vdp->offset = Aoffset)
#define MAINBUFSIZE 512  /* a reasonable number */
#define ARRAYBUFSIZE 256 /* a reasonable number */
/*-----------------------------------------*/ 

/* 512 bytes are reserved for BOOTSTRAP whether or not it actually 
 * fills it all 
 */
#define BTSTRPSIZE	512

/*--------------------------------------------------------*/
/* Various routines to swap bytes on SDD and PDFL structs */
/*--------------------------------------------------------*/
void swapBoot();
void swapDirectory();
void swapData();
void printBoot();
void printDirectory();
void printPointers(struct CLASSPTRS *csp);
void InitHeader();
void ReadClasses(off_t position);
int  getPoscode();
short getObsmode();

void swapClassPtr(struct CLASSPTRS *csp);
void swapClass1(struct CLASS1 *c);
void swapClass2(struct CLASS2 *c);
void swapClass3(struct CLASS3 *c);
void swapClass4(struct CLASS4 *c);
void swapClass5(struct CLASS5 *c);
void swapClass6(struct CLASS6 *c);
void swapClass7(struct CLASS7 *c);
void swapClass8(struct CLASS8 *c);
void swapClass9(struct CLASS9 *c);
void swapClass10(struct CLASS10 *c);
void swapClass11(struct CLASS11 *c);
void swapClass12(struct CLASS12 *c);
void swapClass13(struct CLASS13 *c);

/* and move around in those structs... */
void SeekBoot(int fd);
void Rewind(int fd);
/*--------------------------------------------------------*/

/*--------------------------------------------------------*/
/* And these are the structs I was talking about          */
/*--------------------------------------------------------*/
static struct pdfl_BOOTSTRAP pdfl_Boot;
static struct pdfl_DIRECTORY pdfl_dire;
static struct sdd_BOOTSTRAP sdd_Boot;
static struct sdd_DIRECTORY sdd_dire;
static struct CLASSPTRS cptr;
static struct CLASS1 class1;
static struct CLASS2 class2;
static struct CLASS3 class3;
static struct CLASS4 class4;
static struct CLASS5 class5;
static struct CLASS6 class6;
static struct CLASS7 class7;
static struct CLASS8 class8;
static struct CLASS9 class9;
static struct CLASS10 class10;
static struct CLASS11 class11;
static struct CLASS12 class12;
static struct CLASS13 class13;
static struct CLASS_STRINGS ClassStr;
/*--------------------------------------------------------*/


static int PtrToClass[15];/* offset in kofus from header to Class N
                           * kofu == Ken Olson Fortran Unit
                           * conversion of kofus to bytes
                           *      struct HEADER *h;
                           *      int oneptr;                           ! KOFUS
                           *
                           *      oneptr =  (h->oneptr - 1) * 8; ! BYTES
                           *      ...
                           *      lseek(fd, offset+oneptr, SEEK_SET );
		           *
                           * these have to be ints, not shorts so can't
			   * "reuse" cptr.x 
                           */

float NRAOvals[MAXCHN]; /* the actual data values */
char NRAOdatestr[32];   /* a date string */
char ObjectName[16];    /* Source name   */

static char ctype1vals[][4] = {"Fr", "Az", "Dc", "L"}; /* took these values 
						        * from RWW's note to 
						        * Tom Kuiper 
					 		*/
static char nplcType[] =        {  99,     30,    11,    30 };

/*--------------------------------------------------------------------*/
/* These are the possible values for backend, obsmode, taken from the */
/* current control system. 1/92                                       */
/* for the hybrid spectrometer, backend is always "HySpec  "          */
/* for the hybrid spectrometer, obsmode is always "LINE" + one of obstype_strs[]*/
/* for the filterbank/dbe data writer:                                */
/* static char *backend_strs[] = {"        ", "STD A/D ",  */
/*                        "DIGITAL ", "HOLOGRAF", ".025MHZF", ".03MHZFB", */
/*                        ".1MHZFB ", ".25MHZFB", ".5MHZFB ", "1.0MHZFB", */
/*                        "2.0MHZFB", "HYBRCORR"}; */

/*   static char *obstype_strs[] = {"    ", "PS  ", "APS ", "FS  ", "BSP ", */
/*                           "TPON", "TPOF", "ATP ", "PSM ", "APM ", "FSM ", */
/*                           "TPMO", "TPMF", "DRFT", "PCAL", "BCAL", "BLNK", */
/*                           "SEQ ", "FIVE", "MAP ", "FOC ", "NSFC", "TTIP", */
/*                           "STIP", "D_ON", "CAL ", "FSPS", "EWFC", "ZERO"}; */

/* static char *obsmode_strs[] = {"LINE    ","CONT    "};              */
/*---------------------------------------------------------------------*/


int cSubscan = 0;   	/* current subscan */
int subscanReq = 1;   	/* current requested subscan */
static double fbCntr;	/* CRVal1==offset in MHz from Restfreq of CrPix1 */

/*------------------------------------------------------------------------ */
/* scan2 stuff so's we can use PrintScan2ScanHdr() 			   */
/* The memory for all of the headers and data will be obtained from malloc */
/*------------------------------------------------------------------------ */
extern char *hdrPtr[3];         /* Pointers to the headers.  The Scan follows
                                 * its hdr & array data its hdr
			         */
extern int hdrSize[3];		/* size of headers */
extern char *strValue;     /* String buffer set up by malloc */
extern int strValueSize;   /* Current size of buffer */
extern int strValueLoc;    /* Loc of string being returned by scan2_gets*/

extern int scanBufSize;    /* Size of block allocated for scan hdr */
			   /* shared with scan2.c 		   */

extern int valDesSize;
/*------------------------------------------------------------------------ */

/**
 * Open a PDFL format file 
 */
int OpenPDFL() 
{
	bzero(&pdfl_Boot,sizeof(struct pdfl_BOOTSTRAP) );
	Rewind(sci.fd);
	if(read(sci.fd, &pdfl_Boot, sizeof(struct pdfl_BOOTSTRAP))<0 ) {
    		error_("OpenPDFL: Could not read PDFL Bootstrap");
    	 	return(BAD);
  	}

	sci.fileType=PDFL;
	swapBoot(); /* swap bytes if necessary */
/*
 * Tests for non-pdfl file here. 
 * As of 2/3/93, 224--> 232 (nbdt)
 *                56--> DIRECTORYSIZE  (nbpe)
 */
	if( (pdfl_Boot.nbdt!= 232) || (pdfl_Boot.nemax!=2048) || 
	   (pdfl_Boot.nbpe!=DIRECTORYSIZE) ) {
		return(BAD);
	}
/* negative values are always bad */
	if( (pdfl_Boot.nxte< 0) || (pdfl_Boot.nxtde < 0) || 
	    (pdfl_Boot.ndb < 0) || (pdfl_Boot.nbcal < 0) ||
	    (pdfl_Boot.nbsv < 0) || (pdfl_Boot.nbwrk < 0) ||
	    (pdfl_Boot.nbscr < 0) || (pdfl_Boot.pftype < 0) ) {
		return(BAD);
	}

	sci.fileType=PDFL;

	InitHeader();
	return(GOOD);
}



/**
 * Open an SDD format file
 */
int OpenSDD() 
{

	bzero(&sdd_Boot,512);
	Rewind(sci.fd);

	if(read(sci.fd, &sdd_Boot, sizeof(struct sdd_BOOTSTRAP)) <0) 
	{
    		error_("OpenSDD: Could not read SDD Bootstrap");
    	 	return(BAD);
  	}

	sci.fileType=SDD;
	swapBoot(); /* swap boot block if necessary */


/* test for non-SDD here */
	if(sdd_Boot.typesdd != 1 && sdd_Boot.typesdd !=0)
		return(BAD);
	if(sdd_Boot.version != 1 && sdd_Boot.version != 0)
		return(BAD);
/* now more restrictive testing */
	if(sdd_Boot.num_index_rec < 1 || sdd_Boot.num_data_rec < 0
           || sdd_Boot.num_entries_used < 0 
           || sdd_Boot.bytperent > sdd_Boot.bytperrec )
		return(BAD);

/* ok it's probably SDD by now...throw out types we don't know about */
	if(sdd_Boot.version == 0) {
		error_("I don't understand SDD version 0 yet.");
		return(BAD);
	}
	if(sdd_Boot.typesdd == 1) {
		error_("I don't understand SDD type 1 (Green Bank) yet.");
		return(BAD);
	}

	sci.fileType=SDD;
	InitHeader();
	return(GOOD);
}

/**
 * Initialize the header arrays and such.
 */
void InitHeader() {

	struct val_description *vdp;
	int stringSize = 20;
	Rewind(sci.fd);
    	sci.lastScan = 0;
	LastScan();
    	sci.nscan = sci.lastScan + 1;
	sci.cNumArrays = 1;
	sci.cArray = 0;
	sci.cNumChans = 0;
 	/* allocate space for headers and such 			*/
	/* ala ReadPreamble(), this is done only once 		*/
        /* max we will need is size of all classes of interest 	*/
        /* Note that Bob does not malloc a separate chunk for the array header,
         * in scan2.c. hdrPtr[ARRAY_HDR] points to ArrayLoc in the scan
         * header. Therefore, add ARRAYHDRSIZE to scanBufSize and then
         * don't malloc for hdrPtr[ARRAY_HDR].
         */
        scanBufSize = sizeof(struct CLASS1) +
                sizeof(struct CLASS2) + sizeof(struct CLASS4) +
                sizeof(struct CLASS5) + sizeof(struct CLASS6) +
                sizeof(struct CLASS7) + sizeof(struct CLASS8) +
                sizeof(struct CLASS9) + sizeof(struct CLASS10) +
                sizeof(struct CLASS12) + ARRAYBUFSIZE;
        noint_();
        if(hdrPtr[SCAN_HDR])
            ChkFree(hdrPtr[SCAN_HDR]);
     	if(hdrPtr[MAIN_HDR])
              ChkFree(hdrPtr[MAIN_HDR]);

        hdrSize[MAIN_HDR] = MAINBUFSIZE;
        hdrSize[SCAN_HDR] = scanBufSize;
        hdrSize[ARRAY_HDR] = ARRAYBUFSIZE ;
    	hdrPtr[MAIN_HDR] = ChkAlloc(MAINBUFSIZE, "Main hdr of NRAO file");
	bzero(hdrPtr[MAIN_HDR],hdrSize[MAIN_HDR]);
        hdrPtr[SCAN_HDR] = ChkAlloc(scanBufSize, "NRAO Scan hdr buffer");
	bzero(hdrPtr[SCAN_HDR],hdrSize[SCAN_HDR]);
        hdrPtr[ARRAY_HDR] = hdrPtr[SCAN_HDR]+scanBufSize - ARRAYBUFSIZE;

       /* Set up the string buffer */
       if((stringSize *= 2) > strValueSize) {
        if(strValue)
                ChkFree(strValue);
        strValueSize = stringSize;
        strValue = ChkAlloc(strValueSize, "Data file str buffer");
	bzero(strValue,strValueSize);
        }  
        yesint_();
        /* add the extras to the value description */

/* the 80-char history string is the source of the core dump bug
 * I have been experiencing. If .History is printed in anyway (with p
 * or pd) thenn the next command will cause a core dump. This must
 * have to do with the way PrintScan2Val works.  I should  investigate this
 * further, but the current fix is to delete this keyword.
 *
 * Also note the openpar description in the POPS manual:
 * "Puts a field of 56 chars (the entire string is 80 chars in length
 *  but the 1st 24 are reserved for stacking) of the data in the currently
 *  referenced array into the adverb, SOPENPAR. This adverb may then be
 *  be printed. These 56 chars are available to the observer for description
 *  of his data processing to be stored in the header of the data."
 *
 * However in Pops.h openpar is declared a double!
 *
 */
/*        strncpy(valDes[HistoryN].keyword ,"History",8); */

        /* Clear the value description */
        for(vdp = valDes; vdp < &valDes[NUMDEFINED+NUMEXTRAS]; vdp++ ) {
                vdp->whichHdr = UNDEFINED;
                vdp->repeat = 0;
                vdp->offset = 0;
        }
        valDesSize = NUMDEFINED+NUMEXTRAS;
/*
 * Re-initialize valDes[SubScanN] for NRAO so that gt returns the correct error
 * message for non-existent requested subscan. Normally, valdes[SubScanN] 
 * won't get set in ReadNRAO() unless the requested subscan exists. Thus, if the
 * requested subscan is invalid, the GI0(SubScanN) test in gt.c returns iDRAGON,
 * which I am using as an indicator of a BLSDF scan!  In that case, gt happily
 * continues, but ultimately returns "Bad backend number", which is not the
 * true error. Below, I set valDes[SubScanN]=0, which is always invalid, but is 
 * not, of course, iDRAGON. This valDes[SubScanN] initialization below get 
 * overwritten in ReadNRAO() when a valid subscan is requested.
 */
        vdp = &valDes[SubScanN];
	vdp->whichHdr = SCAN_HDR;
	vdp->type = 'J';
	vdp->offset=0;
        vdp->repeat = 1;
        PI0(cSubscan,SubScanN);
/* reset it so that it is always ChFreed in ReadScan2()*/
	scanBufSize = 0; 

	Rewind(sci.fd);
	return;
}

/** 
 * Read an NRAO scan, either type SDD or PDFL.
 * This is the way subscan numbers run for NRAO:
 *
 *         Parallel Filterbanks
 * subscan
 *
 *  01  First half First polarization
 *  02  Second half First polarization
 *
 *  03  First half Second Polarization
 *  04  Second half Second Polarization
 *
 *        Series Filterbanks
 *
 *  01  First polarization
 *  03  Second Polarization
 *
 *        Hybrid Correlator
 *
 *  11  First polarization
 *  12  Second Polarization
 *  1x  x is 1 through 4 in four IF mode
 *  1x  x is 1 through 8 for the 8-beam
 */
int ReadNRAO(int scannum) 
{
	extern int round(); /* in image.c */
	int dirno;
	off_t position, max;
	SeekBoot(sci.fd); 

	switch(sci.fileType) {
	case PDFL:
          max = pdfl_Boot.nemax; 
          for( dirno=0; dirno<max ; dirno++ ) {
            if(read(sci.fd, &pdfl_dire, DIRECTORYSIZE) <0 ) {
                error("Error reading %d-th PDFL directory",dirno+1);
            }

            swapDirectory(); /* swap bytes if necessary */
	    cSubscan = pdfl_dire.subscan; 
            if((scannum == pdfl_dire.scan) && (subscanReq == cSubscan)) {
		sci.nscan = (short)scannum;
          	break;
	    }
	  }
	  break;
	case SDD:
	  max =sdd_Boot.num_entries_used;
    	  for( dirno=0; dirno<max; dirno++ ) {
      	    if(read(sci.fd, &sdd_dire, sdd_Boot.bytperent) <0 ) {
        	error("Error reading %d-th SDD directory",dirno+1);
            }

	    swapDirectory(); /* swap bytes if necessary */
	    cSubscan = (int)round(100.*(sdd_dire.scan - (int) sdd_dire.scan)); 
            if((scannum == (int)sdd_dire.scan) && (subscanReq == cSubscan)) {
		sci.nscan = (short)scannum;
          	break;
	    }
    	  }
	  break;
	}
#if DEBUG
	    printDirectory();
#endif

        if(dirno>=max)
		return(GOOD); /* subscan mismatch now checked for in  gt() */

/* Go to the requested scannumber and get the header */
	  switch(sci.fileType) {
		case SDD:
			position = (sdd_dire.start_rec-1)*sdd_Boot.bytperrec;
			break;
		case PDFL:
        		position = (pdfl_dire.blk_fb1-1) * 512;
			break;
	  }
	lseek(sci.fd, position ,SEEK_SET);

        ReadClasses(position);
        return(GOOD);
}

/**
 * read each class separately, read class pointers first 
 */
void ReadClasses(off_t position) {
        extern int round(); /* in image.c */
	struct val_description *vdp;
	struct tm  obstime;
	time_t gmt;
        int i, dirno, day, month, year, oldwarn;
        int Soffset, Moffset, Aoffset; /* offset in Scan/Main/Array header so far */
	int thisObsmode;
        float fdum;
        double ddum;
        char s[16], *sp;

	bzero(ClassStr.obsid,16);        
	bzero(ClassStr.observer,32);    
	bzero(ClassStr.telescop,16);   
 	bzero(ClassStr.projid,16);    
 	bzero(ClassStr.object,32);   
 	bzero(ClassStr.obsmode,16); 
 	bzero(ClassStr.frontend,16);
 	bzero(ClassStr.backend,16);
 	bzero(ClassStr.precis,16);
 	bzero(ClassStr.pt_model,16);
 	  /*bzero(ClassStr.cl11type,16); Jeff H. sez ignore this (2/2/93)*/
 	bzero(ClassStr.coordcd,16);
 	bzero(ClassStr.frame,16);    
 	bzero(ClassStr.veldef,16);   
 	bzero(ClassStr.typecal,16);  
 	  /* bzero(ClassStr.phastN,64); ignore this, too   */
 	bzero(ClassStr.polariz,16); 
 	bzero(ClassStr.rx_info,32);
 	bzero(ClassStr.openpar,128);  
	bzero(NRAOvals,MAXCHN*sizeof(float));
	bzero(NRAOdatestr,32);

/* don't use PtrToClass[0] to make for easy reading */
	if(read(sci.fd,&cptr,sizeof(struct CLASSPTRS)) != sizeof(struct CLASSPTRS))
                error_("Error reading NRAO class pointers"); 

	swapClassPtr(&cptr); /* swap bytes if necessary */

	PtrToClass[1]= (cptr.one-1)*8;
	PtrToClass[2]= (cptr.two-1)*8;
	PtrToClass[3]= (cptr.three-1)*8;
	PtrToClass[4]= (cptr.four-1)*8;
	PtrToClass[5]= (cptr.five-1)*8;
	PtrToClass[6]= (cptr.six-1)*8;
	PtrToClass[7]= (cptr.seven-1)*8;
	PtrToClass[8]= (cptr.eight-1)*8;
	PtrToClass[9]= (cptr.nine-1)*8;
	PtrToClass[10]= (cptr.ten-1)*8;
	PtrToClass[11]= (cptr.eleven-1)*8;
	PtrToClass[12]= (cptr.twelve-1)*8;
	PtrToClass[13]= (cptr.thirteen-1)*8;

	Moffset = Soffset = Aoffset = 0;

/*
 * Since we know know sci.firstScan and sci.lastScan, put them
 * in scan2 hdr vars FrstScan and LastScan.
 */
	vdp = &valDes[FrstScanN];
	VWM; VTJ; VMO;
	vdp->repeat = 1;
        PI0(sci.firstScan,FrstScanN);
	Moffset += 8*JJ; /* times 8, just to make sure it is a word boundary*/
        vdp = &valDes[LastScanN];
        VWM; VTJ; VMO;
        vdp->repeat = 1;
        PI0(sci.lastScan,LastScanN);
        Moffset += 8*JJ; /* times 8, just to make sure it is a word boundary*/

/* now read in the classes */
/* Class 1 */
	lseek(sci.fd,position+PtrToClass[1],SEEK_SET);
	if(read(sci.fd,&class1,sizeof(struct CLASS1) ) < 0 )
                error_("Error reading NRAO class 1");
	
	swapClass1(&class1); /* swap bytes if necessary */

	vdp = &valDes[ScanNumN];
	VWS; VTD; VSO;
	vdp->repeat = 1;
	PD0(class1.scan,ScanNumN);
	Soffset += DD;
	vdp = &valDes[SubScanN];
	VWS; VTJ; VSO;
	vdp->repeat = 1;
	PI0(cSubscan,SubScanN);
	Soffset += 8*JJ;
	bcopy(class1.obsid,ClassStr.obsid,8);
	bcopy(class1.observer,ClassStr.observer,16);
	bcopy(class1.telescop,ClassStr.telescop,8);
	vdp = &valDes[TelescopN];
	VWM; VTA; VMO;
	vdp->repeat = 8;
	PS(ClassStr.telescop,TelescopN);
	Moffset += 8*CC;
        vdp = &valDes[ObserverN];
        VWM; VTA; VMO;
        vdp->repeat = 16;
        PS(ClassStr.observer,ObserverN);
        Moffset += 16*CC;
	bcopy(class1.projid,ClassStr.projid,8);
	vdp = &valDes[ProjIDN];
        VWM; VTA; VMO;
        vdp->repeat = 8;
        PS(ClassStr.projid,ProjIDN);
        Moffset += 8*CC;
	bcopy(class1.object,ClassStr.object,16);
        strncpy(ObjectName,ClassStr.object,16);
/* Bob's trick to trim white space from end of object name */
        i=sizeof(ObjectName);
        while(i-- > 0 && isspace(ObjectName[i]))
                ObjectName[i] = 0;
	vdp = &valDes[ObjectN]; 
	VWS;VTA;VSO;
	vdp->repeat = 16;
	PS(ClassStr.object,ObjectN);
	Soffset += 16*CC;
	bcopy(class1.obsmode,ClassStr.obsmode,8);
/* trim white space to make "type:" comparisons in gt more user-friendly */
	i=8;
        while(i-- > 0 && isspace(ClassStr.obsmode[i]))
                ClassStr.obsmode[i] = 0;
	bcopy(class1.frontend,ClassStr.frontend,8);
	bcopy(class1.backend,ClassStr.backend,8);
	bcopy(class1.precis,ClassStr.precis,8);
	vdp = &valDes[SpecTypeN];
        VWM;VTA;VMO;
        vdp->repeat=8;
        PS(ClassStr.backend,SpecTypeN);
        Moffset +=8*CC;
#if DEBUG
printf("ReadNRAO: headlen %f, datalen %f, scan %.2f\n\n",class1.headlen,
class1.datalen, class1.scan+((float)scan_.nbknd/100.));   
	printf("obsid     :%s:\n",ClassStr.obsid);
	printf("observer  :%s:\n",ClassStr.observer);
	printf("projid    :%s\n",ClassStr.projid);
	printf("object    :%s:\n",ClassStr.object);
	printf("obsmode   :%s:\n",ClassStr.obsmode);
	printf("telescope :%s:\n",ClassStr.telescop);
	printf("frontend  :%s:\n",ClassStr.frontend);
	printf("backend   :%s:\n",ClassStr.backend);
	printf("prec      :%s:\n",ClassStr.precis);
#endif

/* check for valid scan type; Set scan_.iobstp */

	scan_.iobstp = 0;
	vdp = &valDes[ObsTypeN];
	vdp->repeat=8;
	VWS;VTA;VSO;
	PS(ClassStr.obsmode,ObsTypeN);
	Soffset += 8*CC;
	thisObsmode=(int)getObsmode();
	if(thisObsmode > 256+NUMMODES) {
        	i = thisObsmode-512;
	}
	else {
        	i = thisObsmode-256;
	}
	if(i<0)
		error("nrao.c: obsmode < 0!!! programming error??");
	switch(i) {
	 case 0: /* no mode present */
		break;
	 case 4:
                        scan_.iobstp = 5; 
			break;
       	 case 1:
	 case 2:
                        scan_.iobstp = 2; 
                        break;
	 case 8:
	 case 9:
                        scan_.iobstp = 2; 
                        break;
       	 case 3:
	 case 10:
                        scan_.iobstp = 1;
                        break;
	 case 26:
                        scan_.iobstp = 3;
			break;

	 case 13:
	 case 36:
                        scan_.iobstp = 22;
			break;
	 case 14:
	 case 15:
	 case 25:
                        scan_.iobstp = 10;
	  		break;
	 case 7:
	 case 5:
	 case 11:
                        scan_.iobstp = 8;
			break;
	 case 6:
	 case 12:
                        scan_.iobstp = 9;
			break;
/*	 case 19:
	 case 24:
	 case 33:
                        scan_.iobstp = 30;
			break; 
*/
      default :
/* MWP COMMENTED OUT Fri Sep  8 15:49:42 EDT 2000 */
/* since this can cause a segmentation fault for certain data types, we always want
 * to warn, even if warning is currently off
 */
                /*oldwarn=main_.warny;
                 *main_.warny=FALSE;
		 */
                warn("Can't deal properly with \"%s\" observations",modes[i].description);
                /*main_.warny=oldwarn; */
                break;
    	} /* switch */

/* Class 2 */
        lseek(sci.fd,position+PtrToClass[2],SEEK_SET);
        if(read(sci.fd,&class2,sizeof(struct CLASS2) ) < 0 ) 
                error_("Error reading NRAO class 2");

	swapClass2(&class2); /* swap bytes if necessary */

	vdp = &valDes[AzDeltaN];
	vdp->repeat = 1;
	VWS; VTD; VSO;
	ddum = (class2.xpoint + class2.uxpnt)/3600.0;
 	PD0(ddum,AzDeltaN);	
	Soffset += DD;
	vdp = &valDes[ElDeltaN];
	vdp->repeat = 1;
	VWS; VTD; VSO;
	ddum = (class2.ypoint + class2.uypnt)/3600.0;
 	PD0(ddum,ElDeltaN);	
	Soffset += DD;
	bcopy(class2.pt_model,ClassStr.pt_model,8);

/* Class 3 */ 
        lseek(sci.fd,position+PtrToClass[3],SEEK_SET);
        if(read(sci.fd,&class3,sizeof(struct CLASS3) ) < 0 )  
                error_("Error reading NRAO class 3");

	swapClass3(&class3); /* swap bytes if necessary */

	year = round(class3.utdate);
	fdum = class3.utdate - (float)year;   	     /* .MMDD */
	month =  round((fdum*100.));  		     /* (int)(MM.DD) */
	day   = round(((fdum*100. - month)*100.));       /* (int) DD. */
	bzero(s,16);
	HmsPrint(s,0,class3.ut);
	/* kluge. HmsPrint is returning ' [h]h:mm:ss'
         * so change leading space to 'T' and 2nd char to '0'
	 * of it is '0'.  I tried changing HmsPrint but it
	 * totally screwed up other comb hexagesimal labels.
	 * -mwp 
	 */
	sp = s;
	*sp='T';
	if (*++sp == ' ') *sp='0';
	sprintf(NRAOdatestr,"%4d-%02d-%02d%s",year,month,day,s);
	/*printf(" NRAOdatestr:%s\n",NRAOdatestr);*/
	/* now set up the obstime struct so that we can set J2Second */
	obstime.tm_hour=(int)class3.ut;
	obstime.tm_min=(int)((class3.ut-obstime.tm_hour)*60);
	obstime.tm_sec=round( ((class3.ut-obstime.tm_hour)*60 - obstime.tm_min)*60);
	/*printf("h m s %d %d %d\n",obstime.tm_hour,obstime.tm_min,obstime.tm_sec);*/
	obstime.tm_mday=day;
	obstime.tm_mon=month-1;
	obstime.tm_year=year-1900;
	obstime.tm_isdst=0;
	putenv("TZ=GMT");
	gmt=mktime(&obstime);
	vdp = &valDes[J2SecondN];
	VWS; VTJ; VSO;
	vdp->repeat = 1;
	PI0(unixToJ2S(gmt),J2SecondN);
	Soffset += 8*JJ;
	/*printf(" backconverted:%s\n",j2sToDate(GI0(J2SecondN)));*/
  
	if(class3.norchan < 1)
	   error("Less than 1 receiver");

/* Class 4 */ 
        lseek(sci.fd,position+PtrToClass[4],SEEK_SET);
        if(read(sci.fd,&class4,sizeof(struct CLASS4) ) < 0 )  
                error_("Error reading NRAO class 4");

	swapClass4(&class4); /* swap bytes if necessary */

	bcopy(class4.coordcd,ClassStr.coordcd,8);
	vdp = &valDes[AzimuthN];
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class4.az,AzimuthN);
	Soffset += DD;
	vdp = &valDes[ElevatioN];
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class4.el,ElevatioN);
	Soffset += DD;
	vdp = &valDes[PosTypeN];
	vdp->repeat = 8;
	VWS; VSO; VTA;
	PS(ClassStr.coordcd,PosTypeN);
	Soffset += 8*CC;

	/* Class 5 */ 
	lseek(sci.fd,position+PtrToClass[5],SEEK_SET);
	if(read(sci.fd,&class5,sizeof(struct CLASS5) ) < 0 )  
		error_("Error reading NRAO class 5");

	swapClass5(&class5); /* swap bytes if necessary */

	vdp = &valDes[TAmbientN]; /* this has a different meaning 
				   * than scan2 TAmbient */
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class5.tamb,TAmbientN);
	Soffset += DD;	
	vdp = &valDes[PressureN];
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class5.pressure,PressureN);
	Soffset += DD;
	vdp = &valDes[HumidityN];
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class5.humidity,HumidityN);
	Soffset += DD;
	vdp = &valDes[RefracN];
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class5.refrac,RefracN);
	Soffset += DD;
	vdp = &valDes[DewPtN];
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class5.dewpt,DewPtN);
	Soffset += DD;
	vdp = &valDes[MmH2ON];
	vdp->repeat = 1;	
	VWS; VSO; VTD;
	PD0(class5.mmh2o,MmH2ON);
	Soffset += DD;

	/* Class 6 */ 
	lseek(sci.fd,position+PtrToClass[6],SEEK_SET);
	if(read(sci.fd,&class6,sizeof(struct CLASS6) ) < 0 )  
			error_("Error reading NRAO class 6");

	swapClass6(&class6); /* swap bytes if necessary */

	bcopy(class6.frame,ClassStr.frame,8);
 	switch(getPoscode()) {
	  case X1950RADC :
	  case EPOCRADC  :
	  case MEANRADC  :
	  case X2000RADC :
  	  case INDRADC   :
	  case APPRADC   :
		 vdp = &valDes[EpochN];
		 vdp->repeat = 1;
		 VWS, VTD, VSO; 
		 PD0(class4.epoch,EpochN);
		 Soffset+=DD;
		 vdp = &valDes[RaRefN];
		 VWS; VTD; VSO;  
		 vdp->repeat = 1;
		 ddum = class4.xref/15.0;
		 PD0(ddum,RaRefN);
		 Soffset += DD;  
		 vdp = &valDes[DecRefN];
		 VWS; VTD; VSO;  
		 vdp->repeat = 1;
		 PD0(class4.yref,DecRefN);
		 Soffset += DD;  
		 vdp = &valDes[RaMapN];
		 VWS; VTD; VSO;
		 vdp->repeat = 1;
		 ddum= ((class4.xsource-class4.epocra)*(double) 60.);
		 ddum *= cos(class4.ysource*cunits[DEGREES]);
		 PD0(ddum,RaMapN);
		 Soffset += DD;
		 vdp = &valDes[CentRaN];
		 VWS; VTD; VSO;
		 vdp->repeat = 1;
		 ddum  = class4.epocra/(double) 15.;
		 PD0(ddum,CentRaN);
		 Soffset += DD; 
		 vdp = &valDes[DecMapN];
		 VWS; VTD; VSO;
		 vdp->repeat = 1;
		 ddum = (class4.ysource - class4.epocdec)*(double) 60.;
		 PD0(ddum,DecMapN);
		 Soffset += DD; 
		 vdp = &valDes[CentDecN];
		 VWS; VTD; VSO;
		 vdp->repeat = 1;
		 ddum = class4.epocdec;
		 PD0(ddum,CentDecN);
		 Soffset += DD; 
		 break;
	  case GALACTIC  :
/* no idea what to do here is PS instead of APS !!! */
		 vdp = &valDes[LRefN];
		 VWS; VTD; VSO;
		 vdp->repeat = 1;
		 PD0(class4.xref,LRefN);
		 Soffset += DD;
         	 vdp = &valDes[BRefN];
         	 VWS; VTD; VSO;
          	 vdp->repeat = 1;
	         PD0(class4.yref,BRefN);
		 vdp = &valDes[LMapN];
		 VWS; VTD; VSO;
		 vdp->repeat = 1;
		 ddum = class6.xzero/3600.;
		 PD0(ddum,LMapN);
		 Soffset += DD;
		 vdp = &valDes[BMapN]; 
		 VWS; VTD; VSO;
		 vdp->repeat = 1; 
		 ddum = class6.yzero/3600.;
		 PD0(ddum,BMapN); 
		 Soffset += DD; 
		 vdp = &valDes[CentLN];  
		 VWS; VTD; VSO; 
		 vdp->repeat = 1;  
		 ddum = class4.xsource - class6.xzero/3600.; 
		 PD0(ddum,CentLN);  
		 Soffset += DD;
		 vdp = &valDes[CentLN];  
		 VWS; VTD; VSO;     
		 vdp->repeat = 1;   
		 ddum = class4.ysource - class6.yzero/3600.;  
		 PD0(ddum,CentBN);   
		 Soffset += DD;
		 break;
	  case X1950ECL  :
	  case EPOCECL   :
	  case MEANECL   :
	  case APPHADC   :
	  case APPECL    :
	  case AZEL      :
	  case USERDEF   :
		warn("I don't understand this coordinate system: %s\n",ClassStr.coordcd);
		break;
	  case BAD:
		error("Unrecognized position code (not really an NRAO scan?)");
	}
/* Class 7 */ 
        lseek(sci.fd,position+PtrToClass[7],SEEK_SET);
        if(read(sci.fd,&class7,sizeof(struct CLASS7) ) < 0 )  
                error_("Error reading NRAO class 7");

	swapClass7(&class7); /* swap bytes if necessary */

	bcopy(class7.veldef,ClassStr.veldef,8);
	bcopy(class7.typecal,ClassStr.typecal,8);
	vdp = &valDes[BeamFWHMN];
	vdp->repeat = 1;
	VWS; VTD; VSO;
/* make it really the FWHM! */
        class7.bfwhm *=2.0;
	PD0(class7.bfwhm,BeamFWHMN);
	Soffset += DD;
	vdp = &valDes[VAntN];
	VWS; VTD; VSO;     
        vdp->repeat = 1;    
        PD0(class7.rvsys,VAntN);    
        Soffset += DD;	
	vdp = &valDes[VSourceN];
        VWS; VTD; VSO;        
        vdp->repeat = 1;      
        PD0(class7.velocity,VSourceN);    
        Soffset += DD;
	vdp = &valDes[VRefSysN];
	VWS; VTA; VSO;
	vdp->repeat = 8;
	PS(ClassStr.veldef,VRefSysN);
	Soffset += 8*CC;
	vdp = &valDes[TypeCalN];
	vdp->repeat = 8;
	VWS; VTA; VSO;
	PS(ClassStr.typecal,TypeCalN);
	Soffset += 8*CC;
	
/* Class 8 */  
	lseek(sci.fd,position+PtrToClass[8],SEEK_SET); 
        if(read(sci.fd,&class8,sizeof(struct CLASS8) ) < 0 )   
                error_("Error reading NRAO class 8");

	swapClass8(&class8); /* swap bytes if necessary */

	vdp = &valDes[AppEffN];
	VWS; VTD; VSO;
	vdp->repeat = 1;
	PD0(class8.appeff,AppEffN);
	Soffset += DD;
/* antgain, etal, etafss not constant throughout file so
 * put them in Scan header 
 */
	vdp = &valDes[EtaFSSN];
	VWS; VTD; VSO;
	vdp->repeat = 1;
	PD0(class8.etafss,EtaFSSN);
	Soffset += DD;

/* Class 9 */  
          lseek(sci.fd,position+PtrToClass[9],SEEK_SET); 
          if(read(sci.fd,&class9,sizeof(struct CLASS9) ) < 0 )   
                error_("Error reading NRAO class 9");

	  swapClass9(&class9); /* swap bytes if necessary */

	  vdp = &valDes[HMHNN];
          VWS; VTD; VSO;
          vdp->repeat = 1;
          PD0(class9.harmonic,HMHNN);
          Soffset += DD;
	  vdp = &valDes[IFCenterN];
	  VWM; VTD; VMO;
	  vdp->repeat = 1;
	  PD0(class9.firstif,IFCenterN);
	  Moffset += DD;
	  vdp = &valDes[RefAzOffN];
          VWS; VTD; VSO;
          vdp->repeat = 1;
          PD0(class9.razoff,RefAzOffN);
          Soffset += DD;
	  vdp = &valDes[RefElOffN];
          VWS; VTD; VSO;
          vdp->repeat = 1;
          PD0(class9.reloff,RefElOffN);
          Soffset += DD;
	  vdp = &valDes[SidebandN];
	  VWS; VTJ; VSO;
	  vdp->repeat = 1;
	  i = (int) class9.sideband;
          PI0(i,SidebandN);
	  Soffset += 8*JJ; /* word boundary*/
          vdp = &valDes[SigIFN];
          VWS; VTD; VSO;
          vdp->repeat = 1;
/* NRAO software ignores SigIF and RefIF if not
 * frequency-switching !!  So we set them to
 * zero if not frequency-switching.
 */
	  if(scan_.iobstp != 1 && scan_.iobstp!=3) {
		class9.foffsig = 0.0;
		class9.foffref1 = 0.0;
		class9.foffref2 = 0.0;
	  }
          PD0(class9.foffsig,SigIFN);
	  Soffset += DD;
          vdp = &valDes[RefIFN];
          VWS; VTD; VSO;
          vdp->repeat = 1;
          PD0(class9.foffref1,RefIFN);
	  Soffset += DD;
          vdp = &valDes[RefIF2N];
          VWS; VTD; VSO;
          vdp->repeat = 1;
          PD0(class9.foffref2,RefIF2N);
	  Soffset += DD;

#if 0
/* Class 10 */  
/* Class 10 is UNUSED because it cause a core dump. see above. : mwp 4/28/1999 */
        lseek(sci.fd,position+PtrToClass[10],SEEK_SET); 
        if(read(sci.fd,&class10,sizeof(struct CLASS10) ) < 0 )   
                error_("Error reading NRAO class 10");

	swapClass10(&class10); /* swap bytes if necessary */

	bcopy(class10.openpar,ClassStr.openpar,80);
	bzero(ClassStr.openpar,80); 
	vdp = &valDes[HistoryN];
	vdp->repeat = 80;
	VWS; VTA; VSO;
	PS(ClassStr.openpar,HistoryN);
	Soffset += 128*CC;
#endif

/* Class 11 */
/* Ignore these for now. sez Phil Jewell (2/2/93) */
/* 	lseek(sci.fd,position+PtrToClass[11],SEEK_SET);
 	if(read(sci.fd,&class11,sizeof(struct CLASS11) ) < 0 )
	error_("Error reading NRAO class 11");
	swapClass11(&class11); 
*/

/* Class 12 */ 
        lseek(sci.fd,position+PtrToClass[12],SEEK_SET);
        if(read(sci.fd,&class12,sizeof(struct CLASS12) ) < 0 )
                error_("Error reading NRAO class 12");

	swapClass12(&class12); /* swap bytes if necessary */

	sci.cNumChans = class12.noint;
	bcopy(class12.polariz,ClassStr.polariz,8);
	bcopy(class12.rx_info,ClassStr.rx_info,8);
	vdp = &valDes[LineFreqN];
	VWS; VTD; VSO;
	vdp->repeat = 1;
	PD0(class12.restfreq,LineFreqN);
	Soffset += DD;
	vdp = &valDes[CDelt1N];
	VWA; VTD; VAO;
	vdp->repeat = 1;
	PD0(class12.freqres,CDelt1N);
	Aoffset += DD;
        vdp = &valDes[CType1N];
        VWA; VTA; VAO;
        vdp->repeat = 2;
        PS(ctype1vals[0],CType1N);
        Aoffset += 8*CC; /* make it a word boundary or else!   */
			 /* if not , get a bus alignment error */
        vdp = &valDes[BandwidN];
        vdp->repeat = 1;
        VWA; VTD; VAO;
        PD0(class12.bw,BandwidN);
        Aoffset += DD; 
	vdp = &valDes[RecTempN];
	vdp->repeat = 1;
	VWS; VTD; VSO;
	PD0(class12.trx,RecTempN);
	Soffset += DD;
	vdp = &valDes[CalTempN];
	VWS; VTD; VSO;
	vdp->repeat = 1;
	PD0(class12.tcal,CalTempN);
	Soffset += DD;
	vdp = &valDes[SysTempN];
	VWS; VTD; VSO;
	vdp->repeat = 1;
	PD0(class12.stsys,SysTempN);
	Soffset += DD;
	vdp = &valDes[IntTimeN];
	VWS; VTD; VSO;
	vdp->repeat = 1;
	PD0(class12.inttime,IntTimeN);
	Soffset += DD;
        vdp = &valDes[NAxis1N];
        VWA; VTD; VAO;
        vdp->repeat = 1;
        PD0(class12.noint,NAxis1N);
        Aoffset += DD;
        vdp = &valDes[CRVal1N];
        VWA; VTD; VAO;
        vdp->repeat = 1;
/* x0 is velocity at refpt, change it to frequency offset wrt to rest freq */
	fbCntr = (class7.velocity-class12.x0)*class12.restfreq/2.99792458E5; 
        PD0(fbCntr,CRVal1N); 
        Aoffset += DD;
        vdp = &valDes[CRPix1N];
        VWA; VTD; VAO;
        vdp->repeat = 1;
        PD0(class12.refpt,CRPix1N); /* == stk->expch */
        Aoffset += DD;
	vdp = &valDes[PolarizN];
	VWS; VTA; VSO;
	vdp->repeat=8;
	PS(ClassStr.polariz,PolarizN);
	Soffset += 8*CC;
	vdp = &valDes[RxInfoN];
	VWS; VTA; VSO;
	vdp->repeat=8;
	PS(ClassStr.rx_info,RxInfoN);
	Soffset += 8*CC;

/* Class 13 */   
        lseek(sci.fd,position+PtrToClass[13],SEEK_SET); 
        if(read(sci.fd,&class13,sizeof(struct CLASS13) ) < 0 ) 
                error_("Error reading NRAO class 13");

	swapClass13(&class13); /* swap bytes if necessary */

	vdp = &valDes[DataTypeN];
	vdp->repeat = 3;
	VWA; VTA; VAO;
	PS("Tr*",DataTypeN);
	Aoffset += 8*CC; /* make it a word boundary */
	return;
}

int getNRAO() 
/* stuff the values in the (struct of_stk=&_stk[0])stp. 
 * No need to correct from Ta to Tr* (ala get_) as this is 
 * already done before the SDD file is written.
 */
{
	int i, position, mmsb, j;
    	register struct of_stk *stp = &stk_[0];
	char *tsp;
	int ii, i__2, n1, n2;
	float ncnt;
	double navg;
	int nDragon=0;

	if(subscanReq != cSubscan )  /* not the requested subscan */
		return(0);	     /* this won't be exectued */
	bzero((char *)stp, 512);
	stp->nver = 1;
    	stp->ibsln = -1;
    	stp->numst = 1;
    	stp->nlist = 1;
    	stp->indx = 3;
	putfn_();
    	main_.pltd = 0; 	/* Clear the data plotted flag */
    	stp->factt = 1;
	stp->nstk[2] = class1.scan;
 	stp->subscan = (short) cSubscan;

    	if((stp->numpt = class12.noint) > MAXCHN)
        	stp->numpt = MAXCHN;
    	stp->freq = class12.restfreq; 	/* MHz */
    	scan_.tsys = class12.stsys ;    /* Tsys measured at ref position. */
    	stp->time = class12.inttime; 	/* ON + OFF */
	stp->fwid = class12.freqres;	/* MHz */
    	stp->vlsr = class7.velocity ;	/* already in km/s*/
	stp->j2second=GI0(J2SecondN); /* was computed in ReadNRAO*/
	scan_.iobssec=j2sToUnix(stp->j2second); /* just for completeness sake */


/* ok, look class4.xsource is ALWAYS the actual position and 
 * class4.epoc is ALWAYS the map center. Don't even BOTHER with
 * class6.xzero!
 */
	stp->ra = class4.epocra/(double) 15.;
	stp->dec = class4.epocdec;
	stp->epoch = class4.epoch;
	stp->l = class4.gallong;
	stp->b = class4.gallat;
	stp->odra = 0.;
	stp->oddec = 0.;
#if 0
printf("epocra  x0 xs dx: %f(d) %f(s) %f(d) %f(as)\n",class4.epocra,class6.xzero,class4.xsource,class6.deltax);
printf("epocdec y0 ys dy: %f(d) %f(s) %f(d) %f(as)\n",class4.epocdec,class6.yzero,class4.ysource,class6.deltay);
#endif
      	if((strcmp(ClassStr.frame,"POLRSTEP")==0)||  
       		(strcmp(ClassStr.frame,"CARTSTEP")==0)||
		(strcmp(ClassStr.frame,"RA_DEC  ")==0)|| 
	        (strcmp(ClassStr.frame,"        ")==0)) { 
/* RA_DEC is now NRAO default. MWP Tue Jun 14 06:19:02 MST 1994 
 * Former default for RA_DEC  was all spaces. 
 * Still need to deal with AZ_EL case , which is hopefully rare.
 */

/* arcminute offsets aren't stored in pdfl header in this case, so compute them */
		stp->oddec = (class4.ysource - class4.epocdec)*(double) 60.;
		stp->odra = ((class4.xsource-class4.epocra)*(double) 60.);
		stp->odra *= cos(class4.ysource*cunits[DEGREES]);
      	}
    	else if((strcmp(ClassStr.frame,"POLRSCAN")==0)||
            (strcmp(ClassStr.frame,"CARTSCAN")==0)) {
            /* deltax and deltay in arcsec/sec--its a drift scan??*/
	    /* do nothing */
		;
	}
	else 
		warn("Unknown XY reference frame: %s",ClassStr.frame);
	stp->ddec = stp->oddec / (double) 60.;
	stp->dra = (class4.xsource/(double) 15.0) - stp->ra;
	stp->dl = 0; /* this may change if mapping in (l,b) allowed */
	stp->db = 0; /* this too */
/* set coordinate type */
	switch(getPoscode()) {
	  case X1950RADC :
	  case EPOCRADC  :
	  case MEANRADC  :
	  case X2000RADC :
  	  case INDRADC   :
	  case APPRADC   :
                stp->nplc = nplcType[1];
		break;
	  case GALACTIC:
                stp->nplc = nplcType[2];
		break;
	  case AZEL:
                stp->nplc = nplcType[0];
		break;
	  case USERDEF:
                stp->nplc = 20;
		break;
	}
        if(scan_.iobstp < 20 )
                 stp->nprc = 1;
        else  {
                stp->nplc += 300;
                if(scan_.iobstp == 20)
                        stp->nprc = 2000;
                else if(scan_.iobstp == 21)
                        stp->nprc = 1000;
                        else if(scan_.iobstp == 22)
                                stp->nprc = 3000;
/* since ordinate should always be some sort of velocity in NRAO files,
 * we don't need to additionally modify nplc.
 */
        }
	scan_.azimuth = class4.az;
	scan_.elevation = class4.el;
	scan_.fmult =  1.0; 		/* no extra corrections      */
	scan_.tsky = -9999.;		/* a "flag" value used by pl */

    /* Set the weight.  The weight is the equivalent integration time (sec)
     * above the atmosphere with no spillover with a 1000K receiver.
     */
	stp->nwts = 1;
	stp->locwt[0] = 1;
	stp->wght[0] = 1e6 * stp->time / (scan_.tsys * scan_.tsys);
	if(scan_.iobstp < 20)
		stp->rms = 2 / sqrt(stp->fwid * stp->wght[0]);
	else
		stp->rms = 2 / sqrt(64. * stp->wght[0]);  /* use bw of NBE */

/* Should we do frequency processing? 
 * MMSB = "u" if sideband is 0 or 2  
 */
	mmsb = ((class9.sideband == 0)|| (class9.sideband==2)) ? -1: 1;
	scan_.inver = mmsb;  /* for "bc" */
	scan_.ncur = 0; 	/* for "bc" */

   	if((tsp = GS(CType1N)) && strcmp(tsp, "Fr") != 0) { 
       		stp->expch = class12.refpt + fbCntr / stp->fwid;
       		stp->refch = NOTFS;
	} else { /* Do Freq processing */
       		stp->expch = class12.refpt + 
				mmsb*(fbCntr - class9.foffsig)/stp->fwid; 
		if(scan_.iobstp == 1 || scan_.iobstp==3)
          		stp->refch = stp->expch + 
			mmsb*(class9.foffsig - class9.foffref1)/stp->fwid;
       		else
          		stp->refch = NOTFS;
   	}
/*
 * finally fill the data array! 
 */
	switch(sci.fileType) {
		case PDFL:
        		position = (pdfl_dire.blk_fb1-1) * 512 + class1.headlen;
			break;
		case SDD:
			position = (sdd_dire.start_rec-1)*sdd_Boot.bytperrec + 
					class1.headlen;
			break;
	}

        if(read(sci.fd, NRAOvals,(int)class1.datalen) != (int)class1.datalen )
        	error_("Error reading NRAO scan data");

	swapData(); /* swap bytes in the data array, if needed */
	i = stp->numpt;

/* here is where to eliminate bad channels. this code mostly stolen from get1.c 
 * except working with floats instead of shorts (scan_.ndat) 
 */
        for (i = 1; i <= scan_.numbch; i += 2) {
            n1 = scan_.nbadch[i - 1];
            n2 = scan_.nbadch[i];
            if (!(n1 > stp->numpt || n2 <= 0)) {
             if (n1 > 1) {
                ncnt = 1.0;
                navg = NRAOvals[n1 - 2];
             } else {
                ncnt = 0.0;
                navg = 0.0;
             }
             if (n2 < stp->numpt) {
                ncnt+=1.0;
                navg += NRAOvals[n2];
             }
             if (ncnt > 0) {
                navg /= ncnt;
                i__2 = n2;
                for (ii = n1; ii <= i__2; ++ii) 
                    NRAOvals[ii - 1] = navg;
             }
	   } /* if !(n1...) */
        } /* for i */

/* For USB, invert order of data points.
 * This is the comb convention.
 * Above, I have also inverted expch and refch.
 * class9.sideband tells us what sideband we have used.
 * From page 7-3 of the NRAO 12m User's Manual Aug 1990 Edition:
 *
 *         Sideband Number   Harmonic Mixer   Main Mixer
 *         Sideband 1 = ++       Lower          Lower     LSB
 *         Sideband 3 = +-       Upper          Lower     LSB
 *         Sideband 0 = -+       Lower          Upper     USB
 *         Sideband 2 = --       Upper          Upper     USB
 */
	if((class9.sideband == 1) || (class9.sideband == 3))
	   for(i=0;i<stp->numpt;i++) 
		stp->stak[i] = NRAOvals[i];
	else
	   for(i=0;i<stp->numpt;i++) 
		stp->stak[i] = NRAOvals[stp->numpt-1-i];
        coreFull[0] = TRUE;

/* check for DRAGONs -- MWP 5/7/96 */

     if(main_.drgmes!=0) {
       	if( (nDragon=ChkDrag(0)) > 0) {
           switch(main_.drgmes) {
           case 1:
                /* no message */
                break;
           case 2:
                printf(" Found and eliminated %d BLANK%s in scan %d subscan %d\n",
                         nDragon,(nDragon==1)?"":"s",stp->nstk[2],stp->subscan);
                printf(" Use 'op msg:' to alter or eliminate this message.\n");
                break;
           }
	}
      }
	return(1);
}

/**
 * Find the last scan in a SDD file 
 */
void LastScanSDD() 
{
	off_t position;
/* we must read the bootstrap block again, not seek past it, since the
 * counter is in the bootstrap !
 */
        Rewind(sci.fd);
        if(read(sci.fd, &sdd_Boot, sizeof(struct sdd_BOOTSTRAP))<0 ) {
              error_("LastScanSDD: Could not read SDD Bootstrap");
        }

	swapBoot(); /* swap bytes if necessary */

    	if(read(sci.fd, &sdd_dire, sdd_Boot.bytperent) <0 ) 
      		error_("Error reading first SDD directory");

	swapDirectory(); /* swap bytes if necessary */

	sci.firstScan =  sdd_dire.scan;
	position = BTSTRPSIZE+(sdd_Boot.num_entries_used-1)*sdd_Boot.bytperent;
#if DEBUG
 printf("LastScanSDD: first scan: %d(%d)\n position: %d\nnumentries %d\n",
        sci.firstScan,(int)sdd_dire.scan, position,sdd_Boot.num_entries_used);
#endif
/*
 * The only way to find out the scan number of last entry is to
 * seek to it and read it!
 */
    	lseek(sci.fd, position, SEEK_SET );
    	if(read(sci.fd, &sdd_dire, sdd_Boot.bytperent) <0 ) 
      		error_("Error reading last SDD directory");

	swapDirectory(); /* swap bytes if necessary */

#if DEBUG
	printDirectory();
printf("LastScanSDD: last scan=sdd_dire.scan: %d\n",(int)sdd_dire.scan);
#endif
	sci.lastScan = (int) sdd_dire.scan;
	SeekBoot(sci.fd); /* put us back where we started */
	return;
}

/**
 * Find the last scan in a PDFL file 
 */
void LastScanPDFL() 
{

/* we must read the bootstrap block again, not seek past it, since the
 * counter is in the bootstrap !
 *
 * we won't try to read across file boundaries for now! 
 * Read the first and last directory entries and get the scan_numbers 
 */
	off_t position;

        Rewind(sci.fd);
        if(read(sci.fd, &pdfl_Boot, sizeof(struct pdfl_BOOTSTRAP))<0 ) {
                error("LastScanPDFL: Could not read PDFL pdfl_Bootstrap");
        }

	swapBoot();

	SeekBoot(sci.fd); /* lseek past BOOTSTRAP */
    	if(read(sci.fd, &pdfl_dire, DIRECTORYSIZE) <0 ) 
      		error_("Error reading first PDFL directory");

	swapDirectory(); /* swap bytes if necessary */

	sci.firstScan = pdfl_dire.scan;

    	position = pdfl_Boot.nxte - 2;
    	lseek(sci.fd, 512 + position * pdfl_Boot.nbpe, SEEK_SET );
    	if(read(sci.fd, &pdfl_dire, DIRECTORYSIZE) <0 ) 
      		error_("Error reading last PDFL directory");

	swapDirectory(); /* swap bytes if necessary */

	sci.lastScan = pdfl_dire.scan;
	SeekBoot(sci.fd); /* put us back where we started */
	return;
}


/**
 * Print out the NRAO scan values in a formatted way.
 * Works for both SDD and PDFL.
 */
void PrintNRAOVals()
{
        int i,j;
        j = class12.noint;
        printf("Scan %.2f\n", class1.scan);
        for(i=0;i<j;i++) {
                printf((i%10 == 9)? "%.4f\n": "%.4f ", NRAOvals[i]);
        }
        putchar('\n');
        return;
}

/**
 *  Return the position code of the scan
 */
int getPoscode() {
	switch(sci.fileType) {
		case PDFL:
        		if(strcmp(ClassStr.coordcd,"1950RADC") == 0)
				return(X1950RADC);
        		if(strcmp(ClassStr.coordcd,"        ") == 0)
				return(X1950RADC); /* blanks mean 1950RADEC */
        		if(strcmp(ClassStr.coordcd,"2000RADC") == 0)
				return(X2000RADC);
          		if(strcmp(ClassStr.coordcd,"EPOCRADC") == 0) 
				return(EPOCRADC);
          		if(strcmp(ClassStr.coordcd,"APPARADC") == 0)
				return(APPRADC);
          		if(strcmp(ClassStr.coordcd,"APPHRADC") == 0)
				return(APPHADC);
          		if(strcmp(ClassStr.coordcd,"MEANRADC") == 0)
				return(MEANRADC);
          		if(strcmp(ClassStr.coordcd,"2000RADC") == 0) 
				return(X2000RADC);
          		if(strcmp(ClassStr.coordcd,"INDRADC") == 0)
				return(INDRADC);
        		if(strcmp(ClassStr.coordcd,"GALACTIC") == 0)
				return(GALACTIC);
                	if(strcmp(ClassStr.coordcd,"AZEL") == 0)
				return(AZEL);
                	if(strcmp(ClassStr.coordcd,"1950ECL") == 0)
				return(X1950ECL);
                	if(strcmp(ClassStr.coordcd,"EPOCECL") == 0)
				return(EPOCECL);
                	if(strcmp(ClassStr.coordcd,"MEANECL") == 0)
				return(MEANECL);
                	if(strcmp(ClassStr.coordcd,"APPECL") == 0)
				return(APPECL);
                	if(strcmp(ClassStr.coordcd,"USERDEF") == 0)
				return(USERDEF);
		case SDD:
			return(sdd_dire.pos_code);
		default:
			return(BAD);
	}
}

/**
 * Return the observing mode of this scan. 
 *     obsmode == 256*m + mode[n].id   (see modes.h)
 *                         m=2 --> LINE
 *                         m=1 --> CONT
 *                         n=0..NUMMODES
 */
short getObsmode() {
	short theMode=BAD;
	int i;
	char *sp=ClassStr.obsmode;
	sp+=4;
	switch (sci.fileType) {
		case PDFL:
			if(strncmp(ClassStr.obsmode,"LINE",4)==0)
				theMode=512;
			else if(strncmp(ClassStr.obsmode,"CONT",4)==0)
				theMode=256;
			else /* not line or cont, we're in trouble */
				return((short)BAD);
			for(i=0;i<NUMMODES;i++) {
/* use strlen(sp) here because mode[i].field might have a trailing
 * space, but sp (ClassStr.obsmode) will not
 */
				if(strncmp(sp,modes[i].field,strlen(sp))==0) {
					theMode+=i;
					return(theMode);
				}
			}
			return(theMode);
		case SDD:
			return(sdd_dire.obsmode);
		default:
			return((short)BAD);
	}
}

/** 
 * Seek past BOOTSTRAP 
 */
void SeekBoot(int fd)
{
/* The first 512 bytes are reseved for the BOOTSTRAP regardless of
 * whether or not it actually fills it all.
 */
	lseek(fd,(off_t) BTSTRPSIZE,SEEK_SET);
	return;
}

/**
 * Rewind the pointer to the beginning of the file descriptor, fd
 */
void Rewind(int fd)
{
	lseek(fd,0L,SEEK_SET);
	return;
}

/** 
 * Swap the boot structure if necessary 
 */
void swapBoot() 
{
  switch(sci.fileType) {
	case PDFL:
		if(!BYTEREVERSED) {
		   decshort(&pdfl_Boot.nbdt,12);
		}
		break;
	case SDD:
		if(BYTEREVERSED) {
		   declong(&sdd_Boot.num_index_rec,8);
		}
		break;
	default:
		break;
   }
   return;
}

/** 
 * Swap the directory structure if necessary 
 */
void swapDirectory()
{
  switch(sci.fileType) {
	case PDFL:
		vaxdouble(&pdfl_dire.vel,2);
		if(!BYTEREVERSED) {
			declong(&pdfl_dire.scan,1);
			declong(&pdfl_dire.blk_fb1,4);
			decshort(&pdfl_dire.subscan,1);
		}
		break;
	case SDD:
		if(BYTEREVERSED) {
		   declong(&sdd_dire.start_rec,2); 
		   decfloat(&sdd_dire.h_coord,2);
		   decfloat(&sdd_dire.scan,2);
		   decdouble(&sdd_dire.rest_freq,1);
		   decfloat(&sdd_dire.lst,2);
		   decshort(&sdd_dire.obsmode,4);
		}
		break;
	default:
		break;
  }
  return;
}

/**
 * swap the Data array if necessary
 */
void swapData() {
  switch(sci.fileType) {
        case PDFL:
		vaxfloat(&NRAOvals[0],MAXCHN);
                break;
        case SDD:
                if(BYTEREVERSED) {
			decfloat(&NRAOvals[0],MAXCHN);
		}
                break;
        default:
                break;
  }
  return;
}

/* for debugging */
void printBoot() {
        switch(sci.fileType) {
          case PDFL:
		printf("PDFL BOOTSTRAP:\n");
		printf("  nbdt   %d\n", pdfl_Boot.nbdt);
		printf("  nemax  %d\n", pdfl_Boot.nemax);
		printf("  nbpe   %d\n", pdfl_Boot.nbpe);
		printf("  nxte   %d\n", pdfl_Boot.nxte);
		printf("  nxtde  %d\n", pdfl_Boot.nxtde);
		printf("  ndb    %d\n", pdfl_Boot.ndb);
		printf("  nxtb   %d\n", pdfl_Boot.nxtb);
		printf("  nbcal  %d\n", pdfl_Boot.nbcal);
		printf("  nbsv   %d\n", pdfl_Boot.nbsv);
		printf("  nbwrk  %d\n", pdfl_Boot.nbwrk);
		printf("  nbscr  %d\n", pdfl_Boot.nbscr);
		printf("  pftype %d\n", pdfl_Boot.pftype);
		printf(" obs_name  %s\n", pdfl_Boot.obs_name);
		printf(" proj_id   %s\n", pdfl_Boot.proj_id);
		printf(" binfmt_id %s\n", pdfl_Boot.binfmt_id);
                break;
          case SDD:
  		printf("SDD BOOTSTRAP:\n");
  		printf("num_ind_rec %d\nnumdatarec %d\nbytperrec %d\nbyterent %d\n\
numentries %d\ncounter %d\ntypesdd %d\nversion %d\n",
			sdd_Boot.num_index_rec,
			sdd_Boot.num_data_rec,
			sdd_Boot.bytperrec,
			sdd_Boot.bytperent,
			sdd_Boot.num_entries_used,
			sdd_Boot.counter,
			sdd_Boot.typesdd,
			sdd_Boot.version);
                break;
	  default:
		return;
	  }
	return;
}

/* for debugging */
void printPointers(struct CLASSPTRS *csp) {
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		csp->headcls,
		csp-> one, 
		csp->two,     
		csp->three,  
		csp->four,  
		csp->five,  
		csp->six,  
		csp->seven, 
		csp->eight,
		csp->nine, 
		csp->ten,  
		csp->eleven,
		csp->twelve,
		csp->thirteen);

}
/* for debugging */
void printDirectory() {
	switch(sci.fileType) {
          case PDFL:
		printf("PDFL DIRECTORY:\n");
		printf("scan %d sci.firstScan %d\n",pdfl_dire.scan,sci.firstScan);
		printf("obsmode %s\n",pdfl_dire.obsmode);
		printf("blk_fb1 %d\n",pdfl_dire.blk_fb1);
		printf("n_blk_fb1 %d\n",pdfl_dire.n_blk_fb2);
		printf("blk_fb2 %d\n",pdfl_dire.blk_fb1);
		printf("n_blk_fb2 %d\n",pdfl_dire.n_blk_fb2);
		printf("source %s\n", pdfl_dire.source);
		printf("vel %g\n", pdfl_dire.vel);
		printf("freq %g\n", pdfl_dire.restfreq);
		printf("subscan %d\n",pdfl_dire.subscan);
                break;
          case SDD:
		printf("SDD DIRECTORY:\nstart_rec %d",sdd_dire.start_rec);
		printf("\nend_rec %d",sdd_dire.end_rec);
		printf("\nh_coord %f",sdd_dire.h_coord);
		printf("\nv_coord %f",sdd_dire.v_coord);
		printf("\nscan %f",sdd_dire.scan);
		printf("\nfreq_res %f",sdd_dire.freq_res);
		printf("\nrest_freq %f",sdd_dire.rest_freq);
		printf("\nlst %f",sdd_dire.lst);
		printf("\nut %f",sdd_dire.ut);
		printf("\nobsmode %d",sdd_dire.obsmode);
		printf("\nphase_rec %d",sdd_dire.phase_rec);
		printf("\npos_code %d",sdd_dire.pos_code);
		printf("\nunused %d\n",sdd_dire.unused);
		break;
          default:
                break;
  	}
	return;
}

/** 
 * Swap the CLASSPNTR structure if necessary 
 */
void swapClassPtr(struct CLASSPTRS *csp)
{
  switch(sci.fileType) {
        case PDFL:
                if(!BYTEREVERSED) {
		   decshort(&cptr.headcls,16);
                }
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decshort(&csp->headcls,16);
		}
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS1 struct */
void swapClass1(struct CLASS1 *c) 
{ 
  switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->headlen,3);
                break;
        case SDD:
                if(BYTEREVERSED) {
		  decdouble(&c->headlen,3);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS2 struct */
void swapClass2(struct CLASS2 *c) { 
  switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->xpoint,12);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->xpoint,12);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS3 struct */
void swapClass3(struct CLASS3 *c) {
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->utdate,8);
                break;
        case SDD:
                if(BYTEREVERSED) {
	      	   decdouble(&c->utdate,8);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS4 struct */
void swapClass4(struct CLASS4 *c) {
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->epoch,16);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->epoch,16);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS5 struct */
void swapClass5(struct CLASS5 *c) {
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->tamb,6);
                break;
        case SDD:
                if(BYTEREVERSED) {
	   	  decdouble(&c->tamb,6);
                }
                break;
        default:
                break;
  }

  return;
}

/* Swap the bytes in a CLASS6 struct */
void swapClass6(struct CLASS6 *c) {
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->scanang,10);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->scanang,10);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS7 struct */
void swapClass7(struct CLASS7 *c) {
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->bfwhm,5);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->bfwhm,5);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS8 struct */
void swapClass8(struct CLASS8 *c) {
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->appeff,5);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->appeff,5);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS9 struct */
void swapClass9(struct CLASS9 *c) {
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->synfreq,25);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->synfreq,25);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS10 struct */
void swapClass10(struct CLASS10 *c) { 
	/* DO NOTHING WITH OPENPAR! 
	 * PS. WHY IS IT DECLARED AS A DOUBLE IF IT IS SUPPOSED
	 * TO BE CHARACTER HISTORY?
	 */
	return;
}

/* Swap the bytes in a CLASS11 struct */
void swapClass11(struct CLASS11 *c) { 
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->noswvarf,8);
		vaxdouble(&c->varval02,2);
		vaxdouble(&c->varval03,2);
	   	vaxdouble(&c->varval04,2);
		vaxdouble(&c->varval05,2);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->noswvarf,8);
		   decdouble(&c->varval02,2);
		   decdouble(&c->varval03,2);
	   	   decdouble(&c->varval04,2);
		   decdouble(&c->varval05,2);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS12 struct */
void swapClass12(struct CLASS12 *c) { 
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->obsfreq,20);
		vaxdouble(&c->effint,1);
                break;
        case SDD:
                if(BYTEREVERSED) {
		   decdouble(&c->obsfreq,20);
		   decdouble(&c->effint,1);
                }
                break;
        default:
                break;
  }
  return;
}

/* Swap the bytes in a CLASS13 struct */
void swapClass13(struct CLASS13 *c) { 
 switch(sci.fileType) {
        case PDFL:
		vaxdouble(&c->nostac,11);
                break;
        case SDD:
                if(BYTEREVERSED) {
		  decdouble(&c->nostac,11);
                }
                break;
        default:
                break;
  }
  return;
}
