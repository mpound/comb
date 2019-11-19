.sh 2 "Data Types"
.pp
The data types defined in "../coordsys/C.h" are:
.sh 3 "XYPOINT"
This data type represents a point in any two dimensional space.
It is usually used either for offsets in the current relative coordinate
system, or for the projected coordinates of a point in 3 space.
.pp
The fields are:
.pp
double x: the horizontal coordinate (or offset).
.pp
double y: the vertical coordinate (or offset).
.sh 3 "XYZPOINT"
This data type represents a point in three dimensional space.
It is used for points that lie on the sphere used to represent the
celestial sphere.
.pp
The fields are:
.pp
double x: the horizontal coordinate (as seen from any projection).
.pp
double y: the depth coordinate (as seen from any projection).
.pp
double z: the vertical coordinate (as seen from any projection).
.sh 3 "struct for_csys"
This global structure holds all of the information about the current
relative coordinate system - csys which is needed to translate
between csys and Xyz points in the standard system (ie. x in the
direction ra = 0h, dec = 0deg; y in the direction ra = 6, dec = 0; and
z in the direction dec = 90 [1950]).
.pp
The fields are:
.pp
double mat[9]: The rotation matrix for converting between the current
coordinate system and the standard Xyz system (if needed).  The sense of
this matrix is that rotate(&p1,&p1,csys.mat) converts from an XYZPOINT
(p1) in the standard system to an XYZPOINT (p2) in the rel coord sys.
.pp
double x: the horizontal (longitude) coordinate of the center.
.pp
double y: the vertical (lattitude) coordinate of the center.
.pp
float epoch: the epoch for the relative coordinates if they are of type
RADEC.
.pp
char rotate: a flag which is non zero if the rotation matrix is to be used.
.pp
char type: the type of relative coordinates
.pp
char xun: the type of units to use for the harizontal rel coord.
.pp
char yun: the type of units to use for the vertical rel coord.
