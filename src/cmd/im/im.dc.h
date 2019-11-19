extern Node imt_[];
union {
	char c[248];
	short int s[1];
	int l[1];
	double d[1];
} imb;
#define fact (imb.d + 0)
#define set (imb.d + 1)
#define llx (imb.d + 2)
#define urx (imb.d + 3)
#define lly (imb.d + 4)
#define ury (imb.d + 5)
#define tpow (imb.d + 6)
#define mina (imb.d + 7)
#define minb (imb.d + 8)
#define src (imb.l + 18)
#define dest (imb.l + 19)
#define im1 (imb.l + 20)
#define im2 (imb.l + 21)
#define im3 (imb.l + 22)
#define imn (imb.l + 23)
#define an (imb.l + 24)
#define op (imb.s + 50)
#define srct (imb.s + 51)
#define destt (imb.s + 52)
#define im1t (imb.s + 53)
#define im2t (imb.s + 54)
#define im3t (imb.s + 55)
#define hlt (imb.s + 56)
#define ophf (imb.s + 57)
#define factt (imb.s + 58)
#define sett (imb.s + 59)
#define imnt (imb.s + 60)
#define ssb (imb.s + 61)
#define ssbf (imb.s + 62)
#define ant (imb.s + 63)
#define llxt (imb.s + 64)
#define urxt (imb.s + 65)
#define llyt (imb.s + 66)
#define uryt (imb.s + 67)
#define tpowt (imb.s + 68)
#define uht (imb.s + 69)
#define userhf (imb.s + 70)
#define minat (imb.s + 71)
#define minbt (imb.s + 72)
#define hl (imb.c + 146)
#define uh (imb.c + 197)
static int src_o = 1;
static int dest_o = 2;
static int im1_o = 1;
static int im2_o = 2;
static int im3_o = 3;
static int imn_o = 1;
static int an_o = 1;
static double llx_o = 0;
static double urx_o = 0;
static double lly_o = 0;
static double ury_o = 0;
static double tpow_o = -1.0;
