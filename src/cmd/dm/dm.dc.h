extern Node dmt_[];
union {
	char c[40];
	short int s[1];
	int l[1];
	double d[1];
} dmb;
#define glob (dmb.s + 0)
#define act (dmb.s + 1)
#define patt (dmb.s + 2)
#define pfl (dmb.s + 3)
#define pat (dmb.c + 8)
