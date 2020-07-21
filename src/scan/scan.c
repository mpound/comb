#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../main/C.h"
#include "C.h"
#include "include.h"
#include "../scan2/scan2.h"
#include "../stacks/C.h"
#include "../parse/P.tree.h"
#include "../parse/P.struct.h"
#include "../parse/P.constant.h"

#define bzero(s, n) memset(s, 0, n)

#define	MAXNSZ	7

static struct OBSTYPE {
	char c[3],i;
} obsType[] = {{"FS",1},{"PS",2},{"MP",2},{"CA",10},{"SK",12},{"SC",20},
	{"BP",5},{"BS",4},{"FP",3},{"GA",11},{"LS",6},{"CI",30},{"CB",13}};
#define NUMOBSTYPE (sizeof(obsType) / sizeof(*obsType))
static char scanType[][3] = {"Az", "El", "Ra", "Dc", "L", "B"};
#define NUMSCANTYPE (sizeof(scanType) / sizeof(scanType[0][0]))

int get1_(), get2();
extern off_t lseek();
static off_t pos;

/* scan.c*/

void LastScan();
static char * NextFileName();

void opend_()
{
	static char dfn[64];

	if (sci.fd <= 0 || strcmp(dfn, scan_.datfn)) {	/* Is file open? */
/*		cmove(scan_.datfn + scan_.ndfn - 3,curscn_.fil,3); */
		memcpy(curscn_.fil,scan_.datfn + scan_.ndfn - 3,3);
		if (access(scan_.datfn, 0))		/* Does file exist? */
			error_("Raw data file does not exist");
		if (sci.fd >= 0) {
			close(sci.fd);
		}
		if ((sci.fd = open(scan_.datfn, 2)) == -1)
			if ((sci.fd = open(scan_.datfn, 0)) == -1)
				error_("Can't open raw data file");
		strcpy(dfn, scan_.datfn);
		if (read(sci.fd, (char *)&scan_.nszsc, 18) != 18) {
			close(sci.fd);
			sci.fd = -1;
			error_("Trouble reading data file superheader");
		}
		if (scan_.nszsc < 1 || scan_.nszsc > MAXNSZ ||
		    scan_.nstrt < 78 || scan_.nstrt > 128 ||
		    (scan_.map & 0xfe)) {
			if(OpenScan2() == 0) {
/* Bell Labs scans don't have Objectnames */
        		    bzero(ObjectName,16); 
			    return;
			}
/*
 * If its not Bell Labs type 1 or type 2, see if it's NRAO SDD (Singledish 
 * Data) format or Kitt Peak PDFL file. NB: PDFL no longer supported by
 * NRAO. Official is now SDD version 1. 
 */
			if(OpenPDFL() == 0) {
			   return;
			}
			if(OpenSDD() == 0) {
			   return;
			}
			close(sci.fd);
			sci.fd = -1;
			sci.fileType=-1;
			error_("Bad data file superheader or unrecognized data file type");
		}
		sci.fileType = OLDBL;
		scan_.lnfil = scan_.nszsc * 128;
		LastScan();
		sci.firstScan = scan_.nfsc + 1;
        	bzero(ObjectName,16); 
	}
}

/* Read iscan from the scan file and do a preliminary look at the header.
 * Return 0 on success, 1 on missing scan in file (scan number == 0)
 */
