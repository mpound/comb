parse("cmnd_.gstr")

"Define relative coordinate system" "rc";
{
	"System name" "name":
	    extern("rcsys.name" "current")
	    str("20") auto("rcsys.name") - "rcsnm";
	"Coordinates for center":fl("ctypf") ex("ctyp");
	{
		"From Stack" "fs":;
		"Ra, dec" "rd";
		{
			"Ra" "ra": real auto("rcsys.x") - "cc1";
			"Dec" "dec": real auto("rcsys.y") - "cc2";
			"Epoch" "ep":real set("EPOCH") - "cep";
		}
		"L, b" "lb";
		{
			"L" "l": real auto("rcsys.x") - "cc1";
			"B" "b": real auto("rcsys.y") - "cc2";
		}
	}
	"Type of offset":fl("ofl") ex("otyp");
	{
		"Dra-ddec" "r":;
		"Dl-db" "g":;
		"User defined" "u";
		{
			"Units for offsets": fl("uufl") ex("uutyp");
			{
				"Arcminutes" "a":;
				"Degrees" "d":;
			}
			"Rotation of equator": fl("rotef") ex("rote");
			{
				"No rotation" "nr":;
				"Position angle of y axis in center sys" "pa":
				    extern("rcsys.rot" "current")
				    real auto("rcsys.rot") - "rota";
				"Point in current rcs to lie on equator" "eqp";
				{
					"Horizontal" "x": real - "ep1";
					"Vertical" "y": real - "ep2";
				}
			}
		}
		"Odra-oddec" "o":;
		"Arcsec offsets" "as":;
	}
	"Test point" "tp";
	{
		"Horizontal" "x":real fl("t1f") auto - "t1";
		"Vertical" "y":real fl("t2f") auto - "t2";
	}
	"Action":ex("act");
	{
		"Change name of current coordinate system" "ch":;
		"Read relative coordinate system from file" "read":;
		"Store current coordinate system locally" "sl":;
		"Store current coordinate system globally" "sg":;
		"Replace current coordinate system locally" "rl":;
		"Replace current coordinate system globally" "rg":;
		"Delete current coordinate system locally" "dl":;
		"Delete current coordinate system globally" "dg":;
		"Print current coordinate system" "p":;
		"Print local coordinate systems" "pl":;
		"Print global coordinate systems" "pg":;
		"Convert total to rel coords & print" "ttor";
		{
			"Horizontal" "x": real - "t1";
			"Vertical" "y": real - "t2";
		}
		"Convert rel to total coords & print" "rtot";
		{
			"Horizontal" "x": real - "t1";
			"Vertical" "y": real - "t2";
		}
		"Name the default in dir 1 to be the cur sys" "nd":;
	}
}
