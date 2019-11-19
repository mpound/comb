.sh 2 "Chk(<var>,<message>,<numvals>,<val1>,<val2> . . .)"
.pp
<var> is an integer.
.pp
<message> is a pointer to a string.
.pp
<numvals> is an integer.
.pp
<val1>, <val2>, <val3>, etc. are integers.
.pp
Chk checks the value of <var> against <numvals> values (<val1>, <val2>,
etc.) and errors out with the message <message> if none of those values
is the same as <var>.
For example, Chk(test,"Sample error message",4,0,6,2,7) would error out
with the message "Sample error message" if test did not have any of the
four values 0, 6, 2, or 7.
.pp
This routine is intended mainly for checking the values of subroutine
arguments that are supposed to be one of a set of defined constants.
