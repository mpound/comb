parse("cmnd_.gstr")

"Contour Plot an image" "cp";
{
	"Image Number" "im": int old("1") - "imn";
	"Plane number(s)" "pl": int range fl("plf") old ("0_0") - "plane";
        "Plane step interval" "pst": int set("1") - "pstep";
	"Specify contour levels":ex("scl") fl("sclf");
	{
		"Auto (~10 levels)" "a":;
		"Step size" "st": extern("stepSize" "prev st size") real
			auto("stepSize") - "st";
		"Step size for color" "stc": extern("stepSize" "prev st size")
			real auto("stepSize") - "st";
		"Number of levels" "nl":extern("numLevels" "") int
			auto("numLevels") - "nlv";
		"Specify contours" "sc";
		{
			"Level" "lv": real list("75") - "clv";
			"Line type" "lt": int list("75") - "clt";
		}
	}
	"Contour Level Limits" "cl";
	{
		"Low contour limit" "lc": real fl("lcf") auto - "lc";
		"High contour limit" "hc": real fl("hcf") auto  - "hc";
	}
	"Overlay" "ovl":int flag - "ovl";
	"Circular border" "circb":int flag - "circb";
	"Reverse x" "revx": int flag - "revx";
	"Reverse y" "revy": int flag - "revy";
	"Hatch mark length" "hcl" : real old("0") - "hcl";
	"Hatch mark spacing" "hcs" : real old("0") - "hcs";
	"Horizontal label" "hlb":str("40") fl("hlbf") auto - "hlb";
	"Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
	"Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
        "Bottom label" "blb":str("40") fl("blbf") auto - "blb";
}
