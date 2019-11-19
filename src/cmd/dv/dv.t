parse("cmnd_.gstr")

"Declare user Variables" "dv";
{
	"Numeric variables" "n": str("1024") fl("nfl") auto - "nnam";
	"String variables" "s": str("1024") fl("sfl") auto - "snam";
	"Make variable local to macro" "l": int flag - "lclf";
	"Print global user variables" "pg": int flag - "pgf";
	"Print local user variables" "pl": int flag - "plf";
}
