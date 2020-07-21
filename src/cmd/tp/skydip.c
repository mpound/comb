#include <math.h>
#include <stdio.h>
#include "nr.h"
#include "nrutil.h"
#include "C.h"

void Clear_skydip(Skydip_data **skydip_last_data_ptr,int *skydip_num_pts) {
  Skydip_data *skydip_curr_data_ptr;
  
  /* We need to reset the skydip
     power array and release the memory associated with it 
     -CLM 5/21/01
  */
  
  while (*skydip_last_data_ptr != NULL) {
    skydip_curr_data_ptr = *skydip_last_data_ptr;
    *skydip_last_data_ptr = (*skydip_last_data_ptr)->prev_ptr;
    free(skydip_curr_data_ptr);
  }
  *skydip_num_pts=0;
  return;
}

void skydip_func(float x,float a[],float *y,float dyda[],int na) {
  *y = a[1] - a[2]*exp(-x*a[3]);
  dyda[1] = 1.0;
  dyda[2] = -exp(-x*a[3]);
  dyda[3] = a[2]*x*exp(-x*a[3]);
  return;
}

#define MA 3

void Reduce_skydip(Skydip_data *skydip_last_data_ptr,int skydip_num_pts,int format) {
  int i,itst,k,*lista,ma;
  float alamda,chisq,ochisq,*x,*y,*sig,**covar,**alpha;
  static float a[MA+1]= {0.0,220.0,150.0,0.7};
  Skydip_data *skydip_curr_data_ptr=skydip_last_data_ptr;
  
  lista=ivector(1,MA);
  x=vector(1,skydip_num_pts);
  y=vector(1,skydip_num_pts);
  sig=vector(1,skydip_num_pts);
  covar=matrix(1,MA,1,MA);
  alpha=matrix(1,MA,1,MA);

  a[1]=220.0;
  a[2]=150.0;

  if (skydip_curr_data_ptr->linefreq > 300000) 
    a[3]=0.7;
  else
    a[3]=0.05;

  for (i=1;i<=skydip_num_pts;i++) {
    x[i] = skydip_curr_data_ptr->airmass;
    y[i] = skydip_curr_data_ptr->totpower;
    sig[i] = skydip_curr_data_ptr->error;
    skydip_curr_data_ptr = skydip_curr_data_ptr->prev_ptr;
  }

  ma = MA;

  for (i=1;i<=MA;i++) lista[i]=i;
  alamda = -1;
  mrqmin(x,y,sig,skydip_num_pts,a,ma,lista,ma,covar,alpha,&chisq,skydip_func,&alamda);
  k=1;
  itst=0;
  while (itst < 2) {
    /*    printf("\n%s %2d %17s %10.4f %10s %9.2e\n","Iteration #",k,
	  "chi-squared:",chisq,"alamda:",alamda);
	  printf("%8s %8s %8s\n",
	  "a[1]","a[2]","a[3]");
	  for (i=1;i<=MA;i++) printf("%9.4f",a[i]);
	  printf("\n");
    */
    k++;
    ochisq=chisq;
    mrqmin(x,y,sig,skydip_num_pts,a,ma,lista,ma,covar,alpha,&chisq,skydip_func,&alamda);
    if (chisq > ochisq)
      itst=0;
    else if (fabs(ochisq-chisq) < 0.1)
      itst++;
  }
  alamda=0.0;
  mrqmin(x,y,sig,skydip_num_pts,a,ma,lista,ma,covar,alpha,&chisq,skydip_func,&alamda);
  /*  printf("\nUncertainties:\n");
  for (i=1;i<=MA;i++) printf("%9.4f",sqrt(covar[i][i]));
  printf("\n");
  */
  if (format) {
    printf("%s%04d %d %f %f %f %f %f %f %f %f %f %f\n",
	   skydip_last_data_ptr->scanfile,
	   skydip_last_data_ptr->scannum,
	   skydip_last_data_ptr->backend,
	   skydip_last_data_ptr->j2seconds/(60*60*24),
	   skydip_last_data_ptr->linefreq,
	   /*	   a[1],sqrt(covar[1][1]), */
	   /*	   a[2],sqrt(covar[2][2]), */
	   a[3],sqrt(covar[3][3]),  /* tau & error in tau */
	   chisq, /* Chi squared */
	   a[1]-a[2],sqrt(covar[1][1]+covar[2][2]), /* T_spill & error */
	/*   a[2]/a[1], */
	/*   (a[2]/a[1])*sqrt((covar[2][2]/(a[2]*a[2]))+
			    (covar[1][1]/(a[1]*a[1]))),  */
	   /*	   skydip_last_data_ptr->TAmbient, */
	   /*	   skydip_last_data_ptr->DTAbs, */
	   skydip_last_data_ptr->TAmbient-skydip_last_data_ptr->DTAbs, /* T_atm */
	   a[2]/(skydip_last_data_ptr->TAmbient-skydip_last_data_ptr->DTAbs), /* eta */
	   (a[1]-a[2])/(1-(a[2]/(skydip_last_data_ptr->TAmbient-
				 skydip_last_data_ptr->DTAbs)))); /* T_sbr   = T_spill/(1-eta) */ 

	   
  } else {
    printf("For the function y = C - B*exp(-X*tau),\n");
    printf(" C       = %8.4f +- %f\n",a[1],sqrt(covar[1][1]));
    printf(" B       = %8.4f +- %f\n",a[2],sqrt(covar[2][2]));
    printf(" tau     = %8.4f +- %f\n",a[3],sqrt(covar[3][3]));
    printf(" chi^2   = %8.4f\n",chisq);
    printf(" T_spill = C-B             = %8.4f +- %8.4f\n",a[1]-a[2],sqrt(covar[1][1]+covar[2][2]));
  /*  printf(" T_atm = C = %f +- %f\n",a[1],sqrt(covar[1][1])); */
  /*  printf(" eta = B/T_atm = %f +- %f\n",a[2]/a[1],
	   (a[2]/a[1])*sqrt((covar[2][2]/(a[2]*a[2]))+
			    (covar[1][1]/(a[1]*a[1])))); */
  /*  printf(" T_sbr = T_spill/(1-eta) = %f\n",(a[1]-a[2])/(1-(a[2]/a[1]))); */
    printf(" T_atm   = T_amb - dTAbs   = %8.4f - %8.4f = %8.4f\n",
	   skydip_last_data_ptr->TAmbient,
	   skydip_last_data_ptr->DTAbs,
	   skydip_last_data_ptr->TAmbient-skydip_last_data_ptr->DTAbs);
    printf(" eta     = B/T_atm         = %8.4f\n",
	   a[2]/(skydip_last_data_ptr->TAmbient-skydip_last_data_ptr->DTAbs));
    printf(" T_sbr   = T_spill/(1-eta) = %8.4f\n",
	   (a[1]-a[2])/(1-(a[2]/(skydip_last_data_ptr->TAmbient-
				 skydip_last_data_ptr->DTAbs))));
  }
  free_matrix(alpha,1,MA,1,MA);
  free_matrix(covar,1,MA,1,MA);
  free_vector(sig,1,skydip_num_pts);
  free_vector(y,1,skydip_num_pts);
  free_vector(x,1,skydip_num_pts);
  free_ivector(lista,1,MA);
  return;
}

