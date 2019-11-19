parse("cmnd_.gstr")

"Print in hms format" "ph";
{
	"Value" "v" : real set("0") - "var";
	"No line feed" "nl" : int flag - "nl";
        "Output": ex("act");
        {
                "Print to Stdout" "so":;
                "Print to a Global String" "gs": int fl("gsf") old("0") - "gs";
                "Print to a Named Variable" "nv" : str("32") - "nvar";
        }
}
