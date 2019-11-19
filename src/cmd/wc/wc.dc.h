extern Node wct_[];
union {
	char c[416];
	short int s[1];
	int l[1];
	double d[1];
} wcb;
#define llx (wcb.d + 0)
#define urx (wcb.d + 1)
#define lly (wcb.d + 2)
#define ury (wcb.d + 3)
#define ir (wcb.d + 4)
#define efd (wcb.d + 5)
#define gs (wcb.d + 6)
#define fwhm (wcb.d + 7)
#define lv (wcb.d + 8)
#define hv (wcb.d + 9)
#define fr1 (wcb.d + 10)
#define fr2 (wcb.d + 11)
#define mwp (wcb.d + 12)
#define rh (wcb.l + 26)
#define rv (wcb.l + 27)
#define sll (wcb.l + 28)
#define slh (wcb.l + 29)
#define dbg (wcb.l + 30)
#define llxt (wcb.s + 62)
#define urxt (wcb.s + 63)
#define llyt (wcb.s + 64)
#define uryt (wcb.s + 65)
#define rht (wcb.s + 66)
#define rvt (wcb.s + 67)
#define irt (wcb.s + 68)
#define it (wcb.s + 69)
#define itf (wcb.s + 70)
#define efdt (wcb.s + 71)
#define gst (wcb.s + 72)
#define fwhmt (wcb.s + 73)
#define lvt (wcb.s + 74)
#define hvt (wcb.s + 75)
#define sllt (wcb.s + 76)
#define slht (wcb.s + 77)
#define frt (wcb.s + 78)
#define mwpt (wcb.s + 79)
#define dbgt (wcb.s + 80)
#define objt (wcb.s + 81)
#define telt (wcb.s + 82)
#define mlbt (wcb.s + 83)
#define mlbf (wcb.s + 84)
#define fft (wcb.s + 85)
#define obj (wcb.c + 172)
#define tel (wcb.c + 241)
#define mlb (wcb.c + 310)
#define ff (wcb.c + 351)
static double llx_o = 0;
static double urx_o = 0;
static double lly_o = 0;
static double ury_o = 0;
static int rh_o = 10;
static int rv_o = 10;
static double ir_o = 0.8;
static double efd_o = 7.;
static double gs_o = 1.;
static double fwhm_o = 1.66667;
static double lv_o = 0;
static double hv_o = 0;
static int sll_o = 10;
static int slh_o = MAXSTKNUM;
static double fr1_o = 0,fr2_o = 990000;
static double mwp_o = 0;
static char ff_o[65] = "wc.fits";
