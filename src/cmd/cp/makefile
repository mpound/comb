include ${COMB}/src/main/include.mk
OBJECT=cpt_.o cp.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../coordsys/C.h ../../stacks/C.h\
	../../coordsys/include.h ../../graphics/C.h\
	../../graphics/include.h ../../image/C.h
LINTLIBS=../../coordsys/llib-lcoord.ln ../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln ../../main/llib-lmain.ln\
	../../stacks/llib-lstack.ln
CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint :
	lint cp.c cpt_.c $(LINTLIBS)
#cp.o : $(INCL) cp.dc.h cp.ec.h
#cpt_.o : $(INCLP)
cpt_.c cp.dc.h cp.ec.h : cp.t
	../../../bin/entree -C cp.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
