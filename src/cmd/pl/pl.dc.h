extern Node plt_[];
union {
	char c[305];
	short int s[1];
	int l[1];
	double d[1];
} plb;
#define vf (plb.d + 0)
#define vt (plb.d + 1)
#define vs (plb.d + 2)
#define voff (plb.d + 3)
#define hf (plb.d + 4)
#define ht (plb.d + 5)
#define hs (plb.d + 6)
#define fch (plb.l + 14)
#define tch (plb.l + 15)
#define mrkn (plb.l + 16)
#define fill (plb.l + 17)
#define lnt (plb.l + 18)
#define ovl (plb.l + 19)
#define tic (plb.l + 20)
#define usar (plb.l + 21)
#define gr (plb.l + 22)
#define fly0 (plb.l + 23)
#define lbs (plb.l + 24)
#define hd (plb.l + 25)
#define vtp (plb.s + 52)
#define vft (plb.s + 53)
#define vff (plb.s + 54)
#define vtt (plb.s + 55)
#define vtf (plb.s + 56)
#define vst (plb.s + 57)
#define vsf (plb.s + 58)
#define vofft (plb.s + 59)
#define htp (plb.s + 60)
#define hft (plb.s + 61)
#define hff (plb.s + 62)
#define htt (plb.s + 63)
#define htf (plb.s + 64)
#define hst (plb.s + 65)
#define hsf (plb.s + 66)
#define unit (plb.s + 67)
#define chngu (plb.s + 68)
#define fcht (plb.s + 69)
#define fchf (plb.s + 70)
#define tcht (plb.s + 71)
#define tchf (plb.s + 72)
#define mode (plb.s + 73)
#define modef (plb.s + 74)
#define mrknt (plb.s + 75)
#define hlbt (plb.s + 76)
#define hlbf (plb.s + 77)
#define vlbt (plb.s + 78)
#define vlbf (plb.s + 79)
#define mlbt (plb.s + 80)
#define mlbf (plb.s + 81)
#define fillt (plb.s + 82)
#define lntt (plb.s + 83)
#define ovlt (plb.s + 84)
#define tict (plb.s + 85)
#define usart (plb.s + 86)
#define grt (plb.s + 87)
#define fly0t (plb.s + 88)
#define lbst (plb.s + 89)
#define hdt (plb.s + 90)
#define hlb (plb.c + 182)
#define vlb (plb.c + 223)
#define mlb (plb.c + 264)
static int lnt_o = 255;
static int tic_o = 0;
static int usar_o = 1;
static int gr_o = 1;
static int fly0_o = 1;
static int lbs_o = 1;
static int hd_o = 1;