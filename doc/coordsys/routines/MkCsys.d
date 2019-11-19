.sh 2 "MkCsys(<rc>)"
.pp
<rc> is a structure given by the typedef RelCoordSys.  The members of
rc are:
.pp
<rc.name[CSNLEN]> is the name of the relative coordinate system
and is not used by MkCsys.
.pp
<rc.x> is a double.
.pp
<rc.y> is a double.
.pp
<rc.cepoch> is a float.
.pp
<rc.oepoch> is a float.
.pp
<rc.ctp> is a char.
If <rc.ctp> is not  equal to RADEC or
LB, then MkCsys errors out with the message
"Illegal type of coordinates for coordinate system center."
.pp
<rc.otp> is a char.
If <rc.otp> is not equal to RADEC, LB or DXDY,
then MkCsys errors out with the message "Illegal type of offsets."
.pp
<rc.xun> is a char.
If <rc.xun> is not DEGREES, RADIANS, HOURS, ARCMINUTES,
or any of these ored with SCALED
(e.g. SCALED | ARCMINUTES)
then MkCsys errors out with the message "Illegal relative coordinate units."
.pp
<rc.yun> is a char.
If <rc.yun> is not DEGREES, RADIANS, HOURS, or ARCMINUTES,
then MkCsys errors out with the message "Illegal relative coordinate units."
.pp
<rc.rot> is a double.
.pp
MkCsys creates the current coordinate system.
The meanings of the arguments are as follows:
.pp
<rc.x> is the horizontal (longitude) coordinate of the center of
this relative coordinate system.
If <rc.ctp> is RADEC, then <rc.x> is right ascension (hours)
with the appropriate epoch (see below).
If <rc.ctp> is LB, then <rc.x> is l (degrees).
.pp
<rc.y> is the vertical (lattitude) coordinate of the center of this
relative coordinate system.
If <rc.ctp> is RADEC, then <rc.y> is declination (degrees)
with the appropriate epoch (see below).
If <rc.ctp> is LB, <rc.y> is b (degrees).
.pp
<rc.cepoch> is the epoch (years) in which the center coordinates are given
if <rc.ctp> is RADEC.
.pp
<rc.oepoch> is the epoch (years) in which the offset coordinates
are to be given if <rc.otp> is RADEC.
.pp
<rc.rot> (degrees) is used to define the position angle of
the 'y' axis of a DXDY system (see below).
.pp
<rc.ctp> is the type of coordinates that the center is expressed in.
RADEC means that the center is given in right ascension and declination.
LB means that the center is given in galactic coordinates.
In the csys structure the center position will be expressed in  type <rc.otp>
coordinates (if rc.otp is RADEC they will be precessed to epoch oepoch).
.pp
<rc.otp> indicates how offsets are to be interpreted.
The following is a description of how the
coordinates of a point are constructed given its offsets in the current
coordinate system and the value of <rc.otp>.
.pp
If <rc.otp> is RADEC,
then the right ascension and declination offsets are
added to the right ascension and declination of the center of the current
coordinate system (<csys.x> and <csys.y>).
and the result is interpreted as the
right ascension and declination of the point.
.pp
If <rc.otp> is LB, then the l and b offsets are added to the l and b coordinates
of the center of the current coordinate system (<csys.x> and <csys.y>), and the
result is interpreted as the right ascension and declination of the point.
.pp
If <rc.otp> is DXDY, then the relative coordinates are in a spherical coordinate
system with (0,0) centered on the center point.
If <rc.ctp> is RADEC, the y (lattitude)
axis of the rel coord sys will have position angle <rc.rot> (see above).
For example if
<rc.rot> is 0, y will be in the positive 'dec' direction and if <rc.rot>
is 90, y will be in the positive 'ra' direction.
If <rc.ctp> is LB, the y axis will be at a position angle <rc.rot>
measured from the positive 'b' direction.
.pp
<rc.xun> is the type of units that the horizontal offset is to be
given in.
If <rc.xun> is DEGREES, the horizontal offset will be given in degrees.
If it is RADIANS, it will be given in radians.
If it is HOURS, it will be given in hours (24 in one rotation).
If it is ARCMINUTES, it will be given in arc minutes (degrees * 60).
If it is any of these values ored with SCALED, then the horizontal offset
will first be divided by the cosine of the vertical coordinate.
.pp
<rc.yun> denotes the units that the vertical offsets will be given in.
The meanings of the values of <rc.yun> correspond with those of <rc.xun>
(above), except that <rc.yun> cannot be ored with SCALED.
