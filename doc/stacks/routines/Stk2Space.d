.sh 2 "Stk2Space(<point1>,<point2>,<xres>,<yres>)"
.pp
<point1> is an XYPOINT (see documentation for coordinate systems).
.pp
<point2> is an XYPOINT (see documentation for coordinate systems).
If <point1>.x is within <radius> of
<point2>.x (where <radius> is the interpolation radius set by SetStkInterp)
then Stk2Space errors out
with the message "Horizontal boundries are too close."
If <point1>.y is within <radius> of <point2>.y then Stk2Space errors out
with the message 
"Vertical boundries are too close."
.pp
<xres> is an integer.
If <xres> is less than 1 or greater than MAXARRAYSIZE
then Stk2Space errors out with the message
"Illegal horizontal resolution <xres>."
.pp
<yres> is an integer.
If <yres> is less than 1 or greater than MAXARRAYSIZE then Stk2Space errors
out with the message "Illegal vertical resolution <yres>."
.pp
Stk2Space (2 dimensional space of stack data)
creates an <xres> by <yres> array filled in with values
computed by GtStkValue (see GtStkValue) and some form of interpolation.
The array represents space by space.
The area searched is a rectangle whose diagonal is defined by
<point1> and <point2> (in the current coordinate system).
Stacks are projected onto the array with the current projection.
.pp
The type of interpolation is determined by the value of the previous
call to SetStkInterp.
.pp
If there isn't enough data near an element of the array to give it a
reasonable value, that element is given the value DRAGON (see documentation
for graphics).
.pp
The array is returned as a pointer to a
SKYARRAY structure (see documentation for
../stacks/C.h).
The space for the structure is allocated with malloc, so routines
calling Stk2Space should use free to deallocate it when they're done
with it.
.pp
Stk2Space ignores all stacks whose stack numbers are outside of the
range set by OkStks and/or whose frequencies are outside of the
range set by OkStkFreqs.
.pp
If there is no open directory, then Stk2Space errors out with the message
"No open stack directory."
