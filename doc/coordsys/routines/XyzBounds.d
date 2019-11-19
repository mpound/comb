.sh 2 "XyzBounds(<lowin>,<highin>,<lowout>,<highout>)"
.pp
<lowin> is an XYPOINT.
.pp
<highin> is an XYPOINT.
If <lowin>.x is greater than <highin>.x then XyzBounds errors out with
the message "Low horizontal bound greater than high."
If <lowin>.y is greater than <highin>.y then XyzBounds errors out with
the message "Low vertical bound greater than high."
.pp
<lowout> is a pointer to an XYZPOINT.
.pp
<highout> is a pointer to an XYZPOINT.
.pp
XyzBounds finds a box in cartesian 3 space that contains all the points
on a part of the celestial sphere that is described in the following way.
<lowin> and <highin> are interpreted as points in the current coordinate
system.
The points that they describe on the celestial sphere are then projected
through the current projection.
The resulting points describe a box in the projection space; the point
projected from <lowin> is the lower left hand corner, and the point
projected from <highin> is the upper right hand corner.
This box represents the part of the celestial sphere that XyzBounds will
be concerned with.
.pp
XyzBounds finds the minimum x, y, and z values that lie on that part of
the sphere, and puts them in the XYZPOINT pointed to by <lowout>.
Likewise, the maximum x, y, and z values are put in the XYZPOINT pointed
to by <highout>.
Note that not all points that lie on the surface of the sphere within
these x, y, and z ranges are necessarily inside the part of the sphere
described by <lowin> and <highin>.
Any point within that part of the sphere, however, is inside those x, y,
and z ranges.
