.sh 2 "WrStkDflt(<name>,<format>,<data>)"
.pp
<name> is a pointer to a string at most MAXNMLEN characters long
(including the null at the end).
.pp
<format> is a string.
.pp
<data> is up to 8 bytes of data of any type.
If a string is to be written, data should be a pointer to the string.
.pp
WrStkDflt is used to write a value to the default value file
associated with a directory.
It first looks through the file for an entry named <name>.
If it finds it, then it replaces that entry with the new entry.
If it doesn't, then it simply adds the new entry to the file.
.pp
To write out the value, it calls fprintf with the format
<format> and the first 8 bytes of <data> as the argument.
This means that <data> may be any of the basic data types.
.pp
If there is no open directory, then WrStkDflt errors out with the message
"No open stack directory for storing defaults."
.pp
For a more in-depth description of default value files, see GtStkDflt.
