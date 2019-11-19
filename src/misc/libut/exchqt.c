int buf1[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int buf2[16] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
main()
{
	int i;

	printf("How many  ");
	scanf("%d",&i);
	exchq(buf1,buf2,i);
	for(i = 0;i < 16;i++)
		printf("%d",buf1[i]);
	printf("\n");
	for(i = 0;i < 16;i++)
		printf("%d",buf2[i]);
	printf("\n");
}
