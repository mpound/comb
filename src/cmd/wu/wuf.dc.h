extern Node wuft_[];
union {
	char c[40];
	short int s[1];
	int l[1];
	double d[1];
} wufb;
#define rap (wufb.d + 0)
#define decp (wufb.d + 1)
#define vp (wufb.d + 2)
#define epp (wufb.d + 3)
#define rapt (wufb.s + 16)
#define decpt (wufb.s + 17)
#define vpt (wufb.s + 18)
#define eppt (wufb.s + 19)
