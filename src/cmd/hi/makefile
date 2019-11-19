include ${COMB}/src/main/include.mk
OBJECT=hi.o hit_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../stacks/F.h ../../main/F.h
CFLAGS=-O
#
.SILENT:
test:
	echo Hi is not an active command.
#test      : hi.o hit_.o
#	../../../bin/LN_TO_OBJ $?
#	
#	touch test
#hi.o      : hi.df.h hi.ef.h $(INCL)
#hit_.o    : $(INCLP)
#hi.df.h hi.ef.h hit_.c : hi.t
#	../../../bin/entree hi.t
print:	hi.f hi.t
	@pr $?
	@touch print
.SILENT:
depend:
#	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
