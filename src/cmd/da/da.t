parse("cmnd_.gstr")

"Define an area of an image" "da";
{
	"Specify boundary" : ex("sbt") fl("sbfl");
	{
		"No change" "nc":;
		"Points on boundary (offset from center in current units)" "po";
		{
			"Horizontal" "x":real list("128") - "X";
			"Vertical" "y":real list("128") - "Y";
		}
		"Cursor read for polygon" "cr":;
	}
	"Area number (1-5)" "an": int set("1") - "an";
	"Print vertices of area" "p": int flag - "pf";
	"Plot area on current plot" "pl": int flag - "plf";
        "Line type" "lt":int old("255") - "lnt";
	"Print area in area" "pa": int flag - "paf";
}
