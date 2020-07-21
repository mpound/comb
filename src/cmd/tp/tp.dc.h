extern Node tpt_[];
union {
	char c[30];
	short int s[1];
	int l[1];
	double d[1];
} tpb;
#define stf (tpb.l + 0)
#define redf (tpb.l + 1)
#define clrf (tpb.l + 2)
#define prtf (tpb.l + 3)
#define tabf (tpb.l + 4)
#define stft (tpb.s + 10)
#define redft (tpb.s + 11)
#define clrft (tpb.s + 12)
#define prtft (tpb.s + 13)
#define tabft (tpb.s + 14)
