include ${COMB}/src/main/include.mk
OBJECT=ftt_.o ft.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/C.h ../../main/C.h ../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#ft.o      : ft.dc.h ft.ec.h $(INCL)
#ftt_.o    : $(INCLP)
ft.dc.h ft.ec.h ftt_.c : ft.t
	../../../bin/entree -C ft.t
print:	ft.c ft.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
