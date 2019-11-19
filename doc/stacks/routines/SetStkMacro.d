.sh 2 "SetStkMacro(<macro>)"
.pp
<macro> is a string.
.pp
SetStkMacro sets the macro to be used in computing single values of
stacks (see GtStkValue).
If the macro is not defined in the stacks directory, then
SetStkMacro will error out with the message "No value macro <macro> in
directory <dir>," where <dir> is the name of the directory.
.pp
If there is no open directory, SetStkMacro errors out with the message
"No open stack directory."
