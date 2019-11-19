.sh 2 "AddScNum(<stacknumber>,<filename>,<scannumber>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTKNUM, then AddScNum
errors out with the message
"Adding scan number to illegal stack number <stacknumber>."
If <stacknumber> is equal to or greater than FRSTONDISK,
then AddScNum errors out with the message
"Can't add scan number to stack not in core."
<stacknumber> may be outside the bounds set by OkStks.
.pp
<filename> is a pointer to a string of characters.
If <filename> is not three characters long, AddScNum errors out with
the message "File name of scan number must be three characters long."
If <filename> does not begin with an alphabetic character, or not all the
characters in <filename> are alphanumeric, then AddScNum errors out with
the message "Illegal scan file name <filename><scannumber>."
.pp
<scannumber> is an integer.
If <scannumber> is less than 0 or greater than MAXSCANNUM, then AddScNum
errors out with the message "Illegal scan number <filename><scannumber>."
.pp
AddScNum adds the scan number described by <filename> and <scannumber>
to the stack <stacknumber>.
(Note: this isn't adding any values, it is simply appending the scan number
onto the list of scan numbers in the stack.)
.pp
If stk_[<stacknumber> - 1].nstk is full, then AddScNum will stick the stack
on stk_[<stacknumber> - 1].nstkx.
If stk_[<stacknumber> - 1].nstkx is full, then AddScNum will error out with
the message "Too many scans in stack <stacknumber>."
.pp
If stack <stacknumber> does not exist, then AddScNum errors out with the
message "No stack <stacknumber>."
.pp
See documentation for struct of_stk for more on the way scan numbers
are stored.
