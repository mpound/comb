include ${COMB}/src/main/include.mk
CFLAGS=-O
#OBJECT=get1.o get2.o ieeeflt.o pdfl.o scan.o scan.global.o sdd.o
OBJECT=get1.o get2.o scan.o scan.global.o nrao.o
#
test      : ${OBJECT}
	../../bin/LN_TO_OBJ $?
	
	touch test
llib : llib_lscan.ln

llib_lscan.ln : scan.c get1.c get2.c nrao.c
	lint -Cscan  scan.c get1.c get2.c nrao.c
lint :
	lint -h scan.c get1.c get2.c nrao.c
depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies

#get1.o: C.h ../stacks/C.h ../main/C.h
#get2.o: C.h ../main/C.h ../stacks/C.h
#get2.o: ../coordsys/C.h ../scan2/scan2.h
#pdfl.o: ../main/C.h
#pdfl.o: ../stacks/C.h ../coordsys/C.h ../scan2/scan2.h ../main/machinedep.h
#pdfl.o: ../error/C.h pdfl.h pops.h C.h
#scan.o: C.h include.h ../main/C.h
#scan.o: ../scan2/scan2.h ../stacks/C.h ../parse/P.tree.h ../main/machinedep.h
#scan.o: ../parse/P.struct.h ../parse/P.constant.h
#scan.global.o: C.h
#sdd.o: ../main/C.h ../main/machinedep.h ../stacks/C.h ../coordsys/C.h
#sdd.o: ../scan2/scan2.h ../error/C.h sdd.h modes.h pdfl.h C.h
#ieeeflt.o: ../main/machinedep.h
