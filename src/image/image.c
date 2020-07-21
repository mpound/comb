/*
 * Subroutines for images. Permanent images are writen in FITS format.
 * Temporary images will have their data in a file containing an array of
 * floats and their header data only in an associated imagehdr structure
 * (image format).  Unrecognized FITS cards will be kept in a temp file
 * associated with the imagehdr into which the FITS header is read.
 *
 * There are up to MAXIMAGES known to comb.  Images 1 to MAXIMAGES-1
 * may be either external FITS files or internally generated tmp files in
 * image format.  Image 0 is used temporarily for creating a new image
 * from old ones.  The resulting image should be immediately moved.
 *
 * If the image in one imnumber is 'stored'
 * in another imnumber, no copy will be made, there will just be two pointers
 * to the same imageheader.  If one of these is subsequently changed, a new
 * temporary file and imagehdr  will be made for it.  If a TMP image is
 * written as a FITS file, its tmp version will remain unchanged
 *
 * 3/27/89 add routines for reading and writing FITS files of spectra using
 * the CLASS or SEST key words.
 *
 * 11/91 Image arithmetic - MWP
 * 1/95  IRAS subroutines - MWP
 * 2/21/95  SubImage routine - MWP
 * 1999-02-09 Conform to FITS Y2K date (YYYY-MM-DD[Thh:mm:ss[.sss])
 * 1999-12-14 Added BASELN keyword for FITS Spectra
 * 1999-12-20 Increased length of FITS string from 16 to 64. Added
 * 	DATE-OBS, ORIGIN, and CREATOR keywords. Use new jdate.c 
 *      routines for date formatting.  Write out DATE-OBS from 
 *      stk.j2second. Small mod to formatting in SFitsCard
 * 2011-08-16 Change all instance of round(x) to (int)(x+0.5) 
 *      Comment out the "include types.h"  statement
 */
# include <unistd.h>
# include "../main/machinedep.h"
# include "../stacks/C.h"
# include "C.h"
# include "../misc/C.h"
# include "include.h"
# include "../main/dragon.h"
# include "../main/C.h"
# include "../coordsys/C.h"
# include <stdio.h>
# include "../error/C.h"
# include "../scan/C.h"

# include <math.h>
# include <fcntl.h>
# include <ctype.h>
# include <string.h>

#define FITSRECL 2880
/*#define DEBUG   /* uncomment if we are */

struct of_imagehdr *imageHdr[MAXIMAGES];

static char empty[] = "";
static char histKey[] = "HISTORY COMB";
static double sixty = 60.;
static double one = 1.0;
static struct of_fitskey fitsKey[] = {

#include "fitskeywords.h"

};
#define FITSKEYLEN ( sizeof(fitsKey) / sizeof(struct of_fitskey) )
static char *fitsBuf = (char *)0;	/* pointer to buffer for fits data */
static char *headPos = (char *)0;	/* pointer to next card position */
static int ffd = -1;		/* file descriptor for fits file or tape */
static int ffdDataStart = 0;	/* Start of data in FITS output file */
static int writingToTape = 0;	/* true if fits output file is a tape */

extern off_t lseek();

#define bcopy(s, d, n) memcpy(d, s, n)
#define bzero(s, n) memset(s, 0, n)

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* image.c */
static void ChkNaxis P_((int im1, int im2));
static double delta P_((double cdelt));
static int imagelinked P_((register int imnumber));
static int readhdr P_((register struct of_imagehdr *ihp));
static struct of_fitskey *findkey P_((char *cardp));
static void sethdr P_((register struct of_fitskey *fk, struct of_imagehdr *ih, char *cp));
static void WriteFitsData P_((int imnumber));
static void NextCrd P_((void));
static void WriteFitsBuf P_((void));
static void PrepareSpectrumHdr P_((int imnumber, struct of_stk *stk, char *obj, char *telescope));
static void WriteSpectrumData P_((int imnumber));
static void SetupStackHdr P_((int imnumber, struct of_stk *stk));
static void ReadFitsStackData P_((int imnumber, struct of_stk *stk));
#undef P_

int JustPrinting=0; /* flag for NextCrd(); default to false ;
		     * is used for "im ph:"
		     */

double taupow;         /* power-law dependence for opacity. used in im() */ 
extern int lband, sband;
extern double tempfact, ccfact;
/*
 * Check that an image is open
 */
void ChkImage(imnumber)
int imnumber;
 {
	ImNumErr(imnumber);
	if( ! imageHdr[imnumber] )
		error("Image %d is not open", imnumber);
 }

/*
 * Open a FITS file and read its header into an imageHdr.  Take care of any
 * existing image at this imnumber.  If asked to reopen the same file which
 * is currently open at the same imnumber, no action is taken.
 */
void OpenFits(imnumber,fname)
register int imnumber;
char *fname;
{
	register struct of_imagehdr *ihp;

	ImNumErr(imnumber);
	if(access(fname, 4) < 0)
		error("Can't access %s",fname);
	NewImage(imnumber);
	ihp = imageHdr[imnumber];
	ihp->ftype = EXTERNAL | FITS;
	ihp->cdelt1 = 1;	/* Defaults for simple headers */
	ihp->cdelt2 = -1;	/* Don't default to LH coordinate system */
/*
 * there really is no standard for the "default" values of
 * crpixN. "It depends on the writer," says FITS policeman Peter Teuben.
 * They will default to 0 here, in order to parse FITS spectral headers
 * which may neglect to include them. Comb will complain if cdelt3 is present
 * but crpix3 is not. I'd like to do this for axis 2 as well, but want to
 * keep the "simple header" functionality above.
 * - MWP Tue Aug 15 10:19:17 EDT 1995
 */
	ihp->crpix1 = ihp->crpix2 = 0.;
	ihp->cdelt3 = ihp->crpix3 = FDRAGON; /* flag value for warn() */
	ihp->blank = iDRAGON;
	ihp->baseln = -1;
	strcpy(ihp->fname, fname);
	if( (ihp->fd = open(fname, O_RDONLY, 0664) ) < 0) {
		CloseImage(imnumber);
		(void)fprintf(stderr, "Error trying to open %s because", fname);
		perror(empty);
		error(empty);
	}
	if(strncmp(fname, "/dev/", 5) == 0)
		ihp->ftype |= TAPE;
	if(readhdr(ihp)) {
		CloseImage(imnumber);
		error("Error reading header of %s", fname);
	}
/* here's the warning, as promised */
        if(ihp->crpix3 == FDRAGON && ihp->cdelt3!=FDRAGON) {
                warn("CDELT3 was present in FITS header but CRPIX3 was not.\nCRPIX3 will be set to 0");
        	ihp->crpix3 = 0.;
        }

/* if they are both still FDRAGON, then neither are present
 * and will both be set to zero. no complaint necessary. */
        if(ihp->crpix3 == FDRAGON && ihp->cdelt3==FDRAGON) {
                ihp->cdelt3=ihp->crpix3 = 0.;
        }

	if(ihp->bscale == 0) {
	/*	static float dmax[3] = {255,  32767,  2147483647};
		static float dmin[3] = {  0, -32767, -2147483647};
	*/
		ihp->bscale = 1;
		switch(ihp->bitpix) {
		case 16:
			ihp->datamax = 32767;
			ihp->datamin = -32767;
			break;
		case 32:
			ihp->datamax = 2147483647;
			ihp->datamin = -2147483647;
			break;
		case  8:
			ihp->datamax = 255;
			ihp->datamin = 0;
			break;
		}
	}
}

/*
 * Start a standard image with the header values which cm and vc will need
 */
void StartStdImage(imnumber)
int imnumber;
{
# include <sys/time.h>
	register struct of_imagehdr *ihp;
	time_t time();

	NewImage(imnumber);
	ihp = imageHdr[imnumber];
	ihp->simple = 1;
	ihp->bitpix = 16;
	ihp->baseln = -1;
	(void)sprintf(ihp->date,"%s",(const char *)jdate(time(0)));
	(void)sprintf(ihp->creator,"COMB %s",VERSION);
}

/*
 * Set up for a new image in the given image number.  Allocate and clear
 * the header.
 */
void NewImage(imnumber)
register int imnumber;
{
	register struct of_imagehdr *ihp;

	CloseImage(imnumber);
	imageHdr[imnumber] = ihp = (struct of_imagehdr *)ChkAlloc(
		sizeof(struct of_imagehdr), "Image Header");
	bzero(ihp, sizeof(struct of_imagehdr));
	ihp->ftype = TMP;
	ihp->fd = -1;
}

/*
 * Link an existing image to a new image number
 */
void LinkImage(imSrc, imDest)
register imSrc, imDest;
{
	ImNumErr(imSrc);
	ImNumErr(imDest);
	if( ! imageHdr[imSrc] )
		error("Image %d is not open", imSrc);
	CloseImage(imDest);
	imageHdr[imDest] = imageHdr[imSrc];
}

/*
 * Release the data buffer associated with an image.  If the imnumber is 
 * < 0, release the buffers for all images.
 */
void ReleaseImageBuff(imnumber)
register int imnumber;
{
	register int end;
	register struct of_imagehdr *ihp;

	if(imnumber < 0) {
		imnumber = 1;
		end = MAXIMAGES - 1;
	} else {
		ImNumErr(imnumber);
		end = imnumber;
	}
	for(;imnumber <= end; imnumber++) {
		if(imageHdr[imnumber] && (ihp = imageHdr[imnumber])->buf) {
/* printf("About to free buffer of image %d\n",imnumber); */
			noint_();
			ChkFree( (char *)ihp->buf);
			ihp->buf = 0;
			yesint_();
		}
	}
}

