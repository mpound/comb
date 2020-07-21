include ${COMB}/src/main/include.mk
OBJECT=wrt_.o wr.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h
CFLAGS=-O
#
#.SILENT:
#test :
#        echo wr is not an active command
# 
# to turn on wr, uncomment the next 4 lines
# NOTE : the token DISABLED is asserted in wr.c by default
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	touch test
	@
wr.dc.h wr.ec.h wrt_.c : wr.t
	../../../bin/entree -C wr.t
print:	wr.c wr.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
