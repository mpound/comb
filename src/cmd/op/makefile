include ${COMB}/src/main/include.mk
OBJECT=opt_.o op.o
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
	lint -u op.c opt_.c $(LINTLIBS)
#op.o	:\
#	$(INCL)\
#	op.dc.h\
#	op.ec.h
#opt_.o	:$(INCLP)
op.dc.h  op.ec.h opt_.c : op.t
	../../../bin/entree -C op.t
print:	op.f op.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
