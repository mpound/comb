bcopy(src, dest, n)
register char *src, *dest;
register int n;
{
	if(src > dest || dest > src + n)
		memcpy(dest, src, n);
	else
		while(--n >= 0)
			dest[n] = src[n];
}

bzero(array, nbytes)
register char *array;
register int nbytes;
{
	while(--nbytes >= 0)
		*array++ = '\0';
}
