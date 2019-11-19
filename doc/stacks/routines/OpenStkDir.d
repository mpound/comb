.sh 2 "OpenStkDir(<path>,<makeflag>)"
.pp
<path> is a string.
.pp
<makeflag> is an integer.
If <makeflag> is neither DONTMAKE or OKTOMAKE,
OpenStkDir errors out with the
message "Illegal make flag for openning directory."
.pp
OpenStkDir opens the stacks directory described by <path> and the previous
call to SetStkPath (see SetStkPath).
If the directory exists, it is opened as either the foreground or background
directory depending on the last call to SetStkDir.
.pp
The various files that must be opened in the directory are opened with
ChkOpen and FChkOpen, so if someone's been messing with the directory
and fouled things up OpenStkDir will error out with a meaningful message.
.pp
The existance of the directory is checked with ChkAccess.
.pp
If the directory does not exist then the behavior of OpenStkDir depends on
the value of <makeflag>.
If <makeflag> is DONTMAKE,
OpenDir errors out with a message provided by ChkAccess.
If <makeflag> is OKTOMAKE, OpenStkDir warns the user with a message provided
by ChkAccess + "Will try to create it,"
and, if the user continues, goes on to create the
directory it is to open.
It then opens that directory.
(Note: the various error messages described above use the full name of
the directory; that is: the path name set by SetStkPath + <path>.)
.pp
Finally, before opening all the files, OpenStkDir will make a more complete
call to ChkAccess that will cause it to error out if permission to use the
stack directory has been denied.
