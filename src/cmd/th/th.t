parse("cmnd_.gstr")

"Temperature Histogram" "th";
{

	"Temperature bin size (Kelvin)" "tbin" : real old("0.1") - "tbin";
        "Stack number limits" "sn";
        {
                "Low" "l": int old("10") - "sll";
                "High" "h": int old("MAXSTKNUM") - "slh";
        }
        "Specify velocity limits":ex("svl") fl("vlfl");
        {
                "All velocities" "a":;
        	"Velocity limits" "vl";
        	{
                	"Low vel" "l":real old("0") - "lv"; 
                	"High vel" "h":real old("0") - "hv"; 
        	}
	}
	"Scale factor for x" "xfact": real old("1") - "xfact";
	"Scale factor for y" "yfact": real old("1") - "yfact";
	"Minimum x value to include" "minx": real set ("-1e30") - "minx";
	"Maximum x value to include" "maxx": real set ("1e30") - "maxx";
	"Plot x axis as log" "logx": int flag - "logx";
	"Plot y axis as log" "logy": int flag - "logy";
        "File Name for temperature histogram data" "fn": str("64") auto("THName()")
                - "fn";
        "Horizontal label" "hlb":str("40") fl("hlbf") auto - "hlb";
        "Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
        "Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
}

