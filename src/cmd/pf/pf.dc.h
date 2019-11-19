extern Node pft_[];
union {
	char c[28];
	short int s[1];
	int l[1];
	double d[1];
} pfb;
#define h1 (pfb.d + 0)
#define h2 (pfb.d + 1)
#define dpfl (pfb.l + 4)
#define h1t (pfb.s + 10)
#define h2t (pfb.s + 11)
#define iacton (pfb.s + 12)
#define dpflt (pfb.s + 13)
static double h1_o = 0;
static double h2_o = 0;
