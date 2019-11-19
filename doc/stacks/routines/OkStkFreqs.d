.sh 2 "OkStkFreqs(<low>,<high>)"
.pp
<low> is a float.
.pp
<high> is a float.
If <low> is greater than <high> then OkStkFreqs errors out with the
message "High bound of frequency range less than low bound."
.pp
OkStkFreqs sets limits to the center frequencies of stacks.
Any stack that has a frequency outside this range is ignored in future
unless specifically asked for -- that is, passed as a parameter.
The upper (<high>) and lower (<low>) bounds of the range are inclusive.
.pp
If there is no open directory, then OkStkFreqs errors out with the message
"No open stack directory."
