.sh 2 "ChkOpen(<file>,<path>,<mode>)"
.pp
<file> is a pointer to a string.
.pp
<path> is a pointer to a string.
.pp
<mode> is an integer.
.pp
ChkOpen opens a file with the command open(<path>,<mode>).
.pp
If open returns -1, then ChkOpen errors out with a meaningful message,
using <file> to describe what file caused the trouble to the user.
Thus <file> should be a human description of the file (e.g. "the default
file for stacks directory /ua/mlm/STKS").
.pp
If open does not return -1, then ChkOpen returns what open returned
(the file descriptor).
