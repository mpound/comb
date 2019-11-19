include ${COMB}/src/main/include.mk
OBJECT=xf.o xft_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h  ../../stacks/include.h ../../coordsys/C.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../scan/llib-lscan.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u xf.c xft_.c $(LINTLIBS)
#xf.o	: xf.dc.h xf.ec.h $(INCL)
#xft_.o	: $(INCLP)
xf.dc.h xf.ec.h xft_.c : xf.t
	../../../bin/entree -C xf.t
print:	xf.c xf.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
