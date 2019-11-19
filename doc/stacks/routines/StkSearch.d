.sh 2 "StkSearch(<point1>,<point2>,<subroutine>,<args>)"
.pp
<point1> is an XYPOINT (see documentation for coordinate systems).
.pp
<point2> is an XYPOINT (see documentation for coordinate systems).
If <point1>.x is within <radius> of
<point2>.x (where <radius> is the interpolation radius set by SetStkInterp)
then StkSearch errors
out with the message "Horizontal boundries are too close."
If <point1>.y is within <radius> of
<point2>.y then StkSearch errors out
with the message "Vertical boundries are too close."
.pp
<subroutine> is a pointer to an integer subroutine.
.pp
<args> is up to 50 bytes of arguments.
.pp
StkSearch is a general routine for searching through a part of the
sky.
The rectangl of the sky whose diagonal is has endpoints <point1> and <point2>
(in the current coordinate system) is searched for stacks.
Each time a stack is found, StkSearch calls <subroutine> with the
stack's stack number, frequency, and XYZPOINT coordinates (see
documentation for coordinate systems).
Then comes an integer which is the number of stacks that have been found
so far (0 on the first call to <subroutine> during the search, 1 on
the next, 2 on the next, etc.).
After those four arguments comes the first 50 bytes of <args>, which
may contain any assortment of data types.
.pp
If <subroutine> returns 0 upon completion, then the search continues.
If it returns 1, the search terminates.
REMEMBER TO MAKE IT RETURN SOMETHING!
If you forget, it usually terminates the search and is generally
annoying.
.pp
StkSearch returns the number of stacks found in the area.
If nothing is found in the area searched, <subroutine> is never called,
and StkSearch returns 0 (predictably enough).
.pp
CountStks may be used before StkSearch to give an estimate of the number
of stacks that will be found (see CountStks).
.pp
If there is no open directory, StkSearch errors out with the message
"No open stack directory."
