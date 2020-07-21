parse("cmnd_.gstr")

"Total power - average chans in stack1 weighted by cal in stack3" "tp";
{
	"Store power at this elevation" "st":int flag - "stf";
	"Reduce stored skydip power values" "reduce":int flag - "redf";
	"Clear stored values from memory without reducing" "clr":int flag - "clrf";
	"Print stored values from memory" "prt":int flag - "prtf";
	"Output skydip reduction in tabular form" "tab":int flag - "tabf";
}
