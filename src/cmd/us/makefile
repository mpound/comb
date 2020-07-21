include ${COMB}/src/main/include.mk
OBJECT=ust_.o us.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h ../../main/machinedep.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#us.o      : us.dc.h us.ec.h $(INCL)
#ust_.o    : $(INCLP)
us.dc.h us.ec.h ust_.c : us.t
	../../../bin/entree -C us.t
print:	us.c us.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
