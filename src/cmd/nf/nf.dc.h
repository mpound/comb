extern Node nft_[];
union {
	char c[137];
	short int s[1];
	int l[1];
	double d[1];
} nfb;
#define prn (nfb.l + 0)
#define nflnt (nfb.s + 2)
#define prnt (nfb.s + 3)
#define nfln (nfb.c + 8)
