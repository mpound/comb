.(x
Appendix B - interactive graphics manipulator
.)x _
.he ''Appendix B''
.hx
.bp
.ce 1
Appendix B - interactive graphics manipulator
.pp
The interactive graphics manipulator, igm, is a program that started out
life as a test program.
It is useful for drawing pictures that have been stored in files and
making little modifications to them.
igm may also be used to draw simple graphs with graphics input.
.pp
igm is in /cdata/comb/bin/, and its source is in /cdata/comb/src/graphics/.
.pp
igm takes simple, one letter commands which are described below:
.in +4
.sp
.ti -4
n - start a new picture.
igm will ask for the format of the picture and the boundaries of the graphics
window.
It calls Pstart(), and then draws a border around the graph window with
3 labels in x and 3 in y.
.sp
.ti -4
w - change window.
igm asks which window subsequent plotting should be done in.
Note: graphics input always relates to the graph window.
.sp
.ti -4
b - draw a boundary around the current window.
.sp
.ti -4
d - draw lines.
The lines are drawn according to the graphics input.
The first location selected by the user is the beginning of the
sequence of lines, and each subsequent location is the endpoint of
the next line.
Use the ending indicator ('esc' on the hp2648) to stop drawing lines.
.sp
.ti -4
c - choose a picture to modify.
Use graphics input to select a box on the screen.
Whatever picture is in that box becomes the current picture.
.sp
.ti -4
m - draw marks.
The user is asked for the type and size of figure to be drawn, and then
graphics input is used to indicate where.
Each point selected is the center of a figure.
The type of figure is a number:
.(l
0 - an x
1 - a rectangle
2 - a triangle pointing up
3 - a triangle pointing down
4 - a cross
5 - an ellipse
6 - a diamond
7 - an "error bar"
8 - an octagon
.)l
.sp
.ti -4
v - view the current picture.
Use graphics input to indicate which box on the screen the picture
is to be drawn in.
.sp
.ti -4
u - update the screen.
Many of the commands do not display their results until this command is given.
.sp
.ti -4
x - divide the screen into an array of boxes.
igm asks for the number of columns and rows (in that order).
.sp
.ti -4
R - read in a picture from a file.
igm asks for the file name.
A single picture may be easily viewed with igm by reading it in with R, and
then showing it with v.
.sp
.ti -4
W - write the current picture to a file.
igm asks for the name of the file.
.sp
.ti -4
p - put some text in a place.
The user is asked for the place number and the text to be put there.
The results aren't displayed until the screen is updated with u.
.sp
.ti -4
s - save the current picture.
A copy of the picture is made in memory.
.sp
.ti -4
r - restore the last picture saved with s.
.sp
.ti -4
i - indicate a specific location in the current window.
This command is like m, except that only one figure is drawn and its
coordinates are given numerically rather than with graphics input.
.sp
.ti -4
* - redraw the screen.
This only works on the hp2648 (even after the graphics package gets more
drivers).
It is basically useless.
.sp
.ti -4
q - quit.
.in -4
