extern Node rint_[];
union {
	char c[8330];
	short int s[1];
	int l[1];
	double d[1];
} rinb;
#define act (rinb.s + 0)
#define actf (rinb.s + 1)
#define fnamet (rinb.s + 2)
#define exstrt (rinb.s + 3)
#define fname (rinb.c + 8)
#define exstr (rinb.c + 137)
