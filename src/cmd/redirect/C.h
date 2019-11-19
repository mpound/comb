#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* redirect.c */
void RedirStdout P_((char *fname, register int truncate));
void PipeStdout P_((char *command));
int PageStdout P_((void));
void ResetStdout P_((void));
void RedirStdin P_((char *fname));
void RedirToStr P_((char *StrNam));

#undef P_
