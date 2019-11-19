parse("cmnd_.gstr")

"Name stacks directory" "ns";
{
	"Directory name" "f":str("64") old("") - "fs";
	"Directory two" "dt":int flag - "dt";
	"Just print current stacks directories" "p":int flag - "prn";
	"Print Search File Header" "psh": int flag - "psh";
	"Close directory" "cl" : int flag - "clf";
        "Test existence (.test)" "t" : int flag - "tf";
	"Be quiet when creating" "q" : int flag - "qf";
}
