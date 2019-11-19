parse("cmnd_.gstr")

"Write data Cube" "wc";
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
	"Center Frequency (MHz)" "fr":real range old("0_990000") - "fr"; 
	"Minimum weight to plot" "mwp" : real old("0") - "mwp";
	"Debug?" "dbg" : int flag - "dbg";
        "Object" "o" : str("68") set("") - "obj";
        "Telescope" "tel": str("68") set("") - "tel";
	"Main label" "mlb":str("40") fl("mlbf") auto - "mlb";
	"Fits file name" "ff":str("64") old("wc.fits") - "ff";
}
