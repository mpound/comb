include ${COMB}/src/main/include.mk
OBJECT=cr.o crt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../graphics/C.h ../../main/C.h ../../parse/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

CFLAGS=-O
#
test      : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u cr.c crt_.c $(LINTLIBS)
#cr.o	: $(INCL) cr.dc.h cr.ec.h
cr.dc.h cr.ec.h crt_.c : cr.t $(INCLP)
	../../../bin/entree -C cr.t
print:	cr.c cr.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
