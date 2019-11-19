parse("cmnd_.gstr")

"Scanf from a file, stdin, or a string variable" "sc";
{
	"Format" "fmt" : str("127") old("%le") - "fmt";
	"Action" : ex("act");
	{
		"Variables to assign results to" "v" : str("128") old("#")
			- "args";
		"Read into stack1 chans" "stch" : int range old("1_1") - "stch";
		"Rewind file only" "rw" :;
	}
	"Source" : ex("srcn");
	{
		"File name" "fn" : str("64") auto("StdinFile()") - "fname";
		"Global String to read" "gs" : int old("0") - "gs";
	 	"Standard input" "stdin" :;
		"A string or string variable" "str": str("8192") - "scstr";
	}
	"Warn if end of file is encountered" "w" : int toggle("yes") - "eofw";
}
