include ${COMB}/src/main/include.mk
OBJECT=lft_.o lf.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	touch test
#lf.o      : lf.dc.h lf.ec.h $(INCL)
#lft_.o    : $(INCLP)
lf.dc.h lf.ec.h lft_.c : lf.t
	../../../bin/entree -C lf.t
print:	lf.c lf.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
