extern Node routt_[];
union {
	char c[215];
	short int s[1];
	int l[1];
	double d[1];
} routb;
#define qf (routb.l + 0)
#define trf (routb.l + 1)
#define act (routb.s + 4)
#define actf (routb.s + 5)
#define fnamet (routb.s + 6)
#define pcmdt (routb.s + 7)
#define StrNamt (routb.s + 8)
#define qft (routb.s + 9)
#define trft (routb.s + 10)
#define fname (routb.c + 22)
#define pcmd (routb.c + 70)
#define StrNam (routb.c + 150)
static char pcmd_o[80] = "${PAGER-more}";
