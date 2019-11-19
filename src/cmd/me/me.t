parse("cmnd_.gstr")

"Map data Extraction" "me";
{
    "Type of pointing": ex("ctp") fl("ctpfl");
    {
	"Ra-dec" "rd";
	{
	    "Center Ra" "cra": real auto - "cra";
	    "Center Dec" "cdec": real auto - "cdec";
	    "Offset range in Ra ('Arc) (Default: take all with matching cntr)"
		"rra": real range fl("rraf") auto - "rra";
	    "Offset range in Dec ('Arc) (Default: take all with matching cntr)"
		"rdec": real range fl("rdecf") auto - "rdec";
	}
	"L-b" "lb";
	{
	    "Center L" "cl": real auto - "cl";
	    "Center B" "cb": real auto - "cb";
	    "Offset range in L (deg) (Default: take all with matching cntr)"
		"rl": real range fl("rlf") auto - "rl";
	    "Offset range in B (deg) (Default: take all with matching cntr)"
		"rb": real range fl("rbf") auto - "rb";
	}
    }
    "Backend": ex("lbetyp") fl("befl");
    {
	"Backend number" "be":int auto("scan_.mbknd") - "lbknd";
	"Broad backend" "bbe":;
	"Narrow backend" "nbe":;
	"Expander" "exp":;
    }
    "Frequency (MHz)" "freq":real range set("0_300000") - "ffreq";
    "Minimum weight before processing" "wght": real set("100") - "fwght";
    "Accept fs scans" "fs": int flag - "fs";
    "Accept ps scans" "ps": int flag - "ps";
    "Accept bs scans" "bs": int flag - "bs";
    "Accept bp scans" "bp": int flag - "bp";
    "Accept sk scans" "sk": int flag - "sk";
    "Accept sl az scans" "sa": int flag - "sa";
    "Accept sl el scans" "se": int flag - "se";
    "Setup macro" "smacro": str("4") set("") - "smac";
    "Data reduction macro" "rmacro": str("4") set("") - "rmac";
    "Clean up macro" "emacro": str("4") set("") - "emac";
    "Range of scan numbers to check (Default to end of files)" "sn":
	scan range fl("snf") auto -"sn";
    "Set starting scan number only" "sss":scan fl("sssfl") auto - "sss";
    "Update the default starting scan number" "update": int flag - "updat";
    "Plot cycle length" "pl": int old("20") - "pl";
}
