include ${COMB}/src/main/include.mk
OBJECT=cat_.o caft_.o ca.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h ../../parse/C.h
LINTLIBS=\
	../../stacks/llib-lstack.ln\
	../../main/llib-lmain.ln\
	../../parse/llib-lparse.ln
CFLAGS=-O
#
test      : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	
	touch test

lint:
	lint -u ca.c cat_.c caft_.c $(LINTLIBS)

#ca.o	: $(INCL) ca.dc.h caf.dc.h ca.ec.h caf.ec.h
#cat_.o	: $(INCLP)
#caft_.o	: $(INCLP)
ca.dc.h ca.ec.h cat_.c : ca.t
	../../../bin/entree -C ca.t
caf.dc.h caf.ec.h caft_.c : caf.t
	../../../bin/entree -C caf.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
