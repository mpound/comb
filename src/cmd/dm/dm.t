parse("cmnd_.gstr")

"Define macro" "dm";
{
	"Macro file":ex("glob");
	{
		"Local" "l":;
		"Global" "g":;
		"Stack directory 1" "st":;
		"Directory two" "dt":;
	}
	"Action":ex("act");
	{
		"Edit with $EDITOR" "ed":;
		"Print macros with optional reg exp match test" "p":
			str("31") fl("pfl") set("^.") - "pat";
	}
}
