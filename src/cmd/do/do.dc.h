extern Node dot_[];
union {
	char c[2099];
	short int s[1];
	int l[1];
	double d[1];
} dob;
#define to (dob.l + 0)
#define tol (dob.s + 2)
#define tot (dob.s + 3)
#define expt (dob.s + 4)
#define cmdt (dob.s + 5)
#define elcmdt (dob.s + 6)
#define elfl (dob.s + 7)
#define exp (dob.c + 16)
#define cmd (dob.c + 97)
#define elcmd (dob.c + 1098)
