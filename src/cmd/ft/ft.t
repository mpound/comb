parse("cmnd_.gstr")

"Fourier operations on baseline" "ft";
{
	"Action":ex("iacton");
	{
		"Replace stack1 by amplitude, stack2 by phase" "see":;
		"Inverse transform stack1 and stack2" "ift":;
		"Remove Fourier components from stack1" "rm";
		{
			"Number of components to filter (chan)" "nc":int old("0") - "nchanf";
			"Number of low-frequency channels to exclude from filter (chan)" "ex":int old("0") - "wid";
		}
	}
}
