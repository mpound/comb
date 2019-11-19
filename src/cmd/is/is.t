parse("cmnd_.gstr")

"Interpolate a spectrun for a given position" "is";
{
	"Relative coordinate" "rc";
	{
		"ra/odra/dl/l/dx" "x":real old("0") - "x";
		"dec/oddec/db/b/dy" "y":real old("0") - "y";
	}
	"Interpolation radius(')" "ir":real old("0.8") - "ir";
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
	"Filter width" "fwid":real old("0") - "fwid";
	"Stack use array" "u":int toggle("no") - "su";
	"Test existence (.test)" "t":int flag - "tflg";
}
