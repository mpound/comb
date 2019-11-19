.sh 2 "SetStkInterp(<interpflag>,<radius>)"
.pp
<interpflag> is an integer.
If <interpflag> is not BUMPINTERP, DUMBINTERP, or SMARTINTERP then
SetStkInterp errors out with the message "Illegal type of interpolation."
.pp
<radius> is a real.
If <radius> is less than or equal to 0 then SetStkInterp errors out with
the message "Illegal interpolation radius <radius>."
.pp
SetStkInterp sets up the type of interpolation to be used in subsequent
calls to StkInterp, Stk1Space, and Stk2Space.
The type of interpolation depends on the value of <interpflag>.
If it is BUMPINTERP, then no real interpolation is done, and points
that do not lie directly on a stack are given the values of the closest
stack within <radius> arcminutes.
If it is DUMBINTERP, then the interpolation is done by taking a weighted
average of all the stacks within <radius> arcminutes of the location
to be interpolated.
The weights are inversely proportional to the square of the distance from
that location.
If it is SMARTINTERP, then the interpolation scheme worked out by
H. E. Rowe is used.
