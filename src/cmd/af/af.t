parse("cmnd_.gstr")

"Attach a FITS file to an image" "af";
{
	"Fits file name" "ff":str("64") - "ff";
	"Action" : ex("act") fl("actf");
	{
		"Image number" "im": int old("1") - "imn";
		"Stack number" "st": int old("1") - "stn";
	}
}
