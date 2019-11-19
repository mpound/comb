include ${COMB}/src/main/include.mk
OBJECT=pl.o plt_.o flt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../graphics/C.h\
	../../graphics/include.h ../../scan/C.h ../../error/C.h ../../misc/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
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
	lint -u pl.c plt_.c flt_.c $(LINTLIBS)
#pl.o	: pl.dc.h  pl.ec.h pl.uc.h\
#	fl.dc.h fl.ec.h fl.uc.h $(INCL)
#plt_.o	: $(INCLP)
pl.dc.h pl.ec.h pl.uc.h plt_.c : pl.t
	../../../bin/entree -C pl.t; dc_to_uc pl
#flt_.o	: $(INCLP)
fl.dc.h  fl.ec.h fl.uc.h flt_.c : fl.t
	../../../bin/entree -C fl.t; dc_to_uc fl
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
