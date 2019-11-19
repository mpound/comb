include ${COMB}/src/main/include.mk
OBJECT=bc.o bct_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../scan/C.h ../../main/C.h ../../scan2/scan2.h\
	../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	
	touch test
#bc.o	:\
#	$(INCL)\
#	bc.dc.h\
#	bc.ec.h
#bct_.o	:$(INCLP)
bc.dc.h bc.ec.h bct_.c : bc.t
	../../../bin/entree -C bc.t
print:	bc.f bc.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
