extern Node hct_[];
union {
	char c[67];
	short int s[1];
	int l[1];
	double d[1];
} hcb;
#define typ (hcb.s + 0)
#define hcfnt (hcb.s + 1)
#define ori (hcb.s + 2)
#define hcfn (hcb.c + 6)
