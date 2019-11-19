parse("cmnd_.gstr")

"Integrate part of a spectrum" "in";
{
	"Horizontal integration interval(plot units)" "h";
	{
		"from" "f" : real old("0") - "h1";
		"to" "t" : real old("0") - "h2";
	}
	"Clip Level" "clip";
	{
		"For Zeroth and First Moments" "area" : real old("0.0") - "clip0";
		"For Second Moment" "disp" : real old("1.0") - "clip1";
	}
	"Don't print" "dp" : int flag - "ndp";
}
