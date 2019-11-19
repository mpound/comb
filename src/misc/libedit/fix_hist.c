#include <stdio.h>
#include "sh_config.h"
#include "history.h"
fix_hist(command,string)
char *command, *string;
{
	/* This code has been copied from read() in editlib.c */
	char buff[MAXLINE];

	sprintf(buff, "%s %s", command, string);
	hist_eof();
	p_setout(hist_ptr->fixfd);
	p_str(buff,0);
	hist_flush();
}

#if 0
	register struct history *fp = hist_ptr;
	histloc location;
	register FILE *fd;
	char s1[MAXLINE];
	int test;

	if (fp==NULL) return;
	hist_flush();
	fd = fp->fixfp;
	printf("fixing up history file\n");
/*	printf("fixing up history file:\n\tcommand before search:%s, fixmax=%d\t",command,fp->fixmax);
	printf("\tfixind=%d\t",fp->fixind);		*/
	location = hist_find(command,fp->fixind,0,-1);
/*	printf(" location after search:%d\n",location.his_command);
	printf("fixind=%d\n",fp->fixind);	*/
	if (location.his_command < 1) return;
	hist_copy(s1,location.his_command,-1);
/*	printf("\tcommand:%s\n",s1);		*/
	fp->fixcnt=hist_position(location.his_command);
/*	printf("\tfixcnt=%d\n",fp->fixcnt);		*/
	fseek(fd,fp->fixcnt,0);
	fputs(command,fd);
	fputs(string,fd);
	putc('\0',fd);
	fp->fixcnt += strlen(command)+strlen(string)+1;
/*	printf("\ttruncated at:%d",fp->fixcnt);		*/
	test=ftruncate(fileno(fd),fp->fixcnt);
/*	printf(" \tftruncate returns:%d\n",test);	*/
	fp->fixind=location.his_command;
	hist_flush();
}
#endif /*0*/
