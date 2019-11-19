include ${COMB}/src/main/include.mk
OBJECT=quit.o quitt_.o
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
	lint -u quit.c quitt_.c $(LINTLIBS)
#quit.o      : quit.dc.h quit.ec.h $(INCL)
#quitt_.o    : $(INCLP)
quit.dc.h quit.ec.h quitt_.c : quit.t
	../../../bin/entree -C quit.t
print:	quit.c quit.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
