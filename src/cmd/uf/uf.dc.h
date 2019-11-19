extern Node uft_[];
union {
	char c[51];
	short int s[1];
	int l[1];
	double d[1];
} ufb;
#define namt (ufb.s + 0)
#define nam (ufb.c + 2)
static char nam_o[49] = "";
