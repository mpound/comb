extern Node gmt_[];
union {
	char c[186];
	short int s[1];
	int l[1];
	double d[1];
} gmb;
#define xs (gmb.d + 0)
#define ys (gmb.d + 1)
#define hpos (gmb.d + 2)
#define vpos (gmb.d + 3)
#define figh (gmb.d + 4)
#define figw (gmb.d + 5)
#define cc (gmb.l + 12)
#define cr (gmb.l + 13)
#define nc (gmb.l + 14)
#define nr (gmb.l + 15)
#define mrkn (gmb.l + 16)
#define fill (gmb.l + 17)
#define lnt (gmb.l + 18)
#define act (gmb.s + 38)
#define dtermstrt (gmb.s + 39)
#define cct (gmb.s + 40)
#define crt (gmb.s + 41)
#define nct (gmb.s + 42)
#define nrt (gmb.s + 43)
#define xst (gmb.s + 44)
#define yst (gmb.s + 45)
#define tit (gmb.s + 46)
#define hpost (gmb.s + 47)
#define vpost (gmb.s + 48)
#define mrknt (gmb.s + 49)
#define fight (gmb.s + 50)
#define figwt (gmb.s + 51)
#define fillt (gmb.s + 52)
#define lntt (gmb.s + 53)
#define dtermstr (gmb.c + 108)
#define ti (gmb.c + 121)
static char dtermstr_o[13] = "";
static int nc_o = 1;
static int nr_o = 1;
static char ti_o[65] = "";
static double hpos_o = 0;
static double vpos_o = 0;
static int mrkn_o = 0;
static int lnt_o = 255;