/*
 * Close a given image number.  If the same imageHdr is attached to another
 * imnumber, just clear the pointer.  Otherwise free the data buffer if any,
 * unlink the file if it is temporary, close the file, and release the
 * imageHdr.  Close all images if imnumber is < 0.
 */
void CloseImage(imnumber)
register int imnumber;
{
	register int end;
	register struct of_imagehdr *ih;

	if(imnumber < 0) {
		imnumber = 0;
		end = MAXIMAGES - 1;
	} else {
		ImNumErr(imnumber);
		end = imnumber;
	}
	for(; imnumber <= end; imnumber++) {
		if( (ih = imageHdr[imnumber]) == 0 )
			continue;
		if( imagelinked(imnumber) ) {
			imageHdr[imnumber] = 0;
			continue;
		}
		if( !(ih->ftype & STACK) )
			ReleaseImageBuff(imnumber);
		noint_();
		if(ih->fd > 0) {
			close(ih->fd);
		}
		if(ih->fname[0] && !(ih->ftype & EXTERNAL) ) {
/* printf("Unlinking file for image %d\n",imnumber); */
			unlink(ih->fname);
		}
		ChkFree((char *)ih);
		imageHdr[imnumber] = (struct of_imagehdr *)0;
		yesint_();
	}
}

/*
 * Check to see that the two given images are similar enough to combine or
 * make a scatter plot.
 */
void ImageSimilar(im1, im2)
int im1, im2;
{
	register struct of_imagehdr *ihp1 = imageHdr[im1];
	register struct of_imagehdr *ihp2 = imageHdr[im2];
	double d1,d2;
	static double tol = 0.1;

	ChkImage(im1);
	ChkImage(im2);
/*
 * ihp->fname is normally only set by "af" or "wf", so make up some
 * fnames when comparing images which have not been read in from disk.
 * This is so the error messages below are complete.  MWP 6/10/93
 */
	if (!strcmp(ihp1->fname,""))
		(void)sprintf(ihp1->fname,"Image %d",im1);
 	if (!strcmp(ihp2->fname,""))
		(void)sprintf(ihp2->fname,"Image %d",im2);
	if( strcmp(ihp1->ctype1, ihp2->ctype1) || strcmp(ihp1->ctype2,
	    ihp2->ctype2) ) {
		(void)fprintf(stderr, "%s has coordinates '%s' and '%s', but %s has '%s' and '%s'\n",
			ihp1->fname, ihp1->ctype1, ihp1->ctype2, ihp2->fname,
			ihp2->ctype1, ihp2->ctype2);
		warn(empty);
	}
	d1 = fabs(ihp1->cdelt1);
	d2 = fabs(ihp1->cdelt2);
	if( cifdif(d1, fabs(ihp2->cdelt1), tol * d1 / ihp1->naxis1 ) ||
	    cifdif(d2, fabs(ihp2->cdelt2), tol * d2 / ihp1->naxis2 ) )
		error("%s and %s have different grids", ihp1->fname,
			ihp2->fname);
	if( cifdif(ihp1->crval1, ihp2->crval1, tol * d1) ||
	    cifdif(ihp1->crval2, ihp2->crval2, tol * d2) ) 
		error("%s and %s have different center positions", ihp1->fname,
			ihp2->fname);
	if( cifdif(ihp1->crota1, ihp2->crota1, 1.0) ||
	    cifdif(ihp1->crota2, ihp2->crota2, 1.0)  )
		error("%s and %s have different rotations", ihp1->fname, 
			ihp2->fname);
}

static void ChkNaxis(im1,im2) /* MWP 6/10/93 */
int im1,im2;
{
        struct of_imagehdr *ihp1 = imageHdr[im1];
        struct of_imagehdr *ihp2 = imageHdr[im2];
	if((ihp1->naxis1 != ihp2->naxis1)||(ihp1->naxis2 != ihp2->naxis2))
		error("%s and %s have different axis lengths", ihp1->fname,
			ihp2->fname);
}

/*
 * Return the address of the requested line of pixels after filling the data
 * buffer if necessary.  The pointer will point to the pixel with the smallest
 * x in the line and x will increase as the pointer is incremented.  This
 * works with either an image in internal format or a FITS file.
 */
float *GetImageLine(imnumber, plane,y,nlines,ydir)
int imnumber;
int plane;			/* Plane number for data cubes (-1 to keep
				 * previous plane) */
int y;				/* line # relative to reference position */
int nlines;			/* required number of lines in buffer */
int ydir;			/* extend buffer to larger or smaller y */
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];
	register int i;
	int reqLine;		/* requested line number in image 0 -> bottom */

	if(plane == -1)
		plane = ihp->bufplane;
	if(plane && (plane < 0 || plane >= ihp->naxis3))
		error("Bad Plane number %d, naxis3 = %d", plane, ihp->naxis3);
	if(nlines > ihp->naxis2)
		nlines = ihp->naxis2;
	if( !ihp->buf) {
		ihp->buflen = nlines;
		ihp->buf = (float *)ChkAlloc(ihp->buflen * ihp->naxis1 *
			sizeof(float) , "Image buffer");
		/* preposterous bufline to force a read */
		ihp->bufline = 1000000000;
	}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * I don't understand this.  It may be right for reading in FITS files, but
 * it is WRONG when writing a FITS file from cp with rev2 != 0.
 */
	/* If the file has cdelt2 < 0, crpix will also be expressed from the
	 * other end */
	if( ihp->cdelt2 > 0 ) {
		reqLine = (int) (ihp->crpix2 + 0.5) - 1 + y;
	} else {
		reqLine = ihp->naxis2 - (int) (ihp->crpix2 + 0.5) + y;
	}

	if( reqLine < ihp->bufline || reqLine >= ihp->bufline +
		ihp->buflen || plane != ihp->bufplane) {

#if 0
(void)printf("reqLine = %d, naxis2 = %d, crpix2 = %g, bufline = %d, buflen = %d\n", reqLine, ihp->naxis2, ihp->crpix2, ihp->bufline, ihp->buflen);
#endif /*0*/
		/* decide what part of the file to read in */
		if(ydir > 0) {
			if(reqLine  > (i = ( ihp->naxis2 - ihp->buflen)))
				ihp->bufline = i;
			else
				ihp->bufline = reqLine;
		} else {
			if( (i = reqLine - ihp->buflen + 1) < 0)
				ihp->bufline = 0;
			else
				ihp->bufline = i;
		}
		if(ihp->ftype & FITS) {
			int fitsLine;	/* Line in Fits array of first line of
					 * buffer (beg of data is 0) */
			int fitsWordLen;/* Len of an int in the fits file */
			int fitsBufLen;	/* Len of fits buffer */
			int line;	/* loop counter */
			register int pixel;	/* int value of Fits pixel */
			char *fb;	/* pointer to fits buffer */
			/* pointer to next byte in fitsbuffer */
			register char *ip;
			/* pointer to next loc in image buf */
			register float *op;
			/* Note, op will always advance regularly, but ip may
			 * have to move backward through lines or pixels if
			 * cdelt2 or cdelt1 is negative.  NextWord and/or
			 * nextLine may be zero.
			 */
			int nextWord;	/* Add to ip after each word */
			int nextLine;	/* Add to ip after each line */

			fitsWordLen = abs(ihp->bitpix) >> 3;
			fitsBufLen = ihp->buflen * ihp->naxis1 * fitsWordLen;
			fb = SafeAlloc(fitsBufLen, "FITS buffer");
			if(ihp->cdelt2 > 0)
				fitsLine = ihp->bufline;
			else
				fitsLine = ihp->naxis2 - ihp->bufline - nlines;
			if( lseek(ihp->fd, (off_t)(ihp->dataStart + (plane *
					ihp->naxis2 + fitsLine) *
					ihp->naxis1 * fitsWordLen), 0) < 0) {
				(void)fprintf(stderr, "Trouble seeking in %s because "
					,ihp->fname);
				perror(empty);
				error(empty);
			}
			if(read(ihp->fd, fb, fitsBufLen) != fitsBufLen)
				error("Trouble reading FITS file %s",
					ihp->fname);
			op = ihp->buf;
			if(ihp->cdelt2 > 0)
				ip = fb;
			else
				ip = fb + fitsBufLen - ihp->naxis1 *
					fitsWordLen;
			/* the common case - cdelt1 < 0 && cdelt2 > 0 */
			nextLine = 2 * ihp->naxis1 * fitsWordLen;
			if(ihp->cdelt1 < 0) {
				/* Start at line end */
				ip += (ihp->naxis1 - 1) * fitsWordLen;
				/* compensate for the increment reading bytes */
				nextWord = -2 * fitsWordLen;
				if(ihp->cdelt2 < 0)
					nextLine = 0;	/* No increment */
			} else {
				nextWord = 0;
				if(ihp->cdelt2 < 0)
					nextLine = -nextLine;
				else
					nextLine = 0;
			}
			for(line = 0; line < ihp->buflen; line++) {
			    for(i = 0; i < ihp->naxis1; i++) {
				switch(fitsWordLen) {
				case 4:
				    if(ihp->bitpix < 0) {
#if BYTEREVERSED
					union {
						int i;
						float f;
					} pix;

					pix.i = *ip++ << 24;
					pix.i |= (*ip++ & 0xff) << 16;
					pix.i |= (*ip++ & 0xff) << 8;
					pix.i |= *ip++ & 0xff;
					if(isnan(pix.f))
					    *op++  = FDRAGON;
					else
					    *op++  = pix.f;
#else
					if(isnan(*(float *)ip))
					    *op++ = FDRAGON;
					else
					    *op++ = *(float *)ip;
					ip += 4;
#endif
				    } else {
#if BYTEREVERSED
					pixel = *ip++ << 24;
					pixel |= (*ip++ & 0xff) << 16;
					pixel |= (*ip++ & 0xff) << 8;
					pixel |= *ip++ & 0xff;
#else
					pixel = *(int *)ip;
					ip += 4;
#endif
				    }
					break;
				case 2:
#if BYTEREVERSED
					pixel = *ip++ << 8;
					pixel |= *ip++ & 0xff;
#else
					pixel = *(short *)ip;
					ip += 2;
#endif
					break;
				case 1:
					pixel = *(unsigned char *)ip++;
					break;
				}
				if(ihp->bitpix > 0)
				    if(pixel == ihp->blank)
					*op++ = FDRAGON;
				    else
					*op++ = pixel*ihp->bscale + ihp->fbzero;

				ip += nextWord;
			    }
			    ip += nextLine;
			}
			SafeFree(fb);
		} else {
		    error("Internal error: Trying to read a non-FITS file");
		    i = ihp->dataStart + ihp->bufline * ihp->naxis1
			* sizeof(float);
		    if( lseek(ihp->fd, (off_t)i, 0) < 0 ) {
			(void)fprintf(stderr, "Trouble seeking in %s because ",
				ihp->fname);
			perror(empty);
			error(empty);
		    }
		    i = ihp->buflen *ihp->naxis1 * sizeof(float);
		    if( read(ihp->fd, (char *)ihp->buf, i) != i)
			error("Trouble reading %d bytes in %s",i,ihp->fname);
		}
		ihp->bufplane = plane;
	}
	return( &ihp->buf[ (reqLine - ihp->bufline) * ihp->naxis1 ]);
}

