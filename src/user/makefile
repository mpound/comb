include ${COMB}/src/main/include.mk
CFLAGS=-O
OBJECT=userpg
#
test	: ${OBJECT}
	
	touch test

userpg: userpg.o
	$(CC) userpg.o -lm -o userpg

ftnuser: ftnuser.o ftnsub.o
	f77 ftnuser.o ftnsub.o -o ftnuser

userpg.o ftnuser.o: userpg.h
 
ftnsub.o: ftnuser.h

depend :
#	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies
