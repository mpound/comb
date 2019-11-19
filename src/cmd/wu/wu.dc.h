extern Node wut_[];
union {
	char c[274];
	short int s[1];
	int l[1];
	double d[1];
} wub;
#define llx (wub.d + 0)
#define urx (wub.d + 1)
#define lly (wub.d + 2)
#define ury (wub.d + 3)
#define lst (wub.d + 4)
#define year (wub.d + 5)
#define latp (wub.d + 6)
#define llxt (wub.s + 28)
#define urxt (wub.s + 29)
#define llyt (wub.s + 30)
#define uryt (wub.s + 31)
#define fnamet (wub.s + 32)
#define lstt (wub.s + 33)
#define yeart (wub.s + 34)
#define yrf (wub.s + 35)
#define latpt (wub.s + 36)
#define hlbt (wub.s + 37)
#define hlbf (wub.s + 38)
#define vlbt (wub.s + 39)
#define vlbf (wub.s + 40)
#define mlbt (wub.s + 41)
#define mlbf (wub.s + 42)
#define fname (wub.c + 86)
#define hlb (wub.c + 151)
#define vlb (wub.c + 192)
#define mlb (wub.c + 233)
static double llx_o = 0;
static double urx_o = 360;
static double lly_o = 0;
static double ury_o = 90;
static double lst_o = 6;
static double latp_o = 40.392;
