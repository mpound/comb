parse("cmnd_.gstr")

"Change use array" "us";
{
	"Range (plot units)" "h":real range fl("hflg") auto - "h";
	"Retrieve" "rt":int flag - "rtflg";
	"Compliment" "co":int flag - "coflg";
	"Action":ex("iactn");
	{
		"Reset" "r":int flag - "actfl";
		"Set" "s":int flag - "actfl";
	}
	"Store" "st":int flag - "stflg";
	"Repeat shifted to reference ch" "ref":int flag - "rfflg";
}
