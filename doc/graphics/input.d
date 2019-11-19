.(x

Graphics input
.)x
.sh 1 "Graphics input"
.pp
Many devices have some mechanism for entering graphics input, be it
graphics cursor, mouse, bit pad, light pen, etc.
These can be quite usefully for graceful interaction with the graphics.
The COMB graphics package supports graphics input with the routine
Pcursor().
.pp
Each graphics input device allows the user to specify a location on the
screen.
Once the location is selected, the user hits a key on the keyboard or pushes
a button to indicate that that is his selection.
Usually there is more than one way of specifying the selection, and
the method chosen by the user is reported to the host computer.
The COMB graphics package defines one of these ways as the "ending indicator."
.pp
The ending indicator is provided as a method of handling variable length
sequences of graphics inputs.
Drawing a sequence of line segments, for example, entails selecting the
starting point with the graphics input device, then selecting a sequence of
points to be drawn to.
The user tells the system that he's done selecting points by using the
ending indicator to end the last graphics input.
In general, a program that uses such a sequence of graphics inputs should
do so with the form:
.(l I
do
{
	var = Pcursor();
	      .
	      .
	      .
}
while(!var.endFlag);
.)l
(see Pcursor() for information on what's going on here).
.pp
An example of a graphics input mechanism can be found on the hp2648.
Here the mechanism is a graphics
cursor.
It is moved around on the screen with the arrow keys until it is in the
location which the user desires.
The user indicates that the location of the cursor is his selection by hitting
any key on the keyboard.
Both the location of the cursor and the key that was hit are sent to the
host computer.
In the COMB graphics package, the escape key is defined to be the ending
indicator.
.pp
If a graphics input is attempted on a device that has no graphics input
mechanism, then the user will be asked to indicate his choice numerically.
.pp
Pcursor() is described below.
.(x
	Pcursor()
.)x
.sh 2 "Pcursor - get graphics input"
.ti -0
Synopsis:
.(l I
GCREAD Pcursor();
GCREAD var;
var = Pcursor();
.)l
.pp
Pcursor() activates the graphics input device of the terminal being used
and waits for the user to select a point on the screen.
It returns a GCREAD structure (defined in "/cdata/comb/src/graphics/C.h")
which gives the following details of the graphics input (expressed here
as fields of <var>):
.in +4
.sp
.ti -4
<var>.isPlot - This is flag which indicates whether or not there is a picture
displayed in the area chosen by the user.
If isPlot is 1, then there is a picture displayed there, if 0, then there is
not.
isPlot is of type int.
.sp
.ti -4
<var>.xBox - This is the column number of the box chosen by the user.
If, for example, there are four boxes on the screen (a situation set up by
"Pboxes(2,2);"), and the user selects the lower left hand corner, then
xBox is 0.
xBox is of type int.
.sp
.ti -4
<var>.yBox - This is the row number of the box chosen by the user.
It is type int.
.sp
.ti -4
<var>.x - This is the scaled x location of the input in the graph window in
the picture chosen by the user.
If, for example, the left boundary of the graph window is -2, and the user
selects the lower left hand corner of the window (not the whole screen, but
the graph window), then x will be -2.
The user may select a location to the left or right of the graph window;
x will simply be given a value outside the left or right boundaries.
If the point selected is too far outside of the window, however, and there
is more than one box on the screen, then the entire graphics input may be
interpreted as being in the next box over.
x is of type double.
.sp
.ti -4
<var>.y - This is the scaled y location of the input.
Its behavior is exactly like that of x except that it is vertical.
y is of type double.
.sp
.ti -4
<var>.endFlag - This is a flag which is set if the user uses the ending
indicator to terminate the graphics input.
It is of type int.
.in -4
