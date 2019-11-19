parse("cmnd_.gstr")

"Printf to standard output or a string variable" "pr";
{
	"Format" "fmt" : str("127") old("%g ") - "fmt";
	"Value" "va" : str("513") / real set("0") - "var0";
	"Value" "vb" : str("513") / real set("0") - "var1";
	"Value" "vc" : str("513") / real set("0") - "var2";
	"Value" "vd" : str("513") / real set("0") - "var3";
	"Value" "ve" : str("513") / real set("0") - "var4";
	"Value" "vf" : str("513") / real set("0") - "var5";
	"Value" "vg" : str("513") / real set("0") - "var6";
	"Value" "vh" : str("513") / real set("0") - "var7";
	"Value" "vi" : str("513") / real set("0") - "var8";
	"Value" "vj" : str("513") / real set("0") - "var9";
	"Output": ex("act");
	{
		"Print to Stdout" "so":;
		"Print to a Global String" "gs": int fl("gsf") old("0") - "gs";
		"Print to a Named Variable" "v" : str("32") - "var";
	}
}
