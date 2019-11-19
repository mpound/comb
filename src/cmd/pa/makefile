include ${COMB}/src/main/include.mk
OBJECT=pa.o pat_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

CFLAGS=-O
#
test      : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u pa.c pat_.c $(LINTLIBS)
#pa.o      : pa.dc.h pa.ec.h $(INCL)
#pat_.o    : $(INCLP)
pa.dc.h pa.ec.h pat_.c : pa.t
	../../../bin/entree -C pa.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
