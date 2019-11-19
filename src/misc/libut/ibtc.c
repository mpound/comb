char itest[] = {0xff,0,0x0f,0xf0,1,2,3};
main()
{
	int i;

	for(;;) {
		printf("bit# ");
		scanf("%d",&i);
		printf("%d\n",ibit(itest,i));
	}
}
