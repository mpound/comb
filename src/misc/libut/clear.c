/*
 * cclear and clear set the first nbytes of array to zero for C or FORTRAN
 */

void cclear(array, nbytes)
register char *array;
register int nbytes;
{
	while(--nbytes >= 0)
		*array++ = '\0';
}

void clear_(array, nbytes)
register char *array;
int *nbytes;
{
	register int n = *nbytes;
	while(--n >= 0)
		*array++ = '\0';
}
