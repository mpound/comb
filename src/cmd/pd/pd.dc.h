extern Node pdt_[];
union {
	char c[2];
	short int s[1];
	int l[1];
	double d[1];
} pdb;
#define act (pdb.s + 0)
