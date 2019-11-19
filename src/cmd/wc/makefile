include ${COMB}/src/main/include.mk
OBJECT=wc.o wct_.o
LINTLIBS=../../coordsys/llib-lcoord.ln ../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln ../../main/llib-lmain.ln\
	../../stacks/llib-lstack.ln
CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint :
	lint wc.c wct_.c $(LINTLIBS)

wct_.c wc.dc.h wc.ec.h : wc.t
	../../../bin/entree -C wc.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
