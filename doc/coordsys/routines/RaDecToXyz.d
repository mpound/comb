.sh 2 "RaDecToXyz(<point>,<epoch>)"
.pp
<point> is an XYPOINT.
.pp
<epoch> is a float.
.pp
RaDecToXyz interprets <point> as right ascension (hours) and
declination (degrees) at the epoch <epoch> (years) and returns
the XYZPOINT that corresponds to it on the celestial sphere.
See CsysToXyz for more on how this is done.
