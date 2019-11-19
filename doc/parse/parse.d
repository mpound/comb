
1. General

The COMB input structure is in the form of a tree.  Each leaf represents a
single datum that may be entered by the user.  The internal nodes define the
relationships between the leaves.  In a plot command, for example, the START,
STOP, and STEP parameters for the x-axis might be grouped together by being
the three children of one node.  That node might be grouped with other nodes.
in a similar way.

In addition to grouping leaves that belong together, internal nodes may group
nodes that are mutually exclusive.  Such nodes are called "exclusive", while
those that group like leaves are "normal".  Generally, only one child of an
exclusive node is used at a time.

Certain rules are followed for deciding what part of the input line will be used
as the data for each leaf.  In general, the input line is read from left to
right and COMB keeps track of the node currently being input.  Various char-
acters and strings can be used to move the current node pointer around on the
tree (sect. 3).

Each leaf corresponds to one parameter of the command and contains information
describing how the input line is to be interpreted, how it should default and
where the resulting datum should be stored.

2. Data Types

The way in which the input is interpreted for a given leaf is dictated by the
"data type" of that leaf.

Each leaf has one of four data types:  STRING, SCAN NUMBER, INTEGER, or REAL.
SCAN NUMBERs, INTEGERs, and REALs may also be RANGEs.  A RANGE is specified by
an upper bound U, and a lower bound L.  It is input as "U_L".  If only one
value is specified for a RANGE, then the upper and lower bounds are both set to
that value.

