parse("cmnd_.gstr")

"Write an image to a FITS file" "wf";
{
	"Fits file name" "ff":str("64") old("/dev/rmt12") - "ff";
	"Action" : ex("act") fl("actf");
	{
		"Image number" "im": int old("1") - "imn";
		"Stack number" "st": int old("1") - "stn";
		"Close tape" "ct":;
	}
        "Object" "o" : str("68") set("") - "obj";
	"Telescope" "tel": str("68") set("") - "tel";
	"Reverse file in y" "revy": int flag - "revy";
	"Min Image Data Value for scaling" "dmin": real set("DRAGON") - "dmin"; 
	"Max Image Data Value for scaling" "dmax": real set("DRAGON") - "dmax"; 
	"Bitpix to use" "bitpix" : int old("16") - "bitpx";
	"Blanking Value:" "blank":real auto - "blnk";

}
