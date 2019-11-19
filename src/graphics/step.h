struct stepspec {
	short div;		/* divisor */
	char fmt;		/* for decimal step, location of lsd wrt '.',
				 * for hms steps, see  hms() */
	char minor;		/* number of minor divisions / maj div. */
};

struct stepvals {
	struct stepspec ss;
	int exp;		/* step = exp10((double)exp) / ss.div */
	int nl, nh;		/* step number of low and high boundary */
};


#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* step.c */
void ChooseDecStep P_((double l, double h, int maxDivs, int expand, struct stepvals *svp));
int ChooseHmsStep P_((double l, double h, int maxDivs, int expand, struct stepvals *svp));
int HmsPrint P_((char *p, int fmt, double v));
int LabelPrint P_((char *p, int fmt, double v));

#undef P_
