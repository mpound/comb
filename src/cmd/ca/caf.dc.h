extern Node caft_[];
union {
	char c[10];
	short int s[1];
	int l[1];
	double d[1];
} cafb;
#define dum2 (cafb.d + 0)
#define dum2t (cafb.s + 4)
