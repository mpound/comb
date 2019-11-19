void swab_(src, dst, nbyt)
register char *src, *dst;
int *nbyt;
{
	register int nb = *nbyt >> 1;
	register int c;
	while(--nb >= 0) {
		c = *src++;
		*dst++ = *src++;
		*dst++ = c;
	}
}
