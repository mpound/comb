/* Routines for handling Bell Labs Spectral Data Files for COMB and OBS */
/* Changes by MWP for PDFL format are indicated by my initials  */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "scan2_incl.h" /* local includes put in this file so 
                           that Tom Kuiper's programs will work easily */
#include "scan2.h"

#define bcopy(s, d, n) memcpy(d, s, n)
#define bzero(s, n) memset(s, 0, n)

#define DEBUG 0

/* define types of number specifications for numArrayType and numChanType
 * NUMBER refers to separate variables  NumArray or NAxis1.
 * REPEAT refers to repeats of ArrayLoc or ScanLoc */
#define NONE 0
#define NUMBER 1
#define REPEAT 2
#define BOTH 3

/* Routines which return a logical value for success or failure return 0 if
 * they succeeded in carrying out their function and otherwise a non
 * zero error code. */

static time_t fileModTime;	/* Used by ReadMainHdr to know if file might
				 * have added scans. */
/* The memory for all of the headers and data will be obtained from malloc */
/* The next two can't be static or ReadPDFL/SDD can't use use them: MWP */
char *hdrPtr[3];                /* Pointers to the headers.  The Scan follows
                                 * its hdr & array data its hdr*/
int hdrSize[3];                 /* Size of the headers */
static char hdrChar[3] = {'M','S','A'};
static int mHdrStrt;		/* offset of main hdr in file */
static int dataStart;		/* offset in file to data */
static int maxScans;		/* Max num of scans;repeat of ScanLoc - 1 */
static int scanSizeGiven;	/* Does ScanSize exist? or use ScanLoc */
static int numArrayType;	/* Type of specification of arrays/scan */
static int numChansType;	/* Type of specification of channels in data */
static int cScanSize;		/* Values for the current scan */
static int cScanLoc;		/* Offset of current scan from beg of file */
int scanBufSize;                /* Size of block allocated for scan w/hdr */
                                /* shared with pdfl.c: MWP                */

/* Size of the hdr types A   C D E       I J in bytes */
static char sizeTab[] = {1,0,1,8,4,0,0,0,2,4};
static int hdrLineNumber;

/* String buffer for returning string values.  Allocated as 40 bytes or
 * 2X the length of the longest string. */
#define MAXSTRSIZE 256
/* these can't be static or ReadPDFL can't use use it */
char *strValue = 0;	/* String buffer set up by malloc */
int strValueSize = 0;	/* Current size of buffer */
int strValueLoc = 0;	/* Loc of string being returned by scan2_gets*/

/* The file value description table */
struct val_description valDes[NUMDEFINED + NUMEXTRAS] = {
#include "scan2_entries.h"
};
int valDesSize = NUMDEFINED;	/* number of predefined + extra entries */

/* information shared with other routines */
struct scan_info sci = {-1, -1, -1, -1, -1};

/* It looks as though the new standards don't support fdopen */
#if __STDC__
extern FILE     *fdopen(int, const char *);
#endif

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* scan2.c */
static void BadLine P_((int n, char *s));
static int ReadMainHdr P_((void));
#undef P_

