include ${COMB}/src/main/include.mk
OBJECT=rmt_.o rm.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#rm.o      : rm.dc.h rm.ec.h $(INCL)
#rmt_.o    : $(INCLP)
rm.dc.h rm.ec.h rmt_.c : rm.t
	../../../bin/entree -C rm.t
print:	rm.c rm.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
