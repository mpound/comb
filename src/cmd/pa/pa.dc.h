extern Node pat_[];
union {
	char c[14];
	short int s[1];
	int l[1];
	double d[1];
} pab;
#define time (pab.d + 0)
#define type (pab.s + 4)
#define tpfl (pab.s + 5)
#define timet (pab.s + 6)
static double time_o = 1.0;