int ReadPreamble()
{
    int size, totSize;	/* size of this hdr element and array of elements */

    /* stream for reading in the header */
    FILE *fp;

    /* The current positions in the headers as the file description
     * preamble is read.  They will be zero at the end of reading the
     * preamble if there are no entries for the respective hdr */
    int mainPos, scanPos, arrayPos;

    register struct val_description *vdp;
    struct val_description *cvdp;		/* Cur pos while searching */
    char keyword[12];				/* Read from hdr */
    int stringSize = 20;
    int c, n, i;				/* temporary ints */

    /* Temporarily open a stream (and new file descriptor) for data file */
    if((fp = fdopen(i = dup(sci.fd), "r")) == NULL) {
	close(i);
	error("Can't open stream");
    }
    rewind(fp);

    /* Look for the blsdf line at the beginning of the file */
    if(fscanf(fp, "%10s", keyword) != 1 || strcmp(keyword, "blsdf")) {
	fclose(fp);
	sci.fileType = 0;
	return(7);
    }
    sci.fileType = 1;
    hdrLineNumber = 2;
    while((c = getc(fp)) != '\n' && c != EOF) 
	;

    /* Initialize a few things */
    mainPos = scanPos = arrayPos = 0;
    numArrayType = numChansType = NONE;

    /* Clear the value description */
    /* NUMEXTRAS is zero for OBS */
    for(vdp = valDes; vdp < &valDes[NUMDEFINED+NUMEXTRAS]; vdp++ ) {
	vdp->whichHdr = UNDEFINED;
	vdp->offset=vdp->repeat = 0;
    }
    valDesSize = NUMDEFINED;

    /* Now we are ready to read through the file description and set
     * up the value description. */
    for(cvdp = valDes;;) {
	/* Each description line should start with a keyword */
	if((n = fscanf(fp, "%10s", keyword)) != 1)
		BadLine(n, "keyword");
#if DEBUG
	printf("Found keyword %-9s", keyword);
#endif
	if(keyword[0] == '/') {	/* check for comment line */
#if DEBUG
	    printf(" - Ignored\n");
#endif
	    goto NextLine;
	}
	if(strcmp(keyword, "END") == 0) {
		while((c = getc(fp)) != '\n' && c != EOF) 
			;
		break;
	}
	/* Search for this keyword */
	for(vdp = cvdp;;) {
	    if(strncmp(keyword, vdp->keyword, 8) == 0) {
		cvdp = vdp;
		break;
	    }
	    if(++vdp >= &valDes[NUMDEFINED])
		vdp = valDes;
	    if(vdp == cvdp) {
		printf("Unrecognized keyword %s in line %d\n",
			keyword, hdrLineNumber);
		/* Process the unrecognized keyword so its space in the header
		 * will be accounted for and the user will have access. */
		if(valDesSize < NUMDEFINED + NUMEXTRAS) {
		    vdp = &valDes[valDesSize++];
		    strncpy(vdp->keyword, keyword, 8);
		} else {
		    /* oops, too many extra keywords for the reserved space
		     * over write the last one */
		    vdp = &valDes[valDesSize - 1];
		    fprintf(stderr,
			"NUMEXTRAS too small, dropping extra keyword %s\n",
			vdp->keyword);
		    strncpy(vdp->keyword, keyword, 8);
		}
		break;
	    }
	}
	/* We have found the val_description for this keyword */
	n = fscanf(fp, "%1s %hd%c", &vdp->whichHdr, &vdp->repeat,
	    &vdp->type);
	if(n != 3) {
	    vdp->whichHdr = UNDEFINED;
	    BadLine(n, "Description");
	    goto NextLine;
	}
	/* Get the size of each element of this type */
	if((n = vdp->type - 'A') < 0 || n >= sizeof(sizeTab) ||
		(size = sizeTab[n]) == 0 ||
		((totSize = size * vdp->repeat) > 32767 && vdp !=
		&valDes[ScanLocN])  || totSize < 0) {
	    vdp->whichHdr = UNDEFINED;
	    BadLine(1, "Type");
	    goto NextLine;
	}
	if(vdp->repeat <= 0) {
	    switch(vdp - valDes) {
	    case ArrayLocN:
		if(vdp->whichHdr != 'S' || !HEXISTS(NumArrayN)) {
		    vdp->whichHdr = UNDEFINED;
		    BadLine(1, "ArrayLoc");
		    goto NextLine;
		}
		numArrayType = NUMBER;
		break;
	    case DataN:
		if(vdp->whichHdr == 'M' || !HEXISTS(NAxis1N)) {
		    vdp->whichHdr = UNDEFINED;
		    BadLine(1, "Data");
		    goto NextLine;
		}
		numChansType = NUMBER;
		break;
	    default:
		vdp->whichHdr = UNDEFINED;
		BadLine(1, "Repeat");
		goto NextLine;
	    }
	}
	if(vdp->type == 'A' && vdp->repeat > stringSize) {
	    if(vdp->repeat > MAXSTRSIZE) {
		vdp->repeat = MAXSTRSIZE;
		fprintf(stderr, "Truncating %s to %d chars\n", vdp->keyword,
			vdp->repeat);
	    }
	    stringSize = vdp->repeat;
	}
#if DEBUG
	printf("%c %4d%c size %d\n", vdp->whichHdr, vdp->repeat,
		vdp->type, size);
#endif
	switch(vdp->whichHdr) {
	case 'S':
	    vdp->whichHdr = SCAN_HDR;
	    n = scanPos & (size - 1);
	    if(n)
		printf("Skipping %d bytes before %s in scan hdr\n",
		    size - n, keyword);
	    vdp->offset = scanPos = (scanPos + size - 1) & ( -size);
	    scanPos += totSize;
	    break;
	case 'A':
	    vdp->whichHdr = ARRAY_HDR;
	    n = arrayPos & (size - 1);
	    if(n)
		printf("Skipping %d bytes before %s in array hdr\n",
		    size - n, keyword);
	    vdp->offset = arrayPos = (arrayPos + size - 1) & ( -size);
	    arrayPos += totSize;
	    break;
	case 'M':
	    vdp->whichHdr = MAIN_HDR;
	    n = mainPos & (size - 1);
	    if(n)
		printf("Skipping %d bytes before %s in main hdr\n",
		    size - n, keyword);
	    vdp->offset = mainPos = (mainPos + size - 1) & ( -size);
	    mainPos += totSize;
	    break;
	default:
	    vdp->whichHdr = UNDEFINED;
	    BadLine(1, "which Header");
		goto NextLine;
	};
	

NextLine:
	hdrLineNumber++;
	while((c = getc(fp)) != '\n' && c != EOF) 
		;
    }
    /* determine the position of the main hdr & data part of the file */
    mHdrStrt = (ftell(fp) + 7) & (-8);
    dataStart = mHdrStrt + (hdrSize[MAIN_HDR] = (mainPos + 7) & (-8));
    hdrSize[SCAN_HDR] = scanPos;
    hdrSize[ARRAY_HDR] = arrayPos;
#if DEBUG
    printf("mHdrStrt = %d, dataStart = %d\n", mHdrStrt, dataStart);
#endif

    /* close the stream used to read the main header */
    fclose(fp);

    /* Set up the string buffer */
    if((stringSize *= 2) > strValueSize) {
	strValueSize = stringSize;
	noint_();
	if(strValue)
		ChkFree(strValue);
	strValue = ChkAlloc(strValueSize, "Data file str buffer");
        bzero(strValue,strValueSize); /* MWP 2/4/93 */
	yesint_();
    }

    /* Check that a few header values are present and ok */

    /* Check on ScanSize & ScanLoc */
    if((WHICHHDR(ScanSizeN) == MAIN_HDR) == (WHICHHDR(ScanLocN) == MAIN_HDR))
	error("Need ScanLoc or ScanSize, not both");
    if(WHICHHDR(ScanLocN) == MAIN_HDR) {
	vdp = &valDes[ScanLocN];
	maxScans = vdp->repeat - 1;
	if(vdp->offset + vdp->repeat * sizeTab[vdp->type - 'A'] != mainPos)
	    error("ScanLoc must be at end of main hdr");
	scanSizeGiven = 0;
    } else {
	scanSizeGiven = 1;
    }

    /* Check on ArrayLoc & NumArray*/
    /* arrayPos gives the current position in ARRAY_HDR.  0 if no ARRAY_HDR */
    if(arrayPos > 0) {
	vdp = &valDes[ArrayLocN];
	if(vdp->whichHdr < 0)
	    error("Array entries without ArrayLoc");
	/* We already know that if repeat == 0, ArrayLoc is in SCAN_HDR  */
	if(vdp->repeat == 0) {
	    if(vdp->offset != scanPos)
		error("ArrayLoc must be at end of scan hdr if repeat == 0");
	} else {
	    numArrayType = (HEXISTS(NumArrayN))? BOTH: REPEAT;
	}
    } else {
	hdrPtr[ARRAY_HDR] = 0;
    }

    /* Check up on Data & NAxis1 specification */
    vdp = &valDes[DataN];
    if(numChansType) {
	if(vdp->whichHdr == SCAN_HDR && vdp->offset != scanPos ||
		vdp->whichHdr == ARRAY_HDR && vdp->offset != arrayPos)
	    error("Data must be at end of its hdr if repeat == 0");
    } else {
	numChansType = (HEXISTS(NAxis1N))? BOTH: REPEAT;
    }
    vdp = &valDes[NAxis1N];
    if(vdp->whichHdr < ARRAY_HDR) {
	if(vdp->repeat != 1 &&
		valDes[ArrayLocN].repeat != vdp->repeat)
	    error("Bad specification of NAxis1");
    }
    return(0);
}

