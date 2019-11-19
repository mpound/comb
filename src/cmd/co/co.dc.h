extern Node cot_[];
union {
	char c[30];
	short int s[1];
	int l[1];
	double d[1];
} cob;
#define locad (cob.l + 0)
#define locsm (cob.l + 1)
#define addf (cob.l + 2)
#define dcf (cob.l + 3)
#define acf (cob.l + 4)
#define locadt (cob.s + 10)
#define locsmt (cob.s + 11)
#define addft (cob.s + 12)
#define dcft (cob.s + 13)
#define acft (cob.s + 14)
