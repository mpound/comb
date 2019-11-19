extern Node bct_[];
union {
	char c[28];
	short int s[1];
	int l[1];
	double d[1];
} bcb;
#define h1 (bcb.d + 0)
#define h2 (bcb.d + 1)
#define prn (bcb.l + 4)
#define ht (bcb.s + 10)
#define rflag (bcb.s + 11)
#define iact (bcb.s + 12)
#define prnt (bcb.s + 13)