static void BadLine(n, s)
int n;
char *s;
{
	if(n == EOF)
		error("EOF in main hdr of data file");
	fprintf(stderr,
		"\nTrouble with %s in line %d of data file main hdr, skipped\n",
		s, hdrLineNumber);
}

double scan2_getd(hdrN, ind)
int hdrN;
int ind;		/* Index used only if repeat != 1 */
{
    register struct val_description *vdp = &valDes[hdrN];
    register unsigned char *ptr;

    if(vdp->whichHdr < 0 )
	return(dragon_.NaN);
    ptr = (unsigned char *)hdrPtr[vdp->whichHdr] + vdp->offset;
    if(vdp->type == 'A') {
	double d;
	char cs[8];

	sprintf(cs, "%%%dlf", vdp->repeat);
	if(sscanf((char *)ptr, cs, &d) != 1)
	    return(dragon_.NaN);
	else
	    return(d);
    }
    if(vdp->repeat != 1) {
	if(vdp->repeat > 1 && (ind < 0 || ind >= vdp->repeat))
	    error("Bad index %d for %s in '%c' hdr", ind,
		hdrChar[vdp->whichHdr], vdp->keyword);
	ptr += ind * sizeTab[vdp->type - 'A'];
    }
#if BYTEREVERSED
    {
	double d;
	float f;

	switch(vdp->type) {
	case 'C':
	    return((double)*(char *)ptr);
	case 'D':
	    *(int *)&d = ptr[4] << 24 | ptr[5] << 16 | ptr[6] << 8 | ptr[7];
	    ((int *)&d)[1] = ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 |
	   	 ptr[3];
	    return(d);
	case 'E':
	    *(int *)&f = ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 | ptr[3];
	    return((double)f);
	case 'I':
	    return((double) (((char *)ptr)[0] << 8 | ptr[1]));
	case 'J':
	    return((double)(ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 |
		ptr[3]));
	}
    }
#else /*BYTEREVERSED*/
	switch(vdp->type) {
	case 'C':
	    return((double)*(char *)ptr);
	case 'D':
	    return( *(double *)ptr);
	case 'E':
	    return((double)*(float *)ptr);
	case 'I':
	    return((double)*(short int *)ptr);
	case 'J':
	    return((double)*(int *)ptr);
	}
#endif /* BYTEREVERSED */
    return(dragon_.NaN);
}

int scan2_geti(hdrN, ind)
int hdrN;
int ind;		/* Index used only if repeat != 1 */
{
    register struct val_description *vdp = &valDes[hdrN];
    register unsigned char *ptr;

    if(vdp->whichHdr < 0)
	return(iDRAGON);
    ptr = (unsigned char *)hdrPtr[vdp->whichHdr] + vdp->offset;
    if(vdp->type == 'A') {
	int i;
	char cs[8];

	sprintf(cs, "%%%dd", vdp->repeat);
	if(sscanf((char *)ptr, cs, &i) != 1)
	    return(iDRAGON);
	else
	    return(i);
    }
    if(vdp->repeat != 1) {
	if(vdp->repeat > 1 && (ind < 0 || ind >= vdp->repeat))
	    error("Bad index %d for %s in '%c' hdr", ind,
		hdrChar[vdp->whichHdr], vdp->keyword);
	ptr += ind * sizeTab[vdp->type - 'A'];
    }
#if BYTEREVERSED
    {
	double d;
	float f;

	switch(vdp->type) {
	case 'C':
	    return((int)*(char *)ptr);
	case 'D':
	    *(int *)&d = ptr[4] << 24 | ptr[5] << 16 | ptr[6] << 8 | ptr[7];
	    ((int *)&d)[1] = ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 |
	   	 ptr[3];
	    return((int)d);
	case 'E':
	    *(int *)&f =  ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 | ptr[3];
	    return((int)f);
	case 'I':
	    return((int) (((char *)ptr)[0] << 8 | ptr[1]));
	case 'J':
	    return(ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 | ptr[3]);
	}
    }
#else /*BYTEREVERSED*/
	switch(vdp->type) {
	case 'C':
	    return((int)*ptr);
	case 'D':
	    return( (int)*(double *)ptr);
	case 'E':
	    return((int)*(float *)ptr);
	case 'I':
	    return((int)*(short int *)ptr);
	case 'J':
	    return(*(int *)ptr);
	}
#endif /* BYTEREVERSED */
    return(iDRAGON);
}