/*
 * Compute deltax or deltay given the corresponding cdelt.  Attempt to round
 * delta to an even number of Arc min.
 */
static double delta(cdelt)
double cdelt;
{
	register int arcMinPerPix = (int) (one / cdelt + 0.5);
	if(cifdif(one / (double) arcMinPerPix, cdelt, .001 * cdelt))
		return(cdelt * sixty);
	else
		return(sixty / (double) arcMinPerPix);
}

/*
 * Get the x grid values.  Lowx and highx are in pixel numbers relative to the
 * center of the file.  They increase with increasing x coordinate.  Deltax
 * is the positive pixel spacing (in FitsAxisUnits if the coordinate is an
 *  angle), so lowx * delx is the most neg x offset from the center position.
 */
void ImageXGrid(imnumber,lowx,highx,deltax,xunit)
int imnumber;
int *lowx, *highx;		/* min and max pixel numbers */
double *deltax;			/* Absolute value of pixel spacing */
int *xunit;			/* Units of deltax (ARCMINUTES, etc.) */
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];
	register int refpix;
	struct AXISNAMES units;

	refpix = (int) (ihp->crpix1 + 0.5);
	if(ihp->cdelt1 > 0) {
		*lowx = 1 - refpix;
		*highx = ihp->naxis1 - refpix;
		*deltax = ihp->cdelt1;
	} else {
		*lowx = refpix - ihp->naxis1;
		*highx = refpix - 1;
		*deltax = -ihp->cdelt1;
	}
	units = FitsAxisUnits(ihp->ctype1);
	*xunit = units.unit;
	if( (units.type & 0xf) == SPATIAL )
		*deltax = delta( *deltax) * cunits[ARCMINUTES] /
			cunits[units.unit];
}

/*
 * Get the y grid values.  See comments above.
 */
void ImageYGrid(imnumber,lowy,highy,deltay,yunit)
int imnumber;
int *lowy, *highy;		/* min and max pixel (line) numbers */
double *deltay;			/* Absolute value of pixel spacing */
int *yunit;			/* units of deltay (ARCMINUTES, etc.) */
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];
	register int refpix;
	struct AXISNAMES units;

	refpix = (int) (ihp->crpix2 + 0.5);
	if(ihp->cdelt2 > 0) {
		*lowy = 1 - refpix;
		*highy = ihp->naxis2 - refpix;
		*deltay = ihp->cdelt2;
	} else {
		*lowy = refpix - ihp->naxis2;
		*highy = refpix - 1;
		*deltay = -ihp->cdelt2;
	}
	units = FitsAxisUnits(ihp->ctype2);
	*yunit = units.unit;
	if( (units.type & 0xf) == SPATIAL )
		*deltay = delta( *deltay) * cunits[ARCMINUTES] /
			cunits[units.unit];
}

/*
 * Check for a valid imnumber
 */
void ImNumErr(imnumber)
int imnumber;
{
	if(imnumber < 0 || imnumber >= MAXIMAGES)
		error("%d is an illegal image number",imnumber);
}

/*
 * Local routines
 * Check whether the imagehdr corresponding to imnumber is aka another imnumber
 */
static int imagelinked(imnumber)
register int imnumber;
{
	register int i;

	for(i = 1; i < MAXIMAGES; i++) {
		if(i != imnumber && imageHdr[i] == imageHdr[imnumber]) {
/* printf("imagelinked(%d) found linked\n",imnumber); */
			return(1);
		}
	}
	return(0);
}

static int readhdr(ihp)
register struct of_imagehdr *ihp;
{
	register char *cardp;
	register struct of_fitskey *fk;
	register int started = 0;
	char hb[FITSRECL];
	char key[9];

	key[9] = '\0';
	lseek(ihp->fd, 0L, 0);
	for(;;) {
		if(read(ihp->fd, hb, FITSRECL) != FITSRECL) {
			(void)printf("Trouble with read of header\n");
			return(1);
		}
		
		for(cardp = hb ; cardp < hb + FITSRECL; cardp +=80) {
			if( (fk = findkey(cardp)) != 0 ) {
				if(fk->type == LOGICAL && fk->index == END) {
					ihp->dataStart = lseek(ihp->fd, 0L, 1);
					return(0);
				}
				sethdr(fk, ihp, cardp);
				if( !started ) {
				    if( !ihp->simple) {
					(void)printf("No SIMPLE card found\n");
					(void)printf("first card was %.60s\n",cardp);
					return(1);
				    }
				    started++;
				}

			} else if( !started) {
				(void)printf("First card not recognized");
				(void)printf("first card was %.60s\n",cardp);
				return(1);
			}
		}
	}
}

static struct of_fitskey *findkey(cardp)
char *cardp;
{
	char key[9];
	register int i, high, low;
	register int c;

	if(strncmp(cardp, histKey, sizeof(histKey) - 1) == 0)
		cardp += sizeof(histKey);
	for(i = 0; i < 8; i++) {
		if(cardp[i] == ' ') {
			key[i] = '\0';
			break;
		} else {
			key[i] = cardp[i];
		}
	}
	key[8] = '\0';
	high = FITSKEYLEN - 1;
	for(low = 0; high >= low; ) {
		i = (high + low ) / 2;
		if( (c = strcmp(key, fitsKey[i].keyword )) == 0)
			return( &fitsKey[i]);
		if( c < 0)
			high = i - 1;
		else
			low = i + 1;
	}	
/* printf("%s not found\n", key); */
	return( (struct of_fitskey *)0 );
}

static void sethdr(fk, ih, cp)
register struct of_fitskey *fk;
struct of_imagehdr *ih;
char *cp;
{
	register char *ip = cp + 10;
	register char *end;
	register char *op;
	int ipfinal=79; /* last possible column for a string */

	switch(fk->type) {
	case LOGICAL:
		ih->l[fk->index] = (ip[19] == 'T');
		break;
	case INT:
		sscanf(ip,"%d", &ih->i[fk->index]);
		break;
	case STR:
		op = ih->s[fk->index];
/* if we go 10 places and don't find a beginning single quote,
 * it is a bad FITS card. the standard sez column 11 should
 * contain the ', but we allow some slack. decrease ipfinal
 * as ip increases so that ip+ipfinal always points to last
 * possible column.
 */
		for(end = ip + 10; *ip++ != '\''; ) {
			ipfinal--;
			if(ip >= end) goto strerr;
		}
/* the standard sez the end quote should come after column 20,
 * and no later than column 80. 
 * NB: we don't deal with single quotes inside a character string,
 * which are legal and are indicated by two successive quotes.
 */
		for(end = ip; *end != '\''; end++)
			if(end > ip + ipfinal  ) goto strerr;

/* trim any trailing blanks */
		while(end[-1] == ' ')
			end--;
/* this should never be true, but what they hey. if
 * the string is longer than the longest allowable, truncate it 
 */
		if(end > ip + FITSSTRLEN-1)
			end = ip + FITSSTRLEN-1;
		while(ip < end)
			*op++ = *ip++;
		*op = '\0';
		return;

strerr:
		*op = '\0';
		(void)fprintf(stderr,"Bad FITS card: %.50s\n", cp);
		break;
	case IHISTORY:
		ip = strchr(cp + sizeof(histKey), ' ');
		while(*++ip == ' ')
			;
		for(end = cp + 80; *--end == ' '; )
			;
		if(end <= ip)
			goto strerr;
		bcopy(ip, ih->h[fk->index], end + 1 - ip);
		*++end = '\0';
		break;
	case DOUBLE:
		sscanf(ip,"%lf", &ih->d[fk->index]);
		break;
	}
}

/*
 * The Routines which follow are for writing a FITS format file of an image
 */

/*
 * Write an image as a fits file assuming that the image header has been
 * completely filled in including DATAMAX, etc.
 */
void WriteFits(int imnumber, char *fname, char *obj, char *telescope)
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];

	ChkImage(imnumber);
	if(ihp->naxis3 > 1)
		error("WriteFits can't write data cubes");
	if(*obj)
		strcpy(ihp->object, obj);
	if(*telescope)
		strcpy(ihp->telescop, telescope);
	OpenFitsOut(fname);
	WriteFitsHdr(imnumber);
	WriteFitsData(imnumber);
	if(writingToTape)
		WriteEOF(ffd);
	else
		CloseFitsOut();
}

