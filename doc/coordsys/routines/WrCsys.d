.sh 2 "WrCsys(<name>,<file>,<bombflag>)"
.pp
<name> is a pointer to a character string.
.pp
<file> is an integer.
If <file> is not LOCAL or GLOBAL, then WrCsys errors out with the message
"Illegal file selector."
.pp
<bombflag> is an integer.
If <bombflag> is not OKTOWARN, OKTOBOMB, DONTWARN, or DONTBOMB, then
WrCsys errors out with the message "Illegal bomb flag."
.pp
WrCsys writes the current coordinate system to either the local or global
coordinate system file, depending on whether <file> is LOCAL or GLOBAL
respectively.
.pp
It first checks to see if there is a coordinate system already defined
in that file as having the name <name>.
If there is, then its behavior depends on the value of <bombflag>.
If <bombflag> is OKTOWARN, WrCsys warns the user with the message
"System <name> already exists in <which> file" (where <which> is either
"local" or "global," depending on whether <file> is LOCAL or GLOBAL,
respectively).
It then goes on to replace the old coordinate system <name> with the
current coordinate system.
If <bombflag> is OKTOBOMB, then WrCsys errors out with the same message
as above.
If <bombflag> is DONTWARN or DONTBOMB, then WrCsys replaces the old
coordinate system <name> with the current coordinate system.
.pp
If there is no coordinate system named <name> in the file selected by
<file>, then WrCsys adds the current coordinate system to the file with
the name <name>.
