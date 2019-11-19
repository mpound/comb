include ${COMB}/src/main/include.mk
OBJECT=wu.o wut_.o wuft_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../coordsys/C.h ../../graphics/C.h\
	../../coordsys/include.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../main/llib-lmain.ln

CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint:
	lint -u wu.c wut_.c wuft_.c $(LINTLIBS)

#wu.o : $(INCL) wu.dc.h wu.ec.h wuf.dc.h wuf.ec.h
#wut_.o : $(INCLP)
wut_.c wu.dc.h wu.ec.h : wu.t
	../../../bin/entree -C wu.t

#wuft_.o	:$(INCLP)
wuf.dc.h  wuf.ec.h wuft_.c : wuf.t
	../../../bin/entree -C wuf.t
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
