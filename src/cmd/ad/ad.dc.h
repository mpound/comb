extern Node adt_[];
union {
	char c[48];
	short int s[1];
	int l[1];
	double d[1];
} adb;
#define istep (adb.l + 0)
#define lbknd (adb.l + 1)
#define ssn (adb.l + 2)
#define ncare (adb.l + 3)
#define mhflg (adb.l + 4)
#define sn1f (adb.c + 20)
#define sn2f (adb.c + 26)
#define sn1n (adb.s + 12)
#define sn2n (adb.s + 15)
#define snt (adb.s + 16)
#define snf (adb.s + 17)
#define istept (adb.s + 18)
#define lbetyp (adb.s + 19)
#define lbkndt (adb.s + 20)
#define ssnt (adb.s + 21)
#define ncaret (adb.s + 22)
#define mhflgt (adb.s + 23)
static int ssn_o = 1;
