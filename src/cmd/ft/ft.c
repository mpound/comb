/* ft.c --  new version, A. Stark 4 Nov 1998
*/


#include <math.h>
#include "../../stacks/C.h"
#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"
#include "../../error/C.h"



#define SEE 0
#define IFT 1
#define RM  2



#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* ft.c */
double *dvector P_((int nl, int nh));
void free_dvector P_((double *v, int nl, int nh));
void dfour1 P_((double data[],int nn,int isign));
#undef P_


void ft()
{
#include "ft.dc.h"

    /* Local variables */
    int i, j, k;
    int nbeg, nen, npts, nftm;
    int imax;
    double *databuf;
    double tempmax, temp;
    double rtot = 0.125/atan(1.);

    /* Variables for handling weights array */
    float totwtc, tempwtc[15];
    int ichanc;
    short itmplcc[15], nwtsc;

    *nchanf = 0;
    *wid = 0;

#include "ft.ec.h"


    if (coreFull[0] == 0) {
	error_("No data in stack 1");
    }


    switch (*iacton) {

    case SEE: 

	plchns_(&nbeg, &nen);
	if ((npts = nen - nbeg + 1) < 2) error_("data array too small");
	if (npts > MAXCHN) error_("data array too big");

	/* nftm is the number of complex numbers in the FFT, must be 2**N, and bigger than npts */
	for (nftm = 2; nftm < npts; nftm *= 2);

	databuf = dvector(1,2*nftm); 

	if (stk_[0].nprc >= 100) error_("stack 1 must be normal data");
	if (coreFull[1] != 0) error_("stack 2 must be empty");

	/* In the transformed and retransformed spectrum, channels will be shifted
	left by nbeg-1 channels, and new spectrum will be padded on right with zeros
        to a total length of nftm */ 
	stk_[0].expch -= nbeg - 1;
	if (fabs((double)stk_[0].refch - (double) NOTFS) < 0.01 ) stk_[0].refch -= nbeg - 1;

	/* initialize weights variables as required by pkwts_() */
	nwtsc = 0;
	for (i=0; i<15; i++) {
		tempwtc[i]=0.0;
		itmplcc[i]=0;
	}
	/* repack weights for re-transformed spectrum, added zeros have zero weight */
	k = 1;
	for (i=nbeg, j=1; i <= nen; i++,j++) {
		totwtc = wt_(&i, &k);
		pkwts_(&totwtc, &j, tempwtc, itmplcc, &nwtsc);
	}
	totwtc = 0.0;
	for ( ; j <= nftm; j++) pkwts_(&totwtc, &j, tempwtc, itmplcc, &nwtsc);
	for (i=0; i<15; i++) {
		stk_[0].wght[i] = tempwtc[i];
		stk_[0].locwt[i] = itmplcc[i];
	}
	stk_[0].nwts = nwtsc;

	stk_[0].numpt = nftm/2+1;

	/* Copy stack #1 into stack #2. */
	stk_[1] = stk_[0];
	coreFull[1] = 1;
	stk_[1].numst = 2;
	/* Label data type as Fourier transform */
	stk_[0].nprc += 100;
	stk_[1].nprc += 200;

	for (i=nbeg, j=1; i<=nen; i++,j++) {
		databuf[2*j-1]=stk_[0].stak[i-1];
		databuf[2*j]=0.0;
	}
	for ( ; j<=nftm; j++) {
		databuf[2*j-1]=0.0;
		databuf[2*j]=0.0;
	}

	dfour1(databuf,nftm,1);

	/* Put amplitude in stack 1 and phase in stack 2.  Phase is in turns, and
		turns are added as needed to minimize discontinuities */
	j = 0;
	temp = sqrt((double)nftm);
	stk_[0].stak[0]=sqrt(databuf[1]*databuf[1])/temp;
	stk_[1].stak[0]=rtot*atan2(databuf[2],databuf[1]);
	for (i=2; i<=nftm/2+1; i++) {
		stk_[0].stak[i-1]=sqrt(databuf[2*i-1]*databuf[2*i-1]+databuf[2*i]*databuf[2*i])
					/temp;
		stk_[1].stak[i-1]=rtot*atan2(databuf[2*i],databuf[2*i-1])+(double)j;
		if (stk_[1].stak[i-1] - stk_[1].stak[i-2] > +0.5) {
			stk_[1].stak[i-1] -= 1.0;
			j--;
		} else if (stk_[1].stak[i-1] - stk_[1].stak[i-2] < -0.5) {
			stk_[1].stak[i-1] += 1.0;
			j++;
		}
	}
	
	free_dvector(databuf,1,2*nftm);

	break;

    case IFT:

	if (coreFull[1]==0) error_("No data in stack 2");

	nftm = 2*(stk_[0].numpt-1);

	databuf = dvector(1,2*nftm); 

	databuf[1] = stk_[0].stak[0]*cos(stk_[1].stak[0]/rtot);
	databuf[2] = 0.0;
	databuf[nftm+1] = stk_[0].stak[nftm/2+1]*cos(stk_[1].stak[nftm/2+1]/rtot);
	databuf[nftm+2] = 0.0;
	for (i=1; i<=nftm/2-1; i++) {
		databuf[2*i+1] = stk_[0].stak[i]*cos(stk_[1].stak[i]/rtot);
		databuf[2*nftm-2*i+1] = databuf[2*i+1];
		databuf[2*i+2] = stk_[0].stak[i]*sin(stk_[1].stak[i]/rtot);
		databuf[2*nftm-2*i+2] = - databuf[2*i+2];
	}
	dfour1(databuf,nftm,-1);
	temp = sqrt((double)nftm);
	for (i=1; i<=nftm; i++) stk_[0].stak[i-1]= databuf[2*i-1]/temp;
	stk_[0].numpt = nftm;
	stk_[0].nprc -= 100;

	free_dvector(databuf,1,2*nftm);

	break;

    case RM:

	plchns_(&nbeg, &nen);
	if ((npts = nen - nbeg + 1) < 2) error_("data array too small");
	if (npts > MAXCHN) error_("data array too big");

	/* nftm is the number of complex numbers in the FFT, must be 2**N, and bigger than npts */
	for (nftm = 2; nftm < npts; nftm *= 2);
	if (*wid > nftm/2) error_("too many Fourier components to ignore");
	if (*nchanf >= nftm/2-*wid) error_("too many Fourier components to remove");

	databuf = dvector(1,2*nftm); 

	if (stk_[0].nprc >= 100) error_("stack 1 must be normal data");

	/* In the transformed and retransformed spectrum, channels will not be shifted */

	for (i=nbeg, j=1; i<=nen; i++,j++) {
		databuf[2*j-1]=stk_[0].stak[i-1];
		databuf[2*j] = 0.0;
	}
	for ( ; j<=nftm; j++) {
		databuf[2*j-1] = 0.0;
		databuf[2*j] = 0.0;
	}

	dfour1(databuf,nftm,1);
	
	for (j = *nchanf; j > 0; j--) {
		tempmax  = -1.0;
		for (i = *wid+1; i <= nftm/2+1; i++) {
			temp = databuf[2*i-1]*databuf[2*i-1]+databuf[2*i]*databuf[2*i];
			if (temp > tempmax) {
				tempmax = temp;
				imax = i;
			}
		}
		databuf[2*imax-1] = 0.0;
		databuf[2*imax] = 0.0;
		if (imax > 1 && imax < nftm/2+1) {
		databuf[2*nftm-2*imax+3] = 0.0;
		databuf[2*nftm-2*imax+4] = 0.0;
		}
	}
	/* increment stack header .ftrm by the number of channels filtered out */
	stk_[0].ftrm += (short) *nchanf;

	dfour1(databuf,nftm,-1);

	for (i=nbeg, j=1; i<=nen; i++, j++) stk_[0].stak[i-1]= databuf[2*j-1]/(double)nftm;

	free_dvector(databuf,1,2*nftm);

	break;

    default:
	error_("Unknown action in ft");

    }

    main_.pltd = 0;
} /* ft_ */


double *dvector(nl,nh)  /* Numerical Recipies routines modified for COMB error handling */
int nl,nh;
{
	double *v;
	char *SafeAlloc();

	v=(double *)SafeAlloc((unsigned) (nh-nl+1)*sizeof(double),"dvector");
	if (!v) printf("allocation failure in dvector()");
	return v-nl;
}

void free_dvector(v,nl,nh)
double *v;
int nl,nh;
{
	void SafeFree();
	SafeFree((char*) (v+nl));
}



#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

/* From Numerical Recipies, modified for double precision */
void dfour1(data,nn,isign)
double data[];
int nn,isign;
{
	int n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	double tempr,tempi;
	double twopi;

	twopi = 8.0*atan(1.);
	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) {
		if (j > i) {
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m=n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax=2;
	while (n > mmax) {
		istep=2*mmax;
		theta=twopi/(isign*mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}

#undef SWAP

