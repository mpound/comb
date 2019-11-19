# include <math.h>
# include <stdio.h>
# include <stdarg.h>
# include "C.h"
# include "../error/C.h"

/* Matrix to convert between galactic and 1950 equatorial coordinates */
static double gmat[] = {
-.066988739415151/*xx*/,-0.87275576585199/*yx*/,-0.48353891463218/*zx*/,
 0.49272846607532/*xy*/,-0.45034695801996/*yy*/, 0.74458463328303/*zy*/,
-0.86760081115143/*xz*/,-0.18837460172292/*xy*/, 0.46019978478385/*xz*/
};

/* Matrix for precessing from coordinates in epoch y1sv to epoch y2sv */
static double pmat[9];		/* The 3x3 rotation matrix for precession */
static float y1sv,y2sv;		/* The from and to years for the matrix */

/* static double seccon = 206264.8062470964;	/* convert " Arc to radians*/
static double seccon = 3600.;
static double epochtol = .01;
static double clip = CLIP;
static double sphererad = SPHERERAD;
static double one = 1.0;
static char badcent[] = 
	"Illegal type of coordinates for coordinate system center";
static char badproj[] = "Unknown type of projection";

#define bzero(s, n) memset(s, 0, n)

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* stacks/search.c */
extern double dissquare P_((XYZPOINT *p1, XYZPOINT *p2));

/* coord.c */
static void DxdyMat P_((register RelCoordSys *rc, double mat[9]));
static void calcpmat P_((double ep1, double ep2, double mat[9]));
static void rrotate P_((register XYZPOINT *p1, register XYZPOINT *p2, register double *m));
static void rot1 P_((double ang, int axis, register double *matrix));
static void matmult P_((double *m1, register double *m2, double *m3));
static void matprint P_((register double *m));
#undef P_

/*
 * Convert a point in the current relative system 'csys' to an XYZPOINT
 */
XYZPOINT CsysToXyz(p)
XYPOINT p;
{
	XYZPOINT p1,p2;
	p.x *= cunits[csys.xun & 0xf];
	p.y = p.y * cunits[csys.yun] + csys.y;
	if(csys.xun & SCALED) {
		double t = cos(p.y);
		if (t != 0)
			p.x /= t;
		else
			p.x = 0;
	}
	p.x += csys.x;
	SpheretoPoint(p.x,p.y,&p1);
	if(csys.rotate) {
		rrotate(&p1,&p2,csys.mat);
		return(p2);
	}
	return(p1);
}
/*
 * Convert rel coordinates to absolute coordinates in radians
 */
XYPOINT CsysToAbs(p)
XYPOINT p;
{
	p.x *= cunits[csys.xun & 0xf];
	p.y = p.y * cunits[csys.yun] + csys.y;
	if(csys.xun & SCALED) {
		double t = cos(p.y);
		if (t != 0)
			p.x /= t;
		else
			p.x = 0;
	}
	p.x += csys.x;
	return(p);
}

/*
 * Convert an XYZPOINT to relative coordinates in the current system
 */
XYPOINT XyzToCsys(p)
XYZPOINT p;
{
	XYZPOINT p1;
	XYPOINT p2;
	if(csys.rotate) {
		rotate(&p,&p1,csys.mat);
		p = p1;
	}
	PointtoSphere(&p,&p2.x,&p2.y);
	p2.x -= csys.x;
	while(p2.x > maxRcX)
		p2.x -= 2*M_PI;
	while(p2.x < minRcX)
		p2.x += 2*M_PI;
	if(csys.xun & SCALED){
		p2.x *= cos(p2.y);
	}
	p2.x /= cunits[csys.xun & 0xf];
	p2.y = (p2.y - csys.y) / cunits[csys.yun];
	return(p2);
}

void SetRcXLimits(x1, x2)
double x1, x2;
{
        minRcX = (x1 + x2) * cunits[csys.yun] / 2. - M_PI;
        maxRcX = minRcX + M_PI * 2;
#if 0
/* if rd(0,0), then: ? to solve to "2pi" problem" */
	minRcX = -.125*M_PI; /* 1 hour slop?? */
	maxRcX = 2.125*M_PI;
#endif
}

#if 0
void MakeXPos(p)
XYPOINT *p;
{
	if( !(csys.xun & SCALED) && csys.x == 0 && p->x < 0)
		p->x += 2*M_PI / cunits[csys.yun];
}
#endif

