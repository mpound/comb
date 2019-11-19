.sh 2 "ReStk(<stacknumber>,<warnflag>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTKNUM then ReStk errors
out with the message "Restoring illegal stack number <stacknumber>."
If <stacknumber> is less than FRSTONDISK
then ReStk errors out with the message
"Can't restore in core stack number <stacknumber>."
<stacknumber> may be outside the range set by OkStks.
.pp
<warnflag> is an integer.
If <warnflag> is not DONTWARN or OKTOWARN then ReStk errors out with the
message "Illegal warning flag for restoring."
.pp
Restore restores a stack that has been virtually emptied by EmptyStk but
whose actual data has not yet been erased.
It simply resets the flag set by EmptyStk after checking that the data is
still there.
If the data is not there, then ReStk errors out with the message
"Sorry - too late to restore stack <stacknumber>."
.pp
If there is no open directory, ReStk errors out with the message
"No open stack directory."
.pp
If the stack has not been emptied by EmptyStk (that is: the stack is still
full), then the behavior of ReStk depends on the value of <warnflag>.
If <warnflag> is DONTWARN then ReStk does nothing.
If <warnflag> is OKTOWARN then ReStk warns the user with the message
"Stack <stacknumber> has not been emptied," and then does nothing.
