.sh 2 "SetStkPath(<path>)"
.pp
<path> is a string.
.pp
SetStkPath sets the default directory path to <path>.
If <path> does not describe a real directory, then SetStkPath errors
out with a message provided by ChkAccess.
When OpenStkDir is called with a path name that does not begin at the root
(that is, the string doesn't start with "/"), <path> will be prepended
to it.
Thus, if SetStkPath is called with "/ua/mlm/" as an argument, and
OpenStkDir is called with "S187" as an argument, then the directory that
will be openned is "/ua/mlm/S187."
SetStkPath will work equally well when called with "/ua/mlm" as when
called with "/ua/mlm/" -- the ending slash is added if it isn't there.
It will also work with paths that start in the current directory
rather than the root.
A special case of such strings is a null string, meaning that the current
directory is the default directory path.
