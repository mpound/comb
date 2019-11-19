.(x

Plotting
.)x
.sh 1 "Plotting"
.pp
This is the core of the graphics package: the routines for actually
drawing the graph.
These are the routines for drawing lines and symbols.
.pp
All the plotting takes place in windows, which are defined by the format.
By default, the graphics is put into the graphics window: window 1.
Every format also has a window 0, which is the entire screen.
By using window 0, one can get access to essentially "raw" graphics
(though the x and y dimensions are still scaled according to Pbounds()
or Plogbounds()).
Switching from window to window is accomplished with the routine
Pwindow().
.pp
When lines or symbols go outside of the window they are drawn in, overflow
lines are generated.
These are lines just outside the window and parallel to its sides that
indicate where the graphics have gone out of bounds.
Formally, all the lines outside the window to the left are projected into
a line (one dimension) just left of the window; all the graphics below
the window are projected into a line just under the window; etc.
(Graphics numbers are an exception; see below).
This overflow flagging can be turned on and off with Povrflow().
It is turned on automatically
with a call to any picture manipulation routine that
changes the current picture (Pstart(), Pscreen(), Prestore(), and Pread()).
.pp
In addition to lines and symbols, the COMB graphics package can draw
graphics numbers.
These are numbers which are drawn line by line.
They have a variety of properties which straight text do not have (see "Text").
First, they are clipped: if a graphics number goes partly outside the
window, the part outside is not drawn.
Second, they scale with the picture: when a picture is drawn very small
because it is in a small box on the screen (see "Pboxes"), the graphics numbers
are also small.
Last, they erase the graphics behind them: before they are drawn, the
area they are to cover is cleared (if the device can clear part of its
display area).
Graphics numbers can be drawn with the routine Pgrnum().
Graphics numbers do not create overflow lines.
.pp
The package supports 256 different line types.
Obviously no device has so many line types, so the type of a line is
scaled down depending on how many types the device can display.
COMB line types 100 through 199 map into an increasingly
attention grabbing set of line types on any given device.
varying whatever parameters are available on the device.  Types
0 to 99 control 2 dimensions of line type separately.  The units
digit controls the 'dottedness' of the line and the tens digit
controls its width or color.  The available line types on any one
device are, of course, the same whether accessed as a type between
0 and 99 or 100 to 199, but the ordering may be different.  Line
types between 200 and 255 could be used for predefined line types,
but none exist yet.  They all display as type 99.
The hp2648, for example, can display 8 line types.
Line types 0 to 9 are type 7 (dotted) to the terminal.
Types 10 through 19 map into type 9 (small dashes in groups).
Types 20 to 39 all map into terminal line type 6 (dashed).
And so on up to types 90 through 99, which map into type 1 (solid).
Switching from line type to line type is accomplished with the routine
Plinetype().
The default line type at the beginning of a plot is 255.
.pp
There follows a description of all the plotting routines.
.(x
	Pwindow()
.)x
.sh 2 "Pwindow - set current window"
.ti -0
Synopsis:
.(l I
Pwindow(<window>);
int <window>;
.)l
.pp
Pwindow() sets the window to be plotted in to <window>.
All subsequent calls to plotting routines put their results in that window
(until window is called again).
.pp
Every format has at least two windows: the graph window and the screen window.
The graph window is window 1.
It is the one in which most of the graphics should be drawn.
The screen window is window 0.
It is the full screen.
Auxiliary windows start at window 2 (see "Formats").
.pp
Windows may or may not be scaled according to the arguments of Pbounds() or
Plogbounds(), depending on the format.
In fact, some windows may be scaled in x but not in y, or y but not in x.
A window which is not scaled has boundaries 0 to 65535.
Windows 1 and 0 are always scaled in both x and y.
.pp
Pwindow() may be called any number of times during the construction of a
picture (it's not like the setting up routines).
.pp
The default window is window 1 (the graph window) at the beginning of a plot.
.pp
See "Formats" for more about windows.
.(x
	Povrflow()
.)x
.sh 2 "Povrflow - turn overflow marking on or off"
.ti -0
Synopsis:
.(l I
Povrflow(<on?>);
int <on?>;
.)l
.pp
Povrflow() turns the overflow marking on or off according to whether
<on?> is 1 or 0, respectively.
When overflow marking is on, any plotting commands (other than graphics
numbers) that try to draw outside of the current window (as set by
Pwindow()) will generate overflow lines on the outside of the window.
.pp
If overflow marking is turned on for some commands, then turned off for
some others, then turned back on for yet more, the first group generate
overflow lines, the second group don't, and the third group do, as one
would expect.
.(x
	Pborder()
.)x
.sh 2 "Pborder - draw a border around the current window"
.ti -0
Synopsis:
.(l I
Pborder();
.)l
.pp
Pborder() draws a border of the current line type around the current window.
.(x
	Pfigure()
.)x
.sh 2 "Pfigure - draw a graphics figure"
.ti -0
Synopsis:
.(l I
Pfigure(<x>,<y>,<figure>,<width>,<height>);
double <x>,<y>;
int <figure>;
double <width>,<height>;
.)l
.pp
Pfigure() draws one of nine figures at point <x>, <y> in the current window.
<width> is the width of the figure.
<height> is the height of the figure.
<x> and <width> are scaled to the values given by Pbounds() or Plogbounds()
if the x axis of the current window is scaled (as it is in the graph window).
<y> and <height> are scaled if the y axis is scaled (as it is in the
graph window).
In a log-linear or log-log plot, <height> will decrease appropriately as
<y> increases.
Likewise with <width> and <x> in log-log (and linear-log) plots.
.pp
<figure> is the number of the figure to be drawn.
It may be one of the following values
(defined in "/cdata/comb/src/graphics/C.h"):
.(l I
F_X - an x.
F_RECTANGLE - a rectangle.
F_TRIANGLE - a triangle pointing up.
F_DTRIANGLE - a triangle pointing down.
F_CROSS - a cross (or "plus" sign).
F_ELLIPSE - an ellipse.
F_DIAMOND - a diamond.
F_BAR - a shape like a capital 'I', intended
	for use as an error bar.
F_OCTAGON - an octagon.
.)l
.pp
Figures that are drawn outside the window, or partly outside the window
generate overflow lines.
.(x
	Pmove()
.)x
.sh 2 "Pmove - move the current plotting location"
.ti -0
Synopsis:
.(l I
Pmove(<x>,<y>);
double <x>,<y>;
.)l
.pp
Pmove() moves the current plotting location to <x>,<y> in the current window.
This is the location at which the next line or piece of text is to begin.
.pp
<x> is scaled to the values set by Pbounds() or Plogbounds() if the x
dimension of the current window is scaled.
<y> is scaled if the y dimension is scaled.
.(x
	Pline()
.)x
.sh 2 "Pline - draw a line"
.ti -0
Synopsis:
.(l I
Pline(<x>,<y>);
double <x>,<y>;
.)l
.pp
Pline() draws a line from the current plotting location to <x>,<y>, and
sets the current plotting location to <x>,<y>.
To draw a sequence of connected lines, then, a program must first call
Pmove() to move to the beginning of the sequence, and then call Pline()
for each subsequent line.
.pp
<x> is scaled if the x dimension of the current window is scaled.
<y> is scaled if the y dimension is scaled.
.pp
Pline() may only be called after a call to Pmove() or Pline().
.(x
	Pgrnum()
.)x
.sh 2 "Pgrnum - draw a graphics number"
.ti -0
Synopsis:
.(l I
Pgrnum(<x>,<y>,<number>);
double <x>,<y>,<number>;
.)l
.pp
Pgrnum() draws <number> as a graphics number at <x>,<y> in the current
window.
.pp
<x> is scaled if the x dimension of the current window is scaled.
<y> is scaled if the y dimension is scaled.
.(x
	Plinetype()
.)x
.sh 2 "Plinetype - set the line type"
.ti -0
Synopsis:
.(l I
Plinetype(<type>);
int <type>;
.)l
.pp
Plinetype() sets the current line type to <type>.
All subsequent calls to Pfigure(), Pline(), and Pgrnum() will have their
results drawn in that line type (that is, all calls until the next
call to Plinetype()).
<type> must be a number between 0 and 255.
.pp
See the introduction of the "Plotting" section for a discussion of line
types.
