include ${COMB}/src/main/include.mk
OBJECT=v.o vt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint:
	lint -u v.c vt_.c $(LINTLIBS)
#v.o : $(INCL) $(INCLP) v.dc.h v.ec.h
#vt_.o : $(INCLP)
vt_.c v.dc.h v.ec.h : v.t
	../../../bin/entree -C v.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
