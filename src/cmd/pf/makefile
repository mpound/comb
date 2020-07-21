include ${COMB}/src/main/include.mk
OBJECT=pft_.o pf.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#pf.o      : pf.dc.h pf.ec.h $(INCL)
#pft_.o    : $(INCLP)
pf.dc.h pf.ec.h pft_.c : pf.t
	../../../bin/entree -C pf.t
print:	pf.c pf.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
