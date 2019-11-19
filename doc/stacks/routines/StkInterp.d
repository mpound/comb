.sh 2 "StkInterp(<point>)"
.pp
<point> is an XYPOINT (see documentation for coordinate systems).
.pp
StkInterp interpolates a stack at <point> according to the last call to
SetStkInterp and leaves it in stack 1.
If there isn't enough data to make a reasonable interpolation, then
StkInterp errors out with the message "Insufficient data for interpolation."
.pp
If there is no open directory from which to get the data, then StkInterp
errors out with the message "No open stack directory."
