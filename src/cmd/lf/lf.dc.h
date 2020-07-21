extern Node lft_[];
union {
	char c[28];
	short int s[1];
	int l[1];
	double d[1];
} lfb;
#define h1 (lfb.d + 0)
#define h2 (lfb.d + 1)
#define dpfl (lfb.l + 4)
#define h1t (lfb.s + 10)
#define h2t (lfb.s + 11)
#define iacton (lfb.s + 12)
#define dpflt (lfb.s + 13)
static double h1_o = 0;
static double h2_o = 0;
