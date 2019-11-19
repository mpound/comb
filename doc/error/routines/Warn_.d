.sh 2 "Warn_(<message>)"
.pp
<message> is a pointer to a string.
.pp
Warn_ gives the warning message <message> to the user and allows him
to decide whether or not his command string should continue to be
executed.
.pp
When Warn_ is called, it prints:
.in +8
.(l I

Warning
<message>
in command <command>
Do you want to continue ('y' or 'n')?

.)l
.in -8
It also beeps once.
At this point the user may type either 'y' ('Y'), in which case Warn_
returns, or 'n' ('N'), in which case Warn_ jumps back to the main
loop of COMB.
If the user types something other than 'y' ('Y') or 'n' ('N'), then
Warn_ will print "Do you want to continue ('y' or 'n')?" again,
and wait for the
user's response.
This continues until the user has typed an appropriate response.
.pp
CAUTION: Warn_ should never be called while the flag set by NoInt_ is
set (see NoInt_).
