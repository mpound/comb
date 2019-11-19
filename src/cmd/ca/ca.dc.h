extern Node cat_[];
union {
	char c[109];
	short int s[1];
	int l[1];
	double d[1];
} cab;
#define ns (cab.l + 0)
#define fnt (cab.s + 2)
#define nst (cab.s + 3)
#define fn (cab.c + 8)
static int ns_o = 1;
