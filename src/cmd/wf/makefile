include ${COMB}/src/main/include.mk
OBJECT=wft_.o wf.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h
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
	lint -u wf.c wft_.c $(LINTLIBS)
#wf.o      : wf.dc.h wf.ec.h $(INCL) $(INCLP)
#wft_.o    : $(INCLP)
wf.dc.h wf.ec.h wft_.c : wf.t
	../../../bin/entree -C wf.t
print:	wf.c wf.t
	@wf $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
