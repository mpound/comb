extern Node gst_[];
union {
	char c[34];
	short int s[1];
	int l[1];
	double d[1];
} gsb;
#define fnum (gsb.l + 0)
#define lbknd (gsb.l + 1)
#define refstk (gsb.l + 2)
#define hotstk (gsb.l + 3)
#define fnumt (gsb.s + 8)
#define lbetyp (gsb.s + 9)
#define lbkndt (gsb.s + 10)
#define refsp (gsb.s + 11)
#define refstkt (gsb.s + 12)
#define refstkf (gsb.s + 13)
#define hotsp (gsb.s + 14)
#define hotstkt (gsb.s + 15)
#define hotstkf (gsb.s + 16)
static int fnum_o = 1;
static int refstk_o = 2;
static int hotstk_o = 3;
