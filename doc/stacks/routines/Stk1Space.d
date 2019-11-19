.sh 2 "Stk1Space(<point1>,<point2>,<lowvel>,<highvel>,<xres>,<yres>)"
.pp
<point1> is an XYPOINT.
.pp
<point2> is an XYPOINT.
If <point1> is within <radius> of
<point2> (where <radius> is the interpolation radius set by SetStkInterp)
then Stk1Space errors out with the message
"Endpoints of line for space-velocity data are too close"
.pp
<lowvel> is a real.
.pp
<highvel> is a real.
If <highvel> is less than <lowvel> then Stk1Space errors out with the message
"Low frequency less than high."
.pp
<xres> is an integer.
If <xres> is less than 1 or greater than MAXARRAYSIZE
then Stk1Space errors out with the message
"Illegal space resolution <xres>."
.pp
<yres> is an integer.
If <yres> is less than 1 or greater than MAXARRAYSIZE
then Stk1Space errors out with the message
"Illegal velocity resolution <yres>."
.pp
Stk1Space (1 dimensional space of stack data)
creates an <xres> by <yres> array of values, each collumn of which
represents a spectrum of the velocities <lowvel>
to <highvel> at one point in space.
The spectra will be computed along the line from <point1> to <point2>
(in the current coordinate system) by interpolating stacks found
near that line.
.pp
The type of interpolation used depends on the value of the previous
call to SetStkInterp.
.pp
If there is not enough data to compute a reasonable spectrum for a point
on the line, then the collumn representing that point is filled in with
the value DRAGON (see documentation for graphics).
If there are enough spectra to compute the point, but they don't cover the
entire range of velocities, then the missing velocities in the collumn
representing that point will be given the value DRAGON.
.pp
The array is returned as a pointer to a SKYARRAY structure
(see documentation for ../stacks/C.h).
The space for the structure is allocated with malloc, so routines
calling Stk1Space should use free to deallocate it when it's nolonger
neaded.
.pp
Stk1Space ignores all stacks whose stack numbers are outside of the
range set by OkStks and/or whose frequencies are outside of the
range set by OkStkFreqs.
.pp
It should be said explicitly that the x axis of the array represents
space, and the y axis represents velocity.
The x axis of the array represents space, and the y axis represents velocity.
.pp
If there is no open directory, then Stk1Space errors out with the message
"No open stack directory."
