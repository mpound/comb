#include <stdio.h>
#include <string.h>
#include "../../main/C.h"
#include "../../scan/C.h"
#include "../../stacks/C.h"
#include "../../graphics/C.h"
#include "../../graphics/include.h"
#include "../../error/C.h"
#include "../../misc/C.h"
#define NAMFLG 0
/* Fri Jun 11 12:42:13 EDT 1999
 * MWP added voff: option
 */

static double d0 = 0.,d60 = 60.;
static int n1 = 1;
static double hfrom,hto,vfrom,vto;
/* static double xStep,yStep; */
static char *horizontalLabel[5] = {
	"Channels",
	"Frequency(Mhz)",
	"Velocity (km/s)",
	"Stack 2 Antenna Temperature (K)",
	"Phase (turns)"
};


/* pl.c*/

static void pdata();
static void pluse(/*fch,tch*/);

void pl()
{
#include "pl.dc.h"
	static int svhtype = 3, grmode = 0;
	int htype,offset;
	int itemp, i;

#include "pl.ec.h"
/*
printf("vff %d, vf %g, hff %d, hf %g\n",*vff,*vf,*hff,*hf);
printf("vf bytes %d, %d, %d, %d\n",*(char *)vf,((char *)vf)[1],((char *)vf)[2],((char *)vf)[3]);
*/
	MkOpt(CLEAROPT,ENDOPT);
	if(!StkIsFull(1))
		error_("No data in stack 1");
	*lnt &= 255;
	if(*ovl && !cPic)
		error_("No plot on which to overlay");



/**** Get new htype */
	if(*chngu) {			/* New units not specified */
		if(*ovl) {
			htype = (cPic)->htype;	/* Get from old picture */
			if(htype <= 0 || htype > 4)
			    error_("Unsuitable picture for overlay");
		} else {
			/* Previous non-agst htype */
			htype = (main_.htype < 4)? main_.htype: svhtype;
		}
	} else {
		htype = *unit + 1;
	}
	if(htype != 4) {
		svhtype = htype;		/* Save non-agst type */

/**** If plotting against stack 2 get the offset to add to the stack 1
***** channel numbers to get the corresponding stack 2 numbers.  Get the
***** range of stack 1 channel numbers to plot */

	} else {			/* Plot against stack 2 */
		if( !StkIsFull(2))
			error_("No data in stack 2");
		offset = round(stk_[1].expch - stk_[0].expch);
		if(*fchf) {
			*fch = 1 + offset;
			if(*fch < 1)
				*fch = 1;
		}
		if(*tchf) {
			*tch = stk_[1].numpt + offset;
			if(*tch > stk_[0].numpt)
				*tch = stk_[0].numpt;
		}
		if(*fch < 1 || *fch + offset < 1 ||
		   *fch > stk_[0].numpt || *fch + offset > stk_[1].numpt)
			error_("First channel out of range");
		if(*tch < 1 || *tch + offset < 1 ||
		   *tch > stk_[0].numpt || *tch + offset > stk_[1].numpt)
			error_("Last channel out of range");
	}

/**** For an overlay, get the plot's horizontal and vertical ranges */

	if(*ovl) {
		hfrom = (cPic)->left;
		hto = (cPic)->right;
		if(*htp == 0 && htype != 4) {	/* New Horizontal */
			if(*hff == 0 && *hf > hfrom)	/* This changes the */
				hfrom = *hf;	/* # of chans plotted */
			if(*htf == 0 && *ht < hto)
				hto = *ht;
		}
		vfrom = (cPic)->bottom;
		vto = (cPic)->top;
		if(*vff == 0 || *vtf == 0)
			error_("Can't change vertical range on overlay");
	} else {

/**** For a non-overlay  stack1 against stack 2 plot 'agst:' get horiz range */

		if(htype == 4) {
		    if(*htp < 2) {		/* New horiz or (silly) fhr: */
			if(main_.htype != htype && *hff ^ *htf)
				error_("Now I need both hf and ht or neither");
			if(! *hff || ! *htf) {
			    if(! *hff)
				hfrom = *hf;
			    if(! *htf)
				hto = *ht;
			    MkOpt(XRANGE(hfrom,hto),ENDOPT);
			} else {
			    MkOpt(fXLIST(&stk_[1].stak[*fch - 1 + offset],
				*tch + 1 - *fch), ENDOPT);
			}
		    } else {			/* Old horiz */
		        if(main_.htype != htype)
			    error_("Can't use 'oh:' when previous plot is different");
			    MkOpt(XRANGE(hfrom,hto),ENDOPT);
		    }
		} else {

/**** Get horiz range for non-overlay, non-'agst:' plot */

		    if(*htp < 2) {			/* New horiz or 'fhr:' */
			if(main_.htype == 4 || *htp == 1)/* If unsuitable prev plot or*/
			    main_.pltd = 0;	/* 'fhr:' don't use old horiz values */
			if(*hff) {
				if(main_.pltd)
				    hfrom = tran_( &main_.htype,&htype,&hfrom);
				else
				    hfrom = tran_( &n1,&htype,&d0);
			} else {
				hfrom = *hf;
			}
			if(*htf) {
				if(main_.pltd) {
				    hto = tran_( &main_.htype,&htype,&hto);
				} else {
				    hto = (double)stk_[0].numpt;
				    hto = tran_( &n1,&htype,&hto);
				}
			} else {
				hto = *ht;
			}
			if( *hff && *htf & hfrom > hto) {
				offset = hfrom;
				hfrom = hto;
				hto = offset;
			}
		    } else {			/* Old horizontal */
			if(htype != main_.htype) {
			    if( main_.htype != 4) {
				hfrom = tran_( &main_.htype,&htype,&hfrom);
				hto = tran_( &main_.htype,&htype,&hto);
			    } else {
				error_("No suitable old horizontal range");
			    }
			}
		    }
		    MkOpt(XRANGE(hfrom,hto),ENDOPT);
		}
	}

/**** if not 'agst' determine the range of chan numbers to plot */

	if(htype != 4) {
		*fch = tran_(&htype,&n1,&hfrom);
		*tch = tran_(&htype,&n1,&hto);
		if(*fch > *tch) {
			offset = *fch;
			*fch = *tch;
			*tch = offset;
		}
		if(*fch <= 0) {
			*fch = 1;
			if(*tch <= 0)
				*tch = 1;
		}
		if(*tch > stk_[0].numpt) {
			*tch = stk_[0].numpt;
			if(*fch > stk_[0].numpt)
				*fch = *tch;
		}
	}

/**** For a non-overlay, get the plot's vertical range */

	if(!*ovl) {
		Pstart();
		Pformat("comb");
		if(! *vtp ) {			/* New vertical */
		    if( !*vff || !*vtf) {		/* either vf or vt given */
			if( !*vff)
			    vfrom = *vf;
			if( !*vtf)
			    vto = *vt;
			MkOpt(YRANGE(vfrom,vto),ENDOPT);
		    } else {
			MkOpt(fYLIST(&stk_[0].stak[*fch - 1],
				*tch + 1 - *fch),ENDOPT);
		    }
		} else {	
		    MkOpt(YRANGE(vfrom,vto),ENDOPT);
		}
		if(htype != 4)
			MkOpt(YNOISE(stk_[0].rms),ENDOPT);
		MkScale(ENDOPT);

/**** for non overlay determine the options and plot the grid */

#if 0
		if(!*hsf)
			MkOpt(XSTEP(*hs),ENDOPT);
		else if(*htp == 2)
			MkOpt(XSTEP(xStep),ENDOPT);
		if(!*vsf)
			MkOpt(YSTEP(*vs),ENDOPT);
		else if(*vtp == 1)
			MkOpt(YSTEP(yStep),ENDOPT);
#endif /*0*/
		if(!*tic)
			MkOpt(LINES,ENDOPT);
		if(!*gr)
			MkOpt(NO GRID,ENDOPT);
		if(!*lbs)
			MkOpt(NO LABELX,NO LABELY,ENDOPT);
		if(!*fly0)
			MkOpt(NO FLAGY0,ENDOPT);
		Ptexttype(1);
		MkGrid(KEYY,NO FLAGX0,
		/* 	STEPSTOR(&xStep,&yStep), */
			ENDOPT);
		
	}	
	if(htype == 4) {
		MkOpt(fXLIST(&stk_[1].stak[*fch - 1 + offset],
			*tch + 1 - *fch), ENDOPT);
	} else {

/**** For non-'agst', determine the actual range of horizontal plot units
***** covered by the the channels to be plotted so they will be plotted
***** in the correct place */

		hfrom = (double) *fch;
		hfrom = tran_(&n1,&htype,&hfrom);
		hto = (double) *tch;
		hto = tran_(&n1,&htype,&hto);
		MkOpt(XRANGE(hfrom,hto),ENDOPT);
	}

/**** save the h and v ranges of this plot in case of future oh: */

	hfrom = (cPic)->left;
	hto = (cPic)->right;
	vfrom = (cPic)->bottom;
	vto = (cPic)->top;
	(cPic)->htype = htype;

/**** set up the proper type of plot - line, hist, or figure */

	if(!*modef)
		grmode = *mode;
	if(grmode == 1)
		MkOpt(HIST,ENDOPT);
	else if(grmode == 2) {
		MkOpt(FIGURE(*mrkn),FHEIGHT(stk_[0].rms),ENDOPT);
		if(htype == 4)
			MkOpt(FWIDTH(stk_[0].rms),ENDOPT);
		else
			MkOpt(FWIDTH(rtran_(&n1,&htype)),ENDOPT);
	}
	if(*fill)
	       MkOpt(FILLED,ENDOPT);

/****  add any user-given vertical offset from voff: flag      */
/****  note this is done after the vertical range is decided   */ 
/****  otherwise, there isnt much sense to this option, which  */
/****  is for plotting multiple spectra on a single plot, with */ 
/****  each on vertically offset from the last                 */

	for(i=0;i<stk_[0].numpt;i++) {
		stk_[0].stak[i]+=*voff;
	}

	MkGraph(fYLIST(&stk_[0].stak[*fch - 1],*tch + 1 - *fch),
		LTYPE(*lnt),ENDOPT);
	main_.pltd = 1;
	main_.htype = htype;
	if(*usar && !*ovl && htype != 4) {
		pluse((int)*fch,(int)*tch);
	}
	if(!*mlbf)
		Pplace(0,mlb);
	if(!*ovl) {
		if(*hlbf)
			strcpy(hlb,horizontalLabel[htype - 1]);
		Pplace(1,hlb);
		if(*vlbf) {
			if (stk_[0].nprc < 200 || stk_[0].nprc > 299) {
				strcpy(vlb,horizontalLabel[3] + 8);
			} else {
				strcpy(vlb,horizontalLabel[4]);
			}
		}
		Pplace(2,vlb);
		if(*hd)
			pdata();
	}
	
	if(*ovl)
		Pupdate();
	else
		Pshow(bxCol,bxRow);

/****  now, remove any user-given vertical offset */

	for(i=0;i<stk_[0].numpt;i++) {
		stk_[0].stak[i]-=*voff;
	}

#include "pl.uc.h"
}

