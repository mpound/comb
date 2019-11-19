parse("cmnd_.gstr")

"Update a stacks directory" "up";
{
	"Name" "n":str("128") - "drc";
	"Directory two" "dt": int flag - "dt";
	"From VAX (convert Vax stacks file to Sun" "fvax":int flag - "fvax";
	"Convert Big Endian stacks (Sun) to Little Endian (Dec) or reverse"
		"swap": int flag - "swap";
	"Test byte order only" "tbo": int flag - "tbo";
	"From Hp" "fhp": int flag - "fhp";
	"Fix up survey files" "fix":int flag - "fix";
}
