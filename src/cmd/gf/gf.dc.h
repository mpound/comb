extern Node gft_[];
union {
	char c[40];
	short int s[1];
	int l[1];
	double d[1];
} gfb;
#define h1 (gfb.d + 0)
#define h2 (gfb.d + 1)
#define width (gfb.d + 2)
#define dpfl (gfb.l + 6)
#define h1t (gfb.s + 14)
#define h2t (gfb.s + 15)
#define widtht (gfb.s + 16)
#define widfl (gfb.s + 17)
#define iacton (gfb.s + 18)
#define dpflt (gfb.s + 19)
static double h1_o = 0;
static double h2_o = 0;
