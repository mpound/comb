extern Node cct_[];
union {
	char c[32];
	short int s[1];
	int l[1];
	double d[1];
} ccb;
#define nwf (ccb.d + 0)
#define nwv (ccb.d + 1)
#define oe (ccb.d + 2)
#define iactn (ccb.s + 12)
#define nwft (ccb.s + 13)
#define nwvt (ccb.s + 14)
#define oet (ccb.s + 15)
static double oe_o = 0;
