extern Node rtt_[];
union {
	char c[684];
	short int s[1];
	int l[1];
	double d[1];
} rtb;
#define c1 (rtb.d + 0)
#define c2 (rtb.d + 1)
#define ptol (rtb.d + 2)
#define fr1 (rtb.d + 3)
#define fr2 (rtb.d + 4)
#define sn1 (rtb.l + 10)
#define sn2 (rtb.l + 11)
#define dt (rtb.l + 12)
#define su (rtb.l + 13)
#define dest (rtb.l + 14)
#define tflg (rtb.l + 15)
#define stk1 (rtb.l + 16)
#define stk2 (rtb.l + 17)
#define msn (rtb.l + 18)
#define snt (rtb.s + 38)
#define sft (rtb.s + 39)
#define ff (rtb.s + 40)
#define dtt (rtb.s + 41)
#define sut (rtb.s + 42)
#define destt (rtb.s + 43)
#define tflgt (rtb.s + 44)
#define c1t (rtb.s + 45)
#define c1fl (rtb.s + 46)
#define c2t (rtb.s + 47)
#define c2fl (rtb.s + 48)
#define ptolt (rtb.s + 49)
#define ptfl (rtb.s + 50)
#define frt (rtb.s + 51)
#define frfl (rtb.s + 52)
#define stkt (rtb.s + 53)
#define skfl (rtb.s + 54)
#define msfl (rtb.s + 55)
#define msnt (rtb.s + 56)
#define cmdt (rtb.s + 57)
#define cmdfl (rtb.s + 58)
#define sf (rtb.c + 118)
#define cmd (rtb.c + 183)
static int sn1_o = 2,sn2_o = 2;
static int su_o = 0;
static double c1_o = 0;
static double c2_o = 0;
static double ptol_o = 0.3;
static double fr1_o = 0,fr2_o = 990000;
static int stk1_o = 0,stk2_o = MAXSTKNUM;
static int msn_o = 1;
