# include "../../stacks/C.h"
# include "../../image/C.h"
# include "../../main/C.h"
# include "../../main/dragon.h"
# include <stdio.h>
# include <sys/file.h>
# include <sys/types.h>
#if 0
# include <sys/dir.h>
#endif
# include <math.h>
# define USELEN sizeof(use_.nuse)

/* last change: Fri Jan 20 09:00:00 EST 1995, IRAS stuff -- MWP 
 * Feb 1995 -- SubImage() stuff -- mwp 
 */

void im()
{
	extern int JustPrinting; /* a flag for NextCrd() in ../../src/image.c */
	extern double taupow;    /* power law dependence of opacity : image.c */
	struct of_imagehdr *imh;
	double xmin,xmax,ymin,ymax; /* extent of subimage, plot units */

# include "im.dc.h"
# include "im.ec.h"

	switch(*op) {
	case 0:
		LinkImage(*src, *dest);
		break;
	case 1:
		ImageOp(*im1,*im2,*im3,(int) ADD, *mina, *minb);
		SetDataScale(*im3);
        /* add history label reflecting image op. Note that comb will add 
         * "HISTORY COMB IMAGEOP " to this label when writing out to FITS file.
	 */
		imh = imageHdr[*im3];
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
		   sprintf(hl,"im add:%d,%d,%d mina:%.3g minb:%.3g",*im1,*im2,*im3,*mina,*minb);
		}
		strcpy(imh->imageop,hl);
		break;
	case 2:
		ImageOp(*im1,*im2,*im3,(int) SUB, *mina, *minb);
		SetDataScale(*im3);
                imh = imageHdr[*im3]; 
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                   sprintf(hl,"im sub:%d,%d,%d mina:%.3g minb:%.3g",*im1,*im2,*im3,*mina,*minb);
                }
                strcpy(imh->imageop,hl);
		break;
	case 3:
		ImageOp(*im1,*im2,*im3,(int) DIV, *mina, *minb);
		SetDataScale(*im3);
                imh = imageHdr[*im3]; 
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                   sprintf(hl,"im div:%d,%d,%d mina:%.3g minb:%.3g",*im1,*im2,*im3,*mina,*minb);
                }
                strcpy(imh->imageop,hl);
		break;
	case 4:
		ImageOp(*im1,*im2,*im3,(int) MUL, *mina, *minb);
		SetDataScale(*im3);
                imh = imageHdr[*im3]; 
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                   sprintf(hl,"im mul:%d,%d,%d mina:%.3g minb:%.3g",*im1,*im2,*im3,*mina,*minb);
                }
                strcpy(imh->imageop,hl);
		break;
	case 5:
		RescaleImage(*imn,*fact,*set);
		SetDataScale(*imn);
                imh = imageHdr[*imn]; 
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                   sprintf(hl,"im rs:%.3g,%.3g",*fact,*set);
                }
                strcpy(imh->imageop,hl);
		break;
	case 6: /* take a subimage */
		switch(*ssb) {
		 case 0: /* by area */
			ChkArea((int)*an);
			AreaLimits((int)*an,&xmin,&xmax,&ymin,&ymax);
                	if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                           sprintf(hl,"im si:[%d,%d,an:%d] mina:%.3g",
                            *src,*dest,*an,*mina);
                         }
			break;
		 case 1: /* by corners */
			if((*llx == *urx)||(*ury==*lly))
				error_("Requested subimage has no extent.");
			if(*llx < *urx) {
				xmin=*llx;
				xmax=*urx;
			}
			else {
				xmin=*urx;
				xmax=*llx;
			}
			if(*lly < *ury) {
				ymin=*lly;
				ymax=*ury;
			}
			else {
				ymin=*ury;
				ymax=*lly;
			}
                	if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                           sprintf(hl,"im si:[%d,%d,%.2f,%.2f,%.2f,%.2f] \
mina:%.2g", *src,*dest,*llx,*urx,*lly, *ury,*mina);
                         }
			break; 
		 default:
			error_("Internal Error: bad case");
			break;
		}
		SubImage(*src,*dest,xmin,xmax,ymin,ymax,*mina);
                imh = imageHdr[*dest]; 
                strcpy(imh->imageop,hl);
		break;
	case 7: /* IRAS temperature */
		taupow = *tpow;
		ImageOp(*im1,*im2,*im3,(int) TEMP, *mina, *minb);
		SetDataScale(*im3);
                imh = imageHdr[*im3]; 
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                   sprintf(hl,"im temp:%d,%d,%d pow:%.3f mina:%.3g minb:%.3g",*im1,*im2,*im3,*tpow,*mina,*minb);
                }
                strcpy(imh->imageop,hl);
		break;
	case 8: /* IRAS color correction*/
		taupow = *tpow;
		ImageOp(*im1,*im2,*im3,(int) CC, *mina, *minb);
		SetDataScale(*im3);
                imh = imageHdr[*im3]; 
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                   sprintf(hl,"im cc:%d,%d,%d mina:%.3g minb:%.3g",*im1,*im2,*im3,*mina,*minb);
                }
                strcpy(imh->imageop,hl);
		break;
	case 9: /* IRAS opacity */
		taupow = *tpow;
		ImageOp(*im1,*im2,*im3,(int) TAU, *mina, *minb);
		SetDataScale(*im3);
                imh = imageHdr[*im3]; 
                if(*ophf) { /* default HISTORY COMB IMAGEOP label */
                   sprintf(hl,"im tau:%d,%d,%d mina:%.3g minb:%.3g",*im1,*im2,*im3,*mina,*minb);
                }
                strcpy(imh->imageop,hl);
		break;
	case 10:
		ChkImage(*imn);
                imh=imageHdr[*imn];
                if (!strcmp(imh->fname,""))
                     sprintf(imh->fname,"Image %d",*imn);
                printf("%s header:\n",imh->fname);
		JustPrinting=TRUE;
		WriteFitsHdr(*imn);
		JustPrinting=FALSE;
		break;
	case 11:
	/* add user history label. Comb will add "HISTORY COMB USERHIST "
         * to this when writing fits file.
	 */
		ChkImage(*imn);
		if(!*userhf) { /* use ! because default label is empty */
                   imh = imageHdr[*imn]; 
                   strcpy(imh->userhist,uh);
		}
		break;
	default:
		error("bad operation: op = %d",op);
		break;
	}
}
