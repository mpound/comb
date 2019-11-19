parse("cmnd_.gstr")

"Look at where stacks are" "lk";
{
	"Horizontal limits" "xl";
	{
		"Left x" "lx" : real old("0") - "llx";
		"Right x" "rx" : real old("0") - "urx";
	}
	"Vertical limits" "yl";
	{
		"Bottom y" "by" : real old("0") - "lly";
		"Top y" "ty" : real old("0") - "ury";
	}
	"Stack number limits" "sn";
	{
		"Low" "l": int old("10") - "sll";
		"High" "h": int old("262143") - "slh";
	}
        "Plot Symbol" "ps";
        {
                "Figure Number" "fign":int set("F_CROSS") - "mrkn";
                "Figure height" "fh" : real auto("0") -  "figh";
                "Figure width" "fw" : real auto("0") - "figw";
		"Filled"  "fill" : int flag - "fill";
        }
	"Center Frequency (MHz)" "fr":real range old("0_990000") - "fr";
	"Plot stack numbers" "psn": int flag - "psn";
	"Horizontal label" "hlb":str("40") fl("hlbf") auto - "hlb";
	"Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
	"Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
	"Line type" "lt":int old("255") - "lnt";
	"Check search vs stack" "chk" : int flag - "chk";
	"Directory two" "dt":int flag - "dt";
	"Overlay" "ovl":int flag - "ovl";
}
