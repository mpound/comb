include ${COMB}/src/main/include.mk
SRCS=newfile.c scan2.c ts2.c ws2.c

FILES=btlsdf.doc makefile makehdr newfile.c scan2.c scan2.h scan2.h.base\
	scan2_defs.h scan2_entries.h scan2hdr ts2.c ws2.c
OBJECT=newfile.c scan2.c ts2.c ws2.c

CFLAGS=
#all: test newfile ts2
test      : scan2.o
	../../bin/LN_TO_OBJ $?
	
	touch test

rcp:rcptime

rcptime: $(FILES)
	rcp $? hill:/usr/obs/src/scan
	touch rcptime

newfile: newfile.o scan2.o
	cc newfile.o scan2.o -o newfile

ws2: ws2.o scan2.o
	cc ws2.o scan2.o -o ws2

ts2	:scan2.o ts2.o
	cc ts2.o scan2.o -o ts2

llib : llib_lscan2.ln

llib_lscan2.ln : scan2.c scan2.h
	lint -Cscan2  scan2.c
lint :
	lint -h scan2.c

depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies

scan2_entries.h scan2_defs.h: scan2hdr
	makehdr scan2hdr

scan2.h: scan2_defs.h scan2.h.base
	cat scan2_defs.h scan2.h.base > scan2.h

include dependencies

