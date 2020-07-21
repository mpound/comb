include ${COMB}/src/main/include.mk
OBJECT=gst_.o gs.o tschns.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../scan/C.h ../../main/machinedep.h\
	../../misc/C.h
CFLAGS=-I/usr/include/cfitsio -I/opt/local/include
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#gs.o      : gs.dc.h gs.ec.h $(INCL)
#gst_.o    : $(INCLP)
gs.dc.h gs.ec.h gst_.c : gs.t
	../../../bin/entree -C gs.t
# The following is a kludge to allow scan number of type old so &+1 works - RWW
# more kludge added to make 'ss' be of type old. (Bob's sed script won't
# allow this).  -- MWP 1/92
#	sed '/_o/d' gt.ec.h > test; mv test gt.ec.h
	sed 's/ssn_o/SSNO/g' gs.ec.h| sed '/_o/d'|sed 's/SSNO/ssn_o/g'>test;mv test gs.ec.h
#tschns.o  : ../../main/C.h ../../parse/C.h ../../stacks/C.h
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
