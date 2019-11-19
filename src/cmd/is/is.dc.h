extern Node ist_[];
union {
	char c[116];
	short int s[1];
	int l[1];
	double d[1];
} isb;
#define x (isb.d + 0)
#define y (isb.d + 1)
#define ir (isb.d + 2)
#define efd (isb.d + 3)
#define gs (isb.d + 4)
#define fwhm (isb.d + 5)
#define fr1 (isb.d + 6)
#define fr2 (isb.d + 7)
#define fwid (isb.d + 8)
#define sll (isb.l + 18)
#define slh (isb.l + 19)
#define su (isb.l + 20)
#define tflg (isb.l + 21)
#define xt (isb.s + 44)
#define yt (isb.s + 45)
#define irt (isb.s + 46)
#define it (isb.s + 47)
#define itf (isb.s + 48)
#define efdt (isb.s + 49)
#define gst (isb.s + 50)
#define fwhmt (isb.s + 51)
#define sllt (isb.s + 52)
#define slht (isb.s + 53)
#define frt (isb.s + 54)
#define fwidt (isb.s + 55)
#define sut (isb.s + 56)
#define tflgt (isb.s + 57)
static double x_o = 0;
static double y_o = 0;
static double ir_o = 0.8;
static double efd_o = 7.;
static double gs_o = 1.;
static double fwhm_o = 1.66667;
static int sll_o = 10;
static int slh_o = MAXSTKNUM;
static double fr1_o = 0,fr2_o = 990000;
static double fwid_o = 0;
static int su_o = 0;
