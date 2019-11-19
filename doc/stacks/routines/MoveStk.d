.sh 2 "MoveStk(<from>,<to>,<warnflag>)"
.pp
<from> is an integer.
If <from> is less than 1 or greater than MAXSTKNUM then MoveStk errors out
with the message "Retrieving illegal stack number <from>."
<from> may be outside the bounds set by OkStks.
.pp
<to> is an integer.
If <to> is less than 1 or greater than MAXSTKNUM then MoveStk errors out
with the message "Storing to an illegal stack number <to>."
If <from> is equal to <to> then MoveStk warns the user with the message
"Retrieving from and storing to same stack."
<to> may be outside the bounds set by OkStks.
.pp
<warnflag> is an integer.
If <warnflag> is not equal to DONTWARN or OKTOWARN then MoveStk errors
out with the message "Illegal warn flag for storing stack."
.pp
MoveStk gets stack number <from> and stores it into stack number <to>.
If <from> is less than FRSTONDISK,
then the stack is retrieved from memory,
otherwise it is retrieved from disk.
If <to> is less than FRSTONDISK,
then the stack is stored to memory, otherwise
it is stored to disk and the stack number in the header is changed to
<to>.
.pp
If stack number <to> is full, then the behavior of MoveStk depends on
the value of <warnflag>.
If <warnflag> is DONTWARN, then MoveStk does nothing special; it simply
overwrites the old stack.
If <warnflag> is OKTOWARN, then MoveStk warns the user with the message
"Stack <to> already exists."
.pp
If there is no open directory, then MoveStk errors out with the message
"No open stack directory."