int read_(short int *iscan)
{
	sci.nscan = *iscan;
	if(sci.nscan < sci.firstScan)
/* call to error() is incompatible with gt t: - MWP 10/10/2000 */
		/*error("Can't read scan - first scan is %d", sci.firstScan);*/
		return 1;
	if(sci.nscan > sci.lastScan) {
		LastScan();
		if(sci.nscan > sci.lastScan) {
			/*error("Can't read scan - last scan is %d", sci.lastScan);*/
			return 1;
		}
	}
	switch(sci.fileType) {
        case OLDBL:  
          pos = (off_t)((sci.nscan - scan_.nfsc) * scan_.lnfil * 2);
          if (sci.fd < 0)
                error_("Raw data file not open");
          if (lseek(sci.fd, pos, 0) != pos)
                error_("Trouble finding scan");
          if (read(sci.fd, (char *)scan_.ndat, scan_.lnfil * 2) !=
                        (scan_.lnfil * 2)) {
                LastScan();
                error("Trouble reading scan (last = %d)",sci.lastScan);
          }
          if (scan_.ndat[0] != sci.nscan) {
		if(scan_.ndat[0] == 0)
			return(1);
                error_("Scan number mismatch");
	  }
          scan_.iobstp = scan_.ndat[12];
          scan_.ibad = scan_.ndat[33];
          break;
	case NEWBL:
	    if(ReadScan2(sci.nscan))
		error("Trouble reading scan");
	    else {
		int n;
		char *st;

		/* Set a default array in case some hdr value we need is
		 * in an array */
		SetScan2Array(0);
		if(st = GS(ObsTypeN)) {
		    for(n = 0; n < NUMOBSTYPE; n++) {
			if(*st == obsType[n].c[0] && st[1] ==
				obsType[n].c[1]) {
			    scan_.iobstp = obsType[n].i;
			    goto obstypefound;
			}
		    }
		}
		scan_.iobstp = 0;	/* mark as unknown if not found */
obstypefound:
		/* For spatial scans, fix up the scan type */
		if(scan_.iobstp == 20) {
		    if(st = GS(CType1N))
			for(n = 0; n < NUMSCANTYPE; n++) {
			    if(st[0] == scanType[n][0] && st[1] ==
				    scanType[n][1]) {
				scan_.iobstp += n;
				break;
			    }
			}
		}

		scan_.ibad = GI0(BadN);
	    }
	    break;
	case PDFL:
	case SDD:
            if(ReadNRAO(sci.nscan))
                        error("Trouble reading scan");
            break;
        } /* end switch */
	return(0);

}

int get_()
{
        switch(sci.fileType) {
                case OLDBL:
                        return(get1_());
                        break;
                case NEWBL:
                        return(get2());
                        break;
                case PDFL:
                case SDD:
                        return(getNRAO());
                        break;
/* this should never be executed: */
                default:
                        error("get: bad sci.fileType=%d",sci.fileType,"\nConsult a programmer.");
                }

}

void pscanhdr_()
{
	int i;
	
        switch(sci.fileType) {
                case OLDBL:
                        for(i = 0; i < 128; i++) {
                          printf((i%10 == 9)? "%7d\n": "%7d", scan_.ndat[i]);
                        }
                        putchar('\n');
                        break;
                case NEWBL:
                case PDFL:
		case SDD:
                        PrintScan2ScanHdr();
                        break;
        }
}

void pscandat()
{
	int i;

        switch(sci.fileType) {
                case OLDBL:
                        for(i = 129; i < scan_.lnfil; i++) {
                          printf((i%10 == 8)? "%7d\n": "%7d", scan_.ndat[i]);
                        }
                        break;
                case NEWBL:
                        PrintScan2Arrays();
                        break;
                case PDFL:
                case SDD:
                        PrintNRAOVals();
                        break;
        }

}

/* The two following routines find a scan2 variable for the parse HdrVar
 * mechanism.  The first looks up the variable and returns a pointer to
 * a static HdrVar which has the scan2 hdr number in its length and the
 * type set to SCAN2VAR.  The second gets the value if it is numerical.
 */
HdrVar *GetScan2HdrVar(name)
char *name;
{
#define NUMHDRVAR 8
    static HdrVar hv[NUMHDRVAR];
    static nexthv = 0;
    static char names[NUMHDRVAR][16];
    HdrVar *hvp;
    int n;

    if((n = GetHdrByName(name)) < 0)
	return((HdrVar *)0);
    hvp= hv + nexthv;
    strncpy(hvp->nm = names[nexthv], name, 16);
    nexthv = (++nexthv) & (NUMHDRVAR - 1);
    hvp->ln = valDes[n].repeat;
    hvp->lc = n;
    if(valDes[n].type == 'A') {
	hvp->tp = SCAN2STR;
    } else {
	hvp->tp = SCAN2NUM;
    }
    return(hvp);
}

double Scan2HdrNumVal(n, i)
int n, i;
{
	return(GDI(n, i));
}

char *Scan2HdrStrVal(n)
int n;
{
	return(GS(n));
}

void Scan2HdrNumPut(v, n, i)
double v;
int n, i;
{
	PDI(v, n, i);
}

