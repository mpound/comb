.sh 2 "NextStk(<stacknumber>,<findflag>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTKNUM, then NextStk
errors out with the message "Looking for stack after illegal stack number
<stacknumber>."
If <stacknumber> is less than FRSTONDISK
then NextStk warns the user with the message
"Stacks in core ignored when finding next stack."
.pp
<findflag> is an integer.
If <findflag> is not FULL or EMPTY, then NextStk errors out with the message
"Illegal type of stack to look for."
.pp
NextStk finds the next stack after <stacknumber> that is either full or
empty, depending on the value of <findflag>.
It then returns the stack number of this stack.
If <stacknumber> fits the description given by <findflag> (that is:
stack <stacknumber> is full if <findflag> equals FULL, or empty if
<findflag> equals EMPTY), then NextStk returns <stacknumber>.
In other words, the search includes the stack <stacknumber>.
.pp
If there is no stack after <stacknumber> that fits the description,
then NextStk returns 0.
.pp
If there is no open directory, then NextStk errors out with the message
"No open stack directory."
