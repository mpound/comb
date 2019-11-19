.sh 2 "NearStk(<point>,<range>)"
.pp
<point> is an XYPOINT (see documentation for coordinate systems).
.pp
<range> is a real.
.pp
NearStk generally returns the number of the stack closest to <point> in
the current coordinate system.
If the stack is not within <range> arcminutes of <point>, however, NearStk
returns 0.
.pp
NearStk ignores stacks whose numbers and frequencies are outside the
ranges set by OkStks and OkFreqs.
.pp
If there is no open directory, then NearStk errors out with the message
"No open stack directory."
