include ${COMB}/src/main/include.mk
OBJECT=prt_.o pr.o
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
	lint -u pr.c prt_.c $(LINTLIBS)
#pr.o      : pr.dc.h pr.ec.h $(INCL) $(INCLP)
#prt_.o    : $(INCLP)
pr.dc.h pr.ec.h prt_.c : pr.t
	../../../bin/entree -C pr.t
print:	pr.c pr.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
