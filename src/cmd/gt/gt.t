parse("cmnd_.gstr")

"Put scan in st 1" "gt";
{
	"Scan number(s)" "sn": scan range fl("snf") old - "sn";
	"Step size" "st":int set("1") - "istep";
	"First scan in stack1" "fss":int flag - "fss";
	"First Scan in file"  "first": int flag - "first";
	"Last Scan in file"  "last": int flag - "last";
	"Limits on type":ex("lmttyp");
	{
		"Take anything" "dmy":;
		"Any data" "dt":;
		"Freq sw" "fs":;
		"Position sw" "ps":;
		"Beam sw" "bs":;
		"Beam Position" "bp":;
		"Total Power On" "tpon":;
		"Total Power Off" "tpof":;
		"Cal" "ca":;
		"Gains" "ga":;
		"Sky" "sk":;
		"Comb calibration" "cb":;
		"Continuum scan" "sc":;
		"Drift scan" "ds":;
	}
	"Specify backend":ex("lbetyp");
	{
		"Backend number" "be":int auto("scan_.mbknd") - "lbknd";
		"Broad backend" "bbe":;
		"Narrow backend" "nbe":;
		"Expander" "exp":;
	}
	"Specify Subscan Number" "ss":int old("1") - "ssn";
	"Limit type of scan with chars in ObsType" "type":
		str("12") fl("typef") auto - "type";
	"Test for same center and freq (.test)" "ts":int flag - "tsfl";
	"Test existence (.test)" "t":int flag - "tflg";
	"Print current scan number" "p": int flag - "pfl";
}
