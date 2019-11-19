include ${COMB}/src/main/include.mk
OBJECT=el.o elt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#el.o      : el.dc.h el.ec.h $(INCL)
#elt_.o    : $(INCLP)
el.dc.h el.ec.h elt_.c : el.t
	../../../bin/entree -C el.t
print:	el.c el.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
