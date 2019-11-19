include ${COMB}/src/main/include.mk
OBJECT=af.o aft_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h
CFLAGS=-O
#
test      : af.o aft_.o
	../../../bin/LN_TO_OBJ $?
	
	touch test

lint:
	lint af.c aft_.c ../../main/llib-lmain.ln

#af.o      : af.dc.h af.ec.h $(INCL) $(INCLP)
#aft_.o    : $(INCLP)
af.dc.h af.ec.h aft_.c : af.t
	../../../bin/entree -C af.t
print:	af.c af.t
	@af $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
