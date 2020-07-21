#include <stdio.h>
#include <string.h>
#include "../../main/C.h"
#include "../../stacks/C.h"
#include "../../scan/C.h"
#include "../../misc/C.h"
#include "../../parse/C.h"
#include "../../parse/P.struct.h"

void asgnstrhdr(HdrVar *var, char *val);

void pd_()
{
	int i;
	short *sp;
	float *fp;
	char *date;
	char tempstr[80];

#include "pd.dc.h"
#include "pd.ec.h"

	switch(*act) {
	case 0:		/* Print the stack header */
		printf("numst = %d, freq = %.4f\n", stk_[0].numst,
			stk_[0].freq);
		for(i = 0, sp = &stk_[0].numpt;sp < (short *)stk_[0].label; sp++) {
			printf("%7d", *sp);
			if(i == 9) {
				putchar('\n');
				i = 0;
			} else
				i++;
		}
		putchar('\n');
		putchar('\n');
		for(i = 0, fp = &stk_[0].fwid;fp < stk_[0].rsdm; fp++) {
			printf("%14.6g", *fp);
			if(i == 4) {
				putchar('\n');
				i = 0;
			} else
				i++;
		}
		putchar('\n');
		printf("%s\n",stk_[0].label); 
		putchar('\n');
		break;
	case 1:		/* Print the scan header */
		pscanhdr_();
		break;
	case 2:		/* Print the stack data */
		for(i = 0; i < stk_[0].numpt; i++) {
			printf("%9.3f ", stk_[0].stak[i]);
			if((i % 8) == 7) {
				putchar('\n');
			}
		}
		putchar('\n');
		break;
	case 3:		/* Print the scan data */
		pscandat();
		break;
	case 4: /* print Scan2 main header */
		PrintScan2MainHdr();
		break;
	case 5:		/* Print the scan numbers form stack 1 */
	  prtsl_(tempstr,80);
	  puts(tempstr);
	  break;
	case 6:		/* Put the scan filename from stack 1 in a GS */
	  prtsl_(tempstr,80);
	  tempstr[3]='\0';
	  if(sg_[*gs])
	    ChkFree(sg_[*gs]);
	  sg_[*gs] = ChkAlloc(strlen(tempstr) + 1, "Global string");
	  strcpy(sg_[*gs], tempstr);
	  break;
	case 7:		/* Print the date and time of stack 1 */
		if( stk_[0].j2second != 0) 
			printf(" %s",j2sToDate(stk_[0].j2second));
		else
			printf(" No date was stored with this stack.");
	}
}
