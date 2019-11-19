extern Node aft_[];
union {
	char c[83];
	short int s[1];
	int l[1];
	double d[1];
} afb;
#define imn (afb.l + 0)
#define stn (afb.l + 1)
#define fft (afb.s + 4)
#define act (afb.s + 5)
#define actf (afb.s + 6)
#define imnt (afb.s + 7)
#define stnt (afb.s + 8)
#define ff (afb.c + 18)
static int imn_o = 1;
static int stn_o = 1;
