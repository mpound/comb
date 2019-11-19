wcopy_(from, to, np)
register int *from, *to, *np;
{
	wcopy(from, to, *np);
}

wcopy(from, to, n)
register char *from;
register char *to;
register int n;
{
	register char *end;
	register int diff = to - from;

	if(to > from) {
		end = from;
		from += n;
		while(from > end) {
			*(int *)(from + diff) = *(int *)from;
			from -= sizeof(int);
		}
	} else {
		end = from + n;
		while(from < end) {
			*(int *)(from + diff) = *(int *)from;
			from += sizeof(int);
		}
	}
}
