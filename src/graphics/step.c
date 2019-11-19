#include <math.h>
#include <string.h>
#include "step.h"

static double d60 = 60.0;
static double rtol = 0.999999999;

void ChooseDecStep(l, h, maxDivs, expand, svp)
double l, h;			/* low, high end of range to cover */
int maxDivs;			/* maximim number of steps in range */
int expand;			/* true if borders will expand to even step */
struct stepvals *svp;			/* structure to hold results */
{
	static struct stepspec stepSpecs[] = {
		{5, 1, 4}, {4, 2, 5}, {2, 1, 5}, {1, 0, 5}
	};
	double minStep, fact, step;
	int n;

	minStep = (h - l) * rtol / maxDivs;
	svp->exp = (int)floor(log10(minStep));
	fact = pow(10., (double)svp->exp);
	for(n = 3; ;) {
		step = fact / (double)stepSpecs[n].div;
		if(step > minStep) {
			svp->ss = stepSpecs[n];
			/* This is ok if we aren't expanding */
			if(expand) {
				svp->nl = floor((l + step * 0.001) / step);
				svp->nh = ceil((h - step * 0.001) / step);
				if(svp->nh - svp->nl <= maxDivs)
					return;
			} else {
				svp->nl = ceil((l - step * 0.001) / step);
				svp->nh = floor((h + step * 0.001) / step);
				return;
			}
		}

		/* do the loop iteration, adjusting exponent at the end of
		 * the stepspec table */
		if(n == 3) {
			n = 0;
			svp->exp++;
			fact *= 10.;
		} else
			n++;
	}
}

/*
 * Choose a stepsize suitable for hour:min:sec display.  If a sutiable step is
 * found, return a 1, otherwise return 0 to indicate that a decimal display
 * should be used.
 */
int ChooseHmsStep(l, h, maxDivs, expand, svp)
double l, h;			/* low, high end of range to cover */
int maxDivs;			/* maximim number of steps in range */
int expand;			/* true if borders will expand to even step */
struct stepvals *svp;		/* structure to hold results */
{
	static struct stepspec stepSpecs[] = {
		{1800, 0, 4}, {1440, 1, 5},	/* 0:00:02, 0:00:02.5 */
		{720, 0, 5}, {360, 0, 5},	/* 0:00:05, 0:00:10 */
		{180, 0, 4}, {120, 0, 6},	/* 0:00:20, 0:00:30 */
		{60, -1, 6}, {30, -1, 4},	/* 0:01, 0:02 */
		{24, 0, 5}, {12, -1, 5},	/* 0:02:30, 0:05 */
		{6, -1, 5}, {3, -1, 4},		/* 0:10, 0:20 */
		{2, -1, 6}, {1, -2, 6},		/* 0:30, 1:00 */
		{5, -2, 4}, {4, -1, 5},		/* 2:00, 2:30 */
	};
	double minStep, fact, step;
	int n;

	minStep = (h - l) * rtol / maxDivs;
	if(minStep > 2.49999999999 || minStep < .004 / 3600.)
		return(0);
	if(minStep > 1/3600.) {
		svp->exp = 0;
		fact = 1.0;
		if(minStep > 1/60.)
			n = 7;
		else
			n = 0;
	} else {
		svp->exp = -3;
		fact = 0.001;
		n = 3;
	}




	for(;;) {
		step = fact / (double)stepSpecs[n].div;
		if(step > minStep) {
			svp->ss = stepSpecs[n];
			/* This is ok if we aren't expanding */
			if(expand) {
				svp->nl = floor((l + step * 0.001) / step);
				svp->nh = ceil((h - step * 0.001) / step);
				if(svp->nh - svp->nl <= maxDivs)
					return((n==13 || n == 14)? 0:1);
			} else {
				svp->nl = ceil((l - step * 0.001) / step);
				svp->nh = floor((h + step * 0.001) / step);
				return((n==13 || n == 14)? 0:1);
			}
		}

		/* do the loop iteration, adjusting exponent at the end of
		 * the stepspec table */
		if(n == 3 && svp->exp < 0) {
			n = 0;
			if(++svp->exp)
				fact *= 10.;
			else
				fact = 1.0;
		} else {
			n++;

			/* Special case for the last two entries in stepSpecs */
			if(n == 14) {
				svp->exp = 1;
				fact = 10.;
			}

			/* special case for 0:00:00.025 which can't be printed */
			if(n == 1 && svp->exp == -2)
				n++;
		}
	}
}