/*
 * Get the great circle distance between two XYPOINTs in the projected plane.
 * Return the distance in radians.
 */
double XyDistance(p1,p2)
XYPOINT p1,p2;
{
	XYZPOINT zp1,zp2;
	double d;
	zp1 = UnProj(p1);
	zp2 = UnProj(p2);
	d = sqrt(dissquare(&zp1,&zp2))/sphererad;	/* chord distance */
	return 2 * (asin(d/2));				/* arc distance */
}

/*
 * Make a standard XYZPOINT from given ra,dec,epoch.
 */
XYZPOINT RaDecToXyz(ra,dec,epoch)
double ra,dec;
double epoch;
{
	XYZPOINT p1,p2;
	SpheretoPoint(ra * cunits[HOURS],dec * cunits[DEGREES],&p1);
	if(cifdif(epoch,EPOCH,epochtol)) {
		precess(epoch,&p1,EPOCH,&p2);
		return(p2);
	}
	return(p1);
}

/*
 * Inverse of above, Return Ra and dec given an xyzpoint and an epoch.
 */
void XyzToRaDec(p1, epoch, ra, dec)
XYZPOINT *p1;
double epoch;
double *ra, *dec;
{
	if(cifdif(epoch,EPOCH,epochtol)) {
		XYZPOINT p2;
		precess(EPOCH,p1,epoch,&p2);
		PointtoSphere(&p2, ra, dec);
	} else
		PointtoSphere(p1, ra, dec);
	if((*ra /= cunits[HOURS]) < 0)
		*ra += 24;
	*dec /= cunits[DEGREES];
}

/*
 * Make a standard XYZPOINT from given l and b.
 */
XYZPOINT LbToXyz(l,b)
double l,b;
{
	XYZPOINT p1,p2;
	SpheretoPoint(l * cunits[DEGREES],b * cunits[DEGREES],&p1);
	rrotate(&p1,&p2,gmat);
	return(p2);
}

/*
 * return l and b from an XYZPOINT
 */
void XyzToLb(p1, l, b)
XYZPOINT *p1;
double *l, *b;
{
	XYZPOINT p2;
	rotate(p1,&p2,gmat);
	PointtoSphere(&p2, l, b);
	*l /= cunits[DEGREES];
	*b /= cunits[DEGREES];
}

/*
 * The easy user interfaces for conversion between galactic and equatorial coords
 */
void rdtlb(ra, dec, epoch, l, b)
double ra, dec, epoch;
double *l, *b;
{
	XYZPOINT p1;

	p1 = RaDecToXyz(ra, dec, epoch);
	XyzToLb(&p1, l, b);
}

void lbtrd(l, b, epoch, ra, dec)
double l, b, epoch;
double *ra, *dec;
{
	XYZPOINT p1;
	
	p1 = LbToXyz(l, b);
	XyzToRaDec(&p1, epoch, ra, dec);
}

/*
 * Set up the csys structure from a given relative coordinate system
 * The rotation matrix - cproj.mat is made here also as the information
 * is available and the matrix is independent of the projection type.
 */
