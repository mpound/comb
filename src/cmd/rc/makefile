include ${COMB}/src/main/include.mk
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../coordsys/C.h ../../stacks/include.h\
	 ../../coordsys/include.h ../../error/C.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../stacks/llib-lstack.ln

OBJECT=rct_.o rc.o
CFLAGS=-O
#
test:	$(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint:
	lint -u rc.c rct_.c $(LINTLIBS)
rct_.c rc.dc.h rc.ec.h rc.uc.h: rc.t
	../../../bin/entree -C rc.t;dc_to_uc rc
#rc.o : rc.dc.h rc.ec.h rc.uc.h $(INCL)
#rct_.o : $(INCLP)
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
