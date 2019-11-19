include ${COMB}/src/main/include.mk
OBJECT=ex.o ext_.o
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
	lint -u ex.c ext_.c $(LINTLIBS)
#ex.o      : ex.dc.h ex.ec.h $(INCL)
#ext_.o    : $(INCLP)
ex.dc.h ex.ec.h ext_.c : ex.t
	../../../bin/entree -C ex.t
print:	ex.c ex.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
