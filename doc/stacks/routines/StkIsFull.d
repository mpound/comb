.sh 2 "StkIsFull(<stacknumber>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTKNUM then StkIsFull
errors out with the
message "Checking existance of illegal stack <stacknumber>".
<stacknumber> may be outside the range set by OkStks.
.pp
StkIsFull checks to see if the stack <stacknumber> exists (is full).
If it is, StkIsFull returns 1, otherwise, it returns 0.
.pp
If there is no open directory, then StkIsFull errors out with the message
"No open stack directory."
