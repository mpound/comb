include ${COMB}/src/main/include.mk
OBJECT=routt_.o  rint_.o redirect.o
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
	lint -u redirect.c routt_.c rint_.c $(LINTLIBS)
#redirect.o      : rout.dc.h rout.ec.h rout.uc.h  rin.dc.h rin.ec.h $(INCL)
#routt_.o    : $(INCLP)
rout.uc.h rout.dc.h rout.ec.h routt_.c : rout.t
	../../../bin/entree -C rout.t; dc_to_uc rout
#rint_.o    : $(INCLP)
rin.uc.h rin.dc.h rin.ec.h rint_.c : rin.t
	../../../bin/entree -C rin.t; dc_to_uc rin
redirectt_.c : rint_.c routt_.c
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
