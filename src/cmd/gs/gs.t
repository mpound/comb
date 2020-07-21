parse("cmnd_.gstr")

"Put STO data in st 1" "gs";
{
        "File number" "fn":int old("1") - "fnum";
	"Specify backend":ex("lbetyp");
	{
		"Backend number" "be":int auto("STOscan_.mbknd") - "lbknd";
	}
	"Specify reference stack":ex("refsp");
	{
		"stack number" "refstk":int fl("refstkf") old("2") - "refstk";
	}
	"Specify hot stack":ex("hotsp");
	{
		"stack number" "hotstk":int fl("hotstkf") old("3") - "hotstk";
	}
}
