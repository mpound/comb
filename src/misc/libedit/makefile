include ${COMB}/src/main/include.mk
OBJECT=edit.o editlib.o emacs.o fix_hist.o history.o vi.o
LINTSRC=edit.c editlib.c emacs.c fix_hist.c history.c vi.c
# edit the top of history.h for the defines for your system
CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	

lint :
	lint -u $(LINTSRC)
llib : llib-lmisc.ln
llib-lmisc.ln : $(LINTSRC)
	lint -Cmisc $(LINTSRC)

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies
