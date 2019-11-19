extern Node ftt_[];
union {
	char c[14];
	short int s[1];
	int l[1];
	double d[1];
} ftb;
#define nchanf (ftb.l + 0)
#define wid (ftb.l + 1)
#define iacton (ftb.s + 4)
#define nchanft (ftb.s + 5)
#define widt (ftb.s + 6)
static int nchanf_o = 0;
static int wid_o = 0;
