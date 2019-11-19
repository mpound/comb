/*
 * FORTRAN and C callable routines to get the n'th bit out of an array
 */
int ibit_(int *array, int *nbit)
{
	register int bit = *nbit - 1;
	return((array[bit >> 5] >> (bit & 31)) & 1);
}

int ibit(int *array, int bit)
{
	bit--;
	return((array[bit >> 5] >> (bit & 31)) & 1);
}
