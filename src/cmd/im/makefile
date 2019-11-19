include ${COMB}/src/main/include.mk
OBJECT=im.o imt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h  ../../image/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../image/llib-limage.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u im.c imt_.c $(LINTLIBS)

#im.o	: im.dc.h im.ec.h $(INCL)
#
#imt_.o	: $(INCLP)

im.dc.h im.ec.h imt_.c : im.t
	../../../bin/entree -C im.t
print:	im.c im.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
