static double half = 0.5;
static double zero = 0.0;

int round(d)
double d;
{
	return((int)((d < zero)? d - half:d + half));
}

int irond_(f)
float *f;
{
	double d = *f;

	return((int)((d < zero)? d - half:d + half));
}

int irondd_(d)
double *d;
{
	return((int)((*d < zero)? *d - half:*d + half));
}
