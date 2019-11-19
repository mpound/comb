parse("cmnd_.gstr")

"Space-space Contour Map" "cm";
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
	"Number of spatial points" "np";
	{
		"Horizontal" "h": int old("10") - "rh";
		"Vertical" "v": int old("10") - "rv";
	}
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
	"Stack number limits" "sn";
	{
		"Low" "l": int old("10") - "sll";
		"High" "h": int old("MAXSTKNUM") - "slh";
	}
	"Center Frequency (MHz)" "fr":real range old("0_990000") - "fr"; 
	"Z Value":ex("zv") fl("zvfl");
	{
		"Macro" "m":str("40") old("DFLT") - "mcr";
		"Velocity limits of integration" "vl";
		{
			"Low vel" "l":real old("0") - "lv"; 
			"High vel" "h":real old("0") - "hv"; 
		}
	}
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
	"Minimum weight to plot" "mwp" : real old("0") - "mwp";
	"Hatch mark length" "hcl" : real old("0") - "hcl";
	"Hatch mark spacing" "hcs" : real old("0") - "hcs";
	"Debug?" "dbg" : int flag - "dbg";
	"Object" "o" : str("68")  set("") - "obj";
	"Units for z axis in FITS" "u": str("68") set("K-KM/S") - "zun";
	"Horizontal label" "hlb":str("40") fl("hlbf") auto - "hlb";
	"Vertical label" "vlb":str("40") fl("vlbf") auto - "vlb";
	"Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
	"Bottom label" "blb":str("40") fl("blbf") auto - "blb";
}
