static struct of_indx			/* index buffer */
{
        unsigned int loc;                       /* location of stack in file */
        unsigned short length;                  /* length of stack */
						/* if length is 0, then this */
						/* index does not represent */
						/* any space that has been */
						/* allocated */
	char full;				/* 1 if stack is full */
	char unem;				/* 1 if stack recoverable */
} indx;
char fn[128];
main()
{
	int sn1,sn2,fd;
	register int i,j;

newfile:
	printf("Index File Name >");
	scanf("%s",fn);
	if( (fd = open(fn,0,0)) < 0)
		goto newfile;
again:
	printf("first, last stack number > ");
	scanf("%d %d",&sn1,&sn2);
	printf("stnum location length ful unem\n");
	lseek(fd,(sn1 - 1) * sizeof(struct of_indx),0);
	for(i = sn1; i <= sn2; i++) {
		j = read(fd,&indx,sizeof( struct of_indx));
		printf("%5d %8d %6d %3d %3d\n",i ,indx.loc, indx.length,
			indx.full, indx.unem);
	}
	goto again;
}
