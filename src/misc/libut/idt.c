main()
{
	double a,b,fuzz;
	for(;;) {
		printf("a,b,fuzz ");
		scanf("%lf%lf%lf",&a,&b,&fuzz);
		printf("%d\n",cifdif(a,b,fuzz));
	}
}
