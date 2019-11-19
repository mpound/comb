include ${COMB}/src/main/include.mk
OBJECT=file.o alloc.o
CFLAGS=-O
#
test : $(OBJECT)
	../../bin/LN_TO_OBJ $?
	touch test
	
llib :  file.c alloc.c
	lint -Cerror file.c alloc.c
lint :
	lint -h file.c alloc.c ../main/llib-lmain.ln

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

#file.o: C.h
include dependencies
