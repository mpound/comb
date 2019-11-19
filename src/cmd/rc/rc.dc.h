extern Node rct_[];
union {
	char c[125];
	short int s[1];
	int l[1];
	double d[1];
} rcb;
#define cc1 (rcb.d + 0)
#define cc2 (rcb.d + 1)
#define cep (rcb.d + 2)
#define rota (rcb.d + 3)
#define ep1 (rcb.d + 4)
#define ep2 (rcb.d + 5)
#define t1 (rcb.d + 6)
#define t2 (rcb.d + 7)
#define rcsnmt (rcb.s + 32)
#define ctypf (rcb.s + 33)
#define ctyp (rcb.s + 34)
#define cc1t (rcb.s + 35)
#define cc2t (rcb.s + 36)
#define cept (rcb.s + 37)
#define ofl (rcb.s + 38)
#define otyp (rcb.s + 39)
#define uufl (rcb.s + 40)
#define uutyp (rcb.s + 41)
#define rotef (rcb.s + 42)
#define rote (rcb.s + 43)
#define rotat (rcb.s + 44)
#define ep1t (rcb.s + 45)
#define ep2t (rcb.s + 46)
#define t1t (rcb.s + 47)
#define t1f (rcb.s + 48)
#define t2t (rcb.s + 49)
#define t2f (rcb.s + 50)
#define act (rcb.s + 51)
#define rcsnm (rcb.c + 104)
