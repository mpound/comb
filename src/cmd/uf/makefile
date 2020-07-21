include ${COMB}/src/main/include.mk
OBJECT=uft_.o uf.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../main/machinedep.h ../../stacks/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u uf.c uft_.c $(LINTLIBS)
uf.o      : uf.dc.h uf.ec.h $(INCL)
uft_.o    : $(INCLP)
uf.dc.h uf.ec.h uft_.c : uf.t
	../../../bin/entree -C uf.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
