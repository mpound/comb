parse("cmnd_.gstr")

"Cursor read" "cr";
{
	"Number of reads to make" "n" : int set("1") - "reads";
	"Change to box with cursor" "cb" : int flag - "cb";
	"Print cursor coordinates" : ex("pcc") fl("pccf");
		{
			"Print both x and y" "p": int flag - "pxyf";
			"Print x only" "px": int flag - "pxf";
			"Print y only" "py": int flag - "pyf";
		}
}
