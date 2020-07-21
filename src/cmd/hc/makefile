include ${COMB}/src/main/include.mk
OBJECT=hct_.o hc.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../graphics/C.h 
CFLAGS=-O
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln

#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u hc.c hct_.c $(LINTLIBS)
#hc.o      : hc.dc.h  hc.ec.h $(INCL)
#hct_.o    : $(INCLP)
hc.dc.h  hc.ec.h hct_.c : hc.t
	../../../bin/entree -C hc.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
