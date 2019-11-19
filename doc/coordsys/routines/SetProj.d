. sh 2 "SetProj(<projtype>)"
.pp
<projtype> is an integer.
If <projtype> is not CSYS, ORTHO, MERCATOR, or TANPLANE
then SetProj will
error out with the message "Unknown type of projection."
.pp
SetProj sets the type of projection to be used by Proj, UnProj, and
UnProjArray.
The type of projection depends on the value of <projtype>.
The projections are as follows.
.pp
If <projtype> is CSYS, then each point is projected into the current
coordinate system.
A call to Proj will give the same results as a call to XyzToCsys.
.pp
If <projtype> is ORTHO, then an orthographic projection will be used.
The celestial sphere is first rotated so that the center of the current
coordinate system lies on the point (SPHERERAD,0,0), and the point
projected parralel to the x axis.
That is each point (x0, y0, z0) is projected into (x, y) as
x = y/SPHERERAD, and y = z/SPHERERAD.
If x0 is less than 0.0001 * SPHERERAD, however, x and y are
both given the value CLIP.
.pp
If <projtype> is MERCATOR, then a Mercator projection is used.
The celestial sphere is rotated and translated as for ORTHO (see above),
and the projection x, y of each point x0, y0, z0 is given as
x = arctan(y0/x0), and y = z0/((x0^2+y0^2)^.5).
This translates into making the projection by wrapping the two dimensional
projection space in a cylinder around the celestial sphere and tracing a
ray from the center of the sphere through each point to be projected onto
the projection space.
.pp
If <projtype> is TANPLANE, then a tangent plane projection is used.
After the celestial sphere is rotated and translated as described for ORTHO
(see above), each point is projected on to a plane tangent to the
sphere at (SPHERERAD,0,0) (that is: at the center of the current coordinate
system), and tracing a ray from the center of the sphere through the point
to find out where it lands on the plane.
If the point's x coordinate is < 0.0001*SPHERERAD, then its projection is
CLIP, CLIP.
.pp
If <projtype> is POLAR, then a polar projection is used.
The sphere is once again rotated and translated as described for ORTHO
(see above).
A spherical representation of each point to be projected is found in
which the first angle is a rotation about the x axis (a rotation from front
to back), and the second angle is a rotation about the y axis (on a plane
parallel to the x-z plane).
The center of the current coordinate system is at one of the poles of this
spherical representation.
The projection is done by using the first angle as the radius, and the
second angle as the angle in a 2 dimensional polar coordinate system.
All points on the sphere can be projected with this projection, though the
distortion at the point oposite the center of the current coordinate system
is something fierce.
.pp
If <projtype> is EQAREA, then an equal area projection is used.
This is a POLAR projection with corrections made to the radius of
each point on the polar coordinate system to maintain the area of the
surface of the sphere.
If a point is too close to the point directly oposite the center of the
current coordinate system on the celestial sphere, then that points
x and y coordinates are returned as CLIP and CLIP.
.pp
It should be pointed out that of the above projections, ORTHO is the fastest,
and POLAR is the most robust (it can cover almost the entire sphere).
