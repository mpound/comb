extern Node lkt_[];
union {
	char c[267];
	short int s[1];
	int l[1];
	double d[1];
} lkb;
#define llx (lkb.d + 0)
#define urx (lkb.d + 1)
#define lly (lkb.d + 2)
#define ury (lkb.d + 3)
#define figh (lkb.d + 4)
#define figw (lkb.d + 5)
#define fr1 (lkb.d + 6)
#define fr2 (lkb.d + 7)
#define sll (lkb.l + 16)
#define slh (lkb.l + 17)
#define mrkn (lkb.l + 18)
#define fill (lkb.l + 19)
#define psn (lkb.l + 20)
#define lnt (lkb.l + 21)
#define chk (lkb.l + 22)
#define dt (lkb.l + 23)
#define ovl (lkb.l + 24)
#define llxt (lkb.s + 50)
#define urxt (lkb.s + 51)
#define llyt (lkb.s + 52)
#define uryt (lkb.s + 53)
#define sllt (lkb.s + 54)
#define slht (lkb.s + 55)
#define mrknt (lkb.s + 56)
#define fight (lkb.s + 57)
#define figwt (lkb.s + 58)
#define fillt (lkb.s + 59)
#define frt (lkb.s + 60)
#define psnt (lkb.s + 61)
#define hlbt (lkb.s + 62)
#define hlbf (lkb.s + 63)
#define vlbt (lkb.s + 64)
#define vlbf (lkb.s + 65)
#define mlbt (lkb.s + 66)
#define mlbf (lkb.s + 67)
#define lntt (lkb.s + 68)
#define chkt (lkb.s + 69)
#define dtt (lkb.s + 70)
#define ovlt (lkb.s + 71)
#define hlb (lkb.c + 144)
#define vlb (lkb.c + 185)
#define mlb (lkb.c + 226)
static double llx_o = 0;
static double urx_o = 0;
static double lly_o = 0;
static double ury_o = 0;
static int sll_o = 10;
static int slh_o = 262143;
static double fr1_o = 0,fr2_o = 990000;
static int lnt_o = 255;
