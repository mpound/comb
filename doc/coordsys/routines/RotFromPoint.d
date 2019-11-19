
.sh 2 "RotFromPoint(<rc>,<x>,<y>,<epoch>,<otp>,<xun>,<yun>)"
.pp
<point> is a structure of type RelCoordSys.
.pp
<x> is a double.
.pp
<y> is a double.
.pp
The members of the structure rc are defined in the description of
MkCsys.
.pp
<x> is the horizontal (longitude) relative coordinate of the point defining
the x axis of the DXDY system.
.pp
<y> is the vertical (lattitude) relative coordinate of the point.
.pp
RotFromPoint returns the angle to put in rc.rot so that MkCsys(rc) will
make a DXDY system in which the point defined by <x>,<y> in the rc
relative coordinate system is on the positive x axis.
N.B. RotFromPoint changes csys, but doesn't set it to the desired coordinate
system (it is left as though <rc.rot> = 0, <rc.otp> = DXDY, and
<rc.x> = <rc.y> = DEGREES).
