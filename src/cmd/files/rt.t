parse("cmnd_.gstr")

"Retrieve stack" "rt";
{
	"Stack number(s)" "n": int range old("2_2") - "sn";
	"Directory name" "f":str("64") fl("ff") auto - "sf";
	"Use directory two" "dt":int flag - "dt";
	"Stack use array" "u":int toggle("no")- "su";
	"Destination" "dest":int set("1") - "dest";
	"Test existence (.test)" "t":int flag - "tflg";
	"Relative coordinate" "rc";
	{
		"ra/odra/dl/l/dx" "x":real fl("c1fl") old("0") - "c1";
		"dec/oddec/db/b/dy" "y":real fl("c2fl") old("0") - "c2";
	}
	"Position tolerance(')" "ptol":real fl("ptfl") old("0.3") - "ptol";
	"Center Frequency (MHz)" "fr":real range fl("frfl") old("0_990000") - "fr";
	"Stk" "sn":int range fl("skfl") old("0_MAXSTKNUM") - "stk";
	"Match stack": ex("msfl");
	{
		"Don't match stack" "dmy":;
		"Match stack" "ms" : int old("1") - "msn";
	}
	"Cmd string to execute" "c" : cmd("500") fl("cmdfl") auto - "cmd";
}
