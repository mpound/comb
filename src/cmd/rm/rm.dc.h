extern Node rmt_[];
union {
	char c[6];
	short int s[1];
	int l[1];
	double d[1];
} rmb;
#define dp (rmb.l + 0)
#define dpt (rmb.s + 2)
