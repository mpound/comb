parse("cmnd_.gstr")

"Pause in execution" "pa";
{
	"Type of wait":ex("type") fl("tpfl");
	{
		"Wait for '\n'" "w":;
		"Time interval to pause (sec)" "t" : real old("1.0") - "time";
	}
}
