include ${COMB}/src/main/include.mk
OBJECT=sc.o sct_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../parse/C.h ../../parse/P.struct.h
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
	lint -u sc.c sct_.c $(LINTLIBS)
#sc.o      : sc.dc.h sc.ec.h $(INCL) $(INCLP)
#sct_.o    : $(INCLP)
sc.dc.h sc.ec.h sct_.c : sc.t
	../../../bin/entree -C sc.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
