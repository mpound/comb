include ${COMB}/src/main/include.mk
OBJECT=cot_.o co.o cmbn.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	
	touch test
#co.o      : co.dc.h co.ec.h $(INCL)
#cot_.o    : $(INCLP)
co.dc.h co.ec.h cot_.c : co.t
	../../../bin/entree -C co.t
#cmbn.o    : $(INCL) ../../main/machinedep.h ../../misc/C.h
print:	co.f co.t cmbn.f
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
