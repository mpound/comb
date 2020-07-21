include ${COMB}/src/main/include.mk
OBJECT=dot_.o wexpt_.o do.o
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
	lint -u do.c dot_.c $(LINTLIBS)
#do.o      : do.dc.h do.ec.h wexp.dc.h wexp.ec.h $(INCL)
#dot_.o    : $(INCLP)
do.dc.h do.ec.h dot_.c : do.t
	../../../bin/entree -C do.t
#wexpt_.o	  : $(INCLP)
wexp.dc.h wexp.ec.h wexpt_.c : wexp.t
	../../../bin/entree -C wexp.t
print:	do.c do.t wexp.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