void MkCsys(rc)
RelCoordSys rc;
{
	XYZPOINT p1,p2;
	if(rc.xun & ~(0xf | SCALED) || (rc.xun & 0xf) >= NUMUNITS ||
		rc.yun >= NUMUNITS || rc.yun < 0)
		error_("Illegal relative coordinate units");
	switch(rc.otp & 0xf) {
	case RADEC :
	    switch(rc.ctp) {
	    case LB :
		SpheretoPoint(rc.x * cunits[DEGREES],rc.y * cunits[DEGREES],
			&p1);
		rrotate(&p1,&p2,gmat);
	        if(cifdif((double)rc.oepoch,EPOCH,epochtol)) {
			precess(EPOCH,&p2,rc.oepoch,&p1);
			PointtoSphere(&p1,&csys.x,&csys.y);
		} else {
			PointtoSphere(&p2,&csys.x,&csys.y);
		}
		break;
	    case RADEC :
	        if(cifdif((double)rc.cepoch,(double)rc.oepoch,epochtol)) {
			SpheretoPoint(rc.x * cunits[HOURS],rc.y *
				cunits[DEGREES],&p1);
			precess(rc.cepoch,&p1,rc.oepoch,&p2);
			PointtoSphere(&p2,&csys.x,&csys.y);
		} else {
			csys.x = rc.x * cunits[HOURS];
			csys.y = rc.y * cunits[DEGREES];
		}
		break;
	    default:
		error_(badcent);
	    }
	    csys.epoch = rc.oepoch;
	    if(cifdif((double)rc.oepoch,EPOCH,epochtol)) {
		calcpmat(rc.oepoch,EPOCH,csys.mat);
		csys.rotate = 1;
	    } else {
		rotmat(csys.mat,"");
		csys.rotate = 0;
	    }
	    break;
	case LB :
	    switch(rc.ctp) {
	    case RADEC :
		SpheretoPoint(rc.x * cunits[HOURS],rc.y * cunits[DEGREES],&p1);
	    	if(cifdif((double)rc.cepoch,EPOCH,epochtol)) {
			precess(rc.cepoch,&p1,EPOCH,&p2);
			p1 = p2;
		}
		csys.epoch = rc.cepoch;
		rotate(&p1,&p2,gmat);
		PointtoSphere(&p2,&csys.x,&csys.y);
		break;
	    case LB :
		csys.x = rc.x * cunits[DEGREES];
		csys.y = rc.y * cunits[DEGREES];
		break;
	    default :
		error_(badcent);
	    }
/*	    cmove((char *)gmat,(char *)csys.mat,sizeof(gmat)); */
	    memcpy((char *)csys.mat, (char *)gmat, sizeof(gmat));
	    csys.rotate = 1;
	    break;
	case DXDY :
	    csys.x = csys.y = 0.;
	    DxdyMat(&rc,csys.mat);
	    csys.rotate = 1;
/* In this case the projection matrix is the same as the csys matrix, so copy
   it. */
/*	    cmove((char *)csys.mat,(char *)cproj.mat,sizeof(cproj.mat)); */
	    memcpy((char *)cproj.mat, (char *)csys.mat, sizeof(cproj.mat));
	    goto end;
	default :
	    error_("Illegal type of offsets");
	}
	while(csys.x > M_PI)
		csys.x -= 2*M_PI;
	while(csys.x <= -M_PI)
		csys.x += 2*M_PI;
	while(csys.y > M_PI)
		csys.y -= 2*M_PI;
	while(csys.y <= -M_PI)
		csys.y += 2*M_PI;
/* Set up the matrix for projections.  It rotates to the center of the current
   rel coord system. */
	rc.rot = 0;
	DxdyMat(&rc,cproj.mat);

end:
/* These are here rather than the beginning so that if something illegal is
   found above, csys will not be disturbed. */
	csys.type = rc.otp;
	csys.xun = rc.xun;
	csys.yun = rc.yun;
}

/*
 * Calculate the rotation matrix for DXDY coordinates ie. to rotate to the
 * center of the current relative coordinate system.
 */
static void DxdyMat(rc,mat)
register RelCoordSys *rc;
double mat[9];
{
	XYZPOINT p1,p2;
	double tmat[9];
	switch(rc->ctp) {
	case LB :
		rotmat(tmat,"zyx",rc->x,-rc->y, -rc->rot);
		matmult(tmat,gmat,mat);
		break;
	case RADEC :
	        if(cifdif((double)rc->cepoch,EPOCH,epochtol)) {
			SpheretoPoint(rc->x * cunits[HOURS],rc->y *
				cunits[DEGREES],&p1);
			precess(rc->cepoch,&p1,EPOCH,&p2);
			PointtoSphere(&p2,&p1.x,&p1.y);
			p1.x /= cunits[DEGREES];
			p1.y /= cunits[DEGREES];
		} else {
			p1.x = rc->x * 15.;
			p1.y = rc->y;
		}
	    	rotmat(mat,"zyx",p1.x,-p1.y,-rc->rot);
		break;
	default :
		error_(badcent);
	}
}

/*
 * Compute the rotation angle for a user defined coordinate system from a
 * given point on the desired major axis.
 */
double RotFromPoint(rc, p)
RelCoordSys rc;			/* Relative coord sys with rot undefined */
XYPOINT p;			/* Offsets of point on major axis */
{
	XYZPOINT p1,p2;
				/* Find xyz for the point in the current sys */
	p1 = CsysToXyz(p);
			/* locate point in DXDY csys with zero rot angle */
	rc.rot = 0;
	rc.otp = DXDY;
	rc.xun = rc.yun = DEGREES;
	MkCsys(rc);
	rotate(&p1,&p2,csys.mat);
				/* calculate and return rotation angle */
	return(atan2(-p2.z,p2.y)/cunits[DEGREES]);
}

