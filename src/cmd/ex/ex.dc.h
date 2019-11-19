extern Node ext_[];
union {
	char c[261];
	short int s[1];
	int l[1];
	double d[1];
} exb;
#define cmdt (exb.s + 0)
#define cmdf (exb.s + 1)
#define cmd (exb.c + 4)
