include ${COMB}/src/main/include.mk
OBJECT=rst_.o rs.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
FFLAGS=-O -u
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#rs.o      : rs.dc.h rs.ec.h $(INCL)
#rst_.o    : $(INCLP)
rs.dc.h rs.ec.h rst_.c : rs.t
	../../../bin/entree -C rs.t
print:	rs.c rs.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