There follows a description of each data type.

	STRINGs:

	The simplest form of a STRING is a group of characters enclosed in
	double or single quotes (" or ').  There are also five string variables,
	named $0 (or $) to $4, which may be used as STRINGs.  A STRING VARIABLE
	followed by "=" and a STRING will assign that STRING to the STRING
	VARIABLE and have the value of the STRING.  Thus "$1='hello'" will set
	$1 equal to "hello" and will itself have the value of "hello".  A STRING
	VARIABLE not followed by "=" will have the value of that STRING VARI-
	ABLE.  Note that "$2=$1" will set $2 equal to $1.

	STRINGs may be concatinated by placing them end to end with no inter-
	vening characters.  Thus "'hello ''there'" would have the value "hello
	there", and "$1'there'" (after the above example) would have the value
	"hellothere".

	The following are examples of valid STRINGs:

		"hello"
		'hello'
		$="hello"
		$4="hello"
		"hello ""there"
		$4" there"
		$4
		$(2+2)
		$0$4

	SCAN NUMBERs:

	A SCAN NUMBER is a three character alphanumeric file name (of which the
	last character must be alpha) followed by an integral number.  If only
	part of the SCAN NUMBER is given, the result will be the last SCAN
	NUMBER used with the right most characters replaced by the input.  Thus
	"1" after having used SCAN NUMBER "abc100" will yield "abc101".  Next
	typing "d50" will yield "abd50", and then "201" will become "abd201",
	and "003" becomes "abd3".

	SCAN NUMBERs may also be incremented or decremented by given amounts
	with the use of "+" and "-" respectively ("abc5+2").  If only an
	increment or decrement is given then it will be applied to the last
	SCAN NUMBER used ("+2" after "abc5" will become "abc7").

	The following are examples of valid SCAN NUMBERs:

		80a6
		dd7
		12+32
		abc3+2*2	(will be "abc7")
		+3
		-5

	INTEGERs and REALs:

	The only difference between an INTEGER and a REAL is that the INTEGER
	will be rounded to the nearest whole number.  INTEGERs and REALs may be
	given as numbers or they may be arithmetic expressions.  There are
	also ten NUMERIC VARIABLEs, named #0 (or #) through #9, which work the
	same way as STRING VARIABLEs.

	The following arithmetic symbols are recognized:

		"+" and "-"	addition and subtraction
		"*" and "/"	multiplication and division
		"^"		exponentiation
		"e"		*10^
		"ln("...")"	natural log
		"log("...")"	log base 10 (an alternate base
				may be inserted between the "g"
				and the "(" - "log2(16)")
		"bnd("...")"	bound; bnd takes three arguments
				separated by "|".  The value of bnd
				will be the value of the first argument
				if it is above the second and below the
				third.  Otherwise, the value of bnd
				is the second argument if the first is
				equal to or below it, or the third if
				the first is equal to or above it.  Thus
				bnd(3|2|5) = 3, bnd(1|2|5) = 2, and
				bnd(6|2|5) = 5.
		"(" and ")"	grouping
		"="		assignment (for NUMERIC
				VARIABLEs)
		"&"		increment by (for NUMERIC
				VARIABLEs - "#1&2" adds 2 to
				#1)

	The following are examples of valid INTEGERs and REALs:

		34.5
		log3(9)
		log(5e3)
		bnd(#|1|50)
		-4
		2e(4+2)
		2^(1/2)
		#=5
		#2&1
		#(1+1)

3. Moving the Current Node Pointer

As previously noted, COMB keeps track of a current node which describes the
data that is being input.  This node is always pointed to by the "current node
pointer".  The current node pointer starts at the left most leaf.

If the current node pointer were not to move then the entire input line would
be read into the first (left most) leaf.  To give the user control, the pointer
may be moved with the use of a variety of symbols.  Most of them are described
below ("[" and "]" are described in sect 4).

	Comma:

	Commas move the current node pointer one leaf to the right.  Leaves
	related to the current node only through at least one exclusive node,
	however, are skipped.

		Example:

		Suppose the tree looks like this (in outline notation)

		Normal (A)
			Exclusive (B)
				Normal (C)
					Leaf (D)
					Exclusive (E)
						Leaf (F)
						Leaf (G)
				Normal (H)
					Leaf (I)
					Leaf (J)
			Leaf (K)

		(The type of each node and its label are given.  Each node's
		children are indented below it, listed in order from left to
		right.)

		If the current node is leaf F, and a comma is typed, the current
		node pointer will skip leaf G because it is related to leaf F
		through exclusive node E.  It will also skip leaves I and J
		because they are related to leaf F through exclusive node B.
		Leaf K is related to leaf F through the normal node A and is not
		skipped.  Thus, the next current node after typing a comma is
		leaf K.

		Note that only one descendant of exclusive node B is ever the
		current node (leaf F).

	Spaces:

	A group of any number of spaces will act in exactly the same way as
	commas unless they are preceded or followed by special characters.

	Labels:

	Each leaf and normal node has a one to four character name.  When COMB
	sees a group of alphabetic characters not enclosed in quotes and ending
	with a period, it will interpret it as a label.  COMB will then search
	for the given label on the tree and set the current node pointer to
	the left most descendant of that node.

	When COMB searches for a label it always starts with the siblings of
	the current node.  If it does not find the label there it will move on
	to the siblings of the current node's parent, then the current node's
	grandparent, and so on to the root (where it generates an error). Note
	that the current node's cousins are never searched.

	During the search exclusive nodes are treated as if they did not exist.
	When COMB gets to an exclusive node it searches the nodes children.
	It is as if all the exclusive nodes were deleted and their children were
	one generation younger.

	It is often useful to have many labels in a row (to get to one of the
	current node's cousins).  In these cases, the periods between the labels
	aren't needed.  (A word of caution:  when the labels aren't separated
	by periods, the longest possible label will be found.  Thus "ab." will
	always be interpreted as "ab." rather than "a.b." if there is a node
	labeled "ab").

		Example:

		Suppose the tree looks like this

		Node A
			Node B
				Node C
					Node D
					Node E
				Exclusive
					Node F
					Node G
			Node H
				Node I
				Node J
			Node K
				Node L
				Node M

		and the current node is node D.  When COMB sees "K." it will
		search through the tree in the following order: D,E,C,F,G,B,H,
		K.  When it finds K, it sets the current node pointer to L
		(the first descendant of K).  Note that the children of the
		exclusive node (F and G) are searched.

4. Moving the Root

It is possible to move the current node pointer accidently to a leaf you did
not intend.  When this happens, you may think you are entering one parameter
while actually entering another.  To help guard against this, the special
characters "[" and "]" may be used to move the root.  The group of leaves that
the current node pointer can point to is restricted to the the descendants of
the root.  Thus moving the root down the tree reduces the possibility of
reaching an unwanted leaf.

A "[" will move the current node pointer in exactly the same way as a group of
spaces (sect. 3).  It will also move the root down one node toward the resulting
current node (skipping exclusive nodes).  If "[" follows a label (separated
by zero or more spaces), the root will be set to the node described by that
label.

A "]" will move the root to the parent of the current root (skipping exclusive
nodes) and the current node pointer to the left most descendant of the node to
the right of the current root.

	Example:

	Suppose the tree looks like this

	Node A
		Exclusive
			Node B
				Node C
				Node D
			Node E
				Node F
				Node G
		Node H
			Node I
			Node J

	and the current node is C.

	When the user types "[", the root is moved down (skipping the exclusive
	node) to node B.  Only nodes C and D can be reached.  If the user then
	types "]" the root is moved back up (skipping the exclusive node) to
	node A and the current node pointer is moved to node I (the left most
	descendant of node H - the next node after B).  If the user then types
	"E.[", the current node pointer will move to node F (the left most
	descendant of node E) and the root will move to node E (the node
	specified by the label "E.").  Only nodes F and G can be reached.

5. Defaults

It is clear that many leaves might never be set by the user.  When this happens
the parameter corresponding to the leaf will be set to a default value.  The
value that the parameter is set to depends on the "default type" of the leaf.

Each leaf has one of five default types:  SET, OLD, AUTO, FLAG, and OLD FLAG.

There follows a description of each default type.

	SET:

	Parameters that correspond to a leaf with a SET default type default to
	a set value.

	OLD:

	These parameters will default to the last value set for them.  The
	initial default (what they will default to before they have been used)
	is set by the command.

	If a scan number has this default type, then it will default to the
	last scan number used.

	AUTO:

	This is an automatic default.  The command will pick a value for these
	parameters on the basis of other data.  In a plot command, for example,
	the START, STOP, and STEP parameters for the x and y axis would have
	the AUTO default type and default to reasonable numbers given the data
	to be plotted.  This is also used for parameters that may not default.

	FLAG:

	A parameter with the FLAG default type will default to 1 if it is
	explicitly accessed with a label but no value is given, and 0 if not.
	For example, node F has a FLAG default type.  If node F is completely
	left out of the input line, it will default to 0.  If, however, "F."
	is typed, but no value is given (the next character moves the current
	node pointer), then it will default to 1.

	OLD FLAG:

	This is exactly like FLAG except that it defaults to its old value
	when not specified (as opposed to 0).

6. On line documentation and interactive operation

The COMB command parser will give on line documentation when it sees "??"
and "?!".  It will enter an interactive mode when it sees "?".  These three
actions are described below.

	"??":

	When COMB sees "??" on the command line it will print out the
	command's tree structure in outline notation and stop execution of
	the command.

	In the outline of the tree structure, each node is represented with
	a description followed by some information.  The description is meant
	purely for human interpretation.  The format of the information depends
	on the type of node.  For a normal node the information is just the
	label of that node in parenthesis.  For an exclusive node the label
	in parenthesis is replaced by the word "EXCLUSIVE". A leaf has four
	pieces of information after the description arranged in the following
	format:  "(" label ") - " data type "," single/range "," default type.
	The data type is one of the words "STRING", "SCAN", "INTEGER", or 
	"REAL".  Single/range is either "SINGLE" to show that a single value
	is expected, or "RANGE" to show that the leaf represents a range (note
	that STRINGs are always SINGLE).  See section 2 for a description of
	what these mean.  The default type is one of the words "SET", "OLD",
	"AUTOMATIC", "FLAG", or "OLD FLAG".  See section 5 for a description
	of what these mean.  If the default type is not AUTOMATIC, then the
	value that the leaf is currently set to default to is given in paren-
	thesis.

		Example:
	
		>  p ??
		Print a value (p) 
		  Value type (EXCLUSIVE) 
		    Number (n) 
		      Value (v) - REAL,SINGLE,OLD(0)
		      Translate from plot to type (EXCLUSIVE) 
		        As is (as) - INTEGER,SINGLE,FLAG(0)
		        Channel (ch) - INTEGER,SINGLE,FLAG(0)
		        Frequency (fr) - INTEGER,SINGLE,FLAG(0)
		        Velocity (vl) - INTEGER,SINGLE,FLAG(0)
		    Scan number (sc) - SCAN,SINGLE,OLD(0)
		    String (s) - STRING,SINGLE,OLD('')
		    Skip a line (l) - INTEGER,SINGLE,FLAG(0)

		This is the tree for the command p.  Note all the flags
		under the exclusive node "Translate from plot to type";
		these are used to form a selector.  In such cases the value
		they are set to default to is often meaningless.

	"?!":

	When COMB sees "?!" it will print the on line documentation for the
	command if it is available and stops execution of the command.

		Example:

		>q ?!
		q - exit comb
		
		TREE STRUCTURE:
		Exit comb (q)
		
		DESCRIPTION:
		Q simply exits comb.
		
		EXAMPLES:
		q				exit comb

		This is the on line documentation for the command q (quit).

	"?":

	When COMB comes across a question mark (not inside a string), it will
	print out a description of the current node (see "?!" for the form
	of the description) and wait for further input.  The characters input
	are inserted in the command line at the location of the question mark.
	Note that this is recursive; a question mark in the newly typed part
	of the command will be dealt with in the same way.

	It is often usefull to end each input with ",?" as this will do the
	same thing for the next node.  For this reason the special character
	":" may be used to mean the same thing.  If the last character of a
	newly input part of the command line is ":", it will be replaced with
	",?".

		Example:

		> p ?
		Print a value (p) 
		  Value type (n/sc/s/l) 
		    Number (n) 
		      Value (v) - REAL,SINGLE,OLD(0): 3:
		      Translate from plot to type (as/ch/fr/vl) 
		        As is (as) - INTEGER,SINGLE,FLAG(0): fr.?
		        Frequency (fr) - INTEGER,SINGLE,FLAG(1)

		Note that in the descriptions of the exclusive nodes "Value
		type" and "Translate from plot to type" the word "EXCLUSIVE"
		was replaced with the labels of thier children separated
		by slashes.
