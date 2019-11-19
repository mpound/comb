extern Node sqt_[];
union {
	char c[22];
	short int s[1];
	int l[1];
	double d[1];
} sqb;
#define wnool (sqb.d + 0)
#define h (sqb.d + 1)
#define wnoolt (sqb.s + 8)
#define ht (sqb.s + 9)
#define hflg (sqb.s + 10)
