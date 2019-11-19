extern Node wft_[];
union {
	char c[267];
	short int s[1];
	int l[1];
	double d[1];
} wfb;
#define dmin (wfb.d + 0)
#define dmax (wfb.d + 1)
#define blnk (wfb.d + 2)
#define imn (wfb.l + 6)
#define stn (wfb.l + 7)
#define revy (wfb.l + 8)
#define bitpx (wfb.l + 9)
#define fft (wfb.s + 20)
#define act (wfb.s + 21)
#define actf (wfb.s + 22)
#define imnt (wfb.s + 23)
#define stnt (wfb.s + 24)
#define objt (wfb.s + 25)
#define telt (wfb.s + 26)
#define revyt (wfb.s + 27)
#define dmint (wfb.s + 28)
#define dmaxt (wfb.s + 29)
#define bitpxt (wfb.s + 30)
#define blnkt (wfb.s + 31)
#define ff (wfb.c + 64)
#define obj (wfb.c + 129)
#define tel (wfb.c + 198)
static char ff_o[65] = "/dev/rmt12";
static int imn_o = 1;
static int stn_o = 1;
static int bitpx_o = 16;
