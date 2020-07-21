extern Node cct_[];
union {
	char c[42];
	short int s[1];
	int l[1];
	double d[1];
} ccb;
#define nwf (ccb.d + 0)
#define nwv (ccb.d + 1)
#define oe (ccb.d + 2)
#define ssbfr (ccb.d + 3)
#define iactn (ccb.s + 16)
#define nwft (ccb.s + 17)
#define nwvt (ccb.s + 18)
#define oet (ccb.s + 19)
#define ssbfrt (ccb.s + 20)
static double oe_o = 0;
static double ssbfr_o = 0;
