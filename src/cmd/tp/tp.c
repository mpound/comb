#include "C.h"
#include "../../stacks/C.h"
#include "../../coordsys/C.h"
#include "../../stacks/include.h"
#include "../../main/C.h"
#include "../../scan/C.h"
#include <math.h>
#include <malloc.h>
#define NAMFLG 0

extern void Clear_skydip(Skydip_data **skydip_last_data_ptr,int *skydip_num_pts);
extern void Reduce_skydip(Skydip_data *skydip_last_data_ptr,int skydip_num_pts,int format);

void tp()
{
  double sum = 0,totwt = 0,wt,rms;
  register int i;
  extern int ibit_();
  static Skydip_data *skydip_last_data_ptr = NULL;
  static int skydip_num_pts = 0;
  Skydip_data *skydip_curr_data_ptr;
  char tempstr[80];
  
#include "tp.dc.h"
#include "tp.ec.h"
  
  if (*clrf) {
    Clear_skydip(&skydip_last_data_ptr,&skydip_num_pts);
    /* If we've been cleaning up memory, we should stop here and
       return rather than trying to calculate the power in a
       spectrum 
    */
    return;
  }
  
  if (*prtf) {
    skydip_curr_data_ptr = skydip_last_data_ptr;
    if (skydip_curr_data_ptr == NULL)
      error("* No skydip data available to print.");
    printf("*Airmass Power    Error\n");
    while (skydip_curr_data_ptr != NULL) {
      printf("%8.5f %8.5f %8.5f\n",
	     skydip_curr_data_ptr->airmass,
	     skydip_curr_data_ptr->totpower,
	     skydip_curr_data_ptr->error);
      skydip_curr_data_ptr = skydip_curr_data_ptr->prev_ptr;
    }
    
    /* If we've been printing values in memory, we should stop
       here and return rather than trying to calculate the power
       in a spectrum */
    return;
  }
  
  /* If we are either being asked to store a power, or there are 
     no other flags set, we should go through the spectrum in 
     stack 1 and calculate its total power
  */
  if (!(*redf) || (*stf)) {
    
    if(!coreFull[0])
      error("No data in stack 1");
    if(!coreFull[2])
      error("No cal in stack 3");
    if(stk_[0].nprc != stk_[2].nprc || stk_[0].numpt != stk_[2].numpt)
      error("Stacks 1 and 3 not compatible");
    
    for(i = 0;i < stk_[0].numpt;i++) {
      if(ibit(use_.nuse,i) && (wt = stk_[2].stak[i]) > 0.) {
	wt = 1./(wt * wt);
	totwt += wt;
	sum += stk_[0].stak[i] * wt;
      }
    }
    if(totwt > 0) {
      sum /= totwt;
      stk_[0].area = sum;
      rms = sqrt(2./(totwt * stk_[0].fwid * 1000000 * stk_[0].time));
      printf("avg = %g +- %g K\n",sum,rms);
      if (*stf) {
	if(stk_[0].nlist > 1) 
	  error("Only single scans can be used for skydip data.");
	if((i = stk_[0].nstk[0]) < NAMFLG)
	  i = stk_[0].nstk[2];
	if(i != curscn_.num || i == 0)
	  error("To store skydip data you must use a scan file and not a stack file.");
	/* We've been asked to store this total power measurement,
	   so let's allocate some memory for the measurement and
	   populate the structure with the appropriate values */
	if ((skydip_curr_data_ptr = 
	     (Skydip_data *) malloc(sizeof(Skydip_data))) == NULL)
	  error("Failed to allocate memory for skydip data");
	skydip_curr_data_ptr->prev_ptr = skydip_last_data_ptr;
	skydip_last_data_ptr = skydip_curr_data_ptr;
	skydip_curr_data_ptr->totpower = sum;
	skydip_curr_data_ptr->error = rms;
	skydip_curr_data_ptr->elevation = scan_.elevation;
	skydip_curr_data_ptr->TAmbient = scan_.TAmbient;
	skydip_curr_data_ptr->DTAbs = scan_.DTAbs;
	skydip_curr_data_ptr->j2seconds = stk_[0].j2second;
	skydip_curr_data_ptr->airmass = 1.0/sin(scan_.elevation*cunits[DEGREES]);
	skydip_curr_data_ptr->linefreq = stk_[0].freq;
	strncpy(skydip_curr_data_ptr->scanfile,curscn_.fil,3);
	skydip_curr_data_ptr->scanfile[3]='\0';
	skydip_curr_data_ptr->scannum = curscn_.num;
	skydip_curr_data_ptr->backend = scan_.nbknd;
	skydip_num_pts++;
      }
    } else {
      error("No chans to avg");
    }
  }
  if (*redf) {
    Reduce_skydip(skydip_last_data_ptr,skydip_num_pts,*tabf);
    Clear_skydip(&skydip_last_data_ptr,&skydip_num_pts);
  }
  return;
}

