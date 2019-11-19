.sh 2 "ChkAccess(<file>,<path>,<mode>,<warnflag>,<mssg>)"
.pp
<file> is a pointer to a string.
.pp
<path> is a pointer to a string.
.pp
<mode> is an integer.
.pp
<warnflag> is an integer.
.pp
<mssg> is a pointer to a string.
It is only needed if <warnflag> is 1.
.pp
ChkAccess checks the accessibility of a file with the system call access.
<path> is the pathname of the file to be checked, and <mode> is the
mode argument of access.
Thus, ChkAccess calls access(<path>,<mode>).
.pp
<file> is what the file should be called in an error message (e.g.
"the default file for stacks directory /ua/mlm/STKS," as opposed to
the pathname, "/ua/mlm/STKS/default").
.pp
If <warnflag> is 0, then ChkAccess will error out with a meaningful
message when access returns -1.
If <warnflag> is 1, then ChkAccess will warn the user with a meaningful
message when access returns -1, and ChkAccess will then return 1.
<mssg> will be appended to the end of the warning message.
It should be used to tell the user what will happen if he lets the
command continue.
.pp
If access does not return -1, then ChkAccess returns 0.
