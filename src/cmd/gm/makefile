include ${COMB}/src/main/include.mk
OBJECT=gmt_.o gm.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../graphics/C.h 
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u gm.c gmt_.c $(LINTLIBS)
#gm.o	:\
#	$(INCL)\
#	gm.dc.h\
#	gm.ec.h
#gmt_.o	:$(INCLP)
gm.dc.h  gm.ec.h gmt_.c : gm.t
	../../../bin/entree -C gm.t
print:	gm.c gm.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
