#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* get1.c */
int get1_ P_((void));

/* get2.c */
int get2 P_((void));

/* nrao.c */
int OpenPDFL P_((void));
int OpenSDD P_((void));
int ReadNRAO P_((int scannum));
int getNRAO P_((void));
void PrintNRAOVals P_((void));
void LastScanSDD P_((void));
void LastScanPDFL P_((void));
#undef P_