HmsPrint(p,fmt,v)
char *p;		/* pointer to string to concatenate results with */
int fmt;		/* -1 for hh:mm else # of digits past '.' in sec If
 			 * fmt > 8, use fmt - 10 and leave no space for sign */
double v;		/* value */
{
	double round;
	int h,m, neg;
	static char fmtstr[][16] = {	"%2d:%02d",
					"%2d:%02d:%02.0f",
					"%2d:%02d:%04.1f",
					"%2d:%02d:%05.2f"};

	p += strlen(p);

	neg = 0;
	if(fmt > 8) {
	    fmt -= 10;
	} else {
	    if(v < 0. ) {
		neg = 1;
		v = -v;
	    }
	    *p++ = ' ';
	}
	    

	/* round to nearest lsd */
	if(fmt > 2)
		fmt = 2;
	if(fmt < 0) {
		fmt = -1;
		v += 1/120.;
		round = 0.0;
	} else {
		v += (round = 0.5 * pow(10., (double) - fmt)) / 3600.;
		/* v += (1./7200.) / exp10((double)fmt); */
	}

	v = (v - (h = (int)v)) * d60;
	v = (v - (m = (int)v)) * d60;
	if((v -= round) < 0)
		v = 0;
	sprintf(p,fmtstr[fmt + 1],h,m,v);
	if(neg)
		p[(*p == ' ')? 0: -1] = '-';
}

LabelPrint(p, fmt, v)
char *p;		/* pointer to string to concatenate results with */
int fmt;		/* position of lsd - positive to right of '.' */
double v;		/* value */
{
	int expon, prec;

	/* p += strlen(p); */
	expon = 0;
	if(fmt > 4)
		for(expon = 0; fmt + expon > 3; expon -= 3) ;
	else if(fmt < -2)
		for( ; fmt + expon < 0; expon += 3) ;
	if((prec = expon + fmt) <= 0)
		prec = 0;
	sprintf(p, (expon == 0)? "%.*f": "%.*fe%d", prec,
		v * pow(10., (double)-expon), expon);
	if(expon && v == 0) {
		if(p = strchr(p, 'e'))
			while(*p)
				*p++ = ' ';
		
	}
}

#if 0
main()
{
    double l, h, step;
    struct stepvals sv;
    int maxDivs, expand, n;
    int hmsok;
    char hmsfl[16];
    char s[64];

    printf("maxDivs expand hms(y or n) > ");
    scanf("%d %d %s", &maxDivs, &expand, hmsfl);
    for(;;) {
	printf("l, h, > ");
	scanf("%lf %lf", &l, &h);
	if(hmsfl[0] != 'y' || (hmsok = ChooseHmsStep(l, h, maxDivs, expand,
		&sv)) == 0)
		ChooseDecStep(l, h, maxDivs, expand, &sv);

	if(hmsfl[0] == 'y' && hmsok == 0)
		printf("hms selection failed\n");
/*	printf("exp = %d, div = %d, fmt = %d\n", sv.exp, sv.ss.div, sv.ss.fmt);
*/
	step = pow(10., (double)sv.exp) / sv.ss.div;
/*	printf("step = %.17g, nl = %d, nh = %d,\nnewl = %g, newh = %g\n",
		step, sv.nl, sv.nh, step * sv.nl, step * sv.nh);
*/
	for(n = sv.nl; n <= sv.nh; n++) {
		s[0] = 0;

		if(hmsfl[0] == 'y' && hmsok)
			HmsPrint(s, sv.ss.fmt - sv.exp , n * step);
		else
			LabelPrint(s, sv.ss.fmt - sv.exp , n * step);
		printf("%g\t%s\n", n * step, s);
	}
    }
}
#endif /*0*/

#if 0
main()
{
	int h, m, fmt;
	double s;
	char p[128];

	for(;;) {
		printf("h:m:s fmt > ");
		scanf("%d:%d:%lf %d", &h, &m, &s, &fmt);
		s = h + (m + s / 60.) / 60.;
		*p = 0;
		HmsPrint(p, fmt, s);
		printf("%s\n", p);
	}
}
#endif /*0*/
