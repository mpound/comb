.(x

Text
.)x
.sh 1 "Text"
.pp
Most devices have some sort of built in graphics text, text that can
be drawn in graphics.
The COMB graphics package provides a variety of ways of adding this text
to a picture.
Any device which does not have built in graphics text will have graphics
text built into the driver, in order to remain compatible with the
graphics package.
.pp
The package supports three different "text types."
These types may differ in size, font, color, etc. depending on the device.
Some devices may not have more than one or two appropriate types of
text, so some of the types may appear the same.
In the hp2648, for example, types 0 and 1 are the same.
Like line types, the convention is used that a higher numbered text
type should draw more attention than a lower numbered type.
Ptexttype() can be used to switch between text types, though other routines
may choose a type automatically.
The text type defaults to type 0 at the beginning of the plot.
.pp
The most straightforward way of placing text in a picture is with the
routine Ptext(), which draws text at the current plotting location (as
set by Pmove() or Pline()).
The routines Pstext(), Pctext(), and Pcstext() may also be used to draw
text sideways, centered, or centered and sideways, respectively.
.pp
There may also be "text places," defined by the format, which are locations
for text that may be different on different devices.
Text may be drawn in a text place with the routine Pplace().
The predefined formats "full plot" and "full square plot" both have three
text places defined: one for the title (place 0), one for the horizontal
axis (place 1), and one for the vertical axis (place 2).
The formats "small plot" and "small square plot" each have only one place
defined: the title (place 0).
.pp
Finally, Pxlabel() and Pylabel() can be used to draw numeric labels for
the x and y axes of the graph window.
These labels are drawn a short distance outside the window and may or may
not be sideways, depending on the format.
They are drawn with the device's graphics text, not with graphics numbers
(see "Plotting").
.pp
No attempt is made to clip graphics text that goes outside the current window.
.pp
All the routines for manipulating graphics text are described below.
.(x
	Ptext()
.)x
.sh 2 "Ptext - draw graphics text"
.ti -0
Synopsis:
.(l I
Ptext(<text>);
char *<text>;
.)l
.pp
Ptext() draws the character string pointed to by <text> in graphics text
at the current plotting location.
The current plotting location may be set by Pmove() or Pline().
.pp
The current plotting location is the on the left side of the text
horizontally, and in the center vertically.
Thus, the code
.(l I
Pmove(10.,10.);
Ptext("Hello");
.)l
writes "Hello" on the screen with the intersection between the left
bar and cross bar of the "H" at 10,10.
.pp
There is no current plotting location after a call to Ptext().
A call to Pline() after Ptext() will cause an error.
Calling Ptext() after Ptext(), however, will cause the second string
to be drawn right after the first.
.(x
	Pctext()
.)x
.sh 2 "Pctext - draw centered text"
.ti -0
Synopsis
.(l I
Pctext(<text>);
char *<text>;
.)l
.pp
Pctext() is exactly like Ptext() except that the text is centered both
vertically and horizontally around the current plotting location.
.pp
NOTE: some devices may not have this capability, in which case the text
is drawn as if it had been drawn with Ptext().
.(x
	Pstext()
.)x
.sh 2 "Pstext - draw sideways text"
.ti -0
Synopsis
.(l I
Pstext(<text>);
char *<text>;
.)l
.pp
Pstext() is like Ptext() except that the text is drawn sideways reading
bottom to top rather than left to right.
The current plotting location is on the bottom vertically (which is the
beginning of the string) and in the center horizontally.
.pp
NOTE: some devices may not have this capability, in which case the text
is drawn as if it had been drawn with Ptext().
.(x
	Pcstext()
.)x
.sh 2 "Pcstext - draw centered sideways text"
.ti -0
Synopsis
.(l I
Pcstext(<text>);
char *<text>;
.)l
.pp
Pcstext() is exactly like Pstext() except that the text is centered both
vertically and horizontally around the current plotting location.
.pp
NOTE: some devices may not have these capabilities, in which case the
device will do the best it can.
If it can only center the text, it will
center it;
if it can only draw text sideways, it will draw it sideways;
if it can't do either, it will draw the text as if it had been drawn with
Ptext().
.(x
	Pxlabel()
.)x
.sh 2 "Pxlabel - give a numeric label to the x axis"
.ti -0
Synopsis:
.(l I
Pxlabel(<x>);
double <x>;
.)l
.pp
Pxlabel() draws <x> just below the graph window, centered about the horizontal
location that represents <x>.
The number is drawn with text type 0.
The text type will be left 0 after the call to Pxlabel().
.pp
Pxlabel() may only be called when the current window is the graph window
(window 1).
.(x
	Pylabel()
.)x
.sh 2 "Pylabel - give a numeric label to the y axis"
.ti -0
Synopsis:
.(l I
Pylabel(<y>);
double <y>;
.)l
.pp
Pylabel() draws <y> just to the left of the graph window, centered about
the vertical location that represents <y>.
The number is drawn with text type 0.
The text type will be left 0 after the call to Pylabel().
.pp
Pylabel() may only be called when the current window is the graph window
(window 1).
.(x
	Ptexttype()
.)x
.sh 2 "Ptexttype - set the text type"
.ti -0
Synopsis:
.(l I
Ptexttype(<type>);
int <type>;
.)l
.pp
Ptexttype() sets the text type to be used for subsequent calls to Ptext(),
Pctext(), Pstext(), and Pcstext() to type <type>.
<type> may be a value from 0 to 2.
.pp
A call to Pxlabel() or Pylabel() will change the type to 0.
A call to Pplace() will change the type unpredictably.
.(x
	Pplace()
.)x
.sh 2 "Pplace - draw text in a text place"
.ti -0
Synopsis:
.(l I
Pplace(<place>,<text>);
int <place>;
char *<text>;
.)l
Pplace() draws <text> in place <place> according to the definition of that
place in the format.
A text place can be sideways, can have more than one line, can indent
the lines after the first a certain number of spaces, can have each
line centered, or any combination thereof.
The properties of the text place are defined by the format.
.pp
If <text> has more characters than can properly be drawn in text place
<place>, then the last two characters of the place will be "->"
(indicating that there is more in this string than has been drawn).
If the same picture is subsequently drawn on another device (by Psend()
or by storing it to a file and reading it on another terminal) which has
enough room in text place <place> for <text>, then all of <text> will
be drawn.
That is: it doesn't forget what it's left out.
.pp
The text type of the text in a text place is also defined by the format.
As a result, it is not possible to predict what text type will be used
by Ptext(), Pctext(), Pstext(), and Pcstext() after a call to Pplace().
Ptexttype() should be called in between.
.pp
The predefined formats "full plot" and "full square plot" each have
three places: an overall title (place 0), a title for the x axis (place 1),
and a title for the y axis (place 2).
To give a graph the title "Graph of Foobar," for example, the code
.(l I
Pplace(0,"Graph of Foobar");
.)l
would be used.
The formats "small plot" and "small square plot" only have places for
overall titles (place 0).
.pp
For more about text places, see "Formats."
