parse ("cmnd_.gstr")

"Make a Hard copy of the current screen" "hc";
{
	"Type of device":ex("typ");
	{
		"Astro2" "a":;
		"Bima2" "b":;
		"Suns2" "s":;
		"Taurus" "t":;
		"Imagen 1" "unused":;
		"Imagen 2" "unused":;
		"LaserJet 1" "unused":;
		"LaserJet 2" "unused":;
		"HP-GL 1" "unused":;
		"Tektronix 1" "unused":;
		"PostScript file" "pf": str("60") auto("hcfile") - "hcfn";
		"LaserJet file" "lf": str("60") auto("hcfile") - "hcfn";
		"Tektronix file" "tf": str("60") auto("hcfile") - "hcfn";
	}
	"Page Orientation":ex("ori");
	{
		"Landscape" "land":;
		"Portrait" "port":;
	}
}
