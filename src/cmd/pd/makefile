include ${COMB}/src/main/include.mk
OBJECT=pdt_.o pd.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../scan/C.h ../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#pd.o      : pd.dc.h  pd.ec.h $(INCL)
#pdt_.o    : $(INCLP)
pd.dc.h  pd.e pdt_.c : pd.t
	../../../bin/entree -C pd.t
print:	pd.f pd.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
