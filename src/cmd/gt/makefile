include ${COMB}/src/main/include.mk
OBJECT=gt.o gtt_.o tschng.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../scan/C.h ../../main/machinedep.h\
	../../misc/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#gt.o      : gt.dc.h gt.ec.h $(INCL)
#gtt_.o    : $(INCLP)
gt.dc.h gt.ec.h gtt_.c : gt.t
	../../../bin/entree -C gt.t
# The following is a kludge to allow scan number of type old so &+1 works - RWW
# more kludge added to make 'ss' be of type old. (Bob's sed script won't
# allow this).  -- MWP 1/92
#	sed '/_o/d' gt.ec.h > test; mv test gt.ec.h
	sed 's/ssn_o/SSNO/g' gt.ec.h| sed '/_o/d'|sed 's/SSNO/ssn_o/g'>test;mv test gt.ec.h
#tschng.o  : ../../main/C.h ../../parse/C.h ../../stacks/C.h
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
