extern Node nst_[];
union {
	char c[103];
	short int s[1];
	int l[1];
	double d[1];
} nsb;
#define dt (nsb.l + 0)
#define prn (nsb.l + 1)
#define psh (nsb.l + 2)
#define clf (nsb.l + 3)
#define tf (nsb.l + 4)
#define qf (nsb.l + 5)
#define fst (nsb.s + 12)
#define dtt (nsb.s + 13)
#define prnt (nsb.s + 14)
#define psht (nsb.s + 15)
#define clft (nsb.s + 16)
#define tft (nsb.s + 17)
#define qft (nsb.s + 18)
#define fs (nsb.c + 38)
static char fs_o[65] = "";
