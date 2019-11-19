include ${COMB}/src/main/include.mk
OBJECT=sl.o slt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../main/dragon.h ../../image/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u sl.c slt_.c $(LINTLIBS)
#sl.o      : sl.dc.h sl.ec.h $(INCL) $(INCLP)
#slt_.o    : $(INCLP)
sl.dc.h sl.ec.h slt_.c : sl.t
	../../../bin/entree -C sl.t
print:	sl.c sl.t
	@sl $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
