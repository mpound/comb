.sh 2 "NxtScNum(<stacknumber>,<filename>,<scannumber>)"
.pp
<stacknumber> is an integer.
If <stacknumber> is less than 1 or greater than MAXSTACKNUM, then NxtScNum
errors out with the message "Getting scan number out of illegal stack
<stacknumber>."
If <stacknumber> is greater than or equal to FRSTONDISK,
then NxtScNum errors out with the message
"Can't get scan number out of stack not in core."
<stacknumber> may be outside the range set by OkStks.
.pp
<filename> is a pointer to a character string.
.pp
<scannumber> is a pointer to an integer.
.pp
NxtScNum gets the next scan number out of the in-core stack <stacknumber>,
putting its file name in <filename>, and its number in <scannumber>.
.pp
If there are no more scan numbers left, then NxtScNum returns 0 and leaves
<filename> and <scannumber> alone, otherwise it returns 1.
.pp
NxtScNum is meant to be used in conjunction with FrstScNum.
For example, to print out all the scan numbers in stack 1, the following
code might be used:
.in +8
.(l I
char name[4];		/* file name */
int number;		/* scan number */
		.
		.
		.

/* start at beginning of list */
FrstScNum(1,name,&number);

/* print out scans until */
/* there aren't any more */
do
	printf("%s%d\n",name,number);
while(NxtScNum(1,name,&number));
.)l
.in -8
.pp
If stack <stacknumber> does not exist, then NxtScNum will error out with
the message "No stack <stacknumber>."
