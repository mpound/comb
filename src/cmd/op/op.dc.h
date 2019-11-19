extern Node opt_[];
union {
	char c[169];
	short int s[1];
	int l[1];
	double d[1];
} opb;
#define apf (opb.l + 0)
#define ucl (opb.l + 1)
#define ywf (opb.l + 2)
#define msg (opb.l + 3)
#define nosho (opb.l + 4)
#define apft (opb.s + 10)
#define uclt (opb.s + 11)
#define spt (opb.s + 12)
#define proj (opb.s + 13)
#define projf (opb.s + 14)
#define htyp (opb.s + 15)
#define htypef (opb.s + 16)
#define ywft (opb.s + 17)
#define msgt (opb.s + 18)
#define noshot (opb.s + 19)
#define sp (opb.c + 40)
