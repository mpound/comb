/* These routines were originally written for BigEndian
 * machines. Some modification to vaxdouble is inserted
 * for LittleEndian machines.  Also, some explanation of
 * VAX & IEEE floating point is now included in vaxfloat()
 * 
 * These calls have the form
 *   vaxtype( type *p, int n)
 * where type is the variable type and n is the number of
 * variables of type "type" to convert.
 * Note the parameter "n" in these calls and those in
 * fromdec.c can't be negative or you get an infinite loop!
 * - MWP & RWW 1999-May-04
 */
#include <stdio.h>
#include "../main/machinedep.h"
#define DNAN 0x7ff30000
#define RMASK 0x00ff00ff
#define LMASK 0xff00ff00
void vaxdouble(double *dp, int n)
{
	int expAndSign, sign;
	register unsigned int de, df;
	register unsigned int rmask = RMASK, lmask = LMASK;

	if(n<0) /* just to be safe */
		error("Bad call to vaxdouble: n<0");
#if BYTEREVERSED
	decdouble(dp,n); /* for LittleEndian, swap the bytes to start */
#endif
	for(; n--; dp++) {
#if BYTEREVERSED
		df = *(int *)dp;
		de = ((int *)dp)[1];
#else
		de = *(int *)dp;
		df = ((int *)dp)[1];
#endif
		de = ((de >> 8) & rmask) | ((de << 8) & lmask); 
		df = ((df >> 8) & rmask) | ((df << 8) & lmask); 
		expAndSign = de >> 23;
		sign = de & 0x80000000;
		if(expAndSign == 0 || (expAndSign & 0xff) < 2) {
			*(int *)dp = 0;
			((int *)dp)[1] = 0;
		} else if(expAndSign == 0x100)
#if BYTEREVERSED
			((int *)dp)[1] = DNAN;
		else {
			df = (df >> 3) | de << 29;
			de = (de >> 3) & 0x0fffffff;
			*(int *)dp = df;
			((int *)dp)[1] = ((de + 0x37e00000) & 0x7fffffff) | sign;
		}
#else
			*(int *)dp = DNAN;
		else {
			df = (df >> 3) | de << 29;
			de = (de >> 3) & 0x0fffffff;
			*(int *)dp = ((de + 0x37e00000) & 0x7fffffff) | sign;
			((int *)dp)[1] = df;
		}
#endif
	}
}

#define EXPCORR 0x01000000
#define NAN 0x7f8f0000
#define RMASK 0x00ff00ff
#define LMASK 0xff00ff00
void vaxfloat(float *fp, int n)
{
	int expAndSign;
	register int t;
	register unsigned int rmask = RMASK, lmask = LMASK;

	if(n<0) /* just to be safe */
		error("Bad call to vaxfloat: n<0");
#if BYTEREVERSED
	decfloat(fp,n); /* for LittleEndian, swap the bytes to start */
#endif
	for(; n--; fp++) {
		t = *(int *)fp;
/*
 * The VAX floating point is stored in a whacky way, with the sign at
 * bit 15, the exponent at bits 14-7 and bits 6-0 and 31-16 containing
 * the mantissa (6-0 = MSW, 16-23 = LSW; bits in increasing order
 * of significance).
 * 
 * The next line of code exchanges the word order so that it is (I think)
 *
 * Meaning: | Sign| Exp    |              Mantissa                 |
 *          |     |        | fraction MSW| fraction | fraction LSW | 
 * Bits:    | 15  | 14 - 7 | 6 - 0       | 31 - 24  | 16 - 23      |
 *
 * This result is the 32-bit IEEE-754 floating point, i.e.
 *
 *  1.The sign bit is 0 FOR positive, 1 for negative. 
 *  2.The exponent's base is two. 
 *  3.The exponent field contains 127 plus the true exponent for 
 *    single-precision, or 1023 plus the true exponent for double precision. 
 *  4.The first bit of the mantissa is assumed to be 1, and is not 
 *    stored explicitly
 *
 * NB: double precision is similar except it has 32 more bits.
 */
		t = ((t >> 8) & rmask) | ((t << 8) & lmask); 
/* Now, shift right by 23 to isolate bits 15-7 and get the sign and 
 * exponent.
 */
		expAndSign = (unsigned)t >> 23;
/* From the 1980 VAX 11 manual:
 * "an exponent value of 0 together with a sign bit of 0 is taken [as] 0"
 */
		if(expAndSign == 0 || (expAndSign & 0xff) < 2)
			*(int *)fp = 0;
/* From the 1980 VAX 11 manual:
 * "an exponent vaue of 0 togeher with a sign bit of 1 is taken as reserved"
 */
		else if(expAndSign == 0x100)
			*(int *)fp = NAN;
		else
/* remove the "bias" of 127 */
			*(int *)fp = t - EXPCORR;
	}
}

void vaxlong(unsigned int *lp, int n)
{
	unsigned int t;
	unsigned int lmask = 0x00ff0000, rmask = 0x0000ff00;

	if(n<0) /* just to be safe */
		error("Bad call to vaxlong: n<0");
	for(; n--; lp++) {
		t = *lp;
		*lp = (t >> 24) | (t << 24) | ((t << 8) & lmask) |
			((t >> 8) & rmask); 
	}
}

void vaxshort(unsigned short *sp, int n)
{
	register unsigned int t;
	register unsigned int lmask = 0xff00, rmask = 0x00ff;

	if(n<0) /* just to be safe */
		error("Bad call to vaxshort: n<0");
	for(; n--; sp++) {
		t = *sp;
		*sp = ((t << 8) & lmask) | ((t >> 8) & rmask); 
	}
}
/*
vaxshort(unsigned short *sp, int n)
{
	register int t;
	register char *cp;

	for(cp = (char *)sp; n--; cp +=2) {
		t = cp[1];
		cp[1] = cp[0];
		cp[0] = t;
	}
}
*/
