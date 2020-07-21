#include <math.h>
#include <stdio.h>
#include "../tp/nr.h"
#include "../tp/nrutil.h"
#include "../../stacks/C.h"
#include "../../image/C.h"
#include "../../parse/C.h"

#define MA 4
#define max(a,b) (((a) > (b))?(a):(b))

struct of_imagehdr *gaussian_ihp;

void image_gaussian_func(float xind,float a[],float *z,float dyda[],int na) {
  float x,y;
  float exponent;
  int xind_int = round(xind);

  x = ((xind_int%(gaussian_ihp->naxis1))-(round(gaussian_ihp->crpix1)-1))*fabs(gaussian_ihp->cdelt1)*60;
  y = ((xind_int/(gaussian_ihp->naxis1))-(round(gaussian_ihp->crpix2)-1))*fabs(gaussian_ihp->cdelt2)*60;

  exponent=(pow((x-a[3]),2)+pow((y-a[4]),2));
  *z = exp(-exponent/a[2]);
  dyda[1] = *z;
  *z *= a[1];
  dyda[2] = (exponent)*(*z)/pow(a[2],2);
  dyda[3] = 2.0/a[2]*(x-a[3])*(*z);
  dyda[4] = 2.0/a[2]*(y-a[4])*(*z);
  return;
}

void image_gaussian_reduce(struct of_imagehdr *passed_ihp) {
  int i,itst,*lista,ma,mfit,num_pts;
  float alamda,chisq,ochisq,*x,*y,*sig,**covar,**alpha;
  static float a[MA+1]= {0.0,0.0,0.0025,0.0,0.0};
  
  gaussian_ihp=passed_ihp;
  num_pts=gaussian_ihp->naxis1*gaussian_ihp->naxis2;

  lista=ivector(1,MA);
  x=vector(1,num_pts);
  y=vector(1,num_pts);
  sig=vector(1,num_pts);
  covar=matrix(1,MA,1,MA);
  alpha=matrix(1,MA,1,MA);

  a[1]=0.0;
  a[2]=3.0;
  a[3]=0.0;
  a[4]=0.0;

  for (i=1;i<=num_pts;i++) {
    x[i] = i-1;
    y[i] = gaussian_ihp->buf[i-1];
    a[1] = max(a[1],y[i]);
    sig[i] = 1;
    /*
    printf("%4d %8.5f %8.5f %8.5f\n",
	   i-1,
	   (((i-1)%(gaussian_ihp->naxis1))-(round(gaussian_ihp->crpix1)-1))*fabs(gaussian_ihp->cdelt1)*60,
           (((i-1)/(gaussian_ihp->naxis1))-(round(gaussian_ihp->crpix2)-1))*fabs(gaussian_ihp->cdelt2)*60,
	   gaussian_ihp->buf[i-1]);
    */
  }

  ma = MA;

  for (i=1;i<=MA;i++) lista[i]=i;
  /* lista[1]=1;
  // lista[2]=3;
  // lista[3]=4;
  // lista[4]=2;
  */
  mfit=4;
  alamda = -1;
  mrqmin(x,y,sig,num_pts,a,ma,lista,mfit,covar,alpha,&chisq,image_gaussian_func,&alamda);
  /* k=1; */
  itst=0;
  while (itst < 20) {
    /*
    printf("\n%s %2d %17s %10.4f %10s %9.2e\n","Iteration #",k,
	   "chi-squared:",chisq,"alamda:",alamda);
    printf("%8s %8s %8s %8s\n",
	   "a[1]","a[2]","a[3]","a[4]");
    for (i=1;i<=MA;i++) printf("%9.4f",a[i]);
    printf("\n");
    
    k++;
    */
    ochisq=chisq;
    mrqmin(x,y,sig,num_pts,a,ma,lista,mfit,covar,alpha,&chisq,image_gaussian_func,&alamda);
    if (chisq > ochisq)
      itst=0;
    else if (fabs(ochisq-chisq) < 100)
      itst++;
    /*    if (alamda < 1e-30)
      itst=9999999;
    */
  }
  alamda=0.0;
  mrqmin(x,y,sig,num_pts,a,ma,lista,mfit,covar,alpha,&chisq,image_gaussian_func,&alamda);



  printf("Results:\n");
  printf("Peak Intensity   (K km/s): %10.5f +- %10.5f\n",
	 a[1],
	 sqrt(covar[1][1]));
  printf("FWHM             (arcmin): %10.5f +- %10.5f\n",
	 sqrt(a[2]),
	 sqrt(covar[2][2]));
  printf("Peak Dra  offset (arcmin): %10.5f +- %10.5f\n",
	 a[3],
	 sqrt(covar[3][3]));
  printf("Peak DDec offset (arcmin): %10.5f +- %10.5f\n",
	 a[4],
	 sqrt(covar[4][4]));
    
  stk_[0].fwhm=sqrt(a[2]);
  stk_[0].fwerr=sqrt(covar[2][2]);
  stk_[0].fpeak=a[1];
  stk_[0].fperr=sqrt(covar[1][1]);
  stk_[0].chisq=chisq;
  xc[0]=a[3];
  yc[0]=a[4];

  free_matrix(alpha,1,MA,1,MA);
  free_matrix(covar,1,MA,1,MA);
  free_vector(sig,1,num_pts);
  free_vector(y,1,num_pts);
  free_vector(x,1,num_pts);
  free_ivector(lista,1,MA);
  return;
}

