parse("cmnd_.gstr")

"Switch data files" "nf";
{
	"Pathname of new file" "pn": extern("scan_.datfn" "") str("128")
		auto("scan_.datfn") - "nfln";
	"Just print current scan file" "p":int flag - "prn";
}
