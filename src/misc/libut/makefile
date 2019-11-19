include ${COMB}/src/main/include.mk
OBJECT= exchq.o filbts.o ibit.o ifdif.o round.o swab.o
LINTSRC= exchq.c filbts.c ibit.c ifdif.c round.c swab.c
CFLAGS=-O
#
test : $(OBJECT)
	../../../bin/LN_TO_OBJ $?
	touch test
	
lint :
	lint -u $(LINTSRC)
llib : llib-lmisc.ln
llib-lmisc.ln : $(LINTSRC)
	lint -Cmisc $(LINTSRC)

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies 
include dependencies
