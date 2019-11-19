/*
 * Exchange two buffers having an even number of quad words (8 bytes each) 
 * This C version does the work in integer chunks to avoid any testing of
 * the bit patterns of doubles which might be done.
 */

void exchq(src,dest,nq)		
double *src,*dest;
register int nq;		/* number of quad words to exchange */
{
	register int *s = (int *)src;
	register int *d = (int *)dest;
	register int temp;

	while(--nq >= 0) {
		temp = *s;
		*s++ = *d;
		*d++ = temp;
		temp = *s;
		*s++ = *d;
		*d++ = temp;
	}
}
