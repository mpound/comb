include ${COMB}/src/main/include.mk
OBJECT=gf.o gft_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#gf.o      : gf.dc.h gf.ec.h $(INCL)
#gft_.o    : $(INCLP)
gf.dc.h gf.ec.h gft_.c : gf.t
	../../../bin/entree -C gf.t
print:	gf.f gf.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
