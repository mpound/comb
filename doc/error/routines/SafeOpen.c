.sh 2 "SafeOpen(<file>,<path>,<mode>)"
.pp
<file> is a pointer to a string.
.pp
<path> is a pointer to a string.
.pp
<mode> is an integer.
.pp
SafeOpen calls ChkOpen(<file>,<path>,<mode>), and returns the result.
The difference between SafeOpen and ChkOpen is that SafeOpen puts the
file descriptor on a list of files to be closed by CloseAll or an error.
Thus, SafeOpen keeps the file safe from various kinds of interruptions.
.pp
If one COMB command involves both opening and closing a file, then
SafeOpen and SafeClose should be used.
