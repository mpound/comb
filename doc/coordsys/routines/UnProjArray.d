.sh 2 "UnProjArray(<low>,<high>,<xres>,<yres>)"
.pp
<low> is an XYPOINT.
.pp
<high> is an XYPOINT.
If <low>.x is greater than or equal to <high>.x, then UnProjArray errors
out with the message "Low horizontal greater than or equal to high."
If <low>.y is greater than or equal to <high>.y, then UnProjArray errors
out with the message "Low vertical greater than or equal to high."
.pp
<xres> is an integer.
.pp
<yres> is an integer.
If <xres> or <yres> is less than 1, UnProjArray errors out with the
message "UnProjArray: less than 1 point/axis
.pp
UnProjArray returns a pointer to an array of <xres> by <yres> XYZPOINTs.
The array represents a piece of the projection space set by SetProj.
The boundries of the array are determined by <low> and <high>, inclusive.
That is: array element 0,0 represents the point in the projection space
<low>, and the array element <xres>-1,<yres>-1 represents <high>.
Each element of the array holds the XYZPOINT that that element represents
in 3 space.
Thus, the value in array element 0,0 is the same as the value that would
be returned by UnProj(<low>).
.pp
The memory for the array is allocated using malloc, so free should be used
to deallocate it when it is no longer needed.
.pp
If the projection type is CSYS, this routine calls CsysToXyzArray which
goes fast by avoiding most of the sin and cos calls which CsysToXyz
would have if UnProj were used.