/*
 * Project an XYZPOINT into a two dimensional space using the projection
 * in struct cproj.
 */
XYPOINT Proj(point)
XYZPOINT point;
{
	XYZPOINT po;
	XYPOINT pr;
	double r,theta ;

	if(cproj.type == CSYS)
		return(XyzToCsys(point));
	rotate(&point,&po,cproj.mat);

	switch(cproj.type) {
	case ORTHO:
		if(po.x < 0.0001*SPHERERAD) {
			pr.x = pr.y = clip;
			return(pr);
		}
		pr.x = po.y/sphererad;
		pr.y = po.z/sphererad;
		break;
	case MERCATOR:
		pr.x = atan2(po.y,po.x);
		pr.y = po.z/sqrt(po.x*po.x + po.y*po.y);
		break;
	case GNOMONIC:
		if(po.x < 0.0001*SPHERERAD) {
			pr.x = pr.y = clip;
			return(pr);
		}
		pr.x = po.y/po.x;
		pr.y = po.z/po.x;
		break;
	case POLAR:
		r = sqrt(po.y*po.y + po.z*po.z) ;
		theta = atan2(r,po.x) ;
		if(theta < 0) {
			pr.x = pr.y = clip ;
			return(pr) ;
		}
		pr.x = theta * po.y / r ;
		pr.y = theta * po.z / r ;
		break;
	default:
		error_(badproj);
	}
	pr.x /= cunits[csys.xun & 0xf];
	pr.y /= cunits[csys.yun];
	return(pr);
}

/*
 * Convert from a projected XYPOINT back to an XYZPOINT using the projection
 * in cproj.
 */
XYZPOINT UnProj(pr)
XYPOINT pr;
{
	XYZPOINT po,po1;
	double t;

	if(cproj.type == CSYS)
		return(CsysToXyz(pr));

	/* convert from the user's units (rel coord offset units) to radians */
	pr.x *= cunits[csys.xun & 0xf];
	pr.y *= cunits[csys.yun];
	switch(cproj.type) {
	case ORTHO:
		if((t = one - pr.x*pr.x - pr.y*pr.y) < 0) {
			po.x = po.y = po.z = clip;
			return(po);
		}
		po.x = sphererad * sqrt(t);
		po.y = pr.x * sphererad;
		po.z = pr.y * sphererad;
		break;
	case MERCATOR:
		t = pr.y*pr.y;		/* t = tan of angle between line from center
					   to point and x-y plane*/
		if(t > 1.e-12) {
			po.z = sphererad/sqrt(one + one/t);	/* sin from tan */
			if(pr.y < 0.0)
				po.z = -po.z;
		} else {
			po.z = pr.y * sphererad;
		}
		t = sqrt(one/(one + t)) * sphererad;	/* cos from tan ie. length
						   of projection in x-y plane */
		po.x = t*cos(pr.x);
		po.y = t*sin(pr.x);
		break;
	case GNOMONIC:
		t = pr.x*pr.x + pr.y*pr.y;
	/* t = tan of angle between x axis and line from center to point
	   (x of tan plane = 1.0) */
		po.x = sphererad/sqrt(one + t);	/* cos from tan */
		po.y = pr.x*po.x;
		po.z = pr.y*po.x;
		break;
	case POLAR: /* cfg 4/90 : radius in x-y plane corresponds to angular
				  distance on the sky from center of projection */
		t = sqrt(pr.x*pr.x + pr.y*pr.y) ;
		if(t >= M_PI/2.) {
			po.x = po.y = po.z = clip ;
			return(po) ;
		}
		po.y = sphererad * pr.x/t ;
		po.z = sphererad * pr.y/t ;
		t = cos(t) ;
		po.x = sphererad * t ;
		po.y *= sqrt(1. - t*t) ;
		po.z *= sqrt(1. - t*t) ;
		break;
	default:
		error_(badproj);
	}
	rrotate(&po,&po1,cproj.mat);
	return(po1);
}

/*
 * make an array of XYZPOINTs corresponding to a regular array of projected
 * points
 */
