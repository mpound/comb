extern Node cpt_[];
union {
	char c[1200];
	short int s[1];
	int l[1];
	double d[1];
} cpb;
#define st (cpb.d + 0)
#define clv (cpb.d + 1)
#define lc (cpb.d + 76)
#define hc (cpb.d + 77)
#define hcl (cpb.d + 78)
#define hcs (cpb.d + 79)
#define imn (cpb.l + 160)
#define plane1 (cpb.l + 161)
#define plane2 (cpb.l + 162)
#define pstep (cpb.l + 163)
#define nlv (cpb.l + 164)
#define clt (cpb.l + 165)
#define ovl (cpb.l + 240)
#define circb (cpb.l + 241)
#define revx (cpb.l + 242)
#define revy (cpb.l + 243)
#define imnt (cpb.s + 488)
#define planet (cpb.s + 489)
#define plf (cpb.s + 490)
#define pstept (cpb.s + 491)
#define scl (cpb.s + 492)
#define sclf (cpb.s + 493)
#define stt (cpb.s + 494)
#define nlvt (cpb.s + 495)
#define clvt (cpb.s + 496)
#define clve (cpb.s + 497)
#define cltt (cpb.s + 498)
#define clte (cpb.s + 499)
#define lct (cpb.s + 500)
#define lcf (cpb.s + 501)
#define hct (cpb.s + 502)
#define hcf (cpb.s + 503)
#define ovlt (cpb.s + 504)
#define circbt (cpb.s + 505)
#define revxt (cpb.s + 506)
#define revyt (cpb.s + 507)
#define hclt (cpb.s + 508)
#define hcst (cpb.s + 509)
#define hlbt (cpb.s + 510)
#define hlbf (cpb.s + 511)
#define vlbt (cpb.s + 512)
#define vlbf (cpb.s + 513)
#define mlbt (cpb.s + 514)
#define mlbf (cpb.s + 515)
#define blbt (cpb.s + 516)
#define blbf (cpb.s + 517)
#define hlb (cpb.c + 1036)
#define vlb (cpb.c + 1077)
#define mlb (cpb.c + 1118)
#define blb (cpb.c + 1159)
static short clve_o = 1,clte_o = 1;
static int imn_o = 1;
static int plane1_o = 0,plane2_o = 0;
static double hcl_o = 0;
static double hcs_o = 0;
