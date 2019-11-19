include ${COMB}/src/main/include.mk
OBJECT=cm.o cmt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../coordsys/C.h ../../stacks/C.h ../../graphics/C.h\
	../../graphics/include.h ../../stacks/include.h\
	../../coordsys/include.h ../../image/C.h
LINTLIBS=../../coordsys/llib-lcoord.ln ../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln ../../main/llib-lmain.ln\
	../../stacks/llib-lstack.ln
CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint :
	lint cm.c cmt_.c $(LINTLIBS)
#cm.o : $(INCL) cm.dc.h cm.ec.h
#cmt_.o : $(INCLP)
cmt_.c cm.dc.h cm.ec.h : cm.t
	../../../bin/entree -C cm.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
