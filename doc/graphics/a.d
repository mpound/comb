.(x

Appendix A - programming examples
.)x _
.he ''Appendix A''
.hx
.bp
.ce 1
Appendix A - programming examples
.pp
.c
Programming examples
.sp
A program that draws a grid in the graph window of the "full plot" format:
.(l I
# include "/cdata/comb/src/graphics/C.h"

main()
{
	double left, bottom, right, top;/* the boundaries of the */
					/* window */
	double xstep, ystep;		/* step between grid lines */
	double x, y;

	/* enter description of grid */
	printf("Enter x boundaries: ");
	scanf("%f %f", &left, &right);
	printf("Enter step between grid lines in x: ");
	scanf("%f", &xstep);
	printf("Enter y boundaries: ");
	scanf("%f %f", &left, &right);
	printf("Enter step between grid lines in y: ");
	scanf("%f", &ystep);

	/* draw grid */
	Pstart();			/* start picture */
	Pbounds(left, bottom, right, top);/* set bounds of */
					/* graph window */
	Pborder();			/* draw a border around */
					/* window 1 */
	Plinetype(0);			/* use an inconspicuous */
					/* line type for the grid */
	for(x = left; x <= right; x += xstep)
	{
		Pmove(x, bottom);	/* draw grid line */
		Pline(x, top);
		Pxlabel(x);		/* label x axis */
	}
	for(y = bottom; y <= top; y += ystep)
	{
		Pmove(y, left);		/* draw grid line */
		Pline(y, right);
		Pylabel(y);		/* label y axis */
	}
	Pshow();			/* show grid */
}
.)l
.sp
A program for making a graph of the function foo() without a grid (the grid
can be made as above).
From now on, the boring stuff will be left out.
.(l I
# include "/cdata/comb/src/graphics/C.h"

main()
{
	double left, bottom, right, top;/* boundaries of graph window */
	int lines;			/* number of lines to be in */
					/* the graph (i.e. the number */
					/* of samples to take of foo()) */
	double foo();			/* function to be plotted */
	double x;

	/* enter boundaries of graph window */
	/* and number of lines in plot */
		.
		.
		.

	/* make graph */
	Pstart();			/* start picture */
	Pbounds(left, bottom, right, top);/* set boundaries of */
					/* graph window */
	Pmove(left, foo(left));		/* get to beginning of graph */
	for(x = left; x <= right; x += (right - left) / lines)
		Pline(x, foo(x));	/* draw graph */
	Pshow();			/* show graph */
}

double foo(arg)
	double arg;
{
	.
	.
	.
}
.)l
.sp
A program which draws a picture that consists only of a border around
the screen, and displays it in four boxes:
.(l I
# include "/cdata/comb/src/graphics/C.h"

main()
{
	int row, column;		/* row and column in which */
					/* picture is being displayed */

	Pstart();			/* begin picture */
	Pwindow(0);			/* switch to window 0: the */
					/* whole screen */
	Pborder();			/* draw a border around the */
					/* whole screen */
	Pboxes(2, 2);			/* set up the screen as an */
					/* array of four boxes (2 x 2) */
	for(row = 0; row < 2; row++)
		for(column = 0; column < 2; column++)
			Pshow(column, row);/* show the picture */
}
.)l
.sp
A program which draws some pictures on the screen (their details are left
out), and then lets the user choose a picture and location within that
picture at which to draw an ellipse of a given size:
.(l I
# include "/cdata/comb/src/graphics/C.h"

main()
{
	double width, height;		/* width and height of ellipse */
	GCREAD cr;			/* graphics input structure */
	GCREAD Pcursor();		/* routine for getting graphics */
					/* input */
	       .			/* definitions needed for whatever */
	       .			/* pictures are to be drawn */
	       .

	/* draw some pictures on the screen */
	       .
	       .
	       .

	/* enter the width and height of the ellipse */
	       .
	       .
	       .

	/* make ellipse */
	cr = Pcursor();			/* get the graphics input */
	Pscreen(cr.xBox, cr.yBox);	/* set up to modify the */
					/* picture that the user */
					/* chose */
	Pfigure(cr.x, cr.y, F_ELLIPSE, width, height);/* draw ellipse */
	Pupdate();			/* update the screen so */
					/* that the ellipse appears */
}
