extern Node vmt_[];
union {
	char c[146];
	short int s[1];
	int l[1];
	double d[1];
} vmb;
#define sp (vmb.d + 0)
#define ir (vmb.d + 1)
#define efd (vmb.d + 2)
#define gs (vmb.d + 3)
#define fwhm (vmb.d + 4)
#define lv (vmb.d + 5)
#define hv (vmb.d + 6)
#define fr1 (vmb.d + 7)
#define fr2 (vmb.d + 8)
#define fwid (vmb.d + 9)
#define dist (vmb.d + 10)
#define scale (vmb.d + 11)
#define an (vmb.l + 24)
#define sll (vmb.l + 25)
#define slh (vmb.l + 26)
#define vfl (vmb.l + 27)
#define ant (vmb.s + 56)
#define spt (vmb.s + 57)
#define irt (vmb.s + 58)
#define it (vmb.s + 59)
#define itf (vmb.s + 60)
#define efdt (vmb.s + 61)
#define gst (vmb.s + 62)
#define fwhmt (vmb.s + 63)
#define sllt (vmb.s + 64)
#define slht (vmb.s + 65)
#define lvt (vmb.s + 66)
#define hvt (vmb.s + 67)
#define frt (vmb.s + 68)
#define fwidt (vmb.s + 69)
#define distt (vmb.s + 70)
#define scalet (vmb.s + 71)
#define vflt (vmb.s + 72)
static int an_o = 1;
static double sp_o = 1;
static double ir_o = 0.9;
static double efd_o = 7.;
static double gs_o = 1.;
static double fwhm_o = 1.66667;
static int sll_o = 10;
static int slh_o = MAXSTKNUM;
static double lv_o = 0;
static double hv_o = 0;
static double fr1_o = 0,fr2_o = 990000;
static double fwid_o = 0;
static double dist_o = 100;
static double scale_o = 1;
