parse("cmnd_.gstr")

"Image manipulate" "im";
{
	"Operation" : ex("op");
	{
		"Move an image" "mv";
		{
			"Source" "src": int old("1") - "src";
			"Destination" "dest": int old("2") - "dest";
		}

		"Add 2 images" "add";
		{
			"Image Addend 1" "ima": int old("1") - "im1";
			"Image Addend 2" "imb": int old("2") - "im2";
			"Resultant Image" "res": int old("3") - "im3";
			"History label" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Subtract 2 images" "sub";
		{
			"Subtractor" "ima": int auto - "im1";
			"Subtractee" "imb": int auto - "im2";
			"Resultant Image" "res": int auto - "im3";
			"History label for resultant image" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Divide 2 images" "div";
		{
			"Numerator" "ima": int auto - "im1";
			"Denominator" "imb": int auto - "im2";
			"Resultant Image" "res": int auto - "im3";
			"History label for resultant image" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Multiply 2 images" "mul"; 
		{ 
			"Multplier" "ima": int auto - "im1";
			"Multplicand" "imb": int auto - "im2";
			"Resultant Image" "res": int auto - "im3";
			"History label for resultant image" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Rescale and add constant to image" "rs";
		{
		        "Rescale factor" "fact":real set("1") - "fact";
		        "Addend" "ad":real set("0") - "set";
			"Image Number" "im":int old("1") - "imn" ;
			"History label for resultant image" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Take a Subimage"  "si";
		{
			"Source" "src": int auto - "src";
			"Destination" "dest": int auto - "dest";
			"Specify Subimage By":ex("ssb") fl("ssbf");
			{
		         "Area number" "an": int old("1") - "an";
			 "Corner Points" "po"; 
			 {
                		"Left x" "lx" : real old("0") - "llx";
                		"Right x" "rx" : real old("0") - "urx";
                		"Bottom y" "by" : real old("0") - "lly";
                		"Top y" "ty" : real old("0") - "ury";
			 }
			}
		}
		"Compute dust temperature map from IRAS flux maps"   "temp";
		{
			"Short WaveBand Flux Map" "ima": int auto - "im1";
			"Long WaveBand Flux Map" "imb": int auto - "im2";
			"Resultant Image" "res": int auto - "im3";
			"Power-law dependence of opacity on wavelength" "pow" : real old("-1.0") - "tpow";
			"History label for resultant image" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Perform color correction on IRAS flux map" "cc";
		{
			"Flux Map to be corrected" "ima": int auto - "im1";
			"Temperature Map" "imb": int auto - "im2";
			"Resultant Image" "res": int auto - "im3";
			"History label for resultant image" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Compute opacity map from IRAS flux and temperature maps"  "tau";
		{
			"Color-corrected Flux Map" "ima": int auto - "im1";
			"Temperature Map" "imb": int auto - "im2";
			"Resultant Image" "res": int auto - "im3";
			"History label for resultant image" "hl":str("50") fl("ophf") auto - "hl";
		}
		"Print Header information" "ph";
		{
			"Image Number" "im":int auto - "imn" ;
		}
		"Add History information to header" "ah" ;
		{
			"Image Number" "im":int auto - "imn";
			"User History info" "uh":str("50") fl("userhf") set("") - "uh";
		}
	}
	"Minimum value to include in 1st image" "mina": real set ("-1e30") - "mina";
	"Minimum value to include in 2nd image" "minb": real set ("-1e30") - "minb";
}
