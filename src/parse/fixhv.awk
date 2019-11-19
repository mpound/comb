awk '
BEGIN {FS = ",";OFS = ", "}
	{print "{"$1,$3,$4,$2,"MAINMEM},"}
' $1
