include ${COMB}/src/main/include.mk
OBJECT=da.o dat_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../image/C.h ../../main/dragon.h ../../graphics/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u da.c dat_.c $(LINTLIBS)
#da.o      : da.dc.h da.ec.h $(INCL) $(INCLP)
#dat_.o    : $(INCLP)
da.dc.h da.ec.h dat_.c : da.t
	../../../bin/entree -C da.t
print:	da.c da.t
	@da $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