void fl()
{
	double hvalue,t,b;
	int tunit;
	char str[24];
# include "fl.dc.h"

# include "fl.ec.h"
	if(!main_.pltd)
		error_("No current plot to flag");
	if(*unit) {
		tunit = *unit;
		hvalue = tran_(&tunit,&main_.htype,loc);
	} else {
		hvalue = *loc;
	}
	if((cPic)->left < (cPic)->right) {
	    if(hvalue < (cPic)->left || hvalue > (cPic)->right)
		return;
	} else{
	    if(hvalue > (cPic)->left || hvalue < (cPic)->right)
		return;
	}
	Pwindow(1);
	Plinetype((int)*lnt);
	Pmove(hvalue,b = (cPic)->bottom);
	Pline(hvalue,t = (cPic)->top);
	Pmove(hvalue,.95*t + .05 * b);
	sprintf(str,(*unit == 2)?"%.8g":"%.5g",*loc);
	Pctext(str);
	Pupdate();


#include "fl.uc.h"
}

static void pdata()
{
	int i;
	char ts[240],*s;
	double wt_();

	sprintf(ts,"Ra(%6.1f)",stk_[0].epoch);
	HmsPrint(ts,11,stk_[0].ra + stk_[0].dra);
	Pplace(3,ts);
	sprintf(ts,"Dec(%6.1f)",stk_[0].epoch);
	HmsPrint(ts,0,stk_[0].dec + stk_[0].ddec);
	Pplace(4,ts);
	sprintf(ts,"l%9.3f b%8.3f",stk_[0].l + stk_[0].dl,
		stk_[0].b + stk_[0].db);
	Pplace(5,ts);
	sprintf(ts,"Cntr Ra   ");
	HmsPrint(ts,11,stk_[0].ra);
	Pplace(6,ts);
	sprintf(ts,"Cntr Dec   ");
	HmsPrint(ts,0,stk_[0].dec);
	Pplace(7,ts);
	sprintf(ts,"Obs Off Ra%10.2f",stk_[0].odra);
	Pplace(9,ts);
	sprintf(ts,"Obs Off Dec%9.2f",stk_[0].oddec);
	Pplace(10,ts);
	sprintf(ts,"Int Time   ");
	HmsPrint(ts,0,stk_[0].time/3600.);
	Pplace(11,ts);
	sprintf(ts,"Cntr freq%11.10g",stk_[0].freq);
	Pplace(12,ts);
	sprintf(ts,"Vlsr%16.6g",stk_[0].vlsr);
	Pplace(13,ts);
	sprintf(ts,"Filt wid%12.6g",stk_[0].fwid);
	Pplace(14,ts);
	sprintf(ts,"Cntr chan%11.6g",stk_[0].expch);
	Pplace(15,ts);
	if(stk_[0].refch == -9999) {
		Pplace(16,"Ref chan        none");
	} else {
		sprintf(ts,"Ref chan%12.6g",stk_[0].refch);
		Pplace(16,ts);
	}
	i = round(stk_[0].expch);	/* fortran calling conventions */
	sprintf(ts,"Cntr weight %8.3g",wt_(&i,&n1));
	Pplace(17,ts);
	if (stk_[0].ftrm) {
		if(stk_[0].ibsln < 0) {
			sprintf(ts,"Fourier comp rm%5d",stk_[0].ftrm);
		} else {
			sprintf(ts,"Bsln %3d  Ft rm%5d",stk_[0].ibsln,stk_[0].ftrm);
		}
	} else {if(stk_[0].ibsln < 0) {
			sprintf(ts,"Bsln order      none");
		} else {
			sprintf(ts,"Bsln order%10d",stk_[0].ibsln);
		}
	}
	Pplace(18,ts);
	sprintf(ts,"Rescale%13.11g",stk_[0].factt);
	Pplace(19,ts);
	sprintf(ts,"Stack number%8d",stk_[0].numst);
	Pplace(20,ts);

	scanStr(ts, sizeof(ts), 0);
	Pplace(21,ts);
	/* put the date string as determined from j2second.
	 * If j2second is 0, it means the stacks were created
         * before j2second was implemented (16-Dec-1999), so
	 * don't make the string.  This assumes no one is going
	 * to be taking data at 2000-01-01 UT00:00:00 !
	 * mwp
 	 */
	if(stk_[0].j2second!=0) 
		Pplace(29,j2sToDate(stk_[0].j2second));

/*	pscans(); */

	if(stk_[0].nlist == 1) {
		if((i = stk_[0].nstk[0]) < NAMFLG)
			i = stk_[0].nstk[2];
	/*	if(i != *scan_.ndat || i == 0) */
		if(i != curscn_.num || i == 0)
			return;
		switch(scan_.iobstp) {
		case 1:
			s = "FREQUENCY SWITCHED"; break;
		case 2:
			s = "POSITION SWITCHED"; break;
		case 3:
			s = "POSITION + FREQUENCY SWITCHED"; break;
		case 4:
			s = "BEAM SWITCHED"; break;
		case 5:
			s = "BEAM POSITION SWITCHED"; break;
		case 6:
			s = "LOAD SWITCHED"; break;
		case 8:
			s = "TOTAL POWER ON";break;
		case 9:
			s = "TOTAL POWER OFF";break;
		case 10:
			s = "CAL"; break;
		case 11:
			s = "GAINS";break;
		case 12:
			s = "SKY";break;
		case 13:
			s = "COMB CALIBRATION";break;
		case 20:
			s = "AZIMUTH SCAN";break;
		case 21:
			s = "ELEVATION SCAN";break;
		case 22:
			s = "DRIFT SCAN";break;
		case 30:
			s = "CONTINUUM INT";break;
		default:
			s = "UNKNOWN OBS TYPE"; break;
		}
		Pplace(22,s);
		sprintf(ts,"System temp.%8.5g",scan_.tsys);
		Pplace(23,ts);
		if(scan_.tsky != -9999.) /* a flag from getNRAO() */
		  sprintf(ts,"Sky temp.%11.8g",scan_.tsky);
	        else 
		  sprintf(ts,"Subscan%13d",cSubscan);
		Pplace(24,ts);
		if(scan_.azimuth < 0) {
		    if(scan_.ndat[22] > 0) {
			sprintf(ts,"Air mass%12.8g",16384. /
				(double)scan_.ndat[22]);
			Pplace(25,ts);
		    }
		} else {
		    sprintf(ts, "Az %7.3f El %6.3f", scan_.azimuth,
			scan_.elevation);
		    Pplace(25,ts);
		}
		sprintf(ts,"Scaling%13.10g",scan_.fmult);
		Pplace(26,ts);
                sprintf(ts,"Object %s",ObjectName);
                Pplace(28,ts);
	}
}

