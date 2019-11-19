.(x

Error handling
.)x
.sh 1 "Error handling"
.pp
Since this package is going to be used in an interactive program, some
attention has been paid to handling errors gracefully.
Most possible problems with values passed to the routines of the package
will generate some sort of (hopefully) meaningful error message.
.pp
The errors are handled by the routine Error_() (the underscore is to make
it fortran callable), which is given a pointer to the message to be printed.
The supplied version of Error_() simply prints the message and calls
exit(), which is usually an appropriate thing to do with errors.
But in interactive programs it will be desirable to write a different
version of Error_().
.pp
If a different version of Error_() is written, then it should call Pabort()
before printing any text.
Pabort() resets the terminal to normal operation if it is called while
the terminal is in graphics mode.
.pp
Pabort() is described below.
.(x
	Pabort()
.)x
.sh 2 "Pabort - abort plotting"
.ti -0
Synopsis:
.(l I
Pabort()
.)l
While any graphics are being displayed on the screen, a flag is set inside
the graphics package.
Pabort() checks this flag and, if it is set, instructs the terminal driver
to send whatever message must be sent to the terminal to get it back to
text mode.
