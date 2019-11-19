WARNINGS:

	Entire stack range in core
		OkStks
	Getting a value from a stack in core
		GtStkValue
	Lower bound of stack range in core
		OkStks
	Making directory <path>
		OpenStkDir
	Retrieving from and storing to same stack
		MoveStk
	Stack <stacknumber> already empty
		EmptyStk
	Stack <stacknumber> has not been emptied
		ReStk
	Stack <to> already exists
		MoveStk


ERRORS:

	Adding scan number to illegal stack number <stacknumber>
		AddScNum
	Bad foreground/background specifier
		SetStkDir
	Can't add scan number to stack not in core
		AddScNum
	Can't get scan number out of stack not in core
		FrstScNum
		NxtScNum
	Can't open data file '<path>/data'
		OpenStkDir
	Can't open default file '<path>/default'
		OpenStkDir
	Can't open index file '<path>/index'
		OpenStkDir
	Can't open macro file '<path>/macros'
		OpenStkDir
	Can't open search file '<path>/search'
		OpenStkDir
	Can't restore in core stack number <stacknumber>
		ReStk
	Checking existance of illegal stack number <stacknumber>
		StkIsFull
	Directory <path> does not exist
		OpenStkDir
	Emptying illegal stack number <stacknumber>
		EmptyStk
	Endpoints of line for space-velocity data are too close
		Stk1Space
	File name of scan number must be three characters long
		AddScNum
	Getting first scan number of illegal stack number <stacknumber>
		FrstScNum
	Getting scan number out of illegal stack number <stacknumber>
		NxtScNum
	High bound of frequency range less than low bound
		OkStkFreqs
	High bound of stack range is an illegal stack number <high>
		OkStks
	High bound of stack range less than low bound
		OkStks
	Horizontal boundries are too close
		CountStks
		Stk2Space
		StkSearch
	Illegal horizontal resolution <xres>
		Stk2Space
	Illegal interpolation radius <radius>
		SetStkInterp
	Illegal make flag for openning directory
		OpenStkDir
	Illegal scan file name <filename><scannumber>
		AddScNum
	Illegal scan number <filename><scannumber>
		AddScNum
	Illegal space resolution <xres>
		Stk1Space
	Illegal type of interpolation
		SetStkInterp
	Illegal type of stack to look for
		NextStk
	Illegal velocity resolution <yres>
		Stk1Space
	Illegal vertical resolution <yres>
		Stk2Space
	Illegal warn flag for emptying stack
		EmptyStk
	Illegal warn flag for storing stack
		MoveStk
	Illegal warn flag for restoring
		ReStk
	Insufficient data for interpolation
		StkInterp
	Looking for stack after illegal stack number <stacknumber>
		NextStk
	Low frequency less than high
		Stk1Space
	Lower bound of stack range is an illegal stack number <low>
		OkStks
	No default <name> in directory <dir>
		GtStkDflt
	No open background stack directory
		SetStkDir
	No open foreground stack directory
		SetStkDir
	No open stack directory
		CountStks
		EmptyStk
		GtStkValue
		MoveStk
		NearStk
		NextStk
		OkStkFreqs
		OkStks
		ReStk
		SetStkMacro
		Stk1Space
		Stk2Space
		StkInterp
		StkIsFull
		StkSearch
	No open stack directory for storing defaults
		WrStkDflt
	No open stack directory to give defaults
		GtStkDflt
	No stack <stacknumber>
		AddScNum
		FrstScNum
		NxtScNum
	No value macro <macro> in directory <dir>
		SetStkMacro
	Proposed default directory, <path>, does not exist
		SetStkPath
	Restoring illegal stack number <stacknumber>
		ReStk
	Retrieving illegal stack number <from>
		MoveStk
	Sorry - too late to restore stack <stacknumber>
		ReStk
	Stacks in core ignored when finding next stack
		NextStk
	Storing to an illegal stack number <to>
		MoveStk
	Too many scans in stack <stacknumber>
		AddScNum
	Trying to get a value for illegal stack number <stacknumber>
		GtStkValue
	Vertical boundries are too close
		CountStks
		Stk2Space
		StkSearch
