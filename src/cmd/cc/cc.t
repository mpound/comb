parse("cmnd_.gstr")

"Change center channel" "cc";
{
	"":ex("iactn");
	{
		"New freq" "fr": real auto("stk_[0].freq") - "nwf";
		"New vel" "vl": real auto("stk_[0].vlsr") - "nwv";
		"Oscillator error (km/s to shift line)" "oe": real old("0") -
			"oe";
	        "Flip to opposite SSB, and new freq" "ssbflip": real old("0") -
			"ssbfr";
	}
}