XYZPOINT *UnProjArray(low,high,nx,ny)
XYPOINT low,high;			/* ll & ur corners in proj space */
int nx,ny;
{
	XYPOINT po;
	XYZPOINT *array,*CsysToXyzArray();
	register XYZPOINT *p;
	register int i,j;
	double dx,dy;

	if(nx <= 0 || ny <= 0)
		error_("UnProjArray: less than 1 point/axis");
	if(low.x >= high.x || low.y >= high.y)
		error_("UnProjArray: low and high out of order");
	if(cproj.type == CSYS)
		return(CsysToXyzArray(low,high,nx,ny));
	array = p = (XYZPOINT *)SafeAlloc((unsigned)(nx*ny*sizeof(XYZPOINT))
		,"UnPrjArr");
	if(nx > 1)
		dx = (high.x - low.x)/(nx - 1);
	else
		dx = 0.;
	if(ny > 1)
		dy = (high.y - low.y)/(ny - 1);
	else
		dy = 0.;

	for(j = 0,po.y = low.y;j < ny;po.y += dy,j++) {
		for(i = 0,po.x = low.x;i < nx;po.x += dx,i++) {
			*p++ = UnProj(po);
		}
	}
	return(array);
}

/*
 * make an array of XYZPOINTs corresponding to a regular array of points
 * in the current relative coordinate system - csys.
 */
XYZPOINT *CsysToXyzArray(low,high,nx,ny)
XYPOINT low,high;			/* ll & ur corners in proj space */
int nx,ny;
{
	XYZPOINT po,*array;
	register XYZPOINT *p;
	double dy,sy,cy,sdy,cdy;
	double lx,hx,dx,cx0,sx0,cx,sx,cdx,sdx;
	double t;
	int i,j,scaled;

	if(nx <= 0 || ny <= 0)
		error_("CsysToXyzArray: less than 1 point/axis");
	array = p = (XYZPOINT *)SafeAlloc((unsigned)(nx * ny * sizeof(XYZPOINT))
		,"CsToXyzA");
	low.y = low.y * cunits[csys.yun] + csys.y;
	high.y = high.y * cunits[csys.yun] + csys.y;
	if(ny > 1)
		dy = (high.y - low.y)/(ny - 1);
	sy = sin(low.y)*sphererad;
	cy = sphererad * cos(low.y);
	sdy = sin(dy);
	cdy = cos(dy);

	scaled = csys.xun & SCALED;
	low.x *= cunits[csys.xun & 0xf];
	high.x *= cunits[csys.xun & 0xf];

	for(j = 0;j < ny;j++) {
		if(scaled || j == 0) {
			if(scaled && cy > 1.e-6) {
				lx = low.x * sphererad / cy;
				hx = high.x * sphererad / cy;
			} else{
				lx = low.x;
				hx = high.x;
			}
			if(nx > 1)
				dx = (hx - lx)/(nx - 1);
			lx += csys.x;
			sx = sx0 = sin(lx);
			cx = cx0 = cos(lx);
			sdx = sin(dx);
			cdx = cos(dx);
		} else {
			sx = sx0;
			cx = cx0;
		}
		for(i = 0;i < nx;i++) {
			if(csys.rotate) {
				po.x = cy * cx;
				po.y = cy * sx;
				po.z = sy;
				rrotate(&po,p,csys.mat);
			} else {
				p->x = cy * cx;
				p->y = cy * sx;
				p->z = sy;
			}
			t = sx * cdx + cx * sdx;
			cx = cx*cdx - sx*sdx;
			sx = t;
			p++;
		}
		t = sy*cdy + cy*sdy;
		cy = cy*cdy - sy*sdy;
		sy = t;
	}
	return(array);
}

/*
 * make an n element vector of XYZPOINTs along the line from p1 to p2
 */
XYZPOINT *UnProjLine(p1,p2,n)
XYPOINT p1,p2;
int n;
{
	XYPOINT curPoint;
	XYZPOINT *array,UnProj();
	register XYZPOINT *p;
	register int i;
	double dx,dy;

	if(n <= 0)
		error_("UnProjLine: less than 1 point");
	array = p = (XYZPOINT *)SafeAlloc((unsigned)(n * sizeof(XYZPOINT))
		,"UnPrjLin");
	if(n > 1) {
		dx = (p2.x - p1.x)/(n - 1);
		dy = (p2.y - p1.y)/(n - 1);
	} else {
		dx = 0;
		dy = 0;
	}

	for(curPoint = p1,i = 0;i < n;i++) {
		*p++ = UnProj(curPoint);
		curPoint.x += dx;
		curPoint.y += dy;
	}
	return(array);
}
				
/*
 * Set the type of projection in cproj.  The work of setting up the matrix
 * is done by MkCsys() since it has the required information.
 */
