extern Node git_[];
union {
	char c[6];
	short int s[1];
	int l[1];
	double d[1];
} gib;
#define dpfl (gib.l + 0)
#define dpflt (gib.s + 2)
