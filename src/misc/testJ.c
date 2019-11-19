#include <stdio.h>
#include "C.h"
main() {
        /*char *DATE = "1999-12-16T26:25:31.45";*/
        char *DATE = "1999-1x-16T0F:25:31.45";
        char *back;
        int j2second;
fprintf(stderr,"trying datetoJ2S...");
        j2second=dateToJ2S(DATE);
fprintf(stderr,"trying back...");
        back=j2sToDate(j2second);
        printf("%s  %d  %s\n",DATE,j2second,back);

}

