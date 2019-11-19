extern Node ust_[];
union {
	char c[52];
	short int s[1];
	int l[1];
	double d[1];
} usb;
#define h1 (usb.d + 0)
#define h2 (usb.d + 1)
#define rtflg (usb.l + 4)
#define coflg (usb.l + 5)
#define actfl (usb.l + 6)
#define stflg (usb.l + 7)
#define rfflg (usb.l + 8)
#define ht (usb.s + 18)
#define hflg (usb.s + 19)
#define rtflgt (usb.s + 20)
#define coflgt (usb.s + 21)
#define iactn (usb.s + 22)
#define actflt (usb.s + 23)
#define stflgt (usb.s + 24)
#define rfflgt (usb.s + 25)
