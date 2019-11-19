parse("cmnd_.gstr")

"Add scans to stack2" "ad";
{
	"Scan number(s)" "sn" : scan range fl("snf") old - "sn";
	"Step size" "st" : int set("1") - "istep";
	"Specify backend" : ex("lbetyp");
	{
		"Backend number" "be":int auto("scan_.mbknd") - "lbknd";
		"Broad backend" "bbe" :;
		"Narrow backend" "nbe" :;
		"Expander" "exp" :;
	}
        "Specify Subscan Number" "ss":int old("1") - "ssn";
	"Care level" "cr" : int set("2") - "ncare";
	"Matching headers only" "mh" : int flag - "mhflg";
}
