include ${COMB}/src/main/include.mk
OBJECT=li.o lit_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#li.o      : li.dc.h li.ec.h $(INCL)
#lit_.o    : $(INCLP)
li.dc.h li.ec.h lit_.c : li.t
	../../../bin/entree -C li.t
print:	li.c li.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
