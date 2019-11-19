.(x

Formats
.)x
.sh 1 "Formats"
.pp
The sizes and shapes of windows, properties of places, and various other
pieces of information for a given picture all depend on the picture's
format.
A format for a picture is selected with Pformat() from a set of formats
that are defined in the way described in this section.
.pp
Of course, in most cases this section can be ignored, and a programmer
can use one of the predefined formats for his graph.
If, however, a given application (such as COMB) necessitates a variety of
formats other than the predefined ones, or a new device driver is being
written (which needs to have the "predefined" formats defined for it),
then this section is important.
.pp
Each format contains some information that is terminal independent, and
some that is not.
The terminal independent information for all the formats is given in the
file "form.general," while the terminal dependent information is given
in the file "form.devicename" (where "devicename" is the name of the device
whose driver is being used).
The device dependent format information for the hp2648, for example, is
in "form.hp2648".  (This file has been heavily commented and should be consulted
when preparing new format files for other devices.)
The information for the Versatec will be in "form.versatec."
.pp
By default, these files are found in the directory "/ua/comb/src/graphics/,"
but Pformdir() may be used to select a different directory and, hence, a
different set of format definitions.
.pp
Within each of these files is a list of format definitions.
The form of the definitions in "form.general" is different than that of
the ones in the device dependent files (because "form.general" holds
different information).
In general, however, each definition consists of the name of the format
followed by a number of lines describing what the format consists of.
Blank space is ignored (except as a divider between numbers).
Lines that begin with a '#' (after any amount of blank space) are comment
lines, and are ignored.
The information is given as a collection of numbers and "yes" or "no"
answers, which indicate whether the format has certain properties.
All numbers are integral.
.pp
Below are descriptions of the forms of the device independent and device
dependent definitions, followed by a description of Pformdir().
.(x
	Device independent definitions
.)x
.sh 2 "Device independent definitions"
.pp
Each definition, of course, begins with the name of the format to be used
in calls to Pformat().
The next meaningful (non-comment)
line may contain either "yes" or "no" to indicate
whether or not the graph window in this format should be shaped to make
units square.
On the next line comes the number of auxiliary windows.
The following lines give information about each window.
.pp
Each line that gives information about a window has three "yes" or "no"
strings on it.
The first indicates whether the x dimension of that window should be
scaled to the values set by Pbounds() for the left and right sides of
the graph window.
The second indicates whether the y dimension should be scaled.
The third indicates whether this window should be shaped to make units
square.
(Note, if the window does have square units,
and one or the other dimensions are
not scaled, then the window will have a different aspect ratio from the
graph window.)
.pp
After enough lines have been given to describe all the auxiliary windows
in the format, the next line contains the number of places.
.pp
Here is an example of a valid format definition in "form.general":
.(l I
valid format
	# graph window square?
	yes

	# number of auxiliary windows
	2

	# x scaled?	y scaled?	square?
# window 2
	yes		yes		no
# window 3
	yes		yes		yes

	# number of places
	3
.)l
This definition defines the format "valid format."
It can be used by calling Pformat() with "valid format" as an argument.
The graph window is shaped in such a way that units are square.
There are two auxiliary windows.
Both auxiliary windows are scaled in both the x and y dimensions.
The first auxiliary window (window 2)
does not represent square units.
The second auxiliary window (window 3)
is shaped in such a way that units are square.
It will have the same aspect ratio as the graph window.
Finally, this format has three text places.
Notice that the auxiliary windows start with window 2.
This is because window 0 is the whole screen and window 1 is the graph
window.
(The lines reading "# window 2" and "# window 3" are comment lines; they
don't convey any information about which window is which.
The graphics package automatically starts numbering the auxiliary windows
at 2.)
.(x
	Device dependent definitions
.)x
.sh 2 "Device dependent definitions"
.pp
Each definition begins with the name of the format being defined.
The definitions in the device dependent files need not be in the same
order as the definitions in "form.general," nor do they have to all
be there.
If a format is defined in "form.general," but not in the device dependent
definition file, then an error will only result when a program tries to
display a picture with that format on the device for which it is not defined.
However, if a format is defined in a device dependent file, but not in
"form.general," then an error will result any time a program tries to use
the driver for that device.
If "fred the format" is defined in "form.general," then it may or may not
be defined in "form.hp2648," but if "fred the format" is defined in
"form.hp2648," it must be defined in "form.general."
.pp
The first four lines of the device dependent file define the location and
dimensions of the graph window.
All windows are defined with four lines.
Each of the four lines describes how one edge of the window is to be
computed.
The first line describes the left edge, the second describes the bottom edge,
the third describes the right, and the fourth describes the top.
Each line contains seven numbers.
For the left edge the numbers are as follows:
.in +4
.sp
.ti -3
1. The number of pixels between the left hand side of the screen/box and
the left edge of the window (before adjustment for character sizes).
.sp
.ti -3
2. The number of character type 0 half widths to be added to the location of
the left side of the window.
.sp
.ti -3
3. The number of character type 0 half heights to be added to the location of
the left side of the window.
.sp
.ti -3
4. and 5. The number of character type 1 half widths and half heights
to be added.
.sp
.ti -3
6. and 7. The number of character type 2 half widths and half heights
to be added.
.in -4
.sp
When a picture is shown, the left edge of the window is computed by first
scaling number 1 to the size of the box that the picture is being shown
in, half then multiplying number 2 by the width of character type 0 and adding
it in, then multiplying number 3 by half the height of character type 0 and
adding that in, etc.
.pp
The reason for all this monkeying around is that the size of the character
types is fixed, while the size of the picture is not.
When a picture is shown in a box that is one fourth the size of the screen,
everything is scaled down by a factor of four (actually, distances are
scaled by a factor of two) except for the graphics character sizes.
In order to make sure that characters around the outside of the window
(such as the title or the x and y labels) do not run into it, the effects of
their widths and heights on its size and location must be computed separately.
In fact, this turns out to be a fairly easy way to define windows in spite
of the proliferation of numbers because it eliminates the need to do all
the arithmetic necessary for making sure there is enough room outside the
window for text.
.pp
The line describing the bottom of the window is similar to the one describing
the left, except that the first number is the number of pixels up from the
bottom of the screen rather than the number of pixels from the left.
.pp
The line describing the right of the window is similar to the on describing
the left, except that the character heights and widths are subtracted
(because the characters they compensate for are presumably on the right of
the window).
.pp
The top of the window is described like the bottom except that the character
heights and widths are subtracted.
.pp
So ends the description of the location and shape of the graph window.
.pp
The next line contains only two numbers which give the number of pixels
between the graph window and the overflow lines (see "Plotting").
The first number is the number of pixels on the left and right of the
window, while the second is the number of pixels on the top and bottom.
.pp
Next comes a line with another pair of numbers which give the number
of pixels between the graph window and the x and y labels drawn by
Pxlabel() and Pylabel().
The first number is for the x labels, and the second is for the y labels.
.pp
The next line contains two "yes" or "no" strings and indicates whether
the x and y labels drawn by Pxlabel() and Pylabel() should be drawn
sideways.
If the first one is "yes," then the x labels will be drawn sideways,
if the second one is "yes," then the y labels will be drawn sideways.
.pp
Next come the auxiliary window definitions.
These are each defined by a group of four lines exactly like the graph
window, except that the sequence of seven numbers may be replaced
with the string "graph window" to indicate that that edge of the window
is to be at the same location as the corresponding edge of the graph window.
There must be as many window definitions here as there are in the corresponding
format in "form.general."
.pp
Finally come the place definitions.
Each line defines one place.
The number of place definitions must be the same as the number of places
given in the corresponding format in "form.general."
.pp
A place definition consists of eight pieces of information.
They are:
.in +4
.sp
.ti -3
1. The x location of the place (char half sizes wtr graph top center,
bottom center or side center for places 0-2, else pixels).
.sp
.ti -3
2. The y location of the place (char half sizes or pixels).
.sp
.ti -3
3. The text type in which any text in the place should be drawn.
This may be a number from 0 to 2.
.sp
.ti -3
4. The number of characters allowable on each line of the place,
i.e. its width in characters.
.sp
.ti -3
5. The number of lines of text allowable in the place.
Breaks between lines of text will only be made on spaces in order to
avoid splitting up words.
If there are no spaces in the text, then it will be broken at the end
of the line.
.sp
.ti -3
6. The number of characters to indent lines after the first.
Note: this indentation reduces the number of characters that can
be drawn on the lines.
If number 4 (the number of characters wide) is 10, and number 6
is 2, then the first line of the place can have 10 characters on it, but
subsequent lines may only have 8.
.sp
.ti -3
7. Either "yes" or "no" to indicate whether the place is to be drawn
sideways (reading bottom to top).
.sp
.ti -3
8. Either "yes" or "no" to indicate whether text is to be drawn
centered around the location specified with numbers 1 and 2.
If this is "yes," then the first line of text will be centered around
that location, and subsequent lines will be drawn beneath it, and centered
about that location horizontally (or, if the place is sideways, to the
right and centered vertically).
If number 6 (the number of characters to indent) is not 0, then the subsequent
lines will be shifted the specified number of characters.
.in -4
.pp
Here is an example of a device dependent format definition:
.(l I
valid format
	# graph window
# left
	6 0 2 0 0 0 0
# bottom
	46 0 1 0 0 0 0
# right
	440 8 0 0 0 0 0
# top
	340 0 0 0 0 0 4

	# x overflow	y overflow
	2		2

	# x label	y label
	4		4

	# x sideways?	y sideways?
	no		yes

	# auxiliary windows
	# window 2
# left
	graph window
# bottom
	6 0 0 0 0 0 0
# right
	graph window
# top
	40 0 1 0 0 0 0

	# window 3
# left
	460 8 0 0 0 0 0
# bottom
	graph window
# right
	710 0 0 0 0 0 0
# top
	graph window

	# places
# place 0
	233 350 2 9 2 0 no yes
# place 1
	450 350 1 12 2 2 no no
# place 2
	455 215 0 8 1 0 no yes
.)l
This is the definition for the same format defined as an example in
"form.general" above.
The graph window is on the left, with enough room reserved to the left
of it for the y labels (text type 0), above it for a two line text place
that contains text type 2,  to the right of it for
text place 2 (which is eight characters wide), and
below it for the x labels.
Overflow lines on all four sides will be two pixels away.
Both the x and y labels will be four pixels away.
The y labels will be drawn sideways (reading from bottom up).
Window 2 is directly beneath the graph window.
The left and right sides of window 2 are at the same locations as
the left and right sides of the graph window.
It has space reserved above it for the graph window's
x labels.
Window 3 is on the right.
The top and bottom limits of window 3 are at the same locations as
the top and bottom of the graph window.
Since window 3 is shaped in such a way that the units are square, and
the width of window 3 is less than that of the graph window, it may end
up having to lower its top and raise its bottom.
Place 0 is centered above the graph window, is 32 characters wide, and
2 lines long.
Place 1 is above the graph window, and is two lines long with the first
line containing 66 characters and the second containing 60 after being
indented by 6.
Place 2 is nestled between the graph window and window 3.
See appendix C for pictures of this format.
.(x
	Pformdir()
.)x
.sh 2 "Pformdir - set format directory"
.ti -0
Synopsis:
.(l I
Pformdir(<directory>);
char *<directory>;
.)l
.pp
By default, all the format information is read from the appropriate
files in "/cdata/comb/src/graphics/."
Pformdir() can be used to change the directory from which the format
information will be read to <directory>.
.pp
If <directory> does not end with a slash ("/"), then one will be appended.
.pp
Pformdir() should be called before calling any other routines.
