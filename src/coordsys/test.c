#include "C.h"
/*#include "include.h"*/
#include <math.h>
#include <stdio.h>

main()
{
	RelCoordSys rc;
	XYZPOINT p1,p2,p3,*array = (XYZPOINT *)0;
	XYPOINT c1,c2,c3,lo,hi;
	int i,ix,iy,nx,ny;
	char cmd;

	printf("cmds are: r - make rc; o- set offset point; P- print; s- set proj;p- project;\nu- unproj; m- print matrices;a- UnProjArray;l-UnProjLine;e- exam arr element\n");
	printf("Separate arguments with space, not comma\n");

    while(1) {
	while(getchar() != '\n');
	printf("> ");
	scanf("%c",&cmd);
	switch(cmd) {
	case 'r':
		makerc(&rc);
		break;
	case 'o':
		printf("offset c1,c2 ");
		scanf("%lf%lf",&c1.x,&c1.y);
		p1 = CsysToXyz(c1);
		break;
	case 'P':
		xyzprint("point",&p1);
		printf("translates to ");
		c1 = XyzToCsys(p1);
		xyprint("rel coords",&c1);
		break;
	case 's':
		printf("proj type?  ");
		scanf("%d",&i);
		SetProj(i);
		break;
	case 'p':
		c1 = Proj(p1);
		xyprint("rel coords",&c1);
		break;
	case 'u':
		p1 = UnProj(c1);
		xyzprint("point",&p1);
		break;
	case 'm':
		printf("csys.rot =\n");
		matprint(csys.mat);
		printf("\ncproj.mat\n");
		matprint(cproj.mat);
		break;
	case 'a':
		if(array)
			free((char *)array);
		printf("lowx,lowy,hix,hiy,nx,ny ");
		scanf("%lf%lf%lf%lf%d%d",&lo.x,&lo.y,&hi.x,&hi.y,&nx,&ny);
		array = UnProjArray(lo,hi,nx,ny);
		break;
	case 'l':
		if(array)
			free((char *)array);
		printf("x1,y1,x2,y2,n ");
		scanf("%lf%lf%lf%lf%d",&lo.x,&lo.y,&hi.x,&hi.y,&nx);
		array = UnProjLine(lo,hi,nx);
		printf("'e' with iy = 0\n");
		break;
	case 'e':
		printf("ix,iy  ");
		scanf("%d%d",&ix,&iy);
		p1 = *(array + ix + iy*nx);
		xyzprint("array element(in p1) ",&p1);
		break;
	case 'q':
		exit(0);
	}
    }
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
		printf("Point on equator c1,c2  ");
		scanf("%lf%lf",&c1,&c2);
		rc->rot = RotFromPoint(*rc,c1,c2);
	}
	MkCsys(*rc);
}

xyzprint(s,p)
char *s;
register XYZPOINT *p;
{
	printf("%s %16.14f %16.14f %16.14f\n",s,p->x/SPHERERAD
	,p->y/SPHERERAD,p->z/SPHERERAD);
}

xyprint(s,p)
char *s;
register XYPOINT *p;
{
	printf("%s %16.14f %16.14f\n",s,p->x,p->y);
}


matprint(m)
register double *m;
{
	register int i;
	for(i=0;i < 9;i++) {
		if(i %3 ==2)
			printf("%16.14lg\n",*m++);
		else
			printf("%16.14lg  ",*m++);
	}
}


error_(s)
char *s;
{
	printf(s);
	exit(1);
}
/*
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