/* return a pointer to a null terminated string in static storage  which is
 * copied from the header.  Attempt to not overwrite the previous string
 * which was returned.  This  will work if they fit in an  80 char buffer
 * Repeat is limited to 80 as the main header is read in.  If the value
 * is not defined or isn't a string, return 0.
 */
char *scan2_gets(hdrN)
int hdrN;
{
    register struct val_description *vdp = &valDes[hdrN];
    char *ptr, *cp;

    if(vdp->whichHdr < 0 || vdp->type != 'A')
	return(0);
    ptr = hdrPtr[vdp->whichHdr] + vdp->offset;
    if(strValueLoc + vdp->repeat >= strValueSize)
	strValueLoc = 0;
    bcopy(ptr, cp = &strValue[strValueLoc], vdp->repeat);
    cp[vdp->repeat] = '\0';
    return(cp);
}

int scan2_putd(val, hdrN, ind)
double val;
int hdrN;
int ind;		/* Index used only if repeat != 1 */
{
    register struct val_description *vdp = &valDes[hdrN];
    register unsigned char *ptr;

    if(vdp->whichHdr < MAIN_HDR || vdp->type == 'A')
	return(1);
    ptr = (unsigned char *)hdrPtr[vdp->whichHdr] + vdp->offset;
    if(vdp->repeat != 1) {
	if(vdp->repeat > 1 && (ind < 0 || ind >= vdp->repeat))
	    error("Bad index %d for %s in '%c' hdr", ind,
		hdrChar[vdp->whichHdr], vdp->keyword);
	ptr += ind * sizeTab[vdp->type - 'A'];
    }
    {
#if BYTEREVERSED
	union {
	double d;
	float f;
	int i;
	short h;
	unsigned char c[8];
	} u;

	switch(vdp->type) {
	case 'C':
	    *(char *)ptr = (char)val;
	    break;
	case 'D':
	    u.d = val;
	    *(int *)ptr = u.c[4] << 24 | u.c[5] << 16 | u.c[6] << 8 | u.c[7];
	    ((int *)ptr)[1] = u.c[0] << 24 | u.c[1] << 16 | u.c[2] << 8 |
	   	 u.c[3];
	    break;
	case 'E':
	    u.f = (float)val;
	    *(int *)ptr = u.c[0] << 24 | u.c[1] << 16 | u.c[2] << 8 | u.c[3];
	    break;
	case 'I':
	    u.h = (short int)val;
	    *(short int *)ptr = u.c[0] << 8 | u.c[1];
	    break;
	case 'J':
	    u.i = (int)val;
	    *(int *)ptr = u.c[0] << 24 | u.c[1] << 16 | u.c[2] << 8 | u.c[3];
	    break;
	}
#else /*BYTEREVERSED*/
	switch(vdp->type) {
	case 'C':
	    *(char *)ptr = (char)val;
	    break;
	case 'D':
	    *(double *)ptr = val;
	    break;
	case 'E':
	    *(float *)ptr = (float)val;
	    break;
	case 'I':
	    *(short int *)ptr = (short int)val;
	    break;
	case 'J':
	    *(int *)ptr = (int)val;
	    break;
	}
#endif /* BYTEREVERSED */
    }
    return(0);
}

int scan2_puti(val, hdrN, ind)
int val;
int hdrN;
int ind;		/* Index used only if repeat != 1 */
{
    register struct val_description *vdp = &valDes[hdrN];
    register unsigned char *ptr;

    if(vdp->whichHdr < MAIN_HDR || vdp->type == 'A')
	return(1);
    ptr = (unsigned char *)hdrPtr[vdp->whichHdr] + vdp->offset;
    if(vdp->repeat != 1) {
	if(vdp->repeat > 1 && (ind < 0 || ind >= vdp->repeat))
	    error("Bad index %d for %s in '%c' hdr", ind,
		hdrChar[vdp->whichHdr], vdp->keyword);
	ptr += ind * sizeTab[vdp->type - 'A'];
    }
    {
#if BYTEREVERSED
	union {
	double d;
	float f;
	int i;
	short h;
	unsigned char c[8];
	} u;

	switch(vdp->type) {
	case 'C':
	    *(char *)ptr = (char)val;
	    break;
	case 'D':
	    u.d = (double)val;
	    *(int *)ptr = u.c[4] << 24 | u.c[5] << 16 | u.c[6] << 8 | u.c[7];
	    ((int *)ptr)[1] = u.c[0] << 24 | u.c[1] << 16 | u.c[2] << 8 |
	   	 u.c[3];
	    break;
	case 'E':
	    u.f = (float)val;
	    *(int *)ptr = u.c[0] << 24 | u.c[1] << 16 | u.c[2] << 8 | u.c[3];
	    break;
	case 'I':
	    u.h = (short int)val;
	    *(short int *)ptr = u.c[0] << 8 | u.c[1];
	    break;
	case 'J':
	    u.i = (int)val;
	    *(int *)ptr = u.c[0] << 24 | u.c[1] << 16 | u.c[2] << 8 | u.c[3];
	    break;
	}
#else /*BYTEREVERSED*/
	switch(vdp->type) {
	case 'C':
	    *(char *)ptr = (char)val;
	    break;
	case 'D':
	    *(double *)ptr = (double)val;
	    break;
	case 'E':
	    *(float *)ptr = (float)val;
	    break;
	case 'I':
	    *(short int *)ptr = (short int)val;
	    break;
	case 'J':
	    *(int *)ptr = val;
	    break;
	}
#endif /* BYTEREVERSED */
    }
    return(0);
}

