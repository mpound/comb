parse("cmnd_.gstr")

"Redirect command input from a file or string" "ri";
{
	"Action" : ex("act") fl("actf");
	{
		"File name" "fn" : str("128") - "fname";
		"Execute a string as comb commands" "str": str("8192") -
			"exstr";
	}
}
