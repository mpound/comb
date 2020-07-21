include ${COMB}/src/main/include.mk
OBJECT=sqt_.o sq.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#sq.o      : sq.dc.h sq.ec.h $(INCL)
#sqt_.o    : $(INCLP)
sq.dc.h sq.ec.h sqt_.c : sq.t
	../../../bin/entree -C sq.t
print:	sq.c sq.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
