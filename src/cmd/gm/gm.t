parse("cmnd_.gstr")

"Graphics Manipulation" "gm";
{
    "Action" : ex("act");
    {
	"Current box" "cb";
	{
		"Current column" "cc": extern("bxCol" "") int 
			auto("bxCol") - "cc";
		"Current row" "cr": extern("bxRow" "") int 
			auto("bxRow") - "cr";
	}
	"Number of boxes" "nb";
	{
		"Number of columns" "nc": int old("1") - "nc";
		"Number of rows" "nr": int  old("1") - "nr";
	}
	"Scale" "sc";
	{
		"X scale" "xs" : extern("xSquish" "") real auto("xSquish")
			- "xs";
		"Y scale" "ys" : extern("ySquish" "") real auto("ySquish")
			- "ys";
	}
	"Title" "ti" ;
	{
		"text" "t" : str("64")  old("") - "ti";
		"Horizontal position" "h" : real old("0") - "hpos";
		"Vertical position" "v" : real old("0") - "vpos";
	}
	"Move" "mv";
	{
		"Horizontal position" "h" : real auto - "hpos";
		"Vertical position" "v" : real auto - "vpos";
	}
	"Line" "li";
	{
		"Horizontal position" "h" : real auto - "hpos";
		"Vertical position" "v" : real auto - "vpos";
	}
	"Draw Figure" "df";
	{
		"Figure number" "fign" : int old("0") - "mrkn";
		"Horizontal position" "h" : real auto - "hpos";
		"Vertical position" "v" : real auto -  "vpos";
		"Figure height" "fh" : real -  "figh";
		"Figure width" "fw" : real -  "figw";
		"Filled" "fill" : int flag - "fill";
	}
	"Re-plot" "rp":;
    }
    "Line type" "lt":int old("255") - "lnt";
}