void Scan2HdrStrPut(s, n)
char *s;
int n;
{
	PS(s, n);
}

void write_()
{
        if(sci.fileType==PDFL)
                error_("I don't write PDFL files");
        if(sci.fileType==SDD)
                error_("I don't write SDD files");
        if(sci.fileType==NEWBL) {
		ReplaceScan2();
		return;
	}
	if (scan_.ndat[0] != sci.nscan)
		error_("Scan number mismatch");
	if (sci.fd < 0)
		error_("Raw data file not open");
	if (lseek(sci.fd, pos, 0) != pos)
		error_("Trouble finding scan");
	if (write(sci.fd, (char *)scan_.ndat, scan_.lnfil * 2) !=
	    (scan_.lnfil * 2))
		error_("Trouble writing scan");
}

void mrkbad(sn,care)
int  sn;
short int care;
{
        switch(sci.fileType) {
                case OLDBL:
                  pos = (off_t)(((sn- scan_.nfsc) * scan_.lnfil + 33) * 2);
                  if (lseek(sci.fd, pos, 0) != pos)
                  error_("Trouble finding scan");
        	  if (write(sci.fd,(char *)&care,sizeof(short int)) != 
		     sizeof(short int))
                        error_("Trouble writing scan");
                  break;
                case NEWBL:
                        error("Can't mark new scans bad yet");
                        break;
                case PDFL:
                        error("Can't mark PDFL scans bad yet");
                        break;
                case SDD:
                        error("Can't mark SDD scans bad yet");
                        break;
        }
}

void LastScan()
{
        switch(sci.fileType) {
                case OLDBL:
			sci.lastScan = scan_.nfsc - 1 + ((lseek(sci.fd,0L,2) /
			scan_.lnfil) >> 1);
                        break;
                case NEWBL:
                        LastScan2();
                        break;
                case PDFL:
                        LastScanPDFL();
                        break;
                case SDD:
                        LastScanSDD();
                        break;
        }
}


int ScanInFile(sn)
register int sn;
{
	if(sn < sci.firstScan)
		return(0);
	if(sn <= sci.lastScan)
		return(1);
	/* see if the file is longer than last time we looked */
	LastScan();
	if(sn <= sci.lastScan)
		return(1);
	return(0);
}

/* Subroutine to put the file name at the beginning of nstk in stack1.
 */
void putfn_()
{
	register char *ip = curscn_.fil;
	unsigned char *op = (unsigned char *)stk_[0].nstk;
# if BYTEREVERSED
	*op++ = *ip++;
	*op++ = 0x80;
	*op++ = *ip++;
	*op = *ip;
# else
	*op = 0x80;
	op[1] = *ip;
	op[2] = ip[1];
	op[3] = ip[2];
# endif /*BYTEREVERSED*/
}

/* Print the list of scans in stack 1 */
void prtsl_(char *out,int length)
{
	char ts[80];
	register int start = 0;

	while(start < stk_[0].indx - 1) {
		start = scanStr(out, length, start);
	}
}

/* Prepare a string suitable for printing from the scan number list in
 * stack 1.  The string will be no longer than len chars.  Return index
 * to continue with.
 */
int scanStr(cp, len, start)
register char *cp;
int len;				/* length of output string */
int start;				/* index of scan number to start with
					 * ignoring the skip to nstkx */
{
	register short *p;		/* scan number pointer, limit pointer */
	short *pend, *pstart;
	char *bufend = cp + len;	/* end of output buffer */

	if(start < 32)
		p = stk_[0].nstk + start;
	else
		p = stk_[0].nstkx + start - 32;
	pstart = p;			/* save starting output position */

	if(stk_[0].indx < 32)
		pend = &stk_[0].nstk[stk_[0].indx];
	else
		pend = &stk_[0].nstkx[stk_[0].indx - 32];
	while(p < pend) {
		if(*p < 0) {
			if(bufend - cp < 4)	/* Is the buffer too full */
				break;
# if BYTEREVERSED
			*cp++ = *(char *)p;
# else
			*cp++ = ( (char *)p)[1];
# endif /*BYTEREVERSED*/
			if(++p == &stk_[0].nstk[32])
				p = stk_[0].nstkx;
			*cp++ = *( (char *)p);
			*cp++ = *( (char *)(p++) + 1);
			*cp++ = ' ';
		} else {
			if(bufend - cp < 6)	/* Is the buffer too full */
				break;
			sprintf(cp,"%d ",*p++);
			for(; *cp; cp++) ;	/* Move pointer to end */
		}
		if(p == &stk_[0].nstk[32])
			p = stk_[0].nstkx;
	}
	if(p != pstart)		/* Have we written any output? */
		cp--;		/* put '\0' on top of last space */
	*cp = '\0';
	if(p < &stk_[0].nstk[32])
		return(p - stk_[0].nstk);
	else
		return(p - stk_[0].nstkx + 32);
}

