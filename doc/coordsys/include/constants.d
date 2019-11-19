.sh 2 "Constants"
.pp
The constants declared in "../coordsys/C.h" are:
.pp
SPHERERAD: the radius of the sphere used to represent the celestial sphere in
cartesian 3 space.
.pp
HALFSPACE: half of the extent of 3 space in each direction.
Also the value of the x, y, and z coordinates of the center of the celestial
sphere.
.pp
PI: the value of pi (out to 17 digits past the decimal point).
.pp
RADEC: used to mean right ascension, declination.
.pp
LB: used to mean galactic coordinates.
.pp
DXDY: used to mean user defined coordinates.
.pp
DEGREES: used to mean degrees.
.pp
RADIANS: used to mean radians.
.pp
HOURS: used to mean hours.
.pp
ARCMINUTES: used to mean arc minutes.
.pp
NUMUNITS:used for error checking. The number of types of units above.
.pp
SCALED: ored with DEGREES, RADIANS, HOURS, or ARCMINUTES to mean that the
horizontal offset is divided by the cosine of the vertical coordinate.
.pp
CLIP: a value which cannot be the projected value of any point.
Used to indicate the projection of a point near a singularity of the
current projection (as set by SetProj).
.pp
CSYS: project into current coordinate system.
.pp
ORTHO: project orthographically.
.pp
MERCATOR: project with mercator projection.
.pp
TANPLANE: project with tangent plane projection.
.pp
POLAR: project with polar projection.
.pp
EQAREA: project with polar projection adjusted to maintain equal area.
