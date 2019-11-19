.sh 2 "Proj(<point>)"
.pp
<point> is an XYZPOINT.
.pp
Proj returns the XYPOINT that is the projection of <point>.
The type of projection used is set by SetProj.
.pp
If XYZPOINT is too close to a singularity in the projection, then
Proj returns x and y both as the value CLIP.
