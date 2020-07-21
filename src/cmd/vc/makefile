include ${COMB}/src/main/include.mk
OBJECT=vct_.o vc.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../coordsys/C.h ../../stacks/C.h ../../graphics/C.h\
	../../graphics/include.h ../../stacks/include.h\
	../../coordsys/include.h ../../image/C.h ../../main/dragon.h\
	../../error/C.h ../../misc/C.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint:
	lint -u vc.c vct_.c $(LINTLIBS)
#vc.o : $(INCL) vc.dc.h vc.ec.h
#vct_.o : $(INCLP)
vct_.c vc.dc.h vc.ec.h : vc.t
	../../../bin/entree -C vc.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
