.sh 1 "Overview"
.(x
Overview
.)x
.pp
This is a description of the graphics package written for the interactive
data reduction program COMB.
The package, however, is designed to be easy to use outside of COMB, and
it shouldn't be difficult to make fairly nice graphs with "quick and dirty"
programs.
I have tried to give this package all the power needed for a complex
interactive program like COMB without making the construction of a simple
program very difficult.
In the text that follows, I shall indicate the aspects of the package that
can be ignored in simple applications.
.pp
The package is oriented around making various kinds of graphs.
The format of a graph is defined separately for each graphics device that
the package supports, so the same graph can be displayed differently on
different devices, making optimum use of the size and shape of the display
area.
For most graphs, it is possible to use one of the predefined formats and
not worry about the details of COMB graphics formats.
Each format consists of a set of graphics windows, text places, and some
miscellaneous information (see "Formats") which are defined in a file
unique to the device being plotted on.
There is one graphics window (window number 1) which is designated as the
graph window.
This is the window that the graph itself should be made in, the other
windows holding auxiliary information.
The "text places" are locations in which to put text.
They may be sideways, centered, multi-line, and/or indented depending
on the device.
.pp
The actual graphics routines (see "Plotting" and "Text") construct a
sequence of instructions in an
intermediate code which describes the graph in a device independent form.
Each such sequence is called a "picture."
Pictures may be displayed on the screen, stored in variables,
or written to files (see "Pictures").
A picture which has been so displayed, stored, or written can be recovered
and further modified with the graphics routines.
More than one picture may be displayed on the screen at a time.
In addition, the graphics that have been displayed (i.e. the stuff on the
screen) may be sent to another device (such as a pen plotter).
Because the intermediate code is device independent, and the formats are
device independent, anything sent to another device will come out in another
format.
Here again, however, it is usually sufficient to simply construct the
graph and display it on the screen without
worrying about the details of picture
manipulation.
.pp
Because it was designed for an interactive program, the COMB graphics
package also contains a facility for getting graphics input.
The input may come from a graphics cursor, a mouse, a bit pad, a track ball,
etc., depending on the device.
(See "Graphics input.")
.pp
One feature of the package that should make it comparatively easy to use
is that all the scaling is done at a low level (see "Setting up").
If one wishes to make a graph in which x ranges from -2 to 5 and y ranges from
100 to 1000, he does not have to scale each point to the resolution of the
graphics device.
He can treat the graph window as if its horizontal boundaries are -2 to 5
and its vertical boundaries are 100 to 1000.
.pp
Finally, there are high level routines provided for making polished graphs.
