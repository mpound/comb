.(x

Devices
.)x
.sh 1 "Devices"
.pp
The COMB graphics package is intended to be highly device independent.
Of course, there has to be a restriction on the devices it knows how to
use.
At present, there is only one device that the package can actually use:
the hewlett packard hp2648A graphics terminal.
Drivers for other devices, however, should be easy to write (see "Internal
documentation" for a description of the terminal drivers), and drivers
for other Hewlett Packard terminals, Tektronix terminals, VT100s, and
Versatec graphics printers should not be far off.
.pp
Drivers for hard copy devices (like the Versatec or a pen plotter) may
have extra initialization routines for selecting the set of line types
(see "Plotting") and text types (see "Text") as well as setting the
size and shape of the plotting area.
When these routines are written, they should be documented below.
.pp
By default, the driver used by Pshow() and Pupdate()
is decided on by looking at the "TERM" field of
the environment.
If there is no driver for that terminal, then the user is asked to give
another type of terminal.
If the user types 'help' at this point, he is given a list of the supported
devices.
If the user types 'none,' then no driver is used, and any displaying of
graphics doesn't happen.
The automatic selection of drivers makes the device transparent to the
program, and may be ignored in most applications.
.pp
However,
if a program will always be used from a given type of terminal, or the
programmer wishes to choose the terminal in some other way than that described
above, then Pterminal() may be used to set the device
driver to be used.
Pterminal is described below.
.(x
	Pterminal()
.)x
.sh 2 "Pterminal - set driver to be used for the terminal"
.ti -0
Synopsis:
.(l I
Pterminal(<device>);
char *<device>;
.)l
.pp
Pterminal sets the default device driver to the one associated with the
name pointed to by <device>.
If the graphics package does not support <device>, then it's behavior will
be exactly as if it had found a terminal it doesn't support in "TERM."
It will ask the user for a different device the first time it needs to use
the driver.
.pp
If <device> is a null string (""), then the package will go back to using
the device named in "TERM."
.pp
A list of devices may be obtained by making a little program that calls
Pterminal() with a bogus device name, and then typing "help" when it asks
for a terminal type.
NOTE: such a program would have to include calls to Pstart() and Pshow() in
order to get the package to realize it doesn't know what driver to use.
.pp
Calling Pterminal() with a hard copy device (such as the Versatec)
can be hazardous to your karma.
If a program is always to send its output to a hard copy device, it is better
to call Pterminal() with "none" as the argument (meaning that no driver is
to be used), and then call Psend() with the correct device name after
calling Pshow().
(Actually, calling Pterminal() with a hard copy device as an argument
is only bad news if you're planning to use Pboxes() to put more than one
picture on a page, but it shouldn't be done anyway.)
