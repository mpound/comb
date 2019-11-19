include ${COMB}/src/main/include.mk
OBJECT= combedit.o eshell.o fparse.o fromdec.o fromvax.o\
	indxsn.o jdate.o\
	math_fns.o pltoch.o readline.o\
	scanlistsame.o tran.o wts.o
CFLAGS=-O
LINTSRC= combedit.c eshell.c fparse.c fromdec.c fromvax.c indxsn.c jdate.c\
	math_fns.c pltoch.c readline.c scanlistsame.c tran.c wts.c
#
test : ${OBJECT}
	../../bin/LN_TO_OBJ $?
	touch test
	
lint :
	lint -h -u $(LINTSRC)
llib : llib-lmisc.ln
llib-lmisc.ln : $(LINTSRC)
	lint -Cmisc $(LINTSRC)

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

#combedit.o: ../main/machinedep.h
#indxsn.o: ../stacks/C.h
#pltoch.o: ../main/C.h ../stacks/C.h C.h
#readline.o: /usr/include/stdio.h ../main/C.h ../main/machinedep.h
#scanlistsame.o: ../stacks/C.h
#tran.o: ../main/C.h ../stacks/C.h
#wts.o: /usr/include/stdio.h C.h ../main/C.h ../stacks/C.h

include dependencies
