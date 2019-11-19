extern Node tht_[];
union {
	char c[300];
	short int s[1];
	int l[1];
	double d[1];
} thb;
#define tbin (thb.d + 0)
#define lv (thb.d + 1)
#define hv (thb.d + 2)
#define xfact (thb.d + 3)
#define yfact (thb.d + 4)
#define minx (thb.d + 5)
#define maxx (thb.d + 6)
#define sll (thb.l + 14)
#define slh (thb.l + 15)
#define logx (thb.l + 16)
#define logy (thb.l + 17)
#define tbint (thb.s + 36)
#define sllt (thb.s + 37)
#define slht (thb.s + 38)
#define svl (thb.s + 39)
#define vlfl (thb.s + 40)
#define lvt (thb.s + 41)
#define hvt (thb.s + 42)
#define xfactt (thb.s + 43)
#define yfactt (thb.s + 44)
#define minxt (thb.s + 45)
#define maxxt (thb.s + 46)
#define logxt (thb.s + 47)
#define logyt (thb.s + 48)
#define fnt (thb.s + 49)
#define hlbt (thb.s + 50)
#define hlbf (thb.s + 51)
#define vlbt (thb.s + 52)
#define vlbf (thb.s + 53)
#define mlbt (thb.s + 54)
#define mlbf (thb.s + 55)
#define fn (thb.c + 112)
#define hlb (thb.c + 177)
#define vlb (thb.c + 218)
#define mlb (thb.c + 259)
static double tbin_o = 0.1;
static int sll_o = 10;
static int slh_o = MAXSTKNUM;
static double lv_o = 0;
static double hv_o = 0;
static double xfact_o = 1;
static double yfact_o = 1;
