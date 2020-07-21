include ${COMB}/src/main/include.mk
OBJECT=git_.o gaussianfit.o gi.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#gi.o      : gi.dc.h gi.ec.h $(INCL)
#git_.o    : $(INCLP)
gi.dc.h gi.ec.h git_.c : gi.t
	../../../bin/entree -C gi.t
print:	gi.f gi.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