int SetProj(projtype)
int projtype;
{
	int oproj;

	if(projtype < CSYS || projtype > EQAREA)
		error_(badproj);
	oproj = cproj.type;
	cproj.type = projtype;
	return(oproj);
}

/*
 * Conversions between the xyz values in an XYZPOINT structure (on the unit
 * sphere) and longitude - lattitude type spherical coordinates in radians.
 */
void PointtoSphere(po,lon,lat)
register XYZPOINT *po;
register double *lon;
double *lat;
{
	*lon = atan2(po->y,po->x);
	*lat = atan2(po->z,sqrt(po->x*po->x + po->y*po->y));
}

void SpheretoPoint(lon,lat,po)
register double lon,lat;
register XYZPOINT *po;
{
	register double clat;

	clat = cos(lat)*sphererad;
	po->x = clat*cos(lon);
	po->y = clat*sin(lon);
	po->z = sin(lat)*sphererad;
}
/*
 * Precess point1 at epoch1 to epoch2 and put in point2.
 * This subroutine was copied from a naval observatory routine in point.1950
 * and then modified extensively.
 */

void precess(ep1,po1,ep2,po2)
float ep1,ep2;
XYZPOINT *po1,*po2;
{
	if(ep1 == y1sv && ep2 == y2sv) {
		rrotate(po1,po2,pmat);
	} else if(ep1 == y2sv && ep2 == y1sv) {
		rotate(po1,po2,pmat);
	} else {
		calcpmat(ep1,ep2,pmat);
		y1sv = ep1;
		y2sv = ep2;
		rrotate(po1,po2,pmat);
	}
}

static void calcpmat(ep1,ep2,mat)
float ep1,ep2;
double mat[9];
{
	double t0,t,t2,t3,zeta0,zee,theta;
/*
	double czeta0,szeta0,czee,szee,ctheta,stheta;
*/

	t0 = (ep1 - 1900.) / 100.;
	t = (ep2 - ep1) / 100.;
	t3 = (t2 = t * t) * t;
	zeta0 = ((2304.25+ t0*1.396)*t + t2*0.3025+ t3*0.018)/seccon;
	zee = zeta0 + t2*0.791/seccon;
	theta = ((2004.682- t0*0.853)*t - t2*0.4265- t3*0.042)/seccon;
	rotmat(mat,"zyz",zee,-theta,zeta0);
/*
	czeta0 = cos(zeta0);
	szeta0 = sin(zeta0);
	czee = cos(zee);
	szee = sin(zee);
	ctheta = cos(theta);
	stheta = sin(theta);
*/
/* Now compute the rotation matrix */
/*
	mat[0] =   czeta0*ctheta*czee - szeta0*szee;
	mat[1] = - szeta0*ctheta*czee - czeta0*szee;
	mat[2] = - stheta*czee;
	mat[3] =   czeta0*ctheta*szee + szeta0*czee;
	mat[4] = - szeta0*ctheta*szee + czeta0*czee;
	mat[5] = - stheta*szee;
	mat[6] =   czeta0*stheta;
	mat[7] = - szeta0*stheta;
	mat[8] =   ctheta;
printf("t0 %f  t %f\nmat:\n%16.14f %16.14f %16.14f\n%16.14f %16.14f %16.14f\n%16.14f %16.14f %16.14f\n"
,t0,t,mat[0],mat[1],mat[2],mat[3],mat[4],mat[5],mat[6],mat[7],mat[8]);
*/
	/* matprint(mat);  mwp 5/92 */
}

void rotate(p1,p2,m)
register XYZPOINT *p1,*p2;
register double *m;
{
	p2->x = p1->x * m[0] + p1->y * m[1] + p1->z * m[2];
	p2->y = p1->x * m[3] + p1->y * m[4] + p1->z * m[5];
	p2->z = p1->x * m[6] + p1->y * m[7] + p1->z * m[8];
}

static void rrotate(p1,p2,m)
register XYZPOINT *p1,*p2;
register double *m;
{
	p2->x = p1->x * m[0] + p1->y * m[3] + p1->z * m[6];
	p2->y = p1->x * m[1] + p1->y * m[4] + p1->z * m[7];
	p2->z = p1->x * m[2] + p1->y * m[5] + p1->z * m[8];
}
/*
 * Generate a rotation matrix given a string of axis characters and
 * corresponding angles.  For example to generate the equatorial to
 * galactic conversion matrix - rotmat(gmat,"zxz",282.25,62.6,-33);
 * Any number of individual axis rotations can be handled.  The 
 * number is derived from the string of axes.
 */
