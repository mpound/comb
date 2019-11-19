/* The definition for FSafeOpen and FChkOpen require a prev inclusion
 * of <stdio.h> */

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* alloc.c */
char *ChkAlloc P_((int size, char *name));
char *ChkRealloc P_((char *pntr, int size, char *name));
void ChkFree P_((char *memory));
char *SafeAlloc P_((int size, char *name));
void SafeFree P_((char *memory));
void ReleaseAllSafe P_((void));

#undef P_
#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* file.c */
int ChkAccess P_((char *file, char *path, int mode, int warn, char *mssg));
int SafeOpen P_((char *file, char *path, int opType, int mode));
FILE *FSafeOpen P_((char *file, char *path, char *opStr));
int ChkOpen P_((char *file, char *path, int opType, int mode));
FILE *FChkOpen P_((char *file, char *path, char *opStr));
void SafeClose P_((int filDes));
void FSafeClose P_((FILE *filP));
void CloseAll P_((void));

#undef P_