int scan2_puts(str, hdrN)
char *str;
int hdrN;
{
    register struct val_description *vdp = &valDes[hdrN];
    int size;
    register char *ptr;

    if(vdp->whichHdr < MAIN_HDR || vdp->type != 'A')
	return(1);
    if((size = strlen(str)) > vdp->repeat)
	size = vdp->repeat;
    ptr = hdrPtr[vdp->whichHdr] + vdp->offset;
    bcopy(str, ptr, size);
    if(size < vdp->repeat)
	bzero(ptr + size, vdp->repeat - size);
    return(0);
}

/* Assume that the scan number is valid i.e. FirstScan <= sn <= lastScan */
int ReadScan2(sn)
int sn;
{
    /* Find the scan and its size */
    if(scanSizeGiven) {
	cScanSize = GI0(ScanSizeN);
	cScanLoc = (sn - sci.firstScan) * cScanSize + dataStart;
    } else{
	if( (cScanLoc = GII(ScanLocN, (sn - sci.firstScan))) <= 0 ||
	    (cScanSize = GII(ScanLocN, (sn + 1 - sci.firstScan)) -
	    cScanLoc) < hdrSize[SCAN_HDR])
	    return(1);
    }

    /* Check that scan will fit in buffer.  Enlarge buffer if needed */
    if(scanBufSize < cScanSize) {
	noint_();
	if(hdrPtr[SCAN_HDR])
	    ChkFree(hdrPtr[SCAN_HDR]);
	hdrPtr[SCAN_HDR] = ChkAlloc(cScanSize, "Scan buffer");
	scanBufSize = cScanSize;
	yesint_();
    }

    /* clear the scan buffer. MWP 2/4/93                        */
        bzero(hdrPtr[SCAN_HDR], scanBufSize);

    /* Read the scan from the file */
    ChkRRead(sci.fd, cScanLoc, hdrPtr[SCAN_HDR], cScanSize, "Scan");
    if(GI0(ScanNumN) != sn)
	error("Scan number missmatch in scan %d", sn);

    /* Set up number of arrays */
    switch(numArrayType) {
    case NONE:
	sci.cNumArrays = 0;
	break;
    case NUMBER:
	sci.cNumArrays = GI0(NumArrayN);
	break;
    case REPEAT:
	sci.cNumArrays = valDes[ArrayLocN].repeat;
	break;
    case BOTH:
	if((sci.cNumArrays = GI0(NumArrayN)) > valDes[ArrayLocN].repeat)
	    sci.cNumArrays = valDes[ArrayLocN].repeat;
	break;
    }
    return(0);
}

int SetScan2Array(array)
int array;
{
    if(array > sci.cNumArrays)
	error("Bad backend number %d", array);
    sci.cArray = array;
    if(sci.cNumArrays)
	hdrPtr[ARRAY_HDR] = GII(ArrayLocN, array) + hdrPtr[SCAN_HDR];
    switch(numChansType) {
    case NUMBER:
	sci.cNumChans = GII(NAxis1N, array);
	break;
    case BOTH:
	sci.cNumChans = GII(NAxis1N, array);
	if(valDes[NAxis1N].repeat < sci.cNumChans)
	    error("NumChans > Data array size");;
	break;
    case REPEAT:
	sci.cNumChans = valDes[NAxis1N].repeat;
	break;
    }
}

int OpenScan2()
{
    if(ReadPreamble())
	return(7);
    sci.nscan = -1;			/* mark scan invalid */
    if(hdrSize[MAIN_HDR] > 0) {
	/* Allocate space for the main header, but don't bother clearing it */
	AllocateScan2MainHdr(0);	
	fileModTime = 0;
	ReadMainHdr();
	if((sci.firstScan = GI0(FrstScanN)) == iDRAGON)
	    sci.firstScan = 1;
    }
    sci.lastScan = 0;
    LastScan2();
    sci.nscan = sci.lastScan + 1;
    return(0);
}

/* Allocate memory for the main header.  If setup is true, clear the memory
 * and set the first ScanLoc to dataStart.
 */
void AllocateScan2MainHdr(setup)
int setup;
{
	noint_();
	if(hdrPtr[MAIN_HDR])
		ChkFree(hdrPtr[MAIN_HDR]);
	hdrPtr[MAIN_HDR] = ChkAlloc(hdrSize[MAIN_HDR], "Main hdr of scan file");
	if(setup) {
		bzero(hdrPtr[MAIN_HDR], hdrSize[MAIN_HDR]);
		PI0(dataStart, ScanLocN);
	}
	yesint_();
}

#if 0
void PrintScan2ValDes(n)
int n;
{
    register struct val_description *vdp = &valDes[n];
	
    if(vdp->whichHdr >= MAIN_HDR) {
	printf("%-8.8s, hdr %c, type %c, rpt %2d, offset %3d",
		vdp->keyword, hdrChar[vdp->whichHdr], vdp->type,
		vdp->repeat, vdp->offset);
	PrintScan2Val(n);
	putchar('\n');
    } else
	printf("%-8.8s is undefined\n", vdp->keyword);
}
#endif /* 0 */

