parse("cmnd_.gstr")

"Fit and remove a polynomial baseline" "li";
{
	"Order of poly" "o":int old("1") - "nordr";
	"Width to exclude around sig & ref(plot units)" "wid":real fl("widfl")
		auto - "wid";
	"Action":ex("iacton");
	{
		"Remove baseline from stack1" "rm":;
		"Replace stack1 by poly" "see":;
		"Extend removal to stack2" "xt":;
	}
}
