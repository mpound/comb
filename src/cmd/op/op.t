parse("cmnd_.gstr")

"Set options" "op";
{
	"Auto plot control" "ap" : extern("main_.apfl" "") int toggle - "apf"; 
	"Use Upper Case Labels" "ucl" : extern("labelsUC" ""), int toggle -
		"ucl";
	"Default stacks path" "sp" : extern("main_.stkpth" "") str("128")
		old - "sp";
	"Projection to use for maps" : ex("proj") fl("projf");
	{
		"Current rel coordinates" "rc":;
		"Orthographic" "ortho" :;
		"Mercator" "merc" :;
		"Gnomonic" "gnom" :;
		"Polar" "po":;
	}
	"Horiz plot type" : ex("htyp") fl("htypef");
	{
		"Channels" "ch":;
		"Frequency" "fr":;
		"Velocity" "vl":;
	}
	"Answer Yes to Warn" "yw" : extern("main_.warny" "") int toggle - "ywf";
	"Message Level for BLANK" "msg" : extern("main_.drgmes" "") int
		auto("main_.drgmes") - "msg";
	"Don't show plots" "dp": extern("noShow" "") int toggle - "nosho"; 
}
