extern Node pdt_[];
union {
	char c[10];
	short int s[1];
	int l[1];
	double d[1];
} pdb;
#define gs (pdb.l + 0)
#define act (pdb.s + 2)
#define gst (pdb.s + 3)
#define gsf (pdb.s + 4)
static int gs_o = 0;
