include ${COMB}/src/main/include.mk
OBJECT=prnnt_.o crnt_.o pht_.o p.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h
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
	lint -u p.c prnnt_.c $(LINTLIBS)
prnn.dc.h prnn.ec.h prnnt_.c : prnn.t
	../../../bin/entree -C prnn.t; dc_to_uc prnn
crn.dc.h crn.ec.h crnt_.c : crn.t
	../../../bin/entree -C crn.t; dc_to_uc crn
ph.dc.h ph.ec.h pht_.c : ph.t
	../../../bin/entree -C ph.t; dc_to_uc ph
print:	p.c prnn.t
	@pr $?
	@touch print
pt_.c : crnt_.c prnnt_.c pht_.c
depend: 
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