/*
 * SetDataScale finds datamax and datamin by looking through the buffer and
 * sets FITS header values BLANK, BSCALE, BZERO, DATAMAX, and DATAMIN.  It
 * assumes that imageHdr[imnumber]->buf contains the whole image and that only
 * the first two dimensions are > 1.
 */
void SetDataScale(int imnumber)
{
	register struct of_imagehdr * ihp = imageHdr[imnumber];
	register float *pix, *endPix;

	pix = ihp->buf;
	if(ihp->naxis >= 2)
		endPix = pix + ihp->naxis1 * ihp->naxis2;
	else
		endPix = pix + ihp->naxis1;
	while(*pix == FDRAGON)
		if(++pix >= endPix)
			error("No data in image %d\n",imnumber);
	ihp->datamax = ihp->datamin = *pix;
	while(++pix < endPix) {
	    if(*pix != FDRAGON)
		if(*pix > ihp->datamax)
			ihp->datamax = *pix;
		else if(*pix < ihp->datamin)
			ihp->datamin = *pix;
	}
	if(ihp->bitpix < 0) /* bail out here if IEEE floating point format FITS */ 
		return;
	ihp->blank = (-1) << ihp->bitpix - 1;
	ihp->fbzero = (ihp->datamax + ihp->datamin) / 2.;
	if(ihp->bitpix == 16)
		ihp->bscale = (ihp->datamax - ihp->datamin) / 65534.;
	else if(ihp->bitpix == 32)
		ihp->bscale = (ihp->datamax - ihp->datamin) / 4294967294.;
	else if(ihp->bitpix == 8)
		ihp->bscale = (ihp->datamax - ihp->datamin) / 254.;
	else
		error("Can't use BITPIX = %d\n", ihp->bitpix);
}

/* OpenFitsOut and CloseFitsOut are setup with an efficiency kludge for tapes.
 * Any output file in "/dev/" is assumed to be a tape.  When OpenFitsOut is
 * first called with a tape file name, 'writingToTape' is set true and the
 * file name is saved.  Further calls to OpenFitsOut don't try to re-open
 * the tape unless CloseFitsOut is called or the file name changes.  
 * WriteFits[Spectrum] will not automaticly call CloseFitsOut and we hope
 * that the user will do so manually.
 */
void OpenFitsOut(char *fname)
{
    static char saveName[12];

    if( !writingToTape || strncmp(fname, saveName, 12)) {
	if(ffd >= 0)
		SafeClose(ffd);
	ffd = -1;
	writingToTape = 0;
	if(strncmp(fname, "/dev/", 5) == 0) {
		ffd = SafeOpen("Image File",fname,O_WRONLY , 0666);
		strncpy(saveName, fname, 12);
		writingToTape = 1;
	} else {
	    if( access(fname, F_OK) == 0) {
		if( access(fname, W_OK) == 0)
			warn("%s exists and will be overwritten", fname);
		else
			error("%s exists and can't be written", fname);

	    }
	    ffd = SafeOpen("Image File", fname, O_WRONLY | O_CREAT | O_TRUNC,
		0666);
	    writingToTape = 0;
	}
    }
    if( !fitsBuf)
	fitsBuf = SafeAlloc(FITSRECL + 1,"Fits Buffer");
    headPos = fitsBuf;
/*  cfill(fitsBuf,' ',FITSRECL); */
    memset(fitsBuf,' ',FITSRECL);
}

void PadFitsFile()
{
	int len, extra;
	if((len = lseek(ffd, 0, 1)) <= 0)
		error("Trouble seeking in FITS out file");
	if(extra = len % FITSRECL) {
	    len = FITSRECL - extra;	/* amount to pad file */
	    bzero(fitsBuf, len);
	    if(write(ffd,fitsBuf, len) != len)
		error("Trouble writing FITS file");
	}
}

void CloseFitsOut()
{
	if(ffd >= 0)
		SafeClose(ffd);
	ffd = -1;
	if(fitsBuf)
		SafeFree(fitsBuf);
	fitsBuf = 0;
	writingToTape = 0;
}

void WriteEOF(fd)
int fd;
{


# include <sys/ioctl.h>
#if AIX
# include <sys/types.h>
# include <sys/tape.h>
	static struct mtop wef = {STWEOF, (daddr_t)1};

	if(ioctl(fd, STIOCTOP, (char *)&wef) < 0)
		error("Error writing tape EOF");
#else /*AIX*/

#if OSX
		error("Error writing tape EOF");
#else /*OSX*/
# include <sys/types.h>
# include <sys/mtio.h>
	static struct mtop wef = {MTWEOF, 1};

	if(ioctl(fd, MTIOCTOP, (char *)&wef) < 0)
		error("Error writing tape EOF");
#endif /*OSX*/
#endif /*AIX*/
}

/*
 * Write header data from the given imnumber into the fits file which is
 * presumed open.
 */
void WriteFitsHdr(imnumber)
int imnumber;
{
	struct of_fitskey *list[FITSKEYLEN];
	register struct of_imagehdr *ihp = imageHdr[imnumber];
	register struct of_fitskey *fkp;
	register int i;

/*
 * a godforsaken kludge to force RA's to be positive in the output
 * header. A "good" FITS reader should understand negative RA
 * values(?), but many don't (e.g. MIRIAD). So be nice and force
 * RA to lie between 0 and 360. Another manifestation of the
 * "modulo 2PI" problem!  -- MWP Tue Feb 21 16:27:00 PST 1995
 */
        if((i=strncmp(ihp->ctype1,"RA",2)==0) && (ihp->crval1 < 0.))
                ihp->crval1 +=360.0;
        else if((i=strncmp(ihp->ctype2,"RA",2)==0) && (ihp->crval2 < 0.))
                ihp->crval2 +=360.0;

	if(JustPrinting) { 	/* for "im ph:" */ 
		if( !fitsBuf)
			fitsBuf = SafeAlloc(FITSRECL + 1,"Fits Buffer");
		headPos = fitsBuf;
		memset(fitsBuf,' ',FITSRECL);
	}

	for(fkp = fitsKey; fkp < fitsKey + FITSKEYLEN; fkp++)
		list[fkp->sequence] = fkp;
	for(i = 0; i < FITSKEYLEN; i++) {
		fkp = list[i];
		if(fkp->special) {
			switch(fkp->special) {
			case 1:
			case 2:
			case 3:
			case 4:
				if(ihp->naxis < fkp->special) {
					i += MAXAXES - fkp->special;
					continue;
				}
				break;
			case 5:
			case 6:
			case 7:
			case 8:
				if(ihp->naxis < fkp->special - 4) {
					i += 24 + (MAXAXES - fkp->special) * 5;
					continue;
				}
				break;
			case 9:
			/* This is the END card which has no value and should
			 * be preceeded by the extra cards */
				FitsCard(fkp->keyword);
				if(JustPrinting) { /* again, for im ph: */
					if(fitsBuf)
					    SafeFree(fitsBuf);
					fitsBuf = 0;
					return;
				}
				if(headPos > fitsBuf) {
					WriteFitsBuf();
				}
				ffdDataStart = lseek(ffd, 0L, 1);
				return;

			/* case 10 - output only if non zero is handled later */

			case 11:	/* Only used for a stack */
				if( !(ihp->ftype & STACK))
					continue;
				break;
			}
		}
		switch(fkp->type) {
		case LOGICAL:
			if(fkp->special != 10 || ihp->l[fkp->index] !=0)
			   LFitsCard(fkp->keyword, ihp->l[fkp->index]);
			break;
		case INT:
			if(fkp->special != 10 || ihp->i[fkp->index] != 0)
				IFitsCard(fkp->keyword, ihp->i[fkp->index]);
			break;
		case STR:
			if(fkp->special != 10 || *(ihp->s[fkp->index]) != 0)
				SFitsCard(fkp->keyword, ihp->s[fkp->index]);
			break;
		case IHISTORY:
			if(fkp->special != 10 || *(ihp->h[fkp->index]) != 0)
				HFitsCard(fkp->keyword, ihp->h[fkp->index]);
			break;
		case DOUBLE:
			if(fkp->special != 10 || ihp->d[fkp->index] != 0)
				DFitsCard(fkp->keyword, ihp->d[fkp->index]);
			break;
		default:
			error("Internal error: Bad FITS keyword type %d",
				fkp->type);
		}
	}
}

static void WriteFitsData(int imnumber)
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];
	register int i;				/* scaled in value of pixel */
	register char *op;			/* output pointer */
	register float *pix, *endPix;		/* current pixel, last pixel of
						 * current line in image buf */
	char *endBuf;				/* end of fits buffer */
	int xdir, ydir;				/* order of pix and lines in
						 * FITS file */
	int y, endy;				/* range of lines in image */
	double deltay;				/* extra output of ImageYGRid */
	int yunit;				/* more extra output */
#if BYTEREVERSED
        register int t; /* MWP 5/22/96 */
#endif

	ImageYGrid(imnumber, &y, &endy, &deltay, &yunit);
	if(ihp->cdelt2 > 0) {
		ydir = 1;
		endy++;
	} else {
		ydir = -1;
		i = y;
		y = endy;
		endy = i - 1;
	}
	xdir = (ihp->cdelt1 > 0)? 1: -1;
	op = fitsBuf;
	endBuf = op + FITSRECL;

