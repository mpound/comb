parse("cmnd_.gstr")

"Print something" "p";
{
	"Value" "v" : real / str("8192") set("") - "var";
	"Don't print" "dp" : int flag - "dp";
	"No line feed" "nl" : int flag - "nl";
}
