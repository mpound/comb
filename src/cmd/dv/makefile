include ${COMB}/src/main/include.mk
OBJECT=dvt_.o dv.o
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test

dv.dc.h dv.ec.h dvt_.c : dv.t
	../../../bin/entree -C dv.t

depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies
