extern Node pht_[];
union {
	char c[61];
	short int s[1];
	int l[1];
	double d[1];
} phb;
#define var (phb.d + 0)
#define nl (phb.l + 2)
#define gs (phb.l + 3)
#define vart (phb.s + 8)
#define nlt (phb.s + 9)
#define act (phb.s + 10)
#define gst (phb.s + 11)
#define gsf (phb.s + 12)
#define nvart (phb.s + 13)
#define nvar (phb.c + 28)
static int gs_o = 0;
