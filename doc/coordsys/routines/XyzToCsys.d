.sh 2 "XyzToCsys(<point>)"
.pp
<point> is an XYZPOINT.
.pp
XyzToCsys performs the oposite function of CsysToXyz; that is: it takes
an XYZPOINT and returns the cooresponding point in the current coordinate
system.
.pp
To do this, it rotates the point into the current coordinate system
if necessary and finds
the polar representation of the resulting point.
It then subtracts the center coordinates from the two
angle components and converts to the specified units.
.pp
XyzToCsys will make sure that each of the coordinates of the point it
returns are within the range -half_a_rotation to +half_a_rotation.
Thus, if the x coordinate is expressed in degrees, then x will be between
-180 and 180 (the lower bound is exclusive, and the upper bound is
inclusive).
If the y coordinate is expressed in hours, then y will be between -12 and
12 (again, the lower bound is exclusive, and the upper bound is inclusive).
.pp
If <point> lies too close to the one of the poles of the current coordinate
system, then the XYPOINT returned will have x and y coordinates
CLIP, CLIP.
