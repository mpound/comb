.sh 2 "Data Types"
.pp
There are two data types defined in "../stacks/C.h."
.sh 3 "struct of_stk"
This is the structure of the stacks in core.
There is an array of 3 such stacks named stk_
(see documentation for global variables).
.pp
The structure consists of 53 fields which all have meaning but may
or may not be maintained by COMB (many of them are historical).
The fields are as follows:
.pp
.pp
short numpt: number of channels.
.pp
short nver: version number of the header-data format (1 in this case).
.pp
short nprc: data type.
0 = unfolded, unprocessed spectral data.
1 = unfolded, frequency processed spectral data.
5 = folded, frequency processed spectral data.
1000 = spatial elevation scan.
2000 = spatial azimuth scan.
.pp
short nplc: preferred
plotting coordinates -- main position + map offsets + plot ordinate.
main position -- 0 = equatorial coordinates.
1 = galactic coordinates.
map offsets -- 0 = equatorial coordinates.
10 = galactic coordinates.
20 = user's coordinates.
30 = equatorial observing display coordinates.
plot ordinate -- 0 = LSR velocity.
100 = frequency.
200 = channel number.
500 = spatial.
(0, 100, and 200 for spectral data only; 500 for continuum scans.)
.pp
short numst: number of the stack in the directory.
This is maintained by MoveStk.
.pp
short nwts: number of weights in the weight array.
.pp
short locwt[15]: first channel number at the change of each weight.
.pp
short ibsln: order of baseline removed.
.pp
short kuse[32]: stored KUSE array (packed binary).
.pp
short nstk[32]: numbers of the first 32 scans added into the stack.
The first byte of this array is the first character of the file name.
The second byte is always 0x80.
The following two bytes are the second and third characters of the file
name.
Each of the following pairs of bytes is the integer number of a scan
unless the second byte of the pair
is 0x80, in which case there is a new file name.
The first character of the file name is in the byte preceding the 0x80,
and the second two characters are in the following two bytes.
Thus, the list of the three scans "80a100, 80a101, 80b235" would be
encoded '8' 0x80 '0' 'a' 100L 100H 101L 101H '8' 0x80 '0' 'b' 235L 235H
(where 100L refers to the low order byte of 100, and 100H refers to the
high order byte of 100).
NOTE: THIS IS ALL VERY MACHINE DEPENDENT -- TRY TO ACCESS THIS STUFF AS
LITTLE AS POSSIBLE.
Because this is so complicated, the routines AddScNum, FrstScNum, and
NxtScNum are provided to deal with it (see AddScNum, FrstScNum, and
NxtScNum).
.pp
short nstkx[128]: extension of nstk (has data if there are too many
scans to fit in nstk).
This has the same format as nstk, and is also maintained by AddScNum,
FrstScNum, and NxtScNum.
.pp
short nlist: number of scan numbers listed in nstk (above).
.pp
short indx: last location in nstk.
.pp
short isdum[40]: blank (reserved for future use).
.pp
float fwid: filter width (MHz) (spectral data) or sample box size
(degrees) (continuum scans).
.pp
float expch: channel number of the frequency at which the line center
is expected.
.pp
float refch: channel number of the expected line center during the reference
portion of a frequency-switched observation,
or mapping angle (degrees) (continuum scans).
.pp
float vlsr: LSR velocity of "expch" (km/s).
.pp
float exwid: expected line width (# of channels).
.pp
float wght[15]: weights in the weight array.
.pp
float factt: "fudge factor" by which rescaled data has been multiplied.
.pp
float time: sum of the integration times for the component scans.
.pp
float prav: average power per channel (deg K).
.pp
float prer: rms error in pwrav.
.pp
float rms: rms of all channels in the USE array.
.pp
float tmax: temperature of the peak channel within the expected line interval.
.pp
float rnhi: number of channels with temperature greater than "tmax"/2
.pp
float area: total area under the line (deg K-Km/s).
.pp
float rmich: channel number at bisector of the area.
.pp
float fwhm: FWHM of a Gaussian fit or FWZP of a Parabola fit.
.pp
float fwerr: fitting error in fwhm.
.pp
float fpeak: velocity of the peak of the curve from a Gaussian fit.
.pp
float fperr: fitting error in fpeak.
.pp
float ftmax: temperature at the peak of the Gaussian curve.
.pp
float fterr: fitting error in ftmax.
.pp
float rmval: temperature subtracted from the center channel during baseline
removal.
.pp
float ra: 1950.0 right ascension at map center (hours).
.pp
float dec: 1950.0 declination at map center (degrees).
.pp
float l: galactic longitude at map center.
.pp
float b: galactic latitude at map center.
.pp
float dra: right ascension offset (hours).
.pp
float ddec: declination offset (degrees).
.pp
float dl: galactic latitude offset (degrees).
.pp
float db: galactic longitude offset (degrees).
.pp
float dx: horizontal offset of user settable coordinate system.
.pp
float dy: vertical offset of user settable coordinate system.
.pp
float odra: right ascension observing offset (arcminutes).
.pp
float oddec: declination observing display offset (arcminutes).
.pp
float epoch: year of the coordinates of this stack.
.pp
float rsdm[13]: not used.
.pp
float freq: assumed line frequency (MHz).
.pp
float stak[512]: values of channels in stack.
.sh 3 "SKYARRAY"
.pp
This is a data type containing an array of values representing the sky.
The fields are as follows:
.pp
int xres: resolution in x (number of horizontal elements).
Note: in space-velocity maps, x represents space.
.pp
int yres: resolution in y (number of vertical elements).
Note: in space-velocity maps, y represents velocity.
.pp
double lowH, lowV: low horizontal and vertical
coordinates of space represented by this array.
(Current coordinate system.)
.pp
double highH, highV: high horizontal and vertical coordinates of space
represented by this array.
(Current coordinate system.)
Note: when the SKYARRAY represents a space-velocity map, lowH, lowV, highH,
and highV specify the line along which the data was taken.
.pp
double lowVel, highVel: range of velocities in array.
In space-space map, this is actually the range of values.
In space-velocity map, this is the range of velocities represented by
the y dimension.
.pp
float data[]: the data in the array.
The size is xres*yres.
