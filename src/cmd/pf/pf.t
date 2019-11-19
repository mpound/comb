parse("cmnd_.gstr")

"Fit a parabola to part of a spectrum" "pf";
{
	"Horizontal interval of fit (plot units)" "h";
	{
		"From" "f" :real old("0") - "h1";
		"To" "t" : real old("0") - "h2";
	}
	"Action in stack 1":ex("iacton");
	{
		"No action" "na":;
		"Replace with parabola and overlay plot" "see":;
		"Remove (subtract)" "rm":;
	}
	"Don't print" "dp" : int flag - "dpfl";
}
