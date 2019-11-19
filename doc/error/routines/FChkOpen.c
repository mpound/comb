.sh 2 "FChkOpen(<file>,<path>,<mode>)"
.pp
<file> is a pointer to a string.
.pp
<path> is a pointer to a string.
.pp
<mode> is a pointer to a string.
.pp
FChkOpen is the same as ChkOpen except that it uses fopen(<path>,<mode>)
to open the file, and returns the file pointer.
FChkOpen is of type FILE *.
