extern Node slt_[];
union {
	char c[480];
	short int s[1];
	int l[1];
	double d[1];
} slb;
#define x1 (slb.d + 0)
#define y1 (slb.d + 1)
#define x2 (slb.d + 2)
#define y2 (slb.d + 3)
#define fact (slb.d + 4)
#define yadd (slb.d + 5)
#define miny (slb.d + 6)
#define maxy (slb.d + 7)
#define vf (slb.d + 8)
#define vt (slb.d + 9)
#define hf (slb.d + 10)
#define ht (slb.d + 11)
#define figh (slb.d + 12)
#define figw (slb.d + 13)
#define im (slb.l + 28)
#define mf (slb.l + 29)
#define logy (slb.l + 30)
#define mrkn (slb.l + 31)
#define fill (slb.l + 32)
#define lnt (slb.l + 33)
#define ovl (slb.l + 34)
#define fly0 (slb.l + 35)
#define x1t (slb.s + 72)
#define y1t (slb.s + 73)
#define x2t (slb.s + 74)
#define y2t (slb.s + 75)
#define imt (slb.s + 76)
#define fnt (slb.s + 77)
#define mft (slb.s + 78)
#define factt (slb.s + 79)
#define yaddt (slb.s + 80)
#define minyt (slb.s + 81)
#define maxyt (slb.s + 82)
#define logyt (slb.s + 83)
#define vft (slb.s + 84)
#define vff (slb.s + 85)
#define vtt (slb.s + 86)
#define vtf (slb.s + 87)
#define hft (slb.s + 88)
#define hff (slb.s + 89)
#define htt (slb.s + 90)
#define htf (slb.s + 91)
#define mode (slb.s + 92)
#define modef (slb.s + 93)
#define mrknt (slb.s + 94)
#define fight (slb.s + 95)
#define figwt (slb.s + 96)
#define fillt (slb.s + 97)
#define hlbt (slb.s + 98)
#define hlbf (slb.s + 99)
#define vlbt (slb.s + 100)
#define vlbf (slb.s + 101)
#define mlbt (slb.s + 102)
#define mlbf (slb.s + 103)
#define lntt (slb.s + 104)
#define ovlt (slb.s + 105)
#define fly0t (slb.s + 106)
#define fn (slb.c + 214)
#define hlb (slb.c + 279)
#define vlb (slb.c + 350)
#define mlb (slb.c + 415)
static double x1_o = 0;
static double y1_o = 0;
static double x2_o = 0;
static double y2_o = 0;
static int im_o = 1;
static double figh_o = 0;
static double figw_o = 0;
static int lnt_o = 255;
static int fly0_o = 1;
