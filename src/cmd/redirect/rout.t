parse("cmnd_.gstr")

"Redirect output to a file or string variable" "ro";
{
	"Action" : ex("act") fl("actf");
	{
		"File name" "fn" : str("47") auto("StdoutFile()") - "fname";
		"Pipeline for output" "p": str("79") old("${PAGER-more}") -
			"pcmd";
		"String variable name" "str": str("64") - "StrNam";
		"Quit redirecting" "q" : int flag - "qf";
	}
	"Truncate before writing" "t" : int flag - "trf";
}