void PrintScan2MainHdr() /* MWP 4/27/95 for "pd main:"*/
{
    register struct val_description *vdp;
    int itemCnt, n, index;

    if(!hdrPtr[MAIN_HDR])
        error("There is no main header to print.");
    itemCnt = 0;
    for(n = FrstScanN ;n < ScanNumN ; n++) {
	vdp = &valDes[n];
	if(vdp->repeat == 0)
		continue;
	if(vdp->type == 'A') {
	    if(itemCnt > 4) {
		putchar('\n');
		itemCnt = 0;
	    } else if(itemCnt & 1) {
		printf("%13s","");
		itemCnt++;
	    }
	    printf("  %-8.8s = %-13s", vdp->keyword, GS(n));
	    itemCnt += 2;
	} else {
	    if(vdp->repeat + itemCnt > 5) {
		putchar('\n');
		itemCnt = 0;
	    } else if(vdp->repeat == 1 && (itemCnt & 1)) {
		printf("%13s","");
		itemCnt++;
	    }
	    printf("  %-8.8s = ", vdp->keyword);
	    itemCnt++;
	    for(index = 0; index < vdp->repeat; index++) {
		if(itemCnt > 5) {
		    putchar('\n');
		    itemCnt = 0;
		}
		if(vdp->type == 'D' || vdp->type == 'E')
		    printf("%-13.4f", GDI(n, index));
		else
		    printf("%-13d", GII(n, index));
		itemCnt++;
	    }
	}
    }
    if(itemCnt)
	putchar('\n');
}


void PrintScan2ScanHdr()
{
    register struct val_description *vdp;
    int itemCnt, n, index;

    if(!hdrPtr[SCAN_HDR])
	error("There is no scan to print");
    itemCnt = 0;
    for(n = ScanNumN;n < valDesSize; n++) {
	vdp = &valDes[n];
	if(vdp->repeat == 0)
		continue;
	if(vdp->type == 'A') {
	    if(itemCnt > 4) {
		putchar('\n');
		itemCnt = 0;
	    } else if(itemCnt & 1) {
		printf("%13s","");
		itemCnt++;
	    }
	    printf("  %-8.8s = %-13s", vdp->keyword, GS(n));
	    itemCnt += 2;
	} else {
	    if(vdp->repeat + itemCnt > 5) {
		putchar('\n');
		itemCnt = 0;
	    } else if(vdp->repeat == 1 && (itemCnt & 1)) {
		printf("%13s","");
		itemCnt++;
	    }
	    printf("  %-8.8s = ", vdp->keyword);
	    itemCnt++;
	    for(index = 0; index < vdp->repeat; index++) {
		if(itemCnt > 5) {
		    putchar('\n');
		    itemCnt = 0;
		}
		if(vdp->type == 'D' || vdp->type == 'E')
		    printf("%-13.4f", GDI(n, index));
		else
		    printf("%-13d", GII(n, index));
		itemCnt++;
	    }
	}
    }
    if(itemCnt)
	putchar('\n');
}

int GetHdrByName(name)
char *name;
{
    register int n;
    for(n = valDesSize - 1;n >= 0; n--) {
	if(strncmp(name, valDes[n].keyword, 8) == 0)
	    return((HEXISTS(n))? n: -1);
    }
    return(-1);
}

void PrintScan2Arrays()
{
    int array, chan;

    for(array = 0; array < sci.cNumArrays; array++) {

	SetScan2Array(array);
	printf("Array %d\n", sci.cArray);
	for(chan = 0; chan < sci.cNumChans; chan++) {
	    printf("%7d", GII(DataN, chan));
	    if((chan % 10) == 9)
		putchar('\n');
	}
	putchar('\n');
    }
}

#if 0
PrintScan2Val(n)
int n;
{
    register struct val_description *vdp = &valDes[n];
    int index;

    if(vdp->whichHdr < 0)
	return(7);
    if(hdrPtr[vdp->whichHdr]) {
	if(vdp->type == 'A') {
		printf(" %s", GS(n));
	} else {
	    for(index = 0; index < vdp->repeat; index++) {
		if(vdp->repeat > 3 && (index %10) == 0)
		    printf("\n %d   ", index);
		if(vdp->type == 'D' || vdp->type == 'E')
		    printf(" %g", GDI(n, index));
		else
		    printf(" %d", GII(n, index));
	    }
	}
	return(0);
    }
    return(1);
}
#endif /* 0 */

#if 0
printit(s)
char *s;
{
	int i;
	printf("%s\n", s);
	for(i = 0; i < 3; i++) {
		printf("hdrPtr[%d] = %d, hdrSize = %d, hdrChar = %c\n",
			i, hdrPtr[i], hdrSize[i], hdrChar[i]);
	}
	printf("mHdrStrt = %d, dataStart = %d\n", mHdrStrt, dataStart);
}
#endif /* 0 */

void LastScan2()
{
    register int i;

    if(scanSizeGiven) {
	i = lseek(sci.fd, 0L, 2);
	sci.lastScan = sci.firstScan + (i - dataStart) / GI0(ScanSizeN);
    } else {
	if((!ReadMainHdr() || sci.lastScan == 0) &&
		(sci.lastScan = GI0(LastScanN)) == iDRAGON) { 
	    for(i = maxScans - 1; i > 0; i--) {
		if(GII(ScanLocN, i + 1) > 0 && GII(ScanLocN, i + 1)
			> GII(ScanLocN, i)) {
		    sci.lastScan = i + GI0(FrstScanN);
		    return;
		}
	    }
	}
    }
}

