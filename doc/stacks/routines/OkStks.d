.sh 2 "OkStks(<low>,<high>)"
.pp
<low> is an integer.
If <low> is less than 1 or greater than MAXSTKNUM
then OkStks errors out with the message "Illegal
stack number <low>."
If <low> is less than FRSTONDISK
then OkStks warns the user with the message
"Lower bound of stack range in core."
.pp
<high> is an integer.
If <high> is less than 1 or greater than MAXSTKNUM then OkStks errors out
with the message "Illegal stack number <high>."
If <high> is less than <low> then OkStks errors out with the message
"High bound of stack range less than low bound."
If <high> is less than FRSTONDISK
then OkStks warns the user with the message
"Entire stack range in core."
.pp
OkStks restricts the range of stacks that will be used in further
operations.
All subsequent operations will ignore any stack outside of this range,
unless the stack is specifically asked for -- that is, passed as a
parameter.
The upper (<high>) and lower (<low>) bounds of the range are inclusive.
.pp
If there is no open directory, then OkStks errors out with the message
"No open stack directory."
