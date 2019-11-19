#define SIZE 262200
main()
{
	register int i;
	int n;
	char c,t[SIZE+8],f = 'f';
LOOP:
	printf("\ncmd,numb  ");
	scanf("%s%d",&c,&n);
	switch(c) {
	case 'p':
		for(i = 0;i < SIZE && i < n;i += 8) {
			sprintf(t+i,"%07d,",i);
		}
		break;
	case 'C':
		clear_(t,&n);
		break;
	case 'c':
		cclear(t,n);
		break;
	case 'F':
		fill_(t,&f,&n);
		break;
	case 'f':
		cfill(t,f,n);
		break;
	case 'd':
		for(i=0;i < 24;i++) {
			printf("%c",(t[n+i])? t[n+i] : '@');
		}
		printf("!");
		break;
	}
	goto LOOP;
}
