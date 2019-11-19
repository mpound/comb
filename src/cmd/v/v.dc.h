extern Node vt_[];
union {
	char c[10];
	short int s[1];
	int l[1];
	double d[1];
} vb;
#define val (vb.d + 0)
#define valt (vb.s + 4)
