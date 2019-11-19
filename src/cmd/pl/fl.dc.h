extern Node flt_[];
union {
	char c[18];
	short int s[1];
	int l[1];
	double d[1];
} flb;
#define loc (flb.d + 0)
#define lnt (flb.l + 2)
#define loct (flb.s + 6)
#define lntt (flb.s + 7)
#define unit (flb.s + 8)
