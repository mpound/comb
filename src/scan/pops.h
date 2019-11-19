/*
	Cactus file @(#)pops.h	1.4
		SID 1.4
	       Date 9/24/92
*/

/* POPS.H -- BOOTSTRAP and DIRECTORY structures for POPS I/O */

struct pdfl_BOOTSTRAP {
  short  nbdt;          /* Number of blocks in directory table. */
  short  nemax;         /* Maximum number of entries in dir table. */
  short  nbpe;          /* Number of bytes per dir table entry. */
  short  nxte;          /* Next available dir table entry. */
  short  nxtde;         /* Block of next avaliable dir table entry */
  short  ndb;           /* Next block location in file for data. */
  short  nxtb;          /* Last dir table block currently used. */
  short  nbcal;         /* First block where cal data are stored. */
  short  nbsv;          /* First block where save scans are stored. */
  short  nbwrk;         /* First block of integration work area. */
  short  nbscr;         /* First block of link task scratch area. */
  short  pftype;        /* POPS file type. */

  char  obs_name[16];	/* Observer name. */
  char  proj_id[16];	/* Project ID. */

  char  binfmt_id[16];  /* Binary number format ID. */
};

#define DIRECTORYSIZE 58  /* dont use sizeof(struct DIRECTORY) use this */

struct pdfl_DIRECTORY { 
    int scan;
    char obsmode[4];
    int blk_fb1;
    int n_blk_fb1;
    int blk_fb2;
    int n_blk_fb2;
    char source[16];
    double vel;
    double restfreq;
    short subscan;
};