/*VARARGS*/
void rotmat(double *mat, char *s, ...)
#if 0
double *mat;			/* place to put the 3x3 rotation matrix */
char *s;	/* null terminated string [xyz] giving axes of rot */
#endif
{
	double m1[9],m2[9];	/* temporary matricies */
	va_list ap;

	va_start(ap, s);
	if(*s) {
		rot1(va_arg(ap, double) * cunits[DEGREES],*s++,mat);
	} else {
		bzero(mat,72);
		mat[0] = mat[4] = mat[8] = 1.0;
	}
	while(*s) {
		rot1(va_arg(ap, double) * cunits[DEGREES],*s++,m1);
		matmult(m1,mat,m2);
		if(*s) {
			rot1(va_arg(ap, double) * cunits[DEGREES],*s++,m1);
			matmult(m1,m2,mat);
		} else {
/*			cmove((char *)m2,(char *)mat,72); */
			memcpy((char *)mat,(char *)m2,sizeof(m2));
		}
	}
	va_end(ap);
}

#if 0
static void rotmat(mat,s,a1,a2,a3)
double mat[9];	/* place to put the 3x3 rotation matrix */
char *s;	/* null terminated string [xyz] giving axes of rotation */
double a1,a2,a3;	/* rotation angles about axes The number of angle
			arguments must equal or exceed the number of axes */
{
	double m1[9],m2[9];	/* temporary matricies */
	double *ap = &a2;	/* argument pointer */
	if(*s) {
		rot1(a1*cunits[DEGREES],*s++,mat);
	} else {
		bzero(mat,72);
		mat[0] = mat[4] = mat[8] = 1.0;
	}
	while(*s) {
		rot1(*ap++ * cunits[DEGREES],*s++,m1);
		matmult(m1,mat,m2);
		if(*s) {
			rot1(*ap++ * cunits[DEGREES],*s++,m1);
			matmult(m1,m2,mat);
		} else {
/*			cmove((char *)m2,(char *)mat,72); */
			memcpy((char *)mat,(char *)m2,sizeof(m2));
		}
	}
}
#endif

/*
 * Construct a rotation matrix for rotation about one axis
 */
static void rot1(ang,axis,matrix)
double ang;			/* in radians */
char axis;			/* 'x', 'y', or 'z' */
register double *matrix;
{
	static char mindex[15] = {	/* 1,cos,cos,sin,-sin */
		0,4,8,5,7,		/* x */
		4,0,8,6,2,		/* y */
		8,0,4,1,3		/* z */
	};
	register char *c;

	if(axis - 'x' <0 || axis - 'z' > 0) {
		error_("Illegal axis in rot1\n");
	}
	bzero(matrix,9*sizeof(double));
	c = mindex + (axis - 'x')*5;
	matrix[*c] = 1.;
	matrix[*(c + 2)] = matrix[*(c + 1)] = cos(ang);
	matrix[*(c + 4)] = - (matrix[*(c + 3)] = sin(ang));
}

/* This is the straightforward way to write matmult.
static void matmult(m1,m2,m3)
double m1[][3],m2[][3],m3[][3];
{
	register int i,j,k;
	for(i = 0;i < 3;i++) {
	    for(j = 0;j < 3;j++) {
		m3[j][i] = 0.;
		for(k = 0;k < 3;k++) {
		    m3[j][i] += m1[j][k]*m2[k][i];
		}
	    }
	}
}
*/

static void matmult(m1,m2,m3)
double *m1,*m3;
register double *m2;
{
	register double *tm1;
	register int i,j;
	for(i = 0;i < 3;m2++,i++) {
	    for(j = 0;j < 9;j += 3) {
		tm1 = m1 + j;
		m3[j + i] = tm1[0]*m2[0] + tm1[1]*m2[3] + tm1[2]*m2[6];
	    }
	}
}
static void matprint(m)
register double *m;
{
	register int i;
	for(i=0;i < 9;i++) {
		if(i %3 ==2)
			fprintf(stderr,"%16.14lg\n",*m++);
		else
			fprintf(stderr,"%16.14lg  ",*m++);
	}
}

