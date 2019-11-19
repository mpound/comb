include ${COMB}/src/main/include.mk
OBJECT=dm.o dmt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../main/include.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

CFLAGS=-O
FFLAGS=-O -u
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u dm.c dmt_.c $(LINTLIBS)
#dm.o      : dm.dc.h dm.ec.h $(INCL)
#dmt_.o    : $(INCLP)
dm.dc.h dm.ec.h dmt_.c : dm.t
	../../../bin/entree -C dm.t
print:	dm.c dm.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
