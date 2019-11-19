#include "../main/dragon.h"
#include "../stacks/C.h"
#include "C.h"
#include <math.h>
#include "../main/machinedep.h"

#if SYSTEM_V
#define bcopy(s, d, n) memmove(d, s, n)
#define bzero(s, n) memset(s, 0, n)
#endif /*SYSTEM_V*/

struct of_area *areas[NUMAREAS];

char *ChkAlloc();

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif
/* area.c */
static void PutXOnList P_((double x, register XLST *xlp));
#undef P_

void AreaNumChk(anumber)
int anumber;
{
	if(anumber < 0 || anumber > NUMAREAS-1)
		error("Bad area number (must be 0 - %d)",NUMAREAS-1);
}

/*
 * Check that an area is defined and not empty
 */
ChkArea(anumber)
int anumber;
{
	register struct of_area *ap;

	AreaNumChk(anumber);
	ap = areas[anumber];
	if( !ap || ap->l < 3)
		error("Area %d not defined",anumber);
}

struct of_area *AllocArea(anumber)
register int anumber;
{
	register struct of_area *ap;

	if( !areas[anumber])
		areas[anumber] = ap = (struct of_area *)ChkAlloc(
		 	sizeof(struct of_area), "Area");
	else
		ap = areas[anumber];
	ap->l = 0;
	return ap;
}

/*
 * Calculate the area and perimeter of an area and put the results in
 * its structure.
 */
void AreaArea(anumber)
int anumber;
{
	register struct of_area *ap;
	register int i;
	register VERTEX *vp, *prevp, *startvp;
	double dx, dy;

	ap = areas[anumber];
	ap->perimeter = ap->area = 0;
	for(i = 0,vp = ap->v; i <= ap->l; i++,vp++) {
		if( i == 0 || prevp->x == FDRAGON) {
			/* This is the beginning of a new polygon */
			startvp = vp;
			prevp = vp;
			continue;
		} else {
			if(vp->x == FDRAGON)
			/* end of a polygon, process the line to first point */
				vp = startvp;
			dx = vp->x - prevp->x;
			dy = vp->y - prevp->y;
			ap->perimeter += sqrt(dx * dx + dy * dy);
			/* this accumulates twice the area */
			ap->area += dy * (vp->x + prevp->x);
			vp = ++prevp;
		}
	}
	/* compensate for having twice the area */
	ap->area = fabs(ap->area) / 2.;
}

static void PutXOnList(x, xlp)
float x;
register XLST *xlp;
{
	register int i;
	register float *xp;

	/* find the place to insert the new x to keep the list in order */
	for(i = 0, xp = xlp->x; i < xlp->n && x > *xp; i++, xp++)
			;
	/* if we found a duplicate, delete both */
	if(xlp->n > i && x == *xp) {
		bcopy(xp +1, xp, (xlp->n - i - 1) * sizeof(float));
		xlp->n--;
	} else {
		if(xlp->n > i) {
			bcopy(xp, xp + 1, (xlp->n - i) * sizeof(float));
		}
		*xp = x;
		xlp->n++;
	}
}

/*
 * Return a list of the x values where the boundary crosses the given y.
 * Much complication occurs if one or more vertices have the same y as we are
 * given.  If this occurs I change y (which is double) to a value
 * which the float vertices can't match ( unless y == 0 where I set it to
 * a highly unlikely value.  Modify y toward more positive values so that this
 * is consistent with the plan to include equality at the low end of a range
 * but exclude it at the high end.
 */
XLST XList(anumber, y)
int anumber;
double y;
{
	register struct of_area *ap;
	register int i;
	register VERTEX *vp, *prevp;
	VERTEX *startvp;
	XLST xlist;
	double newx;

	ap = areas[anumber];
	xlist.n = 0;
checkagain:
	for(i = 0,vp = ap->v; i <= ap->l; i++,vp++) {
	    if(y == vp->y) {
/* printf("Modifying y from %.18g to",y); */
		if(y > 0)
			y *= 1.0000000001;
		else if(y < 0)
			y *= 0.9999999999;
		else
			y += 1e-22;
/* printf(" %.18g\n",y); */
	    goto checkagain;
	    }
	}
	for(i = 0,vp = ap->v; i <= ap->l; i++,vp++) {
	    if( i == 0 || prevp->x == FDRAGON) {
		/* This is the beginning of a new polygon */
		startvp = vp;
		prevp = vp;
	     } else {
		if(vp->x == FDRAGON) {
		/* end of a polygon, process the line to first point */
			vp = startvp;
		}
		/* Does this side cross y? */
		if( (y - prevp->y) * (y - vp ->y) < 0) {
			/* We know that y != either endpoint, so if the two
			 * endpoints have equal y, we don't get here. */
			newx = prevp->x + (y - prevp->y) * (vp->x - prevp->x)
				/ (vp->y - prevp->y);
			PutXOnList(newx, &xlist);
		}
		vp = ++prevp;
	    }
	}
	return(xlist);
}

/*
 * Return the min and max y in an area.
 */
AreaLimits(anumber, xmin, xmax, ymin, ymax)
int anumber;
register double *xmin, *xmax;
register double *ymin, *ymax;
{
	register VERTEX *vp;
	register struct of_area *ap = areas[anumber];
	register int i;

	vp = ap->v;
	*xmin = *xmax = (vp)->x;
	*ymin = *ymax = (vp++)->y;
	for(i = 1; i < ap->l; i++,vp++) {
		if(vp->x != FDRAGON) {
			if(vp->x > *xmax)
				*xmax = vp->x;
			if(vp->x < *xmin)
				*xmin = vp->x;
			if(vp->y > *ymax)
				*ymax = vp->y;
			if(vp->y < *ymin)
				*ymin = vp->y;
		}
	}
}
