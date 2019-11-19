extern Node emt_[];
union {
	char c[22];
	short int s[1];
	int l[1];
	double d[1];
} emb;
#define istak1 (emb.l + 0)
#define istak2 (emb.l + 1)
#define dt (emb.l + 2)
#define unem (emb.l + 3)
#define istakt (emb.s + 8)
#define dtt (emb.s + 9)
#define unemt (emb.s + 10)
