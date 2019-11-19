parse("cmnd_.gstr")

"Plot stack 1" "pl";
{
	"Vertical":ex("vtp");
	{
		"New vertical" "v";
		{
			"From" "f":real fl("vff") auto - "vf";
			"To" "t":real fl("vtf") auto - "vt";
			"Step" "s":real fl("vsf") auto - "vs";
		}
		"Old vertical" "ov":;
	}
	"Vertical offset" "voff":real set("0") - "voff";
	"Horizontal":ex("htp");
	{
		"New horizontal" "h";
		{
			"From" "f":real fl("hff") auto - "hf";
			"To" "t":real fl("htf") auto - "ht";
			"Step" "s":real fl("hsf") auto - "hs";
		}
		"Full horizontal range" "fhr":;
		"Old horizontal" "oh":;
	}
	"Units":ex("unit") fl("chngu");
	{
		"Channels" "ch":;
		"Frequency" "fr":;
		"Velocity" "vl":;
		"Against stack2" "agst";
		{
			"First channel" "f":int fl("fchf") auto - "fch";
			"Last channel" "t":int fl("tchf") auto - "tch";
		}
	}
	"Mode":ex("mode") fl("modef");
	{
		"Line" "ln":;
		"Histogram" "hst":;
		"Plot Symbol" "ps":int set("F_CROSS") - "mrkn";
	}
	"Horizontal label" "hlb":str("40") fl("hlbf") auto - "hlb";
	"Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
	"Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
	"Use filled symbols" "fill":int flag - "fill";
	"Line type" "lt":int old("255") - "lnt";
	"Overlay" "ovl":int flag - "ovl";
	"Mark grid with ticks" "tk":int toggle("no") - "tic";
	"Plot use array" "u":int toggle("yes") - "usar";
	"Plot grid" "g":int toggle("yes") - "gr";
	"Flag y = 0" "fy":int toggle("yes") - "fly0";
	"Plot labels" "l":int toggle("yes") - "lbs";
	"Plot header data" "d":int toggle("yes") - "hd";
}
