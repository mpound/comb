parse("cmnd_.gstr")

"Eliminate bad chans in stack 1." "el";
{
	"Range of bad chans in plot units" "r":real range old("0_0") - "h";
	"Units location is expressed in":ex("unit");
	{
		"Same as plot" "p":;
		"Channels" "ch":;
		"Frequency" "fr":;
		"Velocity" "vl":;
	}
	"Stack number" "st": int set("1") - "stn";
}
