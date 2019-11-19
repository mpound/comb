#ifndef FILE
#include <stdio.h>
#endif
#include <time.h>

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* combedit.c */
char *edit P_((char *source, char *iedstr, int mlen));

/* eshell.c */
void eshell_ P_((char *command));

/* fparse.c */
void fparse_ P_((char *str, char *tree, char *data));

/* fromdec.c */
void decdouble P_((double *dp, int n));
void decfloat P_((float *fp, int n));
void declong P_((unsigned int *lp, int n));
void decshort P_((unsigned short *sp, int n));

/* fromvax.c */
void vaxdouble P_((double *dp, int n));
void vaxfloat P_((float *fp, int n));
void vaxlong P_((unsigned int *lp, int n));
void vaxshort P_((unsigned short *sp, int n));

/* indxsn.c */
int indxsn_ P_((int *ip));

/* jdate.c */
char *jdate P_((time_t clock));
char *j2sToDate P_((int j2second));
int  dateToJ2S P_((char *s));
int    unixToJ2S P_((time_t clock));
time_t j2sToUnix P_((int j2second));

/* math_fns.c */
double j1ox P_((double x));

/* pltoch.c */
void pltchr_ P_((double *h1, double *h2, register double *r1, register double *r2));
void pltoch_ P_((double *h1, double *h2, int *n1, int *n2));


/* readline.c */
int ReadLine P_((char *buf, FILE *fp));
int SkipLine P_((FILE *fp));
int TReadLine P_((char *buf));


/* scanlistsame.c */
int ScanListSame P_((void));

/* tran.c */
double tran_ P_((int *ufrom, int *uto, double *value));
double rtran_ P_((int *ufrom, int *uto));

/* wts.c */
void pkwts_ P_((float *totwt, int *ichan, float *tempwt, short *itmplc, short *nwts));
double wt_ P_((int *nchan, int *istack));

#undef P_
