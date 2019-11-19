.sh 2 "GtStkValue(<stacknumber>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTKNUM, GtStkValue errors
out with the message "Trying to get a value for illegal stack number
<stacknumber>."
If <stacknumber> is less than 4 then GtStkValue will warn the user with
the message "Getting a value from a stack in core."
<stacknumber> may be outside the range set by OkStks.
.pp
GtStkValue returns a single value for stack <stacknumber> as computed by
the macro set by SetStkMacro.
The value is arrived at by pulling the stack into stack 1 and executing
the macro.
After the macro has been executed, GtStkValue takes global variable #9
as the value.
.pp
This process is not done for every call to GtStkValue.
There is a file in the directory associated with each macro that holds
all the values that have been computed with that macro so far.
Each value is flagged when some routine like MoveStk changes some
data in the corresponding stack or when the macro is changed.
GtStkValue checks the flag.
If the flag is not set, GtStkValue returns the value found in the
file.
If the flag is set, GtStkValue computes the value (by executing the macro),
clears the flag, and returns the resulting value.
.pp
Because the computation of these values is likely to take a fair amount
of time, GtStkValue prints a message every time it has to recompute
a stack.
The first time after a call to SetStkMacro, the message is
"Please wait, computing value for stack: <stacknumber>."
After that, GtStkValue merely prints out the stack numbers as they are
computed, adding a newline every now and then to keep from printing
more than 70 characters in a line.
.pp
If there is no open directory, then GtStkValue.d errors out with the
message "No open stack directory."
