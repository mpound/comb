include ${COMB}/src/main/include.mk
OBJECT=cmd.o macro.o main.o faerr.o main.global.o
CFLAGS=-O
#
test : $(OBJECT)
	../../bin/LN_TO_OBJ $?
	touch test
	
llib : llib-lmain.ln

llib-lmain.ln : cmd.c macro.c main.c main.global.c
	lint -Cmain cmd.c macro.c main.c main.global.c
lint :
	lint -h cmd.c macro.c main.c faerr.c main.global.c

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies
