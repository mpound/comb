.sh 2 "UnProjLine(<p1>,<p2>,<n>)"
.pp
<p1> is an XYPOINT.
.pp
<p2> is an XYPOINT.
.pp
<n> is an integer.
.pp
If <n> is < 1, UnProjLine errors out with the message "UnProjLine: less
than one point".
.pp
UnProjArray returns a pointer to a vector of <n> XYZPOINTs.  The vector
represents a line in the projection space set by SetProj.  The line
goes from  p1 to p2.  Vector element 0 represents p1 and element n - 1
represents p2.
Each element of the array holds the XYZPOINT that that element represents
in 3 space.
Thus, the value in vector element 0 is the same as the value that would
be returned by UnProj(<p1>).
.pp
The memory for the array is allocated using malloc, so free should be used
to deallocate it when it is no longer needed.
.pp
This routine could be implemented to go faster if needed.  It presently just
calls UnProj n times.
