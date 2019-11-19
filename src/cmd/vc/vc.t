parse("cmnd_.gstr")

"Velocity Space Contour Plot" "vc";
{
	"Beginning point" "bp";
	{
		"Horizontal" "x" : extern("p1.x" "") real auto("p1.x") - "p1h";
		"Vertical" "y" : extern("p1.y" "") real auto("p1.y") - "p1v";
	}
	"End point" "ep";
	{
		"Horizontal" "x" : extern("p2.x" "") real auto("p2.x") - "p2h";
		"Vertical" "y" : extern("p2.y" "") real auto("p2.y") - "p2v";
	}
	"Spacing of points('Arc)" "sp": real old("2") - "sp";
	"Interpolation radius(')" "ir" : real old("0.8") - "ir";
        "Interpolation type":ex("it") fl("itf");
        {
                "Cone" "ci":;
                "Near point" "ni":;
                "Bessel" "bi";
                {
                        "Effective Antenna Diameter" "efd": real old("7.") - "efd";
                        "Grid Spacing of Stack Data (')" "gs": real old("1.") -"gs";
                }
                "Gaussian" "gi";
                {
			"FWHM of Gaussian ('Arc)" "fwhm": real old("1.66667") -
				"fwhm";
                }
        }
	"Velocity limits" "vl";
	{
		"Low velocity" "l": real old("0") - "lv";
		"High velocity" "h": real old("0") - "hv";
	}
	"Stack number limits" "sn";
	{
		"Low" "l": int old("10") - "sll";
		"High" "h": int old("MAXSTKNUM") - "slh";
	}
	"Center Frequency (MHz)" "fr": real range old("0_990000") - "fr";
	"Specify contour levels":ex("scl") fl("sclf");
	{
		"Auto (~10 levels)" "a":;
		"Step size" "st": extern("stepSize" "prev st size") real
			auto("stepSize") - "st";
		"Step size for color" "stc": extern("stepSize" "prev st size") real
			auto("stepSize") - "st";
		"Old step size" "os":;
		"Number of levels" "nl":extern("numLevels" "") int
			auto("numLevels") - "nlv";
		"Old contour levels" "oc" :;
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
	"Debug?" "dbg" : int flag - "dbg";
	"Object" "o" : str("68") set("") - "obj";
	"Units for z axis in FITS" "u": str("68") set("K") - "zun";
	"Horizontal label" "hlb":str("70") fl("hlbf") auto - "hlb";
	"Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
	"Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
        "Bottom label" "blb":str("40") fl("blbf") auto - "blb";
}
