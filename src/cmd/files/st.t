parse("cmnd_.gstr")

"Store stack" "st";
{
	"Stack number" "n":int fl("nf") old("2") - "sn";
	"Directory name" "f":str("64") fl("ff") auto - "sf";
	"Use directory two" "dt":int flag - "dt";
	"Source stack" "src":int set("1") - "src";
	"Auto store":ex("autof");
	{
		"Don't auto store" "dmy":;
		"First free stack to use" "a":int old("10") - "frfre";
	}
	"Don't check dest" "dc":int flag - "dc";
}
