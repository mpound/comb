extern Node int_[];
union {
	char c[46];
	short int s[1];
	int l[1];
	double d[1];
} inb;
#define h1 (inb.d + 0)
#define h2 (inb.d + 1)
#define clip0 (inb.d + 2)
#define clip1 (inb.d + 3)
#define ndp (inb.l + 8)
#define h1t (inb.s + 18)
#define h2t (inb.s + 19)
#define clip0t (inb.s + 20)
#define clip1t (inb.s + 21)
#define ndpt (inb.s + 22)
static double h1_o = 0;
static double h2_o = 0;
static double clip0_o = 0.0;
static double clip1_o = 1.0;
