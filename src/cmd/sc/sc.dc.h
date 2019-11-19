extern Node sct_[];
union {
	char c[8549];
	short int s[1];
	int l[1];
	double d[1];
} scb;
#define stch1 (scb.l + 0)
#define stch2 (scb.l + 1)
#define gs (scb.l + 2)
#define eofw (scb.l + 3)
#define fmtt (scb.s + 8)
#define act (scb.s + 9)
#define argst (scb.s + 10)
#define stcht (scb.s + 11)
#define srcn (scb.s + 12)
#define fnamet (scb.s + 13)
#define gst (scb.s + 14)
#define scstrt (scb.s + 15)
#define eofwt (scb.s + 16)
#define fmt (scb.c + 34)
#define args (scb.c + 162)
#define fname (scb.c + 291)
#define scstr (scb.c + 356)
static char fmt_o[128] = "%le";
static char args_o[129] = "#";
static int stch1_o = 1,stch2_o = 1;
static int gs_o = 0;
static int eofw_o = 1;
