.sh 1 "Introduction"
.pp
Stacks, and the information that goes with them, are held in stacks
directories.
Each stacks directory contains from four to fourteen files which hold the
data in the stacks, information about how to retrieve the stacks, and
bits and pieces of information that are associated with the stacks
directory but don't actually relate to the stacks themselves.
.pp
The stack data is held in the file named, appropriately enough, "data."
All stacks, however, are not created equal.
They may have different lengths depending on the number of channels
they have and the number of scans that have been added into them.
Also, people often use non-contiguous scan numbers: their data may start
with stack 235, go on to stack 400, then start again at 800, etc.
To keep from using up disk space for non-existent stacks and data, the
stacks are stored out of order and packed.
There is another file in each stacks directory, "index," which contains
a list of the locations and lengths of stacks, as well as flags for whether
the stack exists and whether it can be recovered.
.pp
The file "search" contains a kd-tree structure describing the locations
of stacks in a cartesian 3 space.
With the routines in S.routines.c, this file can be searched for lists of
stacks within given areas of the sky.
.pp
There may be 0 to 10 files named "val.xxxx" (where "xxxx" may be anything).
These files contain the values of stacks to be used in making maps.
The end of the file name (the "xxxx") is the name of the macro that
was used to arive at the values.
Each entry in these files consists of a value, and a flag for whether or
not that value is up to date.
When a stack is stored, all the flags corresponding to it in all the "val"
files are reset.
Also, when a "val" file is opened, various checks are made to make sure
that the macro it represents hasn't changed since the "val" file was
last written to.
If it has changed, then all the flags in the file are reset.
.pp
The last file in the stacks directory is "default."
It is a file which contains a number of default values for various commands
that should have defaults based on the stacks directory they refer to.
The only default values maintained by the stacks routines are
"range.low horizontal offset," "range.high horizontal offset,"
"range.low vertical offset," and "range.high vertical offset."
These give the range of the data in the default coordinate system of
the stacks directory, "coord.relative coordinate system"
(which is maintained elsewhere).
These are updated any time a default value in the group "range" is asked
for.
