include ${COMB}/src/main/include.mk
OBJECT=fo.o fot_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../misc/C.h

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#fo.o      : fo.dc.h fo.ec.h $(INCL)
#fot_.o    : $(INCLP)
fo.dc.h fo.ec.h fot_.c : fo.t
	../../../bin/entree -C fo.t
print:	fo.c fo.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
