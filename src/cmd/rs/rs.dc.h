extern Node rst_[];
union {
	char c[20];
	short int s[1];
	int l[1];
	double d[1];
} rsb;
#define factr (rsb.d + 0)
#define set (rsb.d + 1)
#define factrt (rsb.s + 8)
#define sett (rsb.s + 9)
