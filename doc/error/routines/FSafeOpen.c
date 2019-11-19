.sh 2 "FSafeOpen(<file>,<path>,<mode>)"
.pp
<file> is a pointer to a string.
.pp
<path> is a pointer to a string.
.pp
<mode> is a pointer to a string.
.pp
FSafeOpen is the same as SafeOpen except that it calls FChkOpen
instead of ChkOpen to open the file.
FSafeOpen is of type FILE *.
