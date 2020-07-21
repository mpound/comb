include ${COMB}/src/main/include.mk
OBJECT=tht_.o th.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../main/dragon.h  ../../stacks/C.h ../../coordsys/C.h\
     ../../stacks/include.h ../../error/C.h ../../graphics/C.h\
     ../../misc/C.h ../../image/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../graphics/llib-lgraph.ln\
	../../stacks/llib-lstacks.ln

CFLAGS=-O
#
test      : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u th.c tht_.c $(LINTLIBS)

#th.o	: th.dc.h th.ec.h $(INCL)
#
#tht_.o	: $(INCLP)

th.dc.h th.ec.h tht_.c : th.t
	../../../bin/entree -C th.t
print:	th.c th.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