#if 0
hms(p,typ,v)
char *p;		/* pointer to string to concatenate results with */
int typ;		/* 0 for hh:mm:ss.s and 1 for +-dd:mm:ss */
double v;		/* value */
{
	register int h,m;
	static char *fmt[2] = { "%2d:%02d:%03.1f","%2d:%02d:%02.0f" };

	p += strlen(p);
	if(v < 0. ) {
		*p++ = '-';
		v = -v;
	} else if(typ) {
		*p++ = ' ';
	}
	v = (v - (h = (int)v)) * d60;
	v = (v - (m = (int)v)) * d60;
	sprintf(p,fmt[typ],h,m,v);
}

static pscans()
{
	char ts[350];
	register short *p,*pend;	/* scan number pointer, limit pointer */
	FILE df;	/* dummy file structure to trick fscanf into storing
			the results of many calls in one long string */

	df._flag = _IOWRT + _IOSTRG;
	df._ptr = (unsigned char *)ts;
	df._cnt = BUFSIZ + sizeof(ts);	/* There would be chaos if this cnt
					ever reached zero */
	p = stk_[0].nstk;
	if(stk_[0].indx < 32)
		pend = &stk_[0].nstk[stk_[0].indx];
	else
		pend = &stk_[0].nstkx[stk_[0].indx - 32];
	while(p < pend) {
		if(*p < NAMFLG) {
			if(df._cnt < BUFSIZ + 4) {
				*df._ptr++ = '*';
				break;
			}
# if BYTEREVERSED
			*df._ptr++ = *(char *)p;
# else
			*df._ptr++ = ( (char *)p)[1];
# endif /*BYTEREVERSED*/
			if(++p == &stk_[0].nstk[32])
				p = stk_[0].nstkx;
			*df._ptr++ = *( (char *)p);
			*df._ptr++ = *( (char *)(p++) + 1);
			*df._ptr++ = ' ';
			df._cnt -= 4;
		} else {
			if(df._cnt < BUFSIZ + 6) {
				*df._ptr++ = '*';
				break;
			}
			fprintf(&df,"%d ",*p++);
		}
		if(p == &stk_[0].nstk[32])
			p = stk_[0].nstkx;
	}
	*df._ptr = '\0';
	Pplace(21,ts);
}
# endif /*0*/

static void pluse(fch,tch)
int fch;		/* from channel */
register int tch;	/* to channel */
{
	int ch;					/* current channel */
	register int curbit = -1,newbit;
	int ibit_();
	double dchan;

	Pwindow(2);
	Plinetype(GRID0lt);
	for(ch = fch;ch <= tch;ch++) {
		if((newbit = ibit_(use_.nuse,&ch)) != curbit) {
			dchan = ch - 0.5;
			if(curbit == 1) {
				Pline(tran_(&n1,&main_.htype,&dchan),d0);
			} else {
				Pmove(tran_(&n1,&main_.htype,&dchan),d0);
			}
			curbit = newbit;
		}
	}
	if(curbit == 1) {
		dchan = tch - 0.5;
		Pline(tran_(&n1,&main_.htype,&dchan),d0);
	}
	Pwindow(1);
}

plchns_(from,to)
int *from,*to;		/* return first,last chans of plot */
{
	if(main_.pltd) {
		pltoch_(&(cPic)->left,&(cPic)->right,from,to);
	} else {
		*from = 1;
		*to = stk_[0].numpt;
	}
}

