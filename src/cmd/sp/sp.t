parse("cmnd_.gstr")

"Make a scatter plot comparing two images" "sp";
{
	"Image number for x axis" "imx": int old("1") - "imx";
	"Image number for y axis (< 0 for none)" "imy": int old("2") - "imy";
	"Area number" "an": int old("1") - "an";
	"File Name for scatter plot data" "fn": str("64") auto("SpName()")
		- "fn";
	"Scale factor for x" "xfact": real old("1") - "xfact";
	"Scale factor for y" "yfact": real old("1") - "yfact";
	"Minimum x value to include" "minx": real set ("-1e30") - "minx";
	"Minimum y value to include" "miny": real set ("-1e30") - "miny";
	"Maximum x value to include" "maxx": real set ("1e30") - "maxx";
	"Maximum y value to include" "maxy": real set ("1e30") - "maxy";
        "Vertical range to plot" "v";
        {
                "From" "f":real fl("vff") auto - "vf";
                "To" "t":real fl("vtf") auto - "vt";
        }
        "Horizontal range to plot" "h";
        {
                "From" "f":real fl("hff") auto - "hf";
                "To" "t":real fl("htf") auto - "ht";
        }
	"Plot x axis as log" "logx": int flag - "logx";
	"Plot y axis as log" "logy": int flag - "logy";
        "Plot Symbol" "ps";
	{
        	"Figure Number" "fign":int set("F_DOT") - "mrkn";
                "Figure height" "fh" : real old("0") -  "figh";
                "Figure width" "fw" : real old("0") -  "figw";
                "Filled" "fill" : int flag - "fill";
	}
        "Horizontal label" "hlb":str("40") fl("hlbf") auto - "hlb";
        "Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
        "Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
        "Overlay" "ovl":int flag - "ovl";
}
