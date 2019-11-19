include ${COMB}/src/main/include.mk
OBJECT=me.o met_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../coordsys/C.h ../../graphics/C.h ../../parse/C.h\
	../../stacks/C.h ../../stacks/include.h ../../scan/C.h\
	../../scan2/scan2.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../scan/llib-lscan.ln\
	../../scan2/llib-lscan2.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u me.c met_.c $(LINTLIBS)
#me.o	:\
#	$(INCL)\
#	me.dc.h me.ec.h me.uc.h
#met_.o	:$(INCLP)
me.dc.h me.ec.h me.uc.h met_.c : me.t
	../../../bin/entree -C me.t; dc_to_uc me
print:	me.c me.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
