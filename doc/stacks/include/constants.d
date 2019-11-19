.sh 2 "Constants"
.pp
There are twelve constants defined in "../stacks/C.h."
They are:
.pp
MAXSTKNUM: the largest value that can be a stack number.
.pp
MAXSCNUM: the largest value that can be a scan number.
.pp
FULL, EMPTY: flags for NxtStk (see NxtStk).
.pp
OKTOMAKE, DONTMAKE: flags for OpenStkDir (see OpenStkDir).
.pp
FOREGROUND, BACKGROUND: specification of foreground or background directory.
(See SetStkDir.)
.pp
BUMPINTERP, DUMBINTERP, SMARTINTERP: the three types of interpolation that
may be used for arriving at values between stacks.
(See SetStkInterp.)
.pp
MAXARRAYSIZE: the maximum resolution of each dimension in a SKYARRAY
(see SKYARRAY).
This might also be used as the maximum number of elements in other arrays
later on.
(In fact, it may be moved to ../comb.h.)
.pp
FRSTONDISK: the stack number of the first stack on disk.
.pp
MAXNMLEN: maximum length of a default value name.
