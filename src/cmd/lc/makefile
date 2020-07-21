include ${COMB}/src/main/include.mk
OBJECT=lct_.o lc.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/include.h ../../main/C.h
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
	lint -u lc.c lct_.c $(LINTLIBS)
#lc.o      : lc.dc.h lc.ec.h $(INCL)
#lct_.o    : $(INCLP)
lc.dc.h lc.ec.h lct_.c : lc.t
	../../../bin/entree -C lc.t
print:	lc.c lc.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