static int ReadMainHdr()
{
    struct stat status;

    /* If file hasn't changed since last read, don't read it */
    if(fstat(sci.fd, &status))
	error("Can't stat data file");
    if(status.st_mtime == fileModTime)
	return(1);
    fileModTime = status.st_mtime;

    ChkRRead(sci.fd, mHdrStrt, hdrPtr[MAIN_HDR], hdrSize[MAIN_HDR],
    	"scan file main hdr");
    return(0);
}

/* #if OBS */
/* The following are used only for writing scans in OBS. 
 * !! WARNING These OBS routines may not work without the ScanLoc array in
 * the main header and don't test for it.
 */

/* Lowest (highest) scan which has been written since updating the main hdr.
 * These are 'scan numbers based on firstScan. */
static int firstNewScan = 0x7fffffff, lastNewScan = -1;

/* Subroutine which copies the preamble in the current file (perhaps containing
 * only the preamble) into a new file.  Assume that the preamble file has been
 * opened (sci.fd is a descriptor for it) and had at least ReadPreamble called.
 */
void CopyScan2Preamble(nfd)
int nfd;			/* New file descriptor */
{
	char buf[BUFSIZ];	/* For copying the preamble */
	int remain, n, max;

	max = lseek(sci.fd, 0, 2);
	if(mHdrStrt < max)
		max = mHdrStrt;
	lseek(sci.fd, 0, 0);
	lseek(nfd, 0, 0);
	for(remain = max; remain > 0; remain -= n) {
		n = (remain > BUFSIZ)? BUFSIZ: remain;
		if(read(sci.fd, buf, n) != n)
			error("Trouble reading Preamble");
		if(write(nfd, buf, n) != n)
			error("Trouble writing new file");
	}
}

/* Call this routine when it is time to write the next scan and there is no
 * more room in the current file.  It is the caller's responsibility to
 * create the new file, open it, and pass its descriptor to StartNextFIle.
 * StartNextFile will copy the preamble of the current file to the new
 * file and write the main header after setting LastScan to FirstScan - 1
 * and setting the corresponding sci values.
 */
void StartNextFile(nfd)
int nfd;	/* file descriptor for next file */
{
	UpdateMainHdr();
	CopyScan2Preamble(nfd);
	fchmod(sci.fd, 0444);
	close(sci.fd);
	sci.fd = nfd;
	sci.lastScan = (sci.nscan = sci.firstScan) - 1;
	PI0(sci.lastScan, LastScanN);
	WriteScan2MainHdr();
}

void WriteScan2MainHdr()
{
	ChkRWrite(sci.fd, mHdrStrt, hdrPtr[MAIN_HDR], hdrSize[MAIN_HDR],
		"Scan file main header");
}

/*
 * Calculate the true size of scan and array headers including ArrayLoc and
 * Data if they have repeat = 0.  Find the total size of a scan and allocate
 * (enough) memory, if needed.  Put values in NAxis1, NumArray, and
 * ArrayLoc if they exist.
 * NOTE !! This routine will probably work with a variety of locations and
 * repeats of NAxis1, NumArray, and ArrayLoc, but it hasn't been tested
 * and doesn't check that values in the main hdr aren't modified.
 */
void AllocateScan2Buf(nArray, nChans)
int nArray;				/* Number of arrays in scan. */
register int nChans[];			/* List of # chans for each array */
{
    int sHdrSize;	/* True size of scan header */
    int aSize;		/* True size of an array [inc Data] */
    int sumArraySize;
    int dataSize;
    register int n;

    /* Get the true size of the scan header including possible extensions
     * for ArrayLoc or Data. */
    dataSize = sizeTab[valDes[DataN].type - 'A'];
    sHdrSize = hdrSize[SCAN_HDR];
    if(numArrayType == NUMBER)	/* ArrayLoc undimensioned at end of Scan Hdr */
	sHdrSize += sizeTab[valDes[ArrayLocN].type - 'A'] * nArray;
    else if(WHICHHDR(DataN) == SCAN_HDR && numChansType == NUMBER)
	sHdrSize += nChans[0] * dataSize;
    sHdrSize = (sHdrSize + 7) & -8;

    /* Get the total size of all of the arrays. */
    sumArraySize = 0;
    if(numArrayType > NONE) {
	for(n = 0; n < nArray; n++) {
	    aSize = hdrSize[ARRAY_HDR];
	    if(numChansType == NUMBER && WHICHHDR(DataN) == ARRAY_HDR)
		aSize += nChans[n] * dataSize;
	    sumArraySize += (aSize + 7) & -8;
	}
    }

    /* Check that there is adequate space in the scan buffer.  Allocate or
     * reallocate more space if needed. */
    if(scanSizeGiven) {
	cScanSize = GI0(ScanSizeN);
	if(cScanSize < sHdrSize + sumArraySize)
	    error("Scan larger than Fixed size in file");
    } else
	cScanSize = sHdrSize + sumArraySize;
/* printf("sHdrSize = %d, cScanSize = %d\n", sHdrSize, cScanSize); */
    if(cScanSize > scanBufSize) {
	noint_();
	if(hdrPtr[SCAN_HDR])
	    hdrPtr[SCAN_HDR] = ChkRealloc(hdrPtr[SCAN_HDR], cScanSize,
		"ScanBuffer");
	else {
	    hdrPtr[SCAN_HDR] = ChkAlloc(cScanSize, "Scan Buffer");
	    bzero(hdrPtr[SCAN_HDR], cScanSize);
	}
	scanBufSize = cScanSize;
	yesint_();
    }

    /* Now that we have scan memory, put values in NAxis1, NumArray, and
     * ArrayLoc. */
    if(numArrayType > NONE) {
	sci.cNumArrays = nArray;
	PI0(nArray, NumArrayN);
	sumArraySize = 0;
	for(n = 0; n < nArray; n++) {
	    PII(sHdrSize + sumArraySize, ArrayLocN, n);
	    SetScan2Array(n);
	    PII(nChans[n], NAxis1N, n);
	    /* Now add the length of this array to the total */
	    aSize = hdrSize[ARRAY_HDR];
	    if(numChansType == NUMBER && WHICHHDR(DataN) == ARRAY_HDR)
		aSize += nChans[n] * dataSize;
	    sumArraySize += (aSize + 7) & -8;
	}
    }
}

