/* From jhagen@heineken.tuc.nrao.edu Wed Sep  8 11:51:56 1993
   Date: Wed, 8 Sep 93 08:51:45 MST
 */
/* defines SDD file, replaces pops.h */

struct sdd_BOOTSTRAP {
  int num_index_rec; /* # of records in index including the bootstrap record */
  int num_data_rec;  /* of data records which follow the index records.      */
  int bytperrec;     /* bytes per record (use 512 in making the file)        */
  int bytperent;     /* # of bytes per index entry (still 64)                */
  int num_entries_used;  /* # of index entries already used.
                         (This is actually misleading.  This field is really
                         the last index entry used.  All index entries after
                         this number are guaranteed to be empty, the index
                         entries prior to this number may or may not be empty.)
                      */
  int counter;       /* Counter which indicates whether the file has been
                         updated.   This field is used by the unipops disk
                         access routines to tell if the file has changed since
                         the last access.  It should be reset to 0 for a 
                         new or reindexed file.  It should be incremented
                         by 1 EVERY TIME the file is changed.  It should
                         reset to 0 if an increment would exceed the max value
                         possible by the storage type. */
  int typesdd;       /* type of SDD: 0 is normal, 1 is records (GB only)*/
  int version;       /* SDD Version = 1 for this version.  This is the field
                         that is guaranteed to be non-zero in the bootstrap
                         block as compared to the previous SDD files. */
  char padd[480];     /* all zero */
};

struct sdd_DIRECTORY {

  int start_rec;  /* Starting Record number for the scan about to be
                      described by this index entry (the bootstrap record is
                      record 1). */
  int end_rec;    /* Last Record number for that scan. */
  float h_coord;   /* Horizontal Coordinate in degrees.
                      (the SOURCEX header word) */
  float v_coord;   /* Vertical Coordinate in degrees.
                      (the SOURCEY header word) */
  char source[16]; /* Source Name. */
  float scan;      /* Scan Number  */
  float freq_res;  /* For LINE scans: the frequency resolution in MHz
                      (the FREQRES header word)
                      For CONT scans: the slew rate of the telescope in
                      degrees/sec (appropriate
                      combination of DELTAXR and DELTAYR
                      taking into account the cos of the
                      declination of the source) */
  double rest_freq;/* For LINE scans: the rest frequency in MHz
                      (the RESTFREQ header word)
                      For CONT scans: the integration time per point in seconds.
                      (the SAMPRAT header word) */
  float lst;       /* LST in hours. */
  float ut;        /* UT date in YYY.MMDD format. */
  short obsmode;   /* Observing mode (Coded). */
  short phase_rec; /* phase and record number for GB records data.
                      Should be -1 for all normal data. */
  short pos_code;  /* Position Code. */
  short unused;
};
/*
Notes:

   The position code was moved to the end so that the alignment on word
    boundaries was still good.  The cookbook descriptions of the
    meaning of position code values is wrong.  The correct code for
    all known position codes (the COORDCD header word) follows:
*/

#define GALACTIC   1    
#define X1950RADC  2    
#define EPOCRADC   3    
#define MEANRADC   4    
#define APPRADC    5    
#define APPHADC    6    
#define X1950ECL   7    
#define EPOCECL    8    
#define MEANECL    9    
#define APPECL    10    
#define AZEL      11    
#define USERDEF   12    
#define X2000RADC 13    
#define INDRADC   14    

/*
     An empty COORDCD string (all blanks) implies 2 (1950RADC).

   The observing mode coding is described corectly in the cookbook.
See the include file ~unipops/test/includes/modes.h for the most
up to date listing of known observing modes and their corresponding
codes.  The file ~unipops/test/source/conline/modefn.c contains some
useful functions for using the information in modes.h.

   The phase_rec field is only used for GB records data currently.
It should be set to -1 for all other data (sdd type = 0 in the bootstrap
record).  If it is not -1, then it corresponds to 64*record_num + phase_num.

   Index entries can not extend beyond the length of a record as
indicated by the bootstrap block (in the current situation, there are
exactly 8 index entries per record, but there is no requirement that
there be exactly N index entries per record [although clearly its
desirable so that space isn't wasted] unused portions of index
records [non-bootstrap and non-data records] are undefined].

*/
