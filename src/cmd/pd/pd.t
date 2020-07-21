parse("cmnd_.gstr")

"Print data" "pd";
{
	"Action":ex("act");
	{
		"Stack 1 Header" "st":;
		"Scan Header" "sc":;
		"Stack 1 Data" "stdt":;
		"Scan Data" "scdt":;
		"Main header" "main":;
		"List scan numbers" "sn":;
		"Put first scan file name in global string" "gs": int fl("gsf") old("0") - "gs";
		"Print date and time of stack 1" "date":;
	}
}