#if 0
(void)printf("WriteFitsData: y = %d, endy = %d, cdelt2 = %g\n", y, endy, ihp->cdelt2);
(void)printf("WriteFitsData:ydir = %d, bufline = %d, buflen = %d\n", ydir, ihp->bufline, ihp->buflen);
#endif /*0*/
	for( ; y != endy; y += ydir) {
	    pix = GetImageLine(imnumber, -1, y, 5, ydir);
	    if(ihp->cdelt1 > 0) {
		endPix = pix + ihp->naxis1;
	    } else {
		endPix = --pix;
		pix += ihp->naxis1;
	    }
	    for( ;pix != endPix; pix += xdir) {
		if(*pix == FDRAGON)
		    i = ihp->blank;
		else
		    i = (*pix - ihp->fbzero) / ihp->bscale;

		switch(ihp->bitpix) {
		case 16:
		    if(i > 32767)
			i = 32767;
		    else if(i < -32768)
			i = -32767;
# if BYTEREVERSED
		    *op++ = i >> 8;
		    *op++ = i;
# else
		    *(short *)op  = i;
		    op += 2;
# endif /*BYTEREVERSED*/
		    break;
		case 32:
# if BYTEREVERSED
		    *op++ = i >> 24;
		    *op++ = i >> 16;
		    *op++ = i >> 8;
		    *op++ = i;
# else
		    *(int *)op  = i;
		    op += 4;
# endif /*BYTEREVERSED*/
	    	    break;
		case -32: /* MWP 5/22/96 */
		    if((*(float *)op=*pix) == FDRAGON)
			*(float *)op = FNaN;
#if BYTEREVERSED
                   t = op[3];
                   op[3] = op[0];
                   op[0] = t;
                   t = op[2];
                   op[2] = op[1];
                   op[1] = t;
#else
#endif /*BYTEREVERSED*/
		   op+=4;
		   break;
		case 8:
		    if(i > 127)
			i = 127;
		    else if(i < -128)
			i = -127;
		    *op++ = i;
		    break;
		} /* end switch */
		if(op >= endBuf) {
		    WriteFitsBuf();
		    op = fitsBuf;
		}
	    } /* for (;pix...) */
	}/* for (( ; y != endy...) */ 

	if(op != fitsBuf) {
	    bzero(op, endBuf - op);
	    WriteFitsBuf();
	}
}

/*
 * Write a line of pixels in a FITS file.  Designed for writing data
 * cubes.  The data type in the file is assumed to be float.  Dragon
 * substitution and byte order reversing will be done in place since
 * wc discards the buffer in any case.
 */
void WriteFitsDataLine(int imnumber, float *data, 
			int line, int plane)
/* imnumber - image number
 * data; - pointer to the data array 
 * int plane, line - offsets in file from ffdDataStart 
 */
{
	struct of_imagehdr *ihp = imageHdr[imnumber];
	int linelength;
	register char *cp;
	char  *endp;
#if BYTEREVERSED
	register int t;
#endif

	linelength = ihp->naxis1 * sizeof(float);
#if 0
	(void)printf("%.6d ",ffdDataStart + (plane * ihp->naxis2 + line) * linelength);
	{	int i;
		for(i = 0; i < ihp->naxis1; i+= 3)
			if(data[i] != FDRAGON)
				(void)printf("%9.2f ", data[i]);
			else
				(void)printf("DRAGON ");
		putchar('\n');
	}
#endif
	if( lseek(ffd, (off_t)(ffdDataStart + (plane * ihp->naxis2 + line) *
			linelength), 0) < 0) {
		perror("Trouble seeking in FITS file because");
		error(empty);
	}
	endp = linelength + (cp = (char *)data);
	for(; cp < endp; cp += 4) {
	    if(*(float *)cp == FDRAGON)
		*(float *)cp = FNaN; 
#if BYTEREVERSED
	    t = cp[3];
	    cp[3] = cp[0];
	    cp[0] = t;
	    t = cp[2];
	    cp[2] = cp[1];
	    cp[1] = t;
#endif
	}
	if(write(ffd, data, linelength) != linelength) {
		perror("Trouble writing in FITS file because");
		error(empty);
	}
}

/* create a Float FITS Card */
void FFitsCard(char *name,double value)
{
	(void)sprintf(headPos,"%-8s= %#20.9G /",name,value);
	headPos[32] = ' ';
	NextCrd();
}

/* create a Double FITS Card */
void DFitsCard(char *name,double value)
{
	(void)sprintf(headPos,"%-8s= %#20.9G /",name,value);
	headPos[32] = ' ';
	NextCrd();
}

/* create a Integer FITS Card */
void IFitsCard(char *name,int value)
{
	(void)sprintf(headPos,"%-8s= %20d /",name,value);
	headPos[32] = ' ';
	NextCrd();
}

/* create a String FITS card */
void SFitsCard(char *name,char *value)
{
	char tempstr[80];
	char fmt[80];
	int len=strlen(value);
	int fmtlen = (len < 19 ) ? 19 - len : 1;
	if(len<8) fmtlen=11;
	(void)sprintf(fmt,"%%-8s= '%%-8s'%%%ds/",fmtlen);
	
	(void)sprintf(tempstr,fmt,name,value,empty);
	FitsCard(tempstr);
}

/* create a History FITS card */
void HFitsCard(char *name,char *value)
{
	(void)sprintf(headPos,"%s %s %.57s",histKey, name, value);
	headPos[strlen(headPos)] = ' ';
	NextCrd();
}

/* create a Logical FITS card */
void LFitsCard(char *name,int value)
{
	value = (value)?'T':'F';
	(void)sprintf(headPos,"%-8s= %20c /",name,value);
	headPos[32] = ' ';
	NextCrd();
}

/* copy up to 80 chars into the current header position */
void FitsCard(char *card)
{
	register char *ip = card, *op = headPos;
	register char *end = op + 80;
	while(*ip && op < end)
		if(islower(*ip))
			*op++ = toupper(*ip++);
		else
			*op++ = *ip++;
	NextCrd();
}

/* write out the card, padding with blanks if necessary */
static void NextCrd()
{
/* We may be just printing to stdout via "im ph:"
 * and we only the most recent card which headBuf,
 * conveniently enough, points to.
 */
	if(JustPrinting) {
		(void)printf("%.80s\n",headPos);
		headPos+=80;
	}
	else {
	  if( (headPos += 80) >= fitsBuf + FITSRECL) {
		WriteFitsBuf();
		memset(fitsBuf,' ',FITSRECL);
	  }
	}
}

/* output the FITS buffer */
static void WriteFitsBuf()
{
	if(write(ffd,fitsBuf,FITSRECL) != FITSRECL)
		error("Trouble writing FITS file");
	headPos = fitsBuf;
}
/* label a spatial axis */
char *FitsAxisLabel(int axis)
{
	static char name[3][2][2][5] = {
		{{"RA  ","DEC"},{"RA--","DEC-"}},
		{{"GLON","GLAT"},{"GLON","GLAT"}},
		{{"DX  ","DY  "},{"DX--","DY--"}}
	};
	static char suffix[5][2][5] = {
		{"    ","    "},{"-SIN","-SIN"},
		{"---X","---X"},{"-TAN","-TAN"},
		{"-NCP","-NCP"}
	};
	static char label[9];
	register char *suf;

	suf = suffix[cproj.type][axis];
	strcpy(label,name[csys.type][*suf != ' '][axis]);
	strcpy(label + 4, suf);
	return(label);
}

/* return the appropriate set of axis names for this image.
 * returned form is an AXISNAME struct 
 */
struct AXISNAMES FitsAxisUnits(char *ctype)
{
	static struct AXISNAMES axisNames[] = {
		{60.,ARCMINUTES,SPATIAL | OFFSETTYPE,"RA",2,"Ra"},
		{60.,ARCMINUTES,SPATIAL | OFFSETTYPE | LATTYPE,"DEC",3, "Dec"},
		{1.,DEGREES,SPATIAL,"GLON",4,"l"},
		{1.,DEGREES,SPATIAL | LATTYPE,"GLAT",4,"b"},
		{1e-3,KMPERSEC,VELOCITY,"VELO",4,"Velocity"},
		{1e-3,KMPERSEC,VELOCITY,"FELO",4,"Velocity"},
		{1,SECONDS,TIME,"TIME",4,"Time"},
		{1e-6,MEGAHERTZ,FREQUENCY,"FREQ",4,"mHz"},
		{1e6,MICROMETERS,WAVELENGTH,"LAMBD",5,"Microns"},
		{1.,NONE,UNDEF,"PIXEL",5,"Pix"},
		{1.,NONE,UNDEF,"     ",5,"Pix"},
	};
	struct AXISNAMES tempName;
	register int n;

	for(n = 0; n < sizeof(axisNames)/sizeof(struct AXISNAMES); n++){
		if( !strncmp(ctype, axisNames[n].fname, axisNames[n].len) )
			return(axisNames[n]);
	}
	/* if we didn't find it, assume degrees */
	tempName = axisNames[2];
	for(n = 0; n < 8; n++) {
		if(ctype[n] == ' ')
			break;
		tempName.lname[n] = ctype[n];
		if(n < tempName.len)
			tempName.fname[n] = ctype[n];
	}
	tempName.lname[n] = '\0';
	return(tempName);
}

