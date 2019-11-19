# include "C.h"

struct for_cproj cproj = {{1,0,0, 0,1,0, 0,0,1},CSYS};
struct for_csys csys = {
	{1,0,0 ,0,1,0 ,0,0,1},
	0,0,EPOCH,0,RADEC,ARCMINUTES|SCALED,ARCMINUTES
};

double minRcX = -M_PI;		/* Controls +/- 2*M_PI to x */
double maxRcX = M_PI;

/* Array to hold units conversion factors */
double cunits[] = { M_PI/180.,1.,M_PI/12.,M_PI/10800.,M_PI/648000.};
/*double cunits[] = { M_PI/180.,1.,M_PI/12.,M_PI/10800.};*/

RelCoordSys rcsys = { "DEFAULT",0,0,EPOCH,EPOCH,0,RADEC,RADEC,
	ARCMINUTES|SCALED,ARCMINUTES};
/*char unitNames[][8] = { "degrees","radians","hours","arcmin","none","seconds",*/
char unitNames[][8] = { "degrees","radians","hours","arcmin","arcsec", "none","seconds",
	"Hz","meters","km/s"};
char horizCoordNames[][4] = {"Ra","l","x"};
char vertCoordNames[][4] = {"Dec","b","y"};
char projNames[][6] = {"rc","ortho","merca","gnom", "po"};
