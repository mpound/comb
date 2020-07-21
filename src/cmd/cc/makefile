include ${COMB}/src/main/include.mk
OBJECT=cct_.o cc.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	
	touch test
#cc.o	:\
#	$(INCL)\
#	cc.dc.h\
#	cc.ec.h
#cct_.o	: $(INCLP)
cc.dc.h cc.ec.h cct_.c : cc.t
	../../../bin/entree -C cc.t
print:	cc.c cc.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
