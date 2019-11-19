parse("cmnd_.gstr")

"Calculate Virial Mass" "vm";
{
	"Area Number" "an":int old("1") - "an";
	"Spacing of points('Arc)" "sp": real old("1") - "sp";
	"Interpolation radius(')" "ir":real old("0.9") - "ir";
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
	"Velocity limits of integration" "vl";
	{
		"Low vel" "l":real old("0") - "lv"; 
		"High vel" "h":real old("0") - "hv"; 
	}
	"Center Frequency (MHz)" "fr":real range old("0_990000") - "fr"; 
	"Filter width" "fwid":real old("0") - "fwid";
	"Cloud distance (pc) for vm calc" "dist": real old("100") - "dist";
	"Scale for mass from integrated line" "scale": real old("1") - "scale";
	"Be verbose" "v": int flag - "vfl";
}
