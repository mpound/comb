extern Node lit_[];
union {
	char c[20];
	short int s[1];
	int l[1];
	double d[1];
} lib;
#define wid (lib.d + 0)
#define nordr (lib.l + 2)
#define nordrt (lib.s + 6)
#define widt (lib.s + 7)
#define widfl (lib.s + 8)
#define iacton (lib.s + 9)
static int nordr_o = 1;