/****************************************************************************
 * These are image arithmatic routines used by "im" and eventually,         *   
 * whatever will compute tau and temp maps from IRAS bands                  *
 *			MWP 11/91                                           *
 * added mina and minb to clip images at selected pixel values              *
 *                      MWP 1/11/93                                         *
 * The routines are:						 	    *
 * ImageOp(ima,imb,imc,OPR,mina,minb) : imc = ima(OPR)imb		    *
 *                                      where ima <= mina && imb <= minb    *
 * RescaleImage(ima,rsfact,added)  : ima = ima*rsfact + addend		    *
 * CopyHdr(ima,imb)		   : fill up imb's hdr with stuff from	    *
 *				     ima. Changed values filled in later    *
 * added IRAS routines: 1/20/95  MWP				 	    *
 * SubImage(A,B,xmn,xmx,ymn,ymx,minpix) :      Takes a specified            *
 *                                      subimage of A and writes it to B    *
 *                                      Tue Feb 21 10:17:13 PST 1995        *
 ****************************************************************************/

void ImageOp(ima,imb,imDest,opr,mina,minb)
int imb, ima, imDest, opr; 
double mina, minb;
{
	struct of_imagehdr *ihpa, *ihpb,*ihpD; 
	float *pixa, *pixb, *pixD, *endPix;
	int stat, c;
	char tmp[8];

/* 
 * the variable stat > 0 if the pixel was set blank during any
 * operation.
 */

/* 	Do a bunch of checks of the input images 	
 *	note ImageSimilar() calls ChkImage()	
 */

	if(ima==imDest || imb==imDest)
		error("source and destination have same image number");
	ImageSimilar(ima, imb);

/* we need to make one more check of images that ImageSimilar doesn't do.
 * and that is to check Naxes  are the same. We don't need to do this
 * for scatter plot (sp) as area is defined for that, hence we don't want
 * to put this check into ImageSimilar 
 */
	ChkNaxis(ima,imb);

	ihpa=imageHdr[ima]; 
	ihpb=imageHdr[imb]; 


 	StartStdImage(imDest);
	ihpD=imageHdr[imDest];
	CopyHdr(ima,imDest);

/* pixa,b will be null pointers unless cp or cm has been run, so check
 * for this. If null, call GetImageLine. 
 */
	if((pixa=ihpa->buf)==NULL) {
		pixa=GetImageLine(ima,-1, (ihpa->cdelt2 > 0)? 
                       1 - (int) (ihpa->crpix2 +0.5): (int) (ihpa->crpix2 +0.5) - ihpa->naxis2, 
                       ihpa->naxis2, 1);
	}

	if((pixb=ihpb->buf)==NULL) {
		pixb=GetImageLine(imb,-1, (ihpb->cdelt2 > 0)? 
                       1 - (int) (ihpb->crpix2 + 0.5): (int) (ihpb->crpix2 +0.5) - ihpb->naxis2, 
                       ihpb->naxis2, 1);
	}
/* make array for resultant image 			*/
	pixD=ihpD->buf=(float *)(ChkAlloc(ihpa->naxis1 * ihpa->naxis2 * sizeof(float),
                "OpImDest"));

	(void)sprintf(tmp,"Image %d",imDest);
	(void)strcpy(ihpD->fname,tmp);
	ihpD->bufline=0;
	ihpD->buflen=ihpa->naxis2;
#ifdef DEBUG
	PrintHdr(ima);
	PrintHdr(imb);
	PrintHdr(imDest);
#endif 

	if(ihpa->naxis >= 2)
                endPix = pixa + ihpa->naxis1 * ihpa->naxis2;
        else
                endPix = pixa + ihpa->naxis1;
	switch(opr) {
		case ADD:
			stat=0;
			while (pixa < endPix) {
			  if( (*pixa!=FDRAGON) && (*pixb != FDRAGON) &&
			(*pixa >= mina) && (*pixb >= minb) )
			     *pixD++ = *pixa + *pixb;
			  else {
			     *pixD++ = FDRAGON;
			     stat++;
			  }
			  pixa++;
			  pixb++;
			}
			break;
		case SUB: 
			stat=0;
			while (pixa < endPix) {
			  if( (*pixa!=FDRAGON) && (*pixb != FDRAGON) &&
			(*pixa >= mina) && (*pixb >= minb) )
			     *pixD++ = *pixa - *pixb;
			  else {
			     *pixD++ = FDRAGON;
			     stat++;
			  }
			  pixa++;
			  pixb++;
			}
			break;
		case DIV:
			stat=0;
			while (pixa < endPix) {
			  if( (*pixa!=FDRAGON) && (*pixb != FDRAGON) &&
			(*pixa >= mina) && (*pixb >= minb) )
			     *pixD++ = *pixa / *pixb;
			  else {
			     *pixD++ = FDRAGON;
			     stat++;
			  }
			  pixa++;
			  pixb++;
			}
/* change the bunit keyword to reflect the op 	
 */
			if((c=strcmp(ihpa->bunit,ihpb->bunit))==0)
		/* now unitless because the units were the same) */
				memset(ihpD->bunit,' ',8); 
			else {
				strcpy(ihpD->bunit,"[");
                		strcat(ihpD->bunit,ihpa->bunit);
                		strcat(ihpD->bunit,"]/[");
                		strcat(ihpD->bunit,ihpb->bunit);
                		strcat(ihpD->bunit,"]");
			}
			break;
		case MUL:
			stat=0;
			while (pixa < endPix) {
			  if( (*pixa!=FDRAGON) && (*pixb != FDRAGON) &&
			    (*pixa >= mina) && (*pixb >= minb) )
			     *pixD++ = *pixa * *pixb;
			  else {
			     *pixD++ = FDRAGON;
			     stat++;
			  }
			  pixa++;
			  pixb++;
			}
/* change the bunit keyword to reflect the op
 */
                          strcpy(ihpD->bunit,"[");
                          strcat(ihpD->bunit,ihpa->bunit);
                          strcat(ihpD->bunit,"]*[");
                          strcat(ihpD->bunit,ihpb->bunit);
                          strcat(ihpD->bunit,"]");

			break;
                case TEMP: /* IRAS temperature */
			stat=0; 
			taut(ima,imb,taupow,opr);
			while (pixa < endPix) {
			  if( (*pixa!=FDRAGON) && (*pixb != FDRAGON) &&
			    (*pixa >= mina) && (*pixb >= minb) )
                            stat += r2temp(*pixa, *pixb, tempfact, sband, pixD);
                        else {
                              *pixD = FDRAGON;
				stat++;
                             }
			  pixa++;
			  pixb++;
			  pixD++;
			}
        		strcpy(ihpD->bunit, "K");
                        break;
                case CC: /* IRAS color correction */
			stat=0;
			taut(ima,imb,taupow,opr);
			while (pixa < endPix) {
			  if( (*pixa!=FDRAGON) && (*pixb != FDRAGON) &&
			    (*pixa >= mina) && (*pixb >= minb) )
                             stat += cc(*pixa, *pixb, ccfact, sband, pixD);
                          else {
                              *pixD = FDRAGON;
				stat++;
                             }
			  pixa++;
			  pixb++;
			  pixD++;
			}
                        /* units after this operation are Jansky */
			(void)printf(" Units changed from %s to ",ihpD->bunit);
                        if(strcmp(ihpD->bunit, "MJY/SR")|| 
                           strcmp(ihpD->bunit, "W/M2/SR") ||
                           strcmp(ihpD->bunit, "JY/SR") )
			     strcpy(ihpD->bunit,"JY/SR");
			else
                             strcpy(ihpD->bunit, "JY") ;

			(void)printf("%s.\n",ihpD->bunit);
                        break;
                case TAU: /* IRAS opacity */
			stat = 0;
			taut(ima,imb,taupow,opr);
			while (pixa < endPix) {
			  if( (*pixa!=FDRAGON) && (*pixb != FDRAGON) &&
			  (*pixa >= mina) && (*pixb >= minb) )
                           stat += taum(*pixa, *pixb, sband, pixD);
                          else {
                              *pixD = FDRAGON;
				stat++;
                             }
			  pixa++;
			  pixb++;
			  pixD++;
			}
        		strcpy(ihpD->bunit, "TAU");
                        break;
		default: /* this should never occur */
			error("ImageOp: bad operation: op = %d",opr);
			break;
	}
	(void)printf(" %d pixels were blanked on output\n",stat);
}

/******************************************/
/* Rescale image by rsfact and add addend */
/******************************************/
void RescaleImage(int imx,double rsfact,double addend) 
/* imx - the image to rescale 
 * rsfact - the rescale factor 
 * addend - additive offset 
 */
{
	struct of_imagehdr *ip=imageHdr[imx];
	float *pix, *endPix;
	int i=0;
	
	ChkImage(imx);

#ifdef DEBUG 
	PrintHdr(imx);
#endif

	if((pix = ip->buf)==NULL)
		pix = GetImageLine(imx,0, (ip->cdelt2 > 0)? 
                       1 - (int)(ip->crpix2 + 0.5): (int)(ip->crpix2 +0.5) - ip->naxis2, 
                       ip->naxis2, 1);
	if(ip->naxis >= 2)
                endPix = pix + ip->naxis1 * ip->naxis2;
        else
                endPix = pix + ip->naxis1;

	while (pix < endPix) {
	/* If its FDRAGON, leave it like that */
		if(*pix != FDRAGON) {
#if DEBUG
	printf("%g ",*pix);
#endif
	 		*pix *= rsfact;
			*pix += addend;
		i++;
		}
	pix++;
#if DEBUG
	if(i==10) {
		printf("\n");
		i=0;
	 }
#endif
	}
	return;
}

