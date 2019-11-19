.(x

Setting up
.)x
.sh 1 "Setting up"
.pp
After calling Pstart() (which is how all pictures are originated), and
before calling any graphics routines (described in "Plotting" and "Text"),
it is necessary to give some information about the graph to come.
In particular, the format in which the graph will be made and the boundaries
of the graph window.
.pp
The format of the graph defaults to "full plot," which is probably ok for
most programs.
However, some may wish to use "full square plot,"
which arranges that the units
in the graph window are square; a given physical distance on the graph
represents the same range of values in both the x and y dimensions.
The command to do this is:
.(l I
Pformat("full square plot");
.)l
.pp
The default values for the boundaries of the graph window are 0 to 65535 in
both dimensions.
These values, I suspect, will be rarely satisfactory, and it is probably
worth it to set the boundaries explicitly with Pbounds() or Plogbounds(),
which allows for log-linear, log-log, and linear-log (if anybody really wants
it) scaling.
.pp
All the routines for setting up are described below.
None of these routines may be called after calling a graphics routine.
.(x
	Pformat()
.)x
.sh 2 "Pformat - select the format for a graph"
.ti -0
Synopsis:
.(l I
Pformat(<name>);
char *<name>;
.)l
.pp
Pformat() sets the format that the following graph is to be drawn in.
<name> is the name of the format as given in the format files (see
"Formats" and "Internal documentation").
.pp
There are four predefined formats: "full plot," "full square plot,"
"small plot," and "small square plot."
The difference between the "square" formats and the non-square formats is
that the graph window will be shaped in such a way that 1 unit in each
dimension is represented by the same distance; that is: units are square.
The difference between the "full" and "small" formats is that the "full"
formats have text places for labeling the x (place 1) and y (place 2)
axes.
The "small" formats are intended for use when multiple graphs are being
placed on the screen, and there is no room for x and y labels.
Actually, the graph window in a "small" format is bigger than that in the
corresponding "full" format because it doesn't have to make room for the
extra labels.
.pp
All the default formats have a text place for a title (place 0).
None of them have any auxiliary windows (the only window they
have is the graph window).
.pp
For more on formats, see "Formats."
.(x
	Pbounds()
.)x
.sh 2 "Pbounds - set the x and y boundaries of a graph"
.ti -0
Synopsis:
.(l I
Pbounds(<left>,<bottom>,<right>,<top>);
double <left>,<bottom>,<right>,<top>;
.)l
.pp
Pbounds() sets the boundaries of the graph window.
The left side of the window represents the value <left>, the bottom of
the window represents the value <bottom>, etc.
All graphics commands executed in the graph window will scale their
coordinates accordingly.
.pp
If <left> is bigger than <right>, then the graph will be reversed.
If <bottom> is bigger than <top>, then the graph will be upside-down.
.(x
	Plogbounds()
.)x
.sh 2 "Plogbounds - make log-linear or log-log graph"
.ti -0
Synopsis:
.(l I
Plogbounds(<left>,<bottom>,<right>,<top>,<xlog?>,<ylog?>);
double <left>,<bottom>,<right>,<top>;
int <xlog?>,<ylog?>;
.)l
.pp
Plogbounds() sets the boundaries of the graph window in the same way
as Pbounds() except that either the x axis, or the y axis, or both axes
may be scaled logarithmicly.
If <xlog?> is 1, then the x axis is logarithmic.
If it is 0, then the x axis is linear.
If <ylog?> is 1, then the y axis is logarithmic.
If it is 0, then the y axis is linear.
.pp
NOTE: square plots are meaningless if Plogbounds() is used to make them
log-linear (or linear-log).
Trying to use a "square" format for such a graph will result in an error.
Using a "square" format for a log-log plot, however, will work.
.pp
If <left> is bigger than <right>, then the graph will be reversed.
If <bottom> is bigger than <top>, then the graph will be upside-down.
