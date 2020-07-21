extern Node cot_[];
union {
	char c[42];
	short int s[1];
	int l[1];
	double d[1];
} cob;
#define locad (cob.l + 0)
#define locsm (cob.l + 1)
#define addf (cob.l + 2)
#define dcf (cob.l + 3)
#define acf (cob.l + 4)
#define nsnlf (cob.l + 5)
#define divf (cob.l + 6)
#define locadt (cob.s + 14)
#define locsmt (cob.s + 15)
#define addft (cob.s + 16)
#define dcft (cob.s + 17)
#define acft (cob.s + 18)
#define nsnlft (cob.s + 19)
#define divft (cob.s + 20)
