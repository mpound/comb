parse("cmnd_.gstr")

"Fit a gaussian function to part of a spectrum" "gf";
{
	"Horizontal interval of fit (plot units)" "h";
	{
		"From" "f" :real old("0") - "h1";
		"To" "t" : real old("0") - "h2";
	}
	"Specify fwhm of gaussian(plot units)" "wid":real fl("widfl") auto -"width";
	"Action in stack 1":ex("iacton");
	{
		"No action" "na":;
		"Replace with Gaussian and overlay plot" "see":;
		"Remove (subtract)" "rm":;
	}
	"Don't print" "dp" : int flag - "dpfl";
}
