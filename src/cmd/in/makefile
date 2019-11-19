include ${COMB}/src/main/include.mk
OBJECT=in.o int_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#in.o      : in.dc.h in.ec.h in.uc.h $(INCL)
#int_.o    : $(INCLP)
in.dc.h in.ec.h in.uc.h int_.c : in.t
	../../../bin/entree -C in.t; dc_to_uc in
print:	in.c in.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
