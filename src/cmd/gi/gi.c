/* gf.f -- translated by f2c (version of 16 May 1991  13:06:06).
   You must link the resulting object file with the libraries:
	-lf2c -lm -lc   (in that order)
*/

#include "../../main/C.h"
#include "../../main/dragon.h"
#include "../../misc/C.h"
#include "../../stacks/C.h"
#include "../../image/C.h"

#if defined(__STDC__) || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

extern void image_gaussian_reduce(struct of_imagehdr *passed_ihp);

void gi_()
{
#include "gi.dc.h"
#include "gi.ec.h"

  image_gaussian_reduce(imageHdr[1]);

  return;
}
