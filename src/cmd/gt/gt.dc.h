extern Node gtt_[];
union {
	char c[91];
	short int s[1];
	int l[1];
	double d[1];
} gtb;
#define istep (gtb.l + 0)
#define fss (gtb.l + 1)
#define first (gtb.l + 2)
#define last (gtb.l + 3)
#define lbknd (gtb.l + 4)
#define ssn (gtb.l + 5)
#define tsfl (gtb.l + 6)
#define tflg (gtb.l + 7)
#define pfl (gtb.l + 8)
#define sn1f (gtb.c + 36)
#define sn2f (gtb.c + 42)
#define sn1n (gtb.s + 20)
#define sn2n (gtb.s + 23)
#define snt (gtb.s + 24)
#define snf (gtb.s + 25)
#define istept (gtb.s + 26)
#define fsst (gtb.s + 27)
#define firstt (gtb.s + 28)
#define lastt (gtb.s + 29)
#define lmttyp (gtb.s + 30)
#define lbetyp (gtb.s + 31)
#define lbkndt (gtb.s + 32)
#define ssnt (gtb.s + 33)
#define typet (gtb.s + 34)
#define typef (gtb.s + 35)
#define tsflt (gtb.s + 36)
#define tflgt (gtb.s + 37)
#define pflt (gtb.s + 38)
#define type (gtb.c + 78)
static int ssn_o = 1;
