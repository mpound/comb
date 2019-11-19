void fill_(array, ch, nbytes)
register char *array;
char *ch;
int *nbytes;
{
	register int c = *ch;
	register int nb = *nbytes;

	while(--nb >= 0)
		*array++ = c;
}

void cfill(array, ch, nbytes)
register char *array;
register int ch;
register int nbytes;
{
	while(--nbytes >= 0)
		*array++ = ch;
}
