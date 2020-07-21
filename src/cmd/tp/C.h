struct skydip_data_struct
{
  double j2seconds;     /* Time scan was taken   */
  double elevation;     /* Elevation in degrees  */
  double airmass;       /* Airmass               */
  double linefreq;      /* Line Frequency in MHz */
  double totpower;      /* Total power measured  */
  double error;         /* Error bars on total power */
  double TAmbient;      /* Ambient temperature in K */
  double DTAbs;         /* dTabs stored with scan */
  char   scanfile[4];   /* Scan file name */
  short  scannum;       /* Scan file scan number */
  int    backend;       /* Backend number */
  struct skydip_data_struct *prev_ptr; /* Pointer to the previous stored point */
};

typedef struct skydip_data_struct Skydip_data;

