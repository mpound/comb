extern Node xft_[];
union {
	char c[76];
	short int s[1];
	int l[1];
	double d[1];
} xfb;
#define fr1 (xfb.d + 0)
#define fr2 (xfb.d + 1)
#define FWID (xfb.d + 2)
#define ptol (xfb.d + 3)
#define mxwt (xfb.d + 4)
#define sn1 (xfb.l + 10)
#define sn2 (xfb.l + 11)
#define sll (xfb.l + 12)
#define slh (xfb.l + 13)
#define v (xfb.l + 14)
#define snt (xfb.s + 30)
#define frt (xfb.s + 31)
#define FWIDt (xfb.s + 32)
#define ptolt (xfb.s + 33)
#define sllt (xfb.s + 34)
#define slht (xfb.s + 35)
#define mxwtt (xfb.s + 36)
#define vt (xfb.s + 37)
static int sn1_o = 10,sn2_o = MAXSTKNUM;
static double fr1_o = 0,fr2_o = 990000;
static double FWID_o = 0;
static double ptol_o = 0.1;
static int sll_o = 10;
static int slh_o = MAXSTKNUM;
static double mxwt_o = 1e30;
static int v_o = 1;
