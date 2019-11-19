.(x

Internal documentation
.)x
.sh 1 "Internal documentation"
.pp
This section of this document is intended for people maintaining the source
code of the COMB graphics package.
The most common changes in the source should be the creation of new device
drivers, and that task is described in the first part of this section.
The second part gives a few of my thoughts on transporting this package to
another machine.
The last part is a rough guide to some of the more weird aspects of the
source.
This is intended for anyone in the position of having to familiarize himself
with this package in order to make modifications or (heaven forbid) fix bugs.
.pp
All the source files are in "/cdata/comb/src/graphics/."
The file "guts.c" contains the code for constructing pictures, and for
doing manipulation that doesn't involve the screen (exception: Pscreen() is
in "guts.c").
It also contains the code for reading the format definitions.
The file "show.c" contains all the code for displaying pictures except for
the actual device drivers themselves (though it contains the routines that
select device drivers).
The device drivers are in separate source files with names of the form
"termtype.driver.c," where "termtype" is something that indicates the types
of terminals that can be used with the drivers in this file (e.g.
"hp.driver.c" or "versatec.driver.c").
.pp
"C.h" is an include file which contains definitions of constants and data
types which are available to programs using the package.
The file "include.h" contains all the definitions that are common to all
the source files of the package.
.pp
All the routines used in the graphics package are static except for the
routines documented above, the routines for setting up the terminal
drivers, and the routine for reading the device dependent format
definitions (which is called by the routines for setting up the terminal
drivers).
.(x
	Constructing terminal drivers
.)x
.sh 2 "Constructing terminal drivers"
.pp
When the COMB graphics package needs to use a particular driver, it calls
a routine that sets up all the information that the package needs.
This is the "driver setup routine."
There is one such routine for each device that the package supports.
.pp
The information that the package needs consists of 10 pointers to routines
for performing various functions, 12 global variables that describe some
important properties of the device, and the device dependent format
definitions.
The 10 pointers and 12 variables are declared in "include.h" (each file of
device drivers should #include "C.h" and "include.h," in that order).
.pp
The format definitions are read in by calling GtSpec() with the name of
the definition file as an argument.
For example, the definitions for the hp2648 are read in with the line
.(l I
GtSpec("form.hp2648");
.)l
NOTE: the file name given to GtSpec() must not contain the directory.
.pp
The setup routine for a given driver is called each time that driver is
needed.
Quite often, a single driver will be used many times in a row without
using any other drivers in between.
In this case, the information about the driver doesn't have to be changed
after the first call to the setup routine.
Since GtSpec() can take a long time, it is undesirable to set up the
information if not absolutely necessary.
There is, therefore, a global string variable, termType, declared in
"include.h" that contains the name of the driver for which
the driver information is set up.
.pp
Before giving values to the global variables that define the driver, and
before calling GtSpec(), a setup routine should compare termType with
the name of the driver it is setting up.
If they are the same, then the driver information should not be given
(and GtSpec() should not be called).
If they are different, then termType should first be set to a null string
("*termType = 0"), then the global variables should be given their values
and GtSpec() called, and then the name of the driver should be copied
into termType.
termType should be null while the driver information is being given so
that if something interrupts the setup routine before it is done, termType
will indicate that no driver is set up.
.pp
Drivers for hard copy devices are exactly the same as other drivers.
It may be desirable, however, to arrange that some of the driver information
be variable (one might wish to be able to change the size of the display
area, or the set of format definitions).
To do this, some variables describing the driver should be maintained in
the file with the source of the driver (please declare them as static if
possible).
Some routines should be provided for changing the values of these variables.
When the setup routine is called, it should not compare termType with
the driver's name; it should just go ahead and set up the driver according
to the variables described above.
NOTE: the setup routine must change termType even if it doesn't compare it
with anything.
Otherwise a different driver might neglect to set itself up.
.pp
The following is a description of the 10 pointers to routines:
.in +4
.sp
.ti -4
d_start - This points to the routine for putting the device in graphics mode.
d_start() is called with no arguments before any other driver routine
(except d_cursor()) is called.
d_start() must set the global variable plotting to 1.
plotting is declared in "include.h."
.sp
.ti -4
d_cleanup - This points to the routine for getting the device out of graphics
mode.
d_cleanup() is called with no arguments when the displaying of graphics is
over.
It must set plotting to 0.
Note: Psend() and Pupdate() call d_cleanup() after ALL of the boxes on the
screen have been drawn.
Thus
it is valid to use d_cleanup() to trigger such things as form-feeds on
hard copy devices.
.sp
.ti -4
d_move - This points to the routine for moving the current plotting location.
Most terminals draw graphics with a sequence of "move" and "draw" commands
which are best understood as applying to an imaginary pen.
d_move() is called with two arguments:
.(l I
d_move(<x>,<y>);
int <x>,<y>;
.)l
It should move the "pen" to location <x>,<y> in the display area.
<x> and <y> are already scaled to the resolution of the display area.
.sp
.ti -4
d_line - This points to the routine for drawing a line.
d_line() is called with two arguments:
.(l I
d_line(<x>,<y>);
int <x>,<y>;
.)l
It should draw a line from the current plotting location (the present
location of the "pen") to <x>,<y> in the display area.
<x> and <y> are already scaled to the resolution of the display area.
.sp
.ti -4
d_setlin - This points to the routine for changing line types.
Most devices keep track of a current line type which can be changed by
a specific command.
d_setlin() should invoke that command.
d_setlin() is called with one argument:
.(l I
d_setlin(<type>);
int <type>;
.)l
<type> may be a number from 0 to 255.
d_setlin() should change the line type to a type related to <type> in such
a way that the higher <type> is, the more the line type should attract
attention.
Thus, if a device can handle 4 different types of lines, which differ in
width, and line type 0 is the thinnest and 3 the thickest, then d_setlin()
should set up the device to draw with line type (int)(<type>/64).
.sp
.ti -4
d_text - This points to the routine for drawing graphics text on the screen.
It is called with two arguments:
.(l I
d_text(<text>,<sideways?>);
char *<text>;
int <sideways?>;
.)l
<text> is the text to be drawn.
If <sideways?> is 0, <text> should be drawn horizontally.
If <sideways?> is 1, <text> should be drawn vertically, reading from bottom
to top.
The text should start at the current plotting location ("pen" position)
and be centered vertically (horizontally if the text is drawn sideways)
around it.
See Ptext() and Pstext() for more on the behavior of graphics text.
.sp
.ti -4
d_ctext - This points to the routine for drawing graphics text centered
on the screen.
It is called with two arguments:
.(l I
d_ctext(<text>,<sideways?>);
char *<text>;
int <sideways?>;
.)l
<text> is the text to be drawn.
If <sideways?> is 0, <text> should be drawn horizontally.
If <sideways?> is 1, <text> should be drawn vertically, reading from bottom
to top.
The text should be centered around the current plotting location.
See Pctext() and Pcstext() for more on the behavior of centered text.
.sp
.ti -4
d_settxt - This points to the routine for setting the text type.
It is called with one argument:
.(l I
d_settxt(<type>);
int <type>;
.)l
<type> is a number from 0 to 2.
Most devices keep track of a current text type that all graphics text is
drawn in.
Types might differ by size, font, "slantedness," color, etc.
This routine should invoke the command for changing the text type.
If <type> is 0, subsequent graphics text should be drawn in an inconspicuous
type.
If <type> is 1, subsequent graphics text should be drawn in a more
conspicuous type.
If <type> is 2, subsequent graphics text should be drawn in an attention
grabbing type.
.sp
.ti -4
d_clear - This points to the routine for clearing a portion of the display
area.
Some devices may not have such a capability (Tektronix terminals, for example),
in which case hasClear should be set to 0 (see below).
d_clear() is called with four arguments:
.(l I
d_clear(<left>,<bottom>,<right>,<top>);
int <left>,<bottom>,<right>,<top>;
.)l
d_clear() should clear the part of the screen inside the rectangle whose
left side is at x location <left>, bottom is at y location <bottom>, right
side is at y location <right>, and top is at y location <top>.
.sp
.ti -4
d_cursor - This points to the routine for getting graphics input.
Some devices may not have a mechanism for graphics input, in which case
hasCursor should be set to 0 (see below).
d_cursor() is called with three arguments:
.(l I
d_cursor(<x>,<y>,<flag>);
int *<x>,*<y>,*<flag>;
.)l
It should activate the graphics input mechanism (if it needs activation) and
read the selected point.
The x and y coordinates of the point should be put in <x> and <y> (note:
<x> and <y> must be pixel coordinates).
d_cursor() should also decide whether or not the user has used the ending
indicator (see Pcursor()).
If he has, <flag> should be set to 1; if not, it should be set to 0.
NOTE: d_start() is not called before d_cursor() -- d_cursor() must be
self-contained.
.in -4
.pp
The following is a description of the 12 global variables:
.in +4
.sp
.ti -4
chX[0] ... chX[2] - These are the widths of spaces in the three types of
graphics characters.
chX[0] is the width of a space in type 0, etc.
These variables are type int.
.sp
.ti -4
chY[0] ... chY[2] - These are the heights of lines in the three types of
graphics characters.
chY[0] is the height of a line in type 0, etc.
These variables are type int.
.sp
.ti -4
xRes - This is the highest pixel number horizontally.
If the horizontal resolution of the device is 720, for example, then
xRes should be 719 (because the pixels are numbered 0 through 719).
Note, if the pixel numbering does not start in the lower left hand corner
at 0,0, then the routines pointed to by d_move(), d_line(), d_clear(), and
d_cursor() must compensate.
The COMB graphics package assumes that 0,0 is the lower left hand corner.
xRes is type int.
.sp
.ti -4
yRes - This is the highest pixel number vertically.
If the vertical resolution of the device is 360, for example, then
yRes should be 359.
yRes is type int.
.sp
.ti -4
xSize - This is the horizontal size of the graphics display area in inches.
xSize is type double.
.sp
.ti -4
ySize - This is the vertical size of the graphics display area in inches.
ySize is type double.
.sp
.ti -4
hasClear - This is a flag indicating whether the device has the capability
of clearing part of the display area.
If it does, hasClear should be 1; if not, it should be 0.
hasClear is type char.
.sp
.ti -4
hasCursor - This is a flag indicating whether the device has a mechanism
for graphics input.
If it does, hasCursor should be 1; if not, it should be 0.
hasCursor is type char.
.in -4
.pp
Once a new driver is written, it can be interfaced with the graphics
package by adding the name of the startup routine to the list in
"/cdata/comb/src/graphics/d.routines," and the name of the device paired
with the name of the startup routine to the list in
"/cdata/comb/src/graphics/d.names."
.(x
	Portability
.)x
.sh 2 "Portability"
.pp
I have made an effort to design the graphics package in such a way that
it will not be inherently untransportable, but there will probably be some
trouble putting it on another machine anyway.
.pp
One almost definite source of trouble is in transferring the package to
a machine with a different word size.
I have relied on 32 bit integers in a number of places.
Most coordinates that are not declared as unsigned short int should probably
be declared long int (I use such coordinates for points that may be outside
the screen, resulting in negative values).
.pp
Another source of trouble, which I have tried to avoid, is in transferring
to a machine that enforces different boundary constraints.
Some machines, for example, insist that a four byte integer be on a four
byte boundary in memory (the address is a multiple of four).
This means that if a three character string is followed by a long integer,
there can easily be a one byte gap between the two.
.pp
This could seriously foul up the interpretation of the intermediate code,
which packs together a variety of different data structures into one
contiguous buffer.
The difficulty would come in interpreting the commands.
A pointer is initially set to the beginning of the buffer, and the first
byte indicates the type of the first instruction.
Following the first byte are the parameters of that instruction (see
"Miscellaneous source documentation").
After executing the instruction, the pointer is incremented by the number
of bytes taken up by the parameters, and the next byte is taken to indicate
the type of the second instruction.
If, for the reasons described above, there is a gap between the first and
second instructions, or between the first byte of an instruction and its
first parameter, then the pointer could easily end up pointing to garbage.
I have tried to design the code so that this won't happen, but if it
does, the most likely result will be that the package errors out with a
message like "Illegal intermediate code instruction."
.pp
Of course, it's the problems I haven't thought of that will cause the most
trouble, but I can hardly relate those here (if I could, I would have thought
of them).
.(x
	Miscellaneous source documentation
.)x
.sh 2 "Miscellaneous source documentation"
.pp
The source code for the COMB graphics package is fairly complicated.
Even worse, it was written in a hurry and is not very tight.
Because I would hate to be in the position of figuring out what was going
on in there without any guide, I shall here try to present at least a
token guide.
.pp
The current picture is pointed to by the global variable cPic, which is
of type PICTURE.
There is also a list of PICTURE variables pointed to by the global variable
boxes (type PICTURE *).
These are the pictures which are currently on the screen.
When the number of boxes is changed, these pointers are deallocated and a
new list are allocated of the appropriate length.
This list is initialized to 0, indicating that there are no pictures on the
screen.
.pp
The PICTURE data type is defined in C.h as a pointer to a pointer to a
structure.
PIC1 is defined as a pointer to the structure, and PIC2 is the structure
itself.
The reason for having the two levels of indirection has to do with
reallocating storage when a picture becomes too large.
If the current picture is on the screen, then the same picture is pointed
to by both cPic and one element of boxes.
As more instructions are added to the intermediate code, the picture
may have to be reallocated to get more room.
This reallocation may change the address of the picture.
To keep from having to change both cPic and the appropriate element of boxes
at this point, I have them both point to a pointer to the picture.
That pointer is the only one that need be changed.
.pp
This is probably not the best way of handling this problem.
Originally, the handling of the current picture was different than it has
ended up.
Before it was redesigned, the pointer to a pointer strategy was necessary
for a variety of reasons I won't go into now.
Unfortunately, I was pretty much stuck with it when I redesigned, which is
why I didn't do some more straightforward thing.
.pp
Another aspect of the PICTURE data type that is not obvious is that the
buffer for the intermediate code is declared to be one byte long.
By allocating more space for the structure than it is defined to occupy,
I arrange for that one byte character array to be followed by an arbitrary
amount of empty space, thus effectively lengthening the array.
.pp
Each command in the intermediate code buffer has associated with it a
data structure that holds its parameters.
This structure is not only used to get the parameters out of the buffer,
but also to indicate where the next instruction begins by its size.
After each instruction is executed, the pointer to that instruction is
incremented by the size of the instruction's parameter structure.
This is done by taking a pointer of the type of the parameter structure,
pointing it to the beginning of the instruction's parameters, incrementing
it by one, and casting it into the type of the pointer to the instruction
(which is character pointer).
Doing things in this way helps prevent the boundary problem described in
"Portability."
The data type PALLARGS is a union of pointers to parameter structures and
is used for this process.
.pp
Instructions that have string parameters have them placed after the
parameter structures in the buffer.
These may be of any length.
.pp
It should be no trouble to add a new field to a parameter structure.
.pp
The construction of the intermediate code is done in "guts.c" by a bunch
of little routines which are almost the same.
Each one makes one instruction: it puts a byte in the buffer indicating what
kind of instruction this is and then fills in a parameter structure and
puts that in the buffer.
.pp
As I said above,
the code is not very tight.
What I meant was that there are a number of things, such as scaling
coordinates, which are done the same way in many places.
Be sure to change all of them if you change one; otherwise, whatever bug
you're fixing will reappear in some remote region of the package.
.pp
One final note on something that may appear odd:
the case in Show() which draws lines (case DLINE) first checks to see that
the next instruction doesn't tell it to continue the line it is to draw.
If the next instruction does, then the case checks the next instruction, and
so on until it hits an instruction that doesn't.
It then draws all the lines at once.
This concatenation of line segments significantly reduces the transmission
time required to draw a graph.
