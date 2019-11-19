parse("cmnd_.gstr")

"Loop through a command string" "do";
{
	"Type of loop" : ex("tol");
	{
		"Number of times to execute" "n" : int - "to";
		"While" "w" : exp("80") - "exp";
		"If" "i" : exp("80") - "exp";
	}
	"Command string" "c" : cmd("1000") - "cmd";
	"Else" "el" : cmd("1000") fl("elfl") auto - "elcmd";
}
