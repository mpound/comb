extern Node crt_[];
union {
	char c[34];
	short int s[1];
	int l[1];
	double d[1];
} crb;
#define reads (crb.l + 0)
#define cb (crb.l + 1)
#define pxyf (crb.l + 2)
#define pxf (crb.l + 3)
#define pyf (crb.l + 4)
#define readst (crb.s + 10)
#define cbt (crb.s + 11)
#define pcc (crb.s + 12)
#define pccf (crb.s + 13)
#define pxyft (crb.s + 14)
#define pxft (crb.s + 15)
#define pyft (crb.s + 16)
