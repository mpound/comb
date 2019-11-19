.sh 2 "RdCsys(<name>,<file>)"
.pp
<name> is a pointer to a string.
.pp
<file> is an integer.
If <file> is not LOCAL, ANY, or GLOBAL, then RdCsys errors out with the
message "Illegal file selector for reading coordinate system."
.pp
RdCsys reads the relative coordinate system named <name>
from either the local or
global coordinate system file depending on the value of <file>.
If <file> is LOCAL, the coordinate system is read from the local file.
If <file> is GLOBAL, the coordinate system is read from the global file.
If <file> is ANY, RdCsys first tries to read the system from the local
file.
If it isn't there, then the system is read from the global file.
.pp
If there is no coordinate system named <name> in the file that RdCsys
is supposed to read it from, then it will error out with one of the
following three messages:
if <file> is LOCAL, the message is "No coordinate system <name> in local file,"
if <file> is GLOBAL, the message is
"No coordinate system <name> in global file,"
if <file> is ANY, the message is
"No coordinate system <name>."
