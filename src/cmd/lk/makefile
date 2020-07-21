include ${COMB}/src/main/include.mk
OBJECT=lkt_.o lk.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../coordsys/C.h ../../stacks/C.h ../../graphics/C.h\
	../../coordsys/include.h ../../stacks/include.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint:
	lint -u lk.c lkt_.c $(LINTLIBS)
#lk.o : $(INCL) lk.dc.h lk.ec.h
#lkt_.o : $(INCLP)
lkt_.c lk.dc.h lk.ec.h : lk.t
	../../../bin/entree -C lk.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
