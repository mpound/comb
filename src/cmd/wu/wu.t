parse("cmnd_.gstr")

"What is up (Plot source positions in az and el)" "wu";
{
	"Azimuth limits" "xl";
	{
		"Left x" "lx" : real old("0") - "llx";
		"Right x" "rx" : real old("360") - "urx";
	}
	"Elevation limits" "yl";
	{
		"Bottom y" "by" : real old("0") - "lly";
		"Top y" "ty" : real old("90") - "ury";
	}
	"File name" "fn": str("64") auto("WuName()")
                - "fname";
	"Sidereal time (hr)" "st": real old("6") - "lst";
	"Year" "yr": real fl("yrf") auto("CurrentYear()") - "year";
	"Latitude" "l": real old("40.392") - "latp";
	"Horizontal label" "hlb":str("40") fl("hlbf") auto - "hlb";
	"Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
	"Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
}
