.sh 2 "UnProj(<point>)"
.pp
<point> is an XYPOINT.
If <point> is CLIP, CLIP, then UnProj errors out with the message
"Trying to unproject point at singularity of projection."
.pp
UnProj returns an XYZPOINT which is the point that would yield <point>
when projected.
That is: Proj(UnProj(anypoint)) = anypoint.
Note that UnProj(Proj(anypoint)) does not necessarily equal anypoint, as
Proj may return CLIP, CLIP.
.pp
The projection used by UnProj is set by SetProj.
