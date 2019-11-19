.sh 2 "FrstScNum(<stacknumber>,<filename>,<scannumber>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTACKNUM, then FrstScNum
errors out with the message "Getting first scan number of illegal stack
<stacknumber>."
If <stacknumber> is greater than or equal to FRSTONDISK,
then FrstScNum errors out with the message
"Can't get scan number out of stack not in core."
<stacknumber> may be outside the range specified by OkStks.
.pp
<filename> is a pointer to a character string.
.pp
<scannumber> is a pointer to an integer.
.pp
FrstScNum gets the first scan number out of the in-core stack <stacknumber>,
putting its file name in <filename>, and its number in <scannumber>.
.pp
If stack <stacknumber> does not exist, then FrstScNum errors out with the
message "No stack <stacknumber>."
.pp
FrstScNum is intended to be used in conjunction with NxtScNum
(see NxtScNum).
