extern Node dat_[];
union {
	char c[2090];
	short int s[1];
	int l[1];
	double d[1];
} dab;
#define X (dab.d + 0)
#define Y (dab.d + 128)
#define an (dab.l + 512)
#define pf (dab.l + 513)
#define plf (dab.l + 514)
#define lnt (dab.l + 515)
#define paf (dab.l + 516)
#define sbt (dab.s + 1034)
#define sbfl (dab.s + 1035)
#define Xt (dab.s + 1036)
#define Xe (dab.s + 1037)
#define Yt (dab.s + 1038)
#define Ye (dab.s + 1039)
#define ant (dab.s + 1040)
#define pft (dab.s + 1041)
#define plft (dab.s + 1042)
#define lntt (dab.s + 1043)
#define paft (dab.s + 1044)
static short Xe_o = 1,Ye_o = 1;
static int lnt_o = 255;