/*
 * Keep track of the last recoverable scan in the file.  Only allow recovery
 * of erased scans when we know that no intermediate scans have been
 * overwritten.  WriteScan2 will keep maxsn the same as sci.lastScan.  When
 * ChangeNextScan2 backs up, it doesn't reset maxsn, so recovery will be
 * possible untill the next scan is written or the program terminated.
 */
static maxsn;

/*
 * Write a scan into the file for OBS.  The structure of ScanLoc requires that
 * scans be written in the file in order.  As each scan is written, we expect
 * that ScanLoc is already set to the place to write it in the file and we set
 * ScanLoc for the next scan.
 *
 * Return 0 for success, -1 when the file is full and 1 for other errors.
 */
int WriteScan2()
{
    register int sn;

    sn = sci.nscan - sci.firstScan;

    if(scanSizeGiven) {
	cScanLoc = sn * cScanSize + dataStart;
    } else{
	if(sn >= maxScans)
		return(-1);	/* Time to change to the next file */
	if( (cScanLoc = GII(ScanLocN, sn)) < dataStart ||
		PII(cScanLoc + cScanSize, ScanLocN, sn + 1))
	    return(1);
    }

    ChkRWrite(sci.fd, cScanLoc, hdrPtr[SCAN_HDR], cScanSize, "Scan");

    maxsn = sn = sci.nscan++;
    if(sn < firstNewScan)
	firstNewScan = sn;
    if(sn > lastNewScan)
	lastNewScan = sn;
    sci.lastScan = sn;
    return(0);
}

/*
 * Replace the current scan (most recently read by ReadScan2) in the file.
 * This is for use in comb.
 */
void ReplaceScan2()
{
    int sn, Size, Loc;

    /* Get and check the scan number in the current scan buffer */

    if((sn = GI0(ScanNumN)) != sci.nscan)
	error("Scan to be replaced doesn't match number of most recent read");

    /* Find the scan's location and size */
    if(scanSizeGiven) {
	Size = GI0(ScanSizeN);
	Loc = (sn - sci.firstScan) * Size + dataStart;
    } else{
	if( (Loc = GII(ScanLocN, (sn - sci.firstScan))) <= 0 ||
	    (Size = GII(ScanLocN, (sn + 1 - sci.firstScan)) -
	    Loc) < hdrSize[SCAN_HDR])
	    error("Bad Size or Loc");
    }
    if(cScanSize != Size || cScanLoc != Loc)
	error("Size or Location mismatch with most recent read");

    ChkRWrite(sci.fd, Loc, hdrPtr[SCAN_HDR], Size, "Scan");
}

void ChangeNextScan2(nextScan)
register int nextScan;
{
    if(nextScan < sci.firstScan)
	error("Can't move behind first scan");
    if(nextScan >= sci.nscan) {
	if(nextScan == sci.nscan)
	    return;
	if(nextScan > maxsn + 1)
	    if(maxsn > sci.lastScan)
		error("Last recoverable scan in file is %d", maxsn);
	    else
		error("No recoverable scans");
    } else if(maxsn < sci.nscan)
	maxsn = sci.lastScan;
    sci.lastScan = firstNewScan = lastNewScan = nextScan - 1;
    sci.nscan = nextScan;
    UpdateMainHdr();
}

void UpdateMainHdr()
{
    register struct val_description *vdp = &valDes[LastScanN];
    int size, offset1;

    if(lastNewScan < firstNewScan)
	return;;
    if(!PI0(lastNewScan, LastScanN))
	ChkRWrite(sci.fd, mHdrStrt + vdp->offset, hdrPtr[MAIN_HDR] +
	    vdp->offset, sizeTab[vdp->type - 'A'], "LastScan");
    if(!scanSizeGiven) {
	vdp = &valDes[ScanLocN];
	size = sizeTab[vdp->type - 'A'];
	offset1 = vdp->offset + size * (firstNewScan - sci.firstScan);
	ChkRWrite(sci.fd, mHdrStrt + offset1, hdrPtr[MAIN_HDR] + offset1,
	    (lastNewScan + 2 - firstNewScan) * size, "ScanLoc");
    }
    lastNewScan = -1;
    firstNewScan = 0x7fffffff;
}
/* #endif OBS */

ChkRRead(fd, loc, buf, size, reason)
int fd, loc;
char *buf;
int size;
char *reason;
{
    if(lseek(fd, loc, 0) < 0)
	error("Error seeking to read %s", reason);
    if(read(fd, buf, size) != size)
	error("Error reading %s", reason);
}

ChkRWrite(fd, loc, buf, size, reason)
int fd, loc;
char *buf;
int size;
char *reason;
{
    if(lseek(fd, loc, 0) < 0)
	error("Error seeking to write %s", reason);
    if(write(fd, buf, size) != size)
	error("Error writing %s", reason);
}

