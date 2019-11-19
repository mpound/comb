extern Node elt_[];
union {
	char c[26];
	short int s[1];
	int l[1];
	double d[1];
} elb;
#define h1 (elb.d + 0)
#define h2 (elb.d + 1)
#define stn (elb.l + 4)
#define ht (elb.s + 10)
#define unit (elb.s + 11)
#define stnt (elb.s + 12)
static double h1_o = 0,h2_o = 0;
