extern Node vct_[];
union {
	char c[1454];
	short int s[1];
	int l[1];
	double d[1];
} vcb;
#define p1h (vcb.d + 0)
#define p1v (vcb.d + 1)
#define p2h (vcb.d + 2)
#define p2v (vcb.d + 3)
#define sp (vcb.d + 4)
#define ir (vcb.d + 5)
#define efd (vcb.d + 6)
#define gs (vcb.d + 7)
#define fwhm (vcb.d + 8)
#define lv (vcb.d + 9)
#define hv (vcb.d + 10)
#define fr1 (vcb.d + 11)
#define fr2 (vcb.d + 12)
#define st (vcb.d + 13)
#define clv (vcb.d + 14)
#define lc (vcb.d + 89)
#define hc (vcb.d + 90)
#define sll (vcb.l + 182)
#define slh (vcb.l + 183)
#define nlv (vcb.l + 184)
#define clt (vcb.l + 185)
#define dbg (vcb.l + 260)
#define p1ht (vcb.s + 522)
#define p1vt (vcb.s + 523)
#define p2ht (vcb.s + 524)
#define p2vt (vcb.s + 525)
#define spt (vcb.s + 526)
#define irt (vcb.s + 527)
#define it (vcb.s + 528)
#define itf (vcb.s + 529)
#define efdt (vcb.s + 530)
#define gst (vcb.s + 531)
#define fwhmt (vcb.s + 532)
#define lvt (vcb.s + 533)
#define hvt (vcb.s + 534)
#define sllt (vcb.s + 535)
#define slht (vcb.s + 536)
#define frt (vcb.s + 537)
#define scl (vcb.s + 538)
#define sclf (vcb.s + 539)
#define stt (vcb.s + 540)
#define nlvt (vcb.s + 541)
#define clvt (vcb.s + 542)
#define clve (vcb.s + 543)
#define cltt (vcb.s + 544)
#define clte (vcb.s + 545)
#define lct (vcb.s + 546)
#define lcf (vcb.s + 547)
#define hct (vcb.s + 548)
#define hcf (vcb.s + 549)
#define dbgt (vcb.s + 550)
#define objt (vcb.s + 551)
#define zunt (vcb.s + 552)
#define hlbt (vcb.s + 553)
#define hlbf (vcb.s + 554)
#define vlbt (vcb.s + 555)
#define vlbf (vcb.s + 556)
#define mlbt (vcb.s + 557)
#define mlbf (vcb.s + 558)
#define blbt (vcb.s + 559)
#define blbf (vcb.s + 560)
#define obj (vcb.c + 1122)
#define zun (vcb.c + 1191)
#define hlb (vcb.c + 1260)
#define vlb (vcb.c + 1331)
#define mlb (vcb.c + 1372)
#define blb (vcb.c + 1413)
static short clve_o = 1,clte_o = 1;
static double sp_o = 2;
static double ir_o = 0.8;
static double efd_o = 7.;
static double gs_o = 1.;
static double fwhm_o = 1.66667;
static double lv_o = 0;
static double hv_o = 0;
static int sll_o = 10;
static int slh_o = MAXSTKNUM;
static double fr1_o = 0,fr2_o = 990000;
