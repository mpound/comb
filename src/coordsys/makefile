include ${COMB}/src/main/include.mk
OBJECT=coord.o coord.global.o
CFLAGS=-O
#
test : $(OBJECT)
	../../bin/LN_TO_OBJ $?
	touch test
	

llib : llib-lcoord.ln

llib-lcoord.ln : coord.c coord.global.c
	lint -Ccoord coord.c coord.global.c

lint : coord.c coord.global.c
	lint -h coord.c coord.global.c ../main/llib-lmain.ln

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

#coord.o: C.h ../error/C.h
#coord.global.o: C.h

include dependencies
