include ${COMB}/src/main/include.mk
OBJECT=tpt_.o tp.o nrutil.o skydip.o mrqcof.o mrqmin.o covsrt.o gaussj.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../coordsys/C.h ../../stacks/include.h\
	../../main/C.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u tp.c tpt_.c $(LINTLIBS)
#tp.o      : tp.dc.h tp.ec.h  $(INCL)
#tpt_.o    : $(INCLP)
tp.dc.h tp.ec.h tpt_.c : tp.t
	../../../bin/entree -C tp.t
print:	tp.c tp.t wexp.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