/* Static variables for scan number/file name advancing */
static short int fScanNum,lScanNum,scanStep;	/* taken from gt or ad */
static short int rangeGiven;			/* Was a scan range given? */
static char fName[4],lName[4];

void scnlmt_(sn1n,sn2n,sn1f,sn2f,step,snf)
short int *sn1n,*sn2n;
int snf, step;
char *sn1f,*sn2f;
{
	fScanNum = *sn1n;
	lScanNum = *sn2n;
	scanStep = step;
	rangeGiven = !snf;
	strncpy(fName,sn1f,3);
	fName[3] = 0;
	strncpy(lName,sn2f,3);
	lName[3] = 0;
	if(rangeGiven && strcmp(fName,lName) * scanStep > 0) {
		strcpy(lName,fName);
		lScanNum = fScanNum;
	}
}

static char * NextFileName()
{
	static char newFile[64];

	strcpy(newFile,scan_.datfn);
	newFile[scan_.ndfn - 1] += (scanStep >= 0)? 1:-1;
	/* AST/RO used letters, e.g. 05a 05b ... 05z 06a 06b ... */
	/* STO is using only numbers 110 111 112 ... -CLM 22-Dec-2011 */
	/*	if (newFile[scan_.ndfn - 1]>'z') {
	  newFile[scan_.ndfn - 2] += (scanStep >= 0)? 1:-1;
	  newFile[scan_.ndfn - 1] = 'a';
	    if (newFile[scan_.ndfn - 2]>'9') {
	    newFile[scan_.ndfn - 3] += (scanStep >= 0)? 1:-1;
	    newFile[scan_.ndfn - 2] = '0';
	  }
	}
	if (newFile[scan_.ndfn - 1]<'a') {
	  newFile[scan_.ndfn - 2] += (scanStep >= 0)? 1:-1;
	  newFile[scan_.ndfn - 1] = 'z';
	    if (newFile[scan_.ndfn - 2]<'0') {
	    newFile[scan_.ndfn - 3] += (scanStep >= 0)? 1:-1;
	    newFile[scan_.ndfn - 2] = '9';
	  }
	}
	*/
	if((rangeGiven && strncmp(newFile + scan_.ndfn - 3,lName,3) *
	    scanStep > 0 ) || access(newFile,4) )
		*newFile = 0;
	return(newFile);
}

int islast_()
{
	register int check;

	check = curscn_.num + scanStep;
	if(rangeGiven ) {
		if( (scanStep >= 0 && check <= lScanNum) ||
			(scanStep < 0 && check >= lScanNum) )
			return(0);
		if(strncmp(scan_.datfn + scan_.ndfn - 3,lName,3) == 0)
			return(1);
	}
	if(ScanInFile(check))
		return(0);
	return( ! *NextFileName() );
}

int nxtscn_()
{
	register int new;
	register char *tfn;

	new = curscn_.num + scanStep;
	if(rangeGiven) {
		if( (scanStep >= 0 && new <= lScanNum) ||
			(scanStep < 0 && new >= lScanNum) ) {
		    if(ScanInFile(new))
			return(new);
		    else
			return(0);
		}
		if(strncmp(scan_.datfn + scan_.ndfn - 3,lName,3) == 0)
			return(0);
	}
	if(ScanInFile(new))
		return(new);
	if( ! *(tfn = NextFileName()))
		return(0);
	strcpy(scan_.datfn,tfn);
	new -= (scanStep > 0)? sci.lastScan: sci.firstScan - 1;
	opend_();
	new += (scanStep > 0)? sci.firstScan - 1: sci.lastScan;
	return(new);
}


