void cmove(src, dst, nbyte)
register char *src, *dst;
register int nbyte;
{
	bcopy(src, dst, nbyte);
}

void move_(src, dst, nbyte)
register char *src, *dst;
int *nbyte;
{
	bcopy(src, dst, *nbyte);
}
