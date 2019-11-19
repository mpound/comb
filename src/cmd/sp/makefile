include ${COMB}/src/main/include.mk
OBJECT=sp.o spt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../image/C.h ../../main/dragon.h ../../graphics/C.h ../../coordsys/C.h ../../coordsys/include.h
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
	lint -u sp.c spt_.c $(LINTLIBS)
#sp.o      : sp.dc.h sp.ec.h $(INCL) $(INCLP)
#spt_.o    : $(INCLP)
sp.dc.h sp.ec.h spt_.c : sp.t
	../../../bin/entree -C sp.t
print:	sp.c sp.t
	@sp $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
