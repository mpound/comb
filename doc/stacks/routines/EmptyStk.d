.sh 2 "EmptyStk(<stacknumber>,<warnflag>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTKNUM then EmptyStk will
error out with the message "Emptying illegal stack number <stacknumber>."
<stacknumber> may be outside the bounds set by OkStks.
.pp
<warnflag> is an integer.
If <warnflag> is not DONTWARN or OKTOWARN then EmptyStk will
error out with the message "Illegal warn flag for emptying stack."
.pp
EmptyStk virtually empties stack number <stacknumber>.
That is: it sets a flag saying that the stack is empty, but it doesn't
actually erase anything until the space occupied by the stack is needed.
ReStk can be used to restore a stack that has not been erased (see
ReStk).
.pp
If the stack is already empty, then the behavior of EmptyStk depends on
<warnflag>.
If <warnflag> is equal to DONTWARN, then EmptyStk does nothing.
If <warnflag> is equal to OKTOWARN, then EmptyStk warns the user with
the message "Stack <stacknumber> already empty," and then does nothing.
.pp
If there is no open directory, EmptyStk errors out with the message
"No open stack directory."
