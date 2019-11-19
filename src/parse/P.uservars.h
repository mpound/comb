#define GLOBAL 0
#define LOCAL 1

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* P.uservars.c */
HdrVar *NewUserVar P_((int where, char *name, int type, int length));
HdrVar *SearchUserVar P_((int where, char *name));
void StoreUserStringVar P_((HdrVar *hv, char *string));
void CleanUserVars P_((void));
void PrintUserVars P_((int where));

#undef P_
