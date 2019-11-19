.sh 2 "CountStks(<point1>,<point2>)"
.pp
<point1> is an XYPOINT (see documentation for coordinate systems).
.pp
<point2> is an XYPOINT (see documentation for coordinate systems).
If <point1>.x is within <radius> of
<point2>.x (where <radius> is the interpolation radius set by SetStkInterp)
then CountStks errors out with the message
"Horizontal boundries are too close."
If <point1>.y is within <radius> of <point2>.y then CountStks errors
out with the message
"Vertical boundries are too close."
.pp
CountStks returns an upper limit on the number of stacks to be found
in the part of the sky described as the rectangle whose diagonal has endpoints
<point1> and <point2> (in the current coordinate system).
The number of stacks found in searching this area might be anywhere
from zero to this upper limit, but it is more likely to be close to
the limit than not.
.pp
If there is no open directory for CountStks to work on, it errors out
with the message "No open stack directory."
.pp
This routine is intended for use in deciding sizes of buffers for
searches.
