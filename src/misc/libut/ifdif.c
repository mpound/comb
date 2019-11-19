/*
 * This subroutine compares a and b. If they differ by more than fuzz
 * it returns 1, otherwise 0. A,b,and fuzz are double.
 */

int cifdif(a,b,fuzz)
double a,b,fuzz;
{
	register double diff;
	diff = a - b;
	if(diff < 0)
		diff = - diff;
	return(diff > fuzz);
}

int ifdif_(a,b,fuzz)
float *a,*b,*fuzz;
{
	register double diff;
	diff = *a - *b;
	if(diff < 0)
		diff = - diff;
	return(diff > *fuzz);
}
