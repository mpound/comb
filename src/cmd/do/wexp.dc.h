extern Node wexpt_[];
union {
	char c[10];
	short int s[1];
	int l[1];
	double d[1];
} wexpb;
#define val (wexpb.d + 0)
#define valt (wexpb.s + 4)