#if 0
/* Test routines follow. */
main()
{
	RelCoordSys rc;
	double m1[9],m2[9],m3[9],angle;
	double ra,dec,tra,tdec,r,d,y1,y2,l,b;
	double a1,a2,a3,a4,a5;
	XYZPOINT p1,p2,p3;
	XYPOINT c1,c2,c3;
	register int i;
	char axis;
	char str[8];

	printf("ra,dec,y1,y2  ");
	scanf("%lf%lf%lf%lf",&ra,&dec,&y1,&y2);
	tra = ra * M_PI/12.;
	tdec = dec * M_PI/180.;
	SpheretoPoint(tra,tdec,&p1);
	if(y1 != y2) {
		precess(y1,&p1,y2,&p2);
	} else {
		p2 = p1;
	}
	PointtoSphere(&p2,&r,&d);
	r *=12./M_PI;
	d *= 180./M_PI;
	printf("precessed ra,dec %16.14lf  %16.14lf\n",r,d);
	printf("x,y,z = %lf %lf %lf\n",p2);
	p3 = RaDecToXYZ(ra,dec,y1);
	printf("RaDecToXyz output  %lf %lf %lf\n",p3);
	rotate(&p1,&p2,gmat);
	PointtoSphere(&p2,&l,&b);
	l *= 180./M_PI;
	b *= 180./M_PI;
	printf("l %20.14f  b %20.14f\n",l,b);
	p3 = LbToXyz(l,b);
	printf("LbToXyz output %lf %lf %lf\n",p3);
	rrotate(&p2,&p3,gmat);
	PointtoSphere(&p3,&r,&d);
	r *=12./M_PI;
	d *= 180./M_PI;
	printf("ra,dec from gal %16.14lf  %16.14lf\n",r,d);
/*
begin:
	bzero(m1,72);
	m1[0] = m1[4] = m1[8] = 1.0;
	printf("axes,angle,angle,angle,angle,angle ");
	scanf("%s%lf%lf%lf%lf%lf",str,&a1,&a2,&a3,&a4,&a5);
	rotmat(m1,str,a1,a2,a3,a4,a5);
	matprint(m1);
	rotate(&p1,&p2,m1);
	PointtoSphere(&p2,&l,&b);
	l *= 180./M_PI;
	b *= 180./M_PI;
	printf("user cord 1 %20.14f  uc2 %20.14f\n",l,b);
	goto begin;

*/
/*
begin:
	makerc(&rc);
	printf("offset c1,c2 ");
	scanf("%lf%lf",&c1.x,&c1.y);
	p1 = CsysToXyz(c1);
	printf("p1: %16.14f %16.14f %16.14f\n",p1.x,p1.y,p1.z);
	printf("new ");
	makerc(&rc);
	c2 = XyzToCsys(p1);
	printf("c1 %20.14f  c2 %20.14f \n",c2);
	goto begin;
}
makerc(rc)
RelCoordSys *rc;
{
	int i,xun,yun;
	double c1,c2;
	float ep;
	printf("rel coord sys c1,c2,cepoch,oepoch,rot  ");
	scanf("%lf%lf%f%f%lf",&rc->x,&rc->y,&rc->cepoch,&rc->oepoch,&rc->rot);
	printf("ctp,otp,xun,yun  ");
	scanf("%d",&i);
	rc->ctp = i;
	scanf("%d",&i);
	rc->otp = i;
	scanf("%d",&i);
	rc->xun = i;
	scanf("%d",&i);
	rc->yun = i;
	if(rc->rot == 999) {
		printf("Point on equator c1,c2,epoch,otp,xun,yun  ");
		scanf("%lf%lf%f%d%d%d",&c1,&c2,&ep,&i,&xun,&yun);
		rc->rot = RotFromPoint(*rc,c1,c2,ep,i,xun,yun);
	}
	MkCsys(*rc);
}
error_(s)
char *s;
{
	printf(s);
	exit(1);
}

output from first part of test program.
ra,dec,y1,y2  5 16 1950 1980
t0 0.500000  t 0.300000
mat:
0.99997327043818 0.00670537128717 0.00291485935833
-0.00670537128864 0.99997751869749 -0.00000977223471
-0.00291485935496 -0.00000977324075 0.99999575174069
precessed ra,dec 5.02870413960008  16.04261906968569
x,y,z = 0.241757 0.930152 0.276352
RaDecToXyz output  0.248793 0.928507 0.275637
l   185.37350881264569  b   -15.30230436702767
LbToXyz output 0.248793 0.928507 0.275637
ra,dec from gal 5.00000000000000  15.99999999999984
*/
#endif
