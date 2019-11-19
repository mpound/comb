extern Node crnt_[];
union {
	char c[8202];
	short int s[1];
	int l[1];
	double d[1];
} crnb;
#define varr (crnb.d + 0)
#define vart (crnb.s + 4100)
#define var (crnb.c + 0)