/***************************************************/
/* print out various header values (for debugging) */
/***************************************************/
void PrintHdr(int imnumber) 
{
	struct of_imagehdr *ip=imageHdr[imnumber];

	(void)printf("name:%s fd:%d ftype:%d\n",ip->fname,ip->fd,ip->ftype);
	(void)printf("dataStart: %d\n",ip->dataStart); 
	(void)printf("   buflen: %d\n",ip->buflen);
	(void)printf("  bufline: %d\n",ip->bufline);
	(void)printf("      buf: %f\n",ip->buf);
	(void)printf("    naxis: %d\n",ip->naxis);
	(void)printf("   naxis1: %d\n",ip->naxis1);
	(void)printf("   naxis2: %d\n",ip->naxis2);
	(void)printf("   crpix1: %f\n",ip->crpix1);
	(void)printf("   crpix2: %f\n",ip->crpix2);
	(void)printf(" bufplane: %d\n",ip->bufplane);
}

/************************************************/
/* Copy Image header from one image to another  */
/*	The header format can be found in "C.h" */
/************************************************/
void CopyHdr(int imfrom,int imto)
{

	struct of_imagehdr *ipf=imageHdr[imfrom];
	struct of_imagehdr *ipt=imageHdr[imto];
	int index;

/*for now copy everything and then overwrite new stuff later */
/* don't copy filename or type or descriptor, of course      */
	
	/*ipt->dataStart=ipf->dataStart;*/ /*NOt these either!*/
	/*ipt->buflen=ipf->buflen;   */
	/*ipt->bufline=ipf->bufline; */
	for(index=0;index<NUMFITSSTR;index++) 
		strcpy(ipt->s[index],ipf->s[index]);
	for(index=0;index<NUMFITSHIST;index++) 
		strcpy(ipt->h[index],ipf->h[index]);
	for(index=0;index<NUMFITSLOG;index++) 
		ipt->l[index]=ipf->l[index];
	for(index=0;index<NUMFITSINT;index++)
		ipt->i[index]=ipf->i[index];
	for(index=0;index<NUMFITSDOUBLE;index++)
		ipt->d[index]=ipf->d[index];

}

/************************************************************/
/* Get a subimage imDest from imSrc                         */
/************************************************************/
/* NOTE : DEBUG on this subroutine will print out the pixel
 *        values of subimage. Potentially many many screens
 *        full of numbers!
 */
void SubImage(imSrc,imDest,xmin,xmax,ymin,ymax,minpix)
int imSrc,imDest;            /* input and output images    */
double xmin,xmax,ymin,ymax;  /* subimage limits            */
double minpix;               /* min pixel value to include */
{
	struct of_imagehdr *ihpS, *ihpD; 
	float *pixS, *pixD, *endPix, *array;
	int xsize, ysize;		 /* axis sizes in pixels of subimage */
	int loxS,loyS,hixS,hiyS;	 /* pixels from center of Source image */
        int lox,hix,loy,hiy;		 /* subimage pixels from center of 
					  * Source image */
	double delx, dely;		 /* scale, e.g. arcmin per pixel     */
	int xunit, yunit ; 		 /* units of delx and dely         */
	int line,xp;
	char tmp[16];
#ifdef DEBUG
        (void)fprintf(stderr,"inputs: %d %d %f %f %f %f %g\n",
		imSrc, imDest, xmin,xmax,ymin,ymax,minpix);
#endif

	ChkImage(imSrc);
	ihpS=imageHdr[imSrc];

/*
 * get delx and dely and (x,y) offsets for source image
 * Note that these are offsets is from crpix so there is a n+1 problem 
 * eg. if crpix1=1, then loxS=0 
 */
	ImageXGrid(imSrc,&loxS,&hixS,&delx,&xunit);
	ImageYGrid(imSrc,&loyS,&hiyS,&dely,&yunit);
/*
 * we want equality at both ends, in contradistinction to the usual
 * convention. 
 */
	(void)printf("Rounding requested subimage to nearest pixel.\n");
 
	lox = (int)(xmin/delx +0.5);
	hix = (int)(xmax/delx +0.5);
	loy = (int)(ymin/dely +0.5); /* to be passed to GetImageLine */
	hiy = (int)(ymax/dely +0.5);
#ifdef DEBUG
        (void)fprintf(stderr,"xSrc: %d %d ySrc: %d %d| xReq:%d %d yReq:%d %d dx:%f dy:%f\n",
                loxS,hixS,loyS,hiyS,lox,hix,loy,hiy,delx,dely);
#endif

        /* if x or y limits exceed image boundaries, set them to
         * image boundaries. This allows lazy people like me to
         * define areas with impunity.-ihpS->crval2)
         */
        if(lox < loxS) {
                lox=loxS;
		(void)printf("Resetting low x to %.2f\n",(float)lox*delx);
	}
	if(loy < loyS) {
		loy=loyS;
		(void)printf("Resetting low y to %.2f\n",(float)loy*dely);
	}
	if(hix > hixS) {
		hix=hixS;
		(void)printf("Resetting hi x to %.2f\n",(float)hix*delx);
	}
	if(hiy > hiyS) {
		hiy=hiyS;
		(void)printf("Resetting hi y to %.2f\n",(float)hiy*dely);
	}

	xsize = hix-lox+1;
	ysize = hiy-loy+1;
	if((xsize > ihpS->naxis1) || (ysize > ihpS->naxis2))
		error("Subimage bigger than image");

/*
 * now allocate the space for the output array 
 */
	StartStdImage(imDest);
	ihpD=imageHdr[imDest];
	CopyHdr(imSrc,imDest);
        pixD=ihpD->buf=(float *)(ChkAlloc(xsize*ysize*sizeof(float), "SubImDest"));
        (void)sprintf(tmp,"Image %d",imDest);
        (void)strcpy(ihpD->fname,tmp);
        ihpD->bufline=0;
        ihpD->buflen=ysize;
	ihpD->naxis1=xsize;
	ihpD->naxis2=ysize;

	for(line=loy;line<=hiy;line++) {
	 pixS = GetImageLine(imSrc,-1,line,1,1) ;
/* pixS now points to the most negative x offset in that row, i.e.
 * pixS[0] = value at loxS
 */
	 for(xp=lox;xp<=hix;xp++) {
	   *pixD = (pixS[xp-loxS] >= minpix) ? pixS[xp-loxS] : FDRAGON;
#ifdef DEBUG
	   if(*pixD!=FDRAGON)
	    (void)fprintf(stderr,"%7.2f  ",*pixD);
	   else
	    (void)fprintf(stderr,".DR  ");
#endif
	   pixD++;
	 }
#ifdef DEBUG
	  (void)fprintf(stderr,"\n");
#endif
	}
/* 
 * recalculate crpix 
 */
	if(ihpS->cdelt1 > 0 )
	 ihpD->crpix1=ihpS->crpix1-(lox-loxS);
	else
/*
 * the +1. is to make ihpD->crpix1=1 if hix=0. see above. 
 */
         ihpD->crpix1=hix+1. + ihpS->crpix1-(int)ihpS->crpix1; 

	if(ihpS->cdelt2 > 0 )
	 ihpD->crpix2=ihpS->crpix2-(loy-loyS);
	else
         ihpD->crpix1=hiy+1. + ihpS->crpix2-(int)ihpS->crpix2; 

#ifdef DEBUG
	PrintHdr(imDest);
#endif
	SetDataScale(imDest);
	return;
}

/*********************************/
/* END of image arithmatic stuff */
/*********************************/
/* Start of routines for FITS tapes of spectra.  These are at the end of the
 * file so that ../stacks/C.h won't bother anything else.
 */

void WriteFitsSpectrum(struct of_stk *stk, char *fn, char *obj, char *telescope)
{
	int imnumber = 0;

	StartStdImage(imnumber);
	PrepareSpectrumHdr(imnumber, stk, obj, telescope);
	SetDataScale(imnumber);
	OpenFitsOut(fn);
	WriteFitsHdr(imnumber);
	WriteSpectrumData(imnumber);
	if(writingToTape)
		WriteEOF(ffd);
	else
		CloseFitsOut();
	CloseImage(imnumber);
}

static void PrepareSpectrumHdr(int imnumber, struct of_stk *stk, 
			       char *obj, char *telescope)
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];

	/* If we have access to the scan header, 
	   add in some more variables -CLM 3-Dec-2004 */

	if(stk->nstk[(stk->nstk[0] < 0)? 2: 0] == curscn_.num) {
	  ihp->tel_azimuth=scan_.azimuth;
	  ihp->tel_elevation=scan_.elevation;
	  ihp->tel_Tamb=scan_.TAmbient;
	  ihp->tel_Tatm=scan_.TAmbient+scan_.DTAbs;
	  ihp->Ftsky=scan_.tsky;
	  ihp->tel_VAnt=scan_.VAnt;
	}

	ihp->ftype |= STACK;
	strcpy(ihp->bunit, "K");
	ihp->naxis = 3;
	ihp->naxis1 = stk->numpt;
	strcpy(ihp->ctype1, "FREQ");
	ihp->Frestfreq = stk->freq * 1e6;
	ihp->cdelt1 = -1e6 * stk->fwid;
	ihp->crpix1 = stk->expch;
	if (stk->refch != stk->expch)
	  ihp->Rcrpix1= stk->refch;
	ihp->naxis2 = 1;
	ihp->naxis3 = 1;
	ihp->baseln = stk->ibsln;
	if((stk->nplc % 10) == 1) {	/* galactic coords */
		strcpy(ihp->ctype2,"GLON");
		ihp->crval2 = stk->l;
		ihp->cdelt2 = stk->dl;
		strcpy(ihp->ctype3,"GLAT");
		ihp->crval3 = stk->b;
		ihp->cdelt3 = stk->db;
	} else {			/* equatorial coordinates */
		strcpy(ihp->ctype2,"RA");
		ihp->crval2 = stk->ra * 15.;
		ihp->cdelt2 = stk->odra / sixty;
		strcpy(ihp->ctype3,"DEC");
		ihp->crval3 = stk->dec;
		ihp->cdelt3 = stk->ddec;
		ihp->Fequinox = stk->epoch;
	}
	ihp->crpix2 = ihp->crpix3 = 0.0;
	ihp->Fdeltav = stk->fwid * 2.99792458e8 / stk->freq;
	ihp->Fvelo_lsr = stk->vlsr * 1000.;
	ihp->scan_num = (double)stk->nstk[(stk->nstk[0] < 0)? 2: 0];
	ihp->Fobstime = stk->time;
	ihp->Ftsys = 1000. * sqrt(stk->time / stk->wght[0]);
	ihp->Ftrms=stk->rms;
	strcpy(ihp->object, obj);
	strcpy(ihp->telescop, telescope);
	/* put the DATE-OBS out */
	if( stk->j2second != 0 )
        	strcpy(ihp->dateobs,(const char *)j2sToDate(stk->j2second));
	/* Indicate where the data is */
	ihp->buf = stk->stak;
	ihp->buflen = 1;
}

