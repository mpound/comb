parse("cmnd_.gstr")

"Designate bad channels" "bc";
{
	"Range of horiz values(plot units)" "r":real range fl("rflag") auto - "h";
	"Action" : ex("iact");
	{
		"Add to list" "ad":;
		"Clear old list first" "cl":;
	}
	"Print currently flagged bad channels" "p": int flag - "prn";
}
