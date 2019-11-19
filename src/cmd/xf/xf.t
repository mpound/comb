parse("cmnd_.gstr")

"Transfer stacks to directory 2 after making them unique (1/position)" "xf";
{
	"Range of stack number(s) to move" "n": int range
		old("10_MAXSTKNUM") - "sn";
	"Center Frequency (MHz)" "fr":real range old("0_990000") - "fr"; 
	"Filter width" "fwid":real old("0") - "FWID";
	"Position tolerance for combining (')" "ptol":real
		old("0.1") - "ptol";
	"Stack number limits for combining" "sn";
	{
		"Low" "l": int old("10") - "sll";
		"High" "h": int old("MAXSTKNUM") - "slh";
	}
	"Maximum weight" "mxwt": real old("1e30") - "mxwt";
	"Be more or less verbose" "v":int old("1") - "v";
}
