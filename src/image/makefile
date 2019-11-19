include ${COMB}/src/main/include.mk
OBJECT= image.o area.o taut.o
CFLAGS=-g
#
test : ${OBJECT}
	../../bin/LN_TO_OBJ $?
	touch test
	
llib : llib-limage.ln
llib-limage.ln : image.c area.c taut.c
	lint -Cimage image.c area.c taut.c
lint :
	lint -h image.c area.c taut.c ../coordsys/llib-lcoord.ln ../main/llib-lmain.ln 

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

#image.o: ../main/machinedep.h ../stacks/C.h C.h ../error/C.h
#image.o: include.h ../main/dragon.h ../coordsys/C.h 
#image.o: fitskeywords.h 
#area.o: ../main/dragon.h ../stacks/C.h C.h ../main/machinedep.h
#taut.o: C.h ../graphics/C.h  ../main/dragon.h

include dependencies
