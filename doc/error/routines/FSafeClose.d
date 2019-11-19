.sh 2 "FSafeClose(<filpoint>)"
.pp
<filpoint> is a file pointer (FILE *).
.pp
FSafeClose is the same as SafeClose except that it closes files that
were opened with FSafeOpen.
