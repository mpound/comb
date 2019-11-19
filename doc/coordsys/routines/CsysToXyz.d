.sh 2 "CsysToXyz(<point>)"
.pp
<point> is an XYPOINT.
.pp
CsysToXyz interprets <point> as being in the current coordinate system
(as set by RdCsys or MkCsys)
and returns the XYZPOINT that corresponds to it.
.pp
The XYZPOINT is a point in cartesian 3 space that lies on the surface of
a sphere of radius SPHERERAD and with center (0,0,0).
Since points in a relative coordinate system (as defined by MkCsys)
only denote directions in 3 space, the XYZPOINT is found by
tracing a ray in that
direction from the center of the sphere and finding the place it hits the
surface.
In other words: a point in a relative coordinate system describes the two
angle components of the spherical representation of a point in 3 space.
CsysToXyz fills in the radius component with SPHERERAD, finds the
cartesian representation of the resulting point.
.pp
For more on how a point in a relative coordinate system is interpreted, see
MkCsys.
