.(x

Pictures
.)x
.sh 1 "Pictures"
.pp
As mentioned above, the graphics routines described in "Plotting" and
"Text" construct a sequence of low level device independent graphics
instructions called a "picture."
This section describes the ways in which pictures may be manipulated.
Simple programs need not incorporate most of what follows -- they can just
have the form:
.(l I
Pstart();

.ul
commands for making graph

Pshow();
.)l
Or, if you wish to store the graph to a file and look at it later with
igm (see appendix B), your program should have the form:
.(l I
Pstart();

.ul
commands for making graph
.)l
.ti +4
Pwrite(
.ul
filename
);
.pp
All the graphics routines for modifying a picture (that is, the routines
described in "Setting up," "Plotting," and "Text") always modify the
"current picture."
The current picture may or may not be one that is currently displayed on
the screen.
If it is, then a call to Pupdate() will cause any changes made in the current
picture (since the last time it was displayed) to appear on the screen.
If it is not, then the only way to see what it looks like is to call Pshow()
to display it.
.pp
By default, only one picture is shown on the screen at a time.
However, the routine Pboxes() may be used to divide the screen into an array
of "boxes" which can each contain a different picture.
.pp
It is also possible to read the current picture from a file (Pread()), write
it to a file (Pwrite()), copy it into a variable (Psave()), copy it from a
variable (Prestore()), and/or set it to one of the pictures currently on the
screen (Pscreen()).
.pp
All the routines for manipulating pictures are described below.
.(x
	Pstart()
.)x
.sh 2 "Pstart - start the current picture"
.ti -0
Synopsis:
.(l I
Pstart();
.)l
.pp
This routine is used for beginning a new picture.
It clears the current picture and arranges that the current picture is not
associated with a picture on the screen.
Thus, after some graphics routines have been used to construct a graph in
the current picture, Pshow() must be used to display it (Pupdate() will do
nothing).
.(x
	Pscreen()
.)x
.sh 2 "Pscreen - set the current picture to a displayed one"
.ti -0
Synopsis:
.(l I
Pscreen();
   or
Pscreen(<column>,<row>);
int <column>,<row>;
.)l
.pp
This routine causes the current picture to become a picture that is currently
displayed on the screen.
.pp
If there is only one box for displaying pictures
on the screen (the situation by default), then Pscreen() may be called with
no arguments (more precisely, if there is only one box on the screen,
Pscreen() will ignore its arguments).
.pp
If Pboxes() has been used to arrange that there is more than one box (and,
possibly, more than one picture) on the screen, then Pscreen() must be given
its arguments.
<column> is the column number, and <row> the row number of the box that
contains the picture that is to become the current picture.
Box 0,0 is in the lower left hand corner.
.pp
Once Pscreen() is called, the current picture becomes the specified picture
on the screen.
If the previous current picture had not been displayed (with Pshow()),
saved (with Psave()), or written to a file (with Pwrite()), then it is lost
for good.
Any graphics routines which follow will change the picture on the screen.
All the changes will appear with the next call to Pupdate().
.(x
	Pfull()
.)x
.sh 2 "Pscreen - set the current picture to a displayed one"
.ti -0
Synopsis:
.(l I
Pfull(<column>,<row>);
int <column>,<row>;
.)l
.pp
This routine reports whether there is a picture in the box.
.(x
	Psave()
.)x
.sh 2 "Psave - save the current picture in a variable"
.ti -0
Synopsis:
.(l I
Psave(&<picture>);
static PICTURE <picture> = 0;
.)l
.pp
Psave() makes a copy of the current picture in the variable <picture>.
The current picture can later be returned to the same sequence of intermediate
commands that it was at the time Psave() was called by using Prestore().
.pp
The PICTURE data type is defined in "/cdata/comb/src/graphics/C.h," and
consists of a complicated structure of pointers.
If the <picture> variable has the value 0 when Psave() is called, then
Psave() will allocate the memory for it.
If <picture> is not 0, Psave() assumes that it has already had memory
allocated, and reallocates it accordingly.
The routine Pfree() is provided for deallocating the storage.
.pp
NOTE: if <picture> is not initialized to 0, horrible things
will most likely happen.
If <picture> is not static or global,
then Pfree() should be called before the end
of whatever routine <picture> is declared in.
(The call should be "Pfree(<picture>);")
.(x
	Prestore()
.)x
.sh 2 "Prestore - restore the current picture from a variable"
.ti -0
Synopsis:
.(l I
Prestore(<picture>);
static PICTURE <picture> = 0;
.)l
.pp
Prestore() restores the current picture to the value it had when
Psave() was called with a pointer to <picture>.
It also arranges that the current picture is not a picture on the screen.
Thus, the code
.(l I
static PICTURE pic = 0;
	.
	.
	.
Psave(&pic);
Prestore(pic);
.)l
(noting that Psave() and Prestore() are called right after each other,
so the current picture is not changed at all between them)
only does nothing if the current picture was not displayed at the time
that Psave() was called.
If that's the case, then the current picture is copied into pic by
"Psave(&pic)," and then pic is copied into the current picture by
"Prestore(pic)," making no actual difference to the current picture.
.pp
If, on the other hand, the current picture was displayed (either Pshow() or
Pscreen() had been called), then the above code would result in the
current picture being a separate copy of what is on the screen.
Changes made to the current picture by subsequent calls to graphics routines
will not appear on the screen on a call to update.
.pp
See Psave() for information on the PICTURE data type.
.(x
	Pwrite()
.)x
.sh 2 "Pwrite - write the current picture to a file"
.ti -0
Synopsis:
.(l I
Pwrite(<file>);
char *<file>;
.)l
.pp
This routine writes the current picture out to the file with the name
pointed to by <file>.
The current picture can later be restored to its present condition by
Pread().
The interplay between Pwrite() and Pread() is exactly analogous to the
interplay between Psave() and Pstore() except that the information
describing the picture is stored on disk instead of in memory.
.pp
Pictures that have been written out to files with Pwrite() may be displayed
and modified interactively with the interactive graphics manipulator, igm,
described in appendix B.
.pp
Only one picture may be written to each file.
If <file> already exists, Pwrite will simply overwrite whatever's in it.
.(x
	Pread()
.)x
.sh 2 "Pread - read the current picture from a file"
.ti -0
Synopsis
.(l I
Pread(<file>);
char *<file>;
.)l
.pp
This routine reads the current picture in from the file with the name pointed
to by <file>.
It arranges that the current picture is not a picture displayed on the screen.
See Pwrite(), Psave(), and Prestore() for more information related to Pread().
.(x
	Pcopy()
.)x
.sh 2 "Pcopy - make a copy of a picture variable"
.ti -0
Synopsis:
.(l I
Pcopy(&<pic1>,<pic2>);
static PICTURE <pic1> = 0, <pic2> = 0;
.)l
.pp
Pcopy() copies <pic2> into <pic1>.
If <pic2> is 0, then Pcopy() returns without doing anything.
If <pic1> is 0, then Pcopy() allocates memory for it, otherwise it reallocates
memory to give it enough room for <pic2>.
.pp
See Psave() and Prestore() for notes and pitfalls on the use of the PICTURE
data type.
.(x
	Pfree()
.)x
.sh 2 "Pfree - deallocate a picture variable"
.ti -0
Synopsis:
.(l I
Pfree(<picture>);
static PICTURE <picture> = 0;
.)l
.pp
This routine deallocates all storage allocated to <picture>.
If <picture> is 0 then Pfree() returns without doing anything.
.pp
NOTE: be sure to set <picture> back to 0 before using it in Psave(),
Pcopy(), or Prestore().
Otherwise, the software will assume that <picture> has memory allocated to
it.
.(x
	Pboxes()
.)x
.sh 2 "Pboxes - divide the screen into an array of boxes"
.ti -0
Synopsis:
.(l I
Pboxes(<columns>,<rows>);
int <columns>,<rows>;
.)l
.pp
This routine divides the screen into an array of "boxes" which may be
thought of as little independent screens.
<columns> is the number of columns of boxes, and <rows> is the number of
<rows>.
.pp
Once Pboxes() has been called with values other than 1 ("Pboxes(1,1);" sets
up the screen to have one box), Pshow() and Pscreen() must be given arguments
to indicate which box they are talking about.
To show the current picture in the lower left hand box, for example, the
command would be "Pshow(0,0);"
.pp
Each box is independent of the others.
Thus, the code
.(l I
Pboxes(2,2);
Pshow(0,1);
Pshow(1,0);
Pscreen(1,0);
.)l
sets up only the picture in the upper left hand corner to be modified.
The one in the lower right is left alone.
.pp
The first call to Pshow(), Pupdate(), or Psend() after a call to Pboxes()
will cause the screen to be cleared.
Moreover, as far as the graphics package is concerned, there is nothing on
the screen after a call to Pboxes(); so a call to Pscreen() before Pshow()
will result in an error.
.(x
	Pshow()
.)x
.sh 2 "Pshow - show the current picture on the screen"
.ti -0
Synopsis:
.(l I
Pshow();
  or
Pshow(<column>,<row>);
int <column>,<row>;
.)l
Pshow() shows the current picture on the screen.
It is the only way to get a picture on the screen.
.pp
If there is only one box on the screen, then Pshow() ignores its arguments
and puts the current picture in that box (which is the whole screen).
If Pboxes() has been used to divide the screen up into more than one box,
then Pshow() must be given its arguments.
<column> is the column number, and <row> the row number of the box in
which the current picture is to be displayed.
.pp
Changes made to the current picture after a call to Pshow() will be shown
on the screen with the next call to Pupdate().
.pp
If the current picture is already displayed on the screen when Pshow() is
called, but is in a different box than the one in which it is to be shown,
then it is copied into the new box and the new box becomes the current picture.
If any modifications had been made to the current picture between the call
to Pshow() that put it in the old box and this call, those modifications will
appear in the old box (where you expect them) with the next call to Pupdate().
.(x
	Psend()
.)x
.sh 2 "Psend - send the screen to another device"
.ti -0
Synopsis:
.(l I
Psend(<device>);
char *<device>;
.)l
.pp
Psend() sends everything on the screen to the device named <device>.
Right now there are no drivers for any of the hard copy devices, so the
only device that Psend() can send the screen to is the screen itself.
This can be useful for cleaning up the screen if it has somehow been mangled.
.pp
If there is no driver for <device>, then the graphics package acts as
if it has found a device name in "TERM" for which it has no driver
(see "Devices").
The user may substitute a different driver at this point.
It will work.
.pp
Psend() calls Pupdate() before sending.
.(x
	Pupdate()
.)x
.sh 2 "Pupdate - show changes that have been made to screen"
.ti -0
Synopsis:
.(l I
Pupdate();
.)l
.pp
Pupdate() updates the screen to show any changes that have been made to any
displayed pictures since the last time they were shown.
If changes have been made to more than one picture on the screen, then all
the changes are displayed.
.pp
Note that nothing can be subtracted from a picture, only added.
Pupdate() is only a mechanism for adding stuff to pictures that are already
on the screen.
