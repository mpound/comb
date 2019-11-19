.sh 2 "GtStkDflt(<name>,<format>,<address>)"
.pp
<name> is a pointer to a string at most MAXNMLEN characters long
(including the null at the end).
.pp
<format> is a string.
.pp
<address> is a pointer to any data type.
.pp
GtStkDflt retrieves a value from the default file associated with a
stack directory.
The entries in the file are of the form "<dfltname>=<value>\\n," where
<dfltname> is a string and <value> is the ascii version of an arbitrary
type.
GtStkDflt looks through the file for a <dfltname> that matches <name>,
leaves the file pointer pointing to the first character after the
equals sign (i.e. the first character of the value), and then calls
fscanf with <format> as the format, and <address> as the address in
which to put the results.
For example, if var1 is an integer, and there is an entry in the default file
reading "example=5\\n," then GtStkDflt("example","%d",&var1) will put the
value 5 into var1.
.pp
If there is no open directory, then GtStkDflt errors out with the
message "No open stack directory to give defaults."
.pp
If there is no entry that matches <name>, then GtStkDflt errors out with
the message "No default <name> in directory <dir>," where <dir>
is the name of the current directory.
If <name> contains a dot ("."), then the above message will show only the
part of <name> that comes after the dot.
.pp
It is suggested that names of default values be constructed like structures
in C.
Since default values will often fall into natural groups, a default value
name for a value in a given group should consist of a group name, followed
by a dot, followed by a specific value name.
For example, the default upper bound in ra for a contour map
should be named "map.high ra," and the lower bound "map.low ra"
(note that spaces may be included in the name).
"map" is the group name, and "high ra" and "low ra" are specific value
names.
This system has two advantages.
First, it reduces the difficulty of constructing unique names and,
consequently, makes the names less cryptic.
(It is fairly important that the names not be cryptic because they are
used in an error message that might be shown to the user.
It's much nicer to say "No default low ra in directory S187" than it
is to say "No default lr1 in directory S187.")
Second, if this naming system is used, then the unix command "sort"
can be used to collect all the default values from one group into
one place in the file.
