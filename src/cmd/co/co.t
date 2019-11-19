parse("cmnd_.gstr")

"Combine two stacks, result in 1 & 2" "co";
{
	"Locadd" "ad" : int set ("1") - "locad";
	"Locsum" "su" : int set ("2") - "locsm";
	"Add stacks" "add" : int flag - "addf";
	"Don't check positions" "dc" : int flag - "dcf";
	"Align center channels" "ac" : int flag - "acf";
}
