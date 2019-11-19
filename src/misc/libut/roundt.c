main()
{
	double d;
beg:
	printf("double  ");
	scanf("%lf",&d);
	printf("round to %d\n",round(d));
	goto beg;
}
