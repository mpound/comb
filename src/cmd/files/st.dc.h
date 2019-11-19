extern Node stt_[];
union {
	char c[103];
	short int s[1];
	int l[1];
	double d[1];
} stb;
#define sn (stb.l + 0)
#define dt (stb.l + 1)
#define src (stb.l + 2)
#define frfre (stb.l + 3)
#define dc (stb.l + 4)
#define snt (stb.s + 10)
#define nf (stb.s + 11)
#define sft (stb.s + 12)
#define ff (stb.s + 13)
#define dtt (stb.s + 14)
#define srct (stb.s + 15)
#define autof (stb.s + 16)
#define frfret (stb.s + 17)
#define dct (stb.s + 18)
#define sf (stb.c + 38)
static int sn_o = 2;
static int frfre_o = 10;
