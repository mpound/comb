extern Node dvt_[];
union {
	char c[2076];
	short int s[1];
	int l[1];
	double d[1];
} dvb;
#define lclf (dvb.l + 0)
#define pgf (dvb.l + 1)
#define plf (dvb.l + 2)
#define nnamt (dvb.s + 6)
#define nfl (dvb.s + 7)
#define snamt (dvb.s + 8)
#define sfl (dvb.s + 9)
#define lclft (dvb.s + 10)
#define pgft (dvb.s + 11)
#define plft (dvb.s + 12)
#define nnam (dvb.c + 26)
#define snam (dvb.c + 1051)
