.sh 2 "Error_(<message>)"
.pp
<message> is a pointer to a string.
.pp
Error_ handles an error in COMB.
It prints information about the error, including <message>, and then
jumps back to the main loop.
.pp
The information is printed out as follows:
.in +8
.(l I

Error
<message>
while executing command <command>

.)l
.in -8
Error_ also beeps twice.
If the error occured after a NoInt_ had been executed and before a
YesInt_, then Error_ will add the message:
.in +8
.(l I

BEWARE: Possible damage to data may have resulted.

.)l
.in -8
It will then call YesInt_.
(See documentation for NoInt_ and YesInt_.)
