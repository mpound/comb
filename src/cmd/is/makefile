include ${COMB}/src/main/include.mk
OBJECT=is.o ist_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../scan/C.h ../../stacks/include.h\
	../../coordsys/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../scan/llib-lscan.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u is.c ist_.c $(LINTLIBS)
#is.o	: is.dc.h is.ec.h $(INCL)
#ist_.o	: $(INCLP)
is.dc.h is.ec.h ist_.c : is.t
	../../../bin/entree -C is.t
print:	is.c is.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
