extern Node spt_[];
union {
	char c[378];
	short int s[1];
	int l[1];
	double d[1];
} spb;
#define xfact (spb.d + 0)
#define yfact (spb.d + 1)
#define minx (spb.d + 2)
#define miny (spb.d + 3)
#define maxx (spb.d + 4)
#define maxy (spb.d + 5)
#define vf (spb.d + 6)
#define vt (spb.d + 7)
#define hf (spb.d + 8)
#define ht (spb.d + 9)
#define figh (spb.d + 10)
#define figw (spb.d + 11)
#define imx (spb.l + 24)
#define imy (spb.l + 25)
#define an (spb.l + 26)
#define logx (spb.l + 27)
#define logy (spb.l + 28)
#define mrkn (spb.l + 29)
#define fill (spb.l + 30)
#define ovl (spb.l + 31)
#define imxt (spb.s + 64)
#define imyt (spb.s + 65)
#define ant (spb.s + 66)
#define fnt (spb.s + 67)
#define xfactt (spb.s + 68)
#define yfactt (spb.s + 69)
#define minxt (spb.s + 70)
#define minyt (spb.s + 71)
#define maxxt (spb.s + 72)
#define maxyt (spb.s + 73)
#define vft (spb.s + 74)
#define vff (spb.s + 75)
#define vtt (spb.s + 76)
#define vtf (spb.s + 77)
#define hft (spb.s + 78)
#define hff (spb.s + 79)
#define htt (spb.s + 80)
#define htf (spb.s + 81)
#define logxt (spb.s + 82)
#define logyt (spb.s + 83)
#define mrknt (spb.s + 84)
#define fight (spb.s + 85)
#define figwt (spb.s + 86)
#define fillt (spb.s + 87)
#define hlbt (spb.s + 88)
#define hlbf (spb.s + 89)
#define vlbt (spb.s + 90)
#define vlbf (spb.s + 91)
#define mlbt (spb.s + 92)
#define mlbf (spb.s + 93)
#define ovlt (spb.s + 94)
#define fn (spb.c + 190)
#define hlb (spb.c + 255)
#define vlb (spb.c + 296)
#define mlb (spb.c + 337)
static int imx_o = 1;
static int imy_o = 2;
static int an_o = 1;
static double xfact_o = 1;
static double yfact_o = 1;
static double figh_o = 0;
static double figw_o = 0;
