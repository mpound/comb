parse("cmnd_.gstr")

"Make a slice through an image" "sl";
{
	"Beginning point" "bp";
	{
		"Horizontal" "x" : real old("0") - "x1";
		"Vertical" "y" :  real old("0") - "y1";
	}
	"End point" "ep";
	{
		"Horizontal" "x" : real old("0") - "x2";
		"Vertical" "y" : real old("0") - "y2";
	}
	"Image number" "im": int old("1") - "im";
	"File Name for slice data" "fn": str("64") auto("SliceName()") - "fn";
	"Merge with previous file" "m": int flag - "mf";
	"Scale factor for y" "fact": real set("1.0") - "fact";
	"Addend factor for y" "add": real set("0.0") - "yadd";
        "Minimum y value to include" "miny": real set ("-1e30") - "miny";
        "Maximum y value to include" "maxy": real set ("1e30") - "maxy";
        "Plot y axis as log" "logy": int flag - "logy";
	"Vertical range to plot" "v";
        {
                "From" "f" : real fl("vff") auto - "vf";
                "To" "t" : real fl("vtf") auto - "vt";
        }
	"Horizontal range to plot" "h";
        {
                "From" "f" : real fl("hff") auto - "hf";
                "To" "t" : real fl("htf") auto - "ht";
        }
        "Mode":ex("mode") fl("modef");
        {
                "Line" "ln":;
		"Plot Symbol" "ps";
        	{
                	"Figure Number" "fign":int set("F_CROSS") -"mrkn";
                	"Figure height" "fh" : real old("0") -  "figh";
                	"Figure width" "fw" : real old("0") -  "figw";
                	"Filled" "fill" : int flag - "fill";
        	}
        }
        "Horizontal label" "hlb":str("70") fl("hlbf") auto - "hlb";
        "Vertical label" "vlb":str("64") fl("vlbf") auto - "vlb";
        "Main label" "mlb":str("64") fl("mlbf") auto - "mlb";
        "Line type" "lt":int old("255") - "lnt";
        "Overlay" "ovl":int flag - "ovl";
        "Flag y = 0" "fy":int toggle("yes") - "fly0";
}
