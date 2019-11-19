#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* T.routines.c */

TFILE Tcreate P_((char *name, int order, int permission));
TFILE Topen P_((char *name, int readOnly));
int Tclose P_((register TFILE tfile));
int ThdrPrint P_((TFILE tfile));
int MakeRoot P_((register TFILE tfile, NODE *node));
void Root P_((TFILE tfile, NODE *node));
int MakeChildren P_((register NODE *node, int heir, NODE *child1, NODE *child2));
void Child P_((register NODE *node, int childNum, register NODE *child));
int RemoveChildren P_((register NODE *node));
void GarbCollect P_((register TFILE tfile));
void CleanUp P_((register TFILE tfile));
int ListBuf P_((register TFILE tfile));
int ListHead P_((register TFILE tfile));

#undef P_