static void WriteSpectrumData(int imnumber)
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];
	register int i;				/* scaled in value of pixel */
	register char *op;			/* output pointer */
	register float *pix, *endPix;		/* current pixel, last pixel of
						 * current line in image buf */
	char *endBuf;				/* end of fits buffer */

	op = fitsBuf;
	endBuf = op + FITSRECL;

	for(endPix = ihp->naxis1 + (pix = ihp->buf); pix < endPix; pix++) {
		if(*pix == FDRAGON)
		    i = ihp->blank;
		else
		    i = (*pix - ihp->fbzero) / ihp->bscale;

		if(ihp->bitpix == 16) {
# if BYTEREVERSED
		    *op++ = i >> 8;
		    *op++ = i;
# else
		    *(short *)op  = i;
		    op += 2;
# endif /*BYTEREVERSED*/
		} else if(ihp->bitpix == 32) {
# if BYTEREVERSED
		    *op++ = i >> 24;
		    *op++ = i >> 16;
		    *op++ = i >> 8;
		    *op++ = i;
# else
		    *(int *)op  = i;
		    op += 4;
# endif /*BYTEREVERSED*/
		} else {
		    *op++ = i;
		}
		if(op >= endBuf) {
		    WriteFitsBuf();
		    op = fitsBuf;
		}
	}
	if(op != fitsBuf) {
	    bzero(op, endBuf - op);
	    WriteFitsBuf();
	}
}

void ReadFitsSpectrum(struct of_stk *stk, char *fn)
{
	int imnumber = 0;

	OpenFits(imnumber, fn);
	SetupStackHdr(imnumber, stk);
	ReadFitsStackData(imnumber, stk);
	CloseImage(imnumber);
}

static void SetupStackHdr(int imnumber, struct of_stk *stk)
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];

	if(ihp->naxis2 > 1 || strncmp(ihp->ctype1, "FREQ",4))
		error("Fits file isn't a spectrum\n");

	bzero((char *)stk, sizeof(*stk));
	ihp->ftype |= STACK;
	if(ihp->naxis1 > MAXCHN) {
		warn("Too many channels to fit in a stack, will truncate");
		stk->numpt = MAXCHN;
	} else
		stk->numpt = ihp->naxis1;
	stk->nver = 1;
	stk->nwts = 1;
	stk->locwt[0] = 1;
	stk->wght[0] = 1e6 * ihp->Fobstime / (ihp->Ftsys * ihp->Ftsys);
	stk->factt = 1.;
	stk->ibsln = ihp->baseln;
	stk->nstk[0] = (int)(ihp->scan_num +0.5);
	stk->nlist = 1;
	stk->indx = 1;
	stk->numst = 1;
	if(ihp->cdelt1 > 0) {
		stk->fwid = ihp->cdelt1 * 1e-6;
		stk->expch = ihp->naxis1 + 1 - ihp->crpix1;
	} else {
		stk->fwid = -ihp->cdelt1 * 1e-6;
		stk->expch = ihp->crpix1;
	}
	stk->nprc = 1;
	/* After adding nphase, need to set nprc and refch and fix up expch*/
	stk->refch = -9999.0;

/* VLSR, VELO, and VELO-LSR have been aliased to the same entry
 * in the imagehdr.  Same for EPOCH and EQUINOX.  RWW 5/95
 */
	stk->vlsr = ihp->Fvelo_lsr * 1e-3;

	stk->time = ihp->Fobstime;
	stk->rms = 2.0 / sqrt(stk->fwid * stk->wght[0]);

	if(ihp->Fequinox > 1899)
		stk->epoch = ihp->Fequinox;
	else
		stk->epoch = 1950;

	if( !strncmp(ihp->ctype2, "RA", 2) && !strncmp(ihp->ctype3, "DEC", 3)) {
		double l, totl, b, totb;

		stk->ra = ihp->crval2 / 15.;
		stk->dec = ihp->crval3;
		stk->ddec =  (1. - ihp->crpix3) * ihp->cdelt3;
		stk->oddec = stk->ddec * 60.;
		stk->odra = (1. - ihp->crpix2) * ihp->cdelt2 * 60.;
		stk->dra = stk->odra / (900. * cos((stk->dec + stk->ddec) *
			cunits[DEGREES]));
		stk->nplc = 30;

		/* compute galactic coordinates */
		rdtlb(stk->ra, stk->dec, stk->epoch, &l, &b);
		rdtlb(stk->ra + stk->dra, stk->dec + stk->ddec, stk->epoch,
			&totl, &totb);
		stk->l = l;
		stk->b = b;
		stk->dl = totl - l;
		stk->db = totb - b;
	} else if( !strncmp(ihp->ctype2, "GLON", 4) &&
			!strncmp(ihp->ctype3, "GLAT", 4)) {
		double ra, dec, totra, totdec;

		stk->l = ihp->crval2;
		stk->b = ihp->crval3;
		stk->dl = (1. - ihp->crpix2) * ihp->cdelt2;
		stk->db = (1. - ihp->crpix3) * ihp->cdelt3;
		stk->nplc = 11;

		/* compute Ra and Dec */
		lbtrd(stk->l, stk->b, stk->epoch, &ra, &dec);
		lbtrd(stk->l + stk->dl, stk->b + stk->db, stk->epoch,
			&totra, &totdec);
		stk->ra = ra;
		stk->dec = dec;
		stk->dra = totra - ra;
		stk->ddec = totdec - dec;
		stk->odra = 900. * (totra - ra) * cos(totdec * cunits[DEGREES]);
		stk->oddec = 60 * (totdec - dec);
	}
	stk->freq = ihp->Frestfreq * 1e-6;
/* convert the FITS 2000 compliant date-obs to j2second. 
 * if non-compliant, j2second = 0 
 */
	stk->j2second = dateToJ2S(ihp->dateobs); 
}

static void ReadFitsStackData(int imnumber, struct of_stk *stk)
{
	register struct of_imagehdr *ihp = imageHdr[imnumber];
	int fitsWordLen;	/* Len of an int in the fits file */
	register int pixel;	/* int value of Fits pixel */
	char *fb;		/* pointer to fits buffer */
	register char *ip;	/* pointer to next byte in fitsbuffer */
	register char *fbend;	/* pointer beyond end of fits buffer */
	register float *op;	/* pointer to next loc in stack buf */
	register int dir;	/* +4 or -4, dir. of putting data in stack */
	register float *end;	/* pointer to end of stack buf - either
				 * &stak[numpt] or &stak[-1] */
	/* Note, ip will always advance regularly, but op will
	 * have to move backward through stack if cdelt1 is positive.
	 */

	fitsWordLen = abs(ihp->bitpix) >> 3;

	fb = SafeAlloc(FITSRECL, "FITS buffer");
	/* set ip to end of buffer to force a read */
	ip = fbend = fb + FITSRECL;

	if(ihp->cdelt1 > 0) {		/* Start at end of stack */
		dir = -1;
		op = (end = stk->stak - 1) + stk->numpt;
	} else {
		dir = 1;
		end = (op = stk->stak) + stk->numpt;
	}

	while( op != end) {
		if(ip >= fbend) {
			if(read(ihp->fd, fb, FITSRECL) != FITSRECL)
				error("Trouble reading FITS file %s",
					ihp->fname);
			ip = fb;
		}
		switch(fitsWordLen) {
		case 4:
		    if(ihp->bitpix < 0) {
#if BYTEREVERSED
			union {
				int i;
				float f;
			} pix;

			pix.i = *ip++ << 24;
			pix.i |= (*ip++ & 0xff) << 16;
			pix.i |= (*ip++ & 0xff) << 8;
			pix.i |= *ip++ & 0xff;
			*op  = pix.f;
#else
			*op = *(float *)ip;
			ip += 4;
#endif
		    } else {
#if BYTEREVERSED
			pixel = *ip++ << 24;
			pixel |= (*ip++ & 0xff) << 16;
			pixel |= (*ip++ & 0xff) << 8;
			pixel |= *ip++ & 0xff;
#else
			pixel = *(int *)ip;
			ip += 4;
#endif
		    }
			break;
		case 2:
#if BYTEREVERSED
			pixel = *ip++ << 8;
			pixel |= *ip++ & 0xff;
#else
			pixel = *(short *)ip;
			ip += 2;
#endif
			break;
		case 1:
			pixel = *(unsigned char *)ip++;
			break;
		}
		if(ihp->bitpix > 0)
		    if(pixel == ihp->blank)
			*op = FDRAGON;
		    else
			*op = pixel * ihp->bscale + ihp->fbzero;
		op += dir;
	}
	if(ihp->ftype & TAPE) {
		while(read(ihp->fd, fb, FITSRECL) > 0)
			; 
	}
	SafeFree(fb);
}
