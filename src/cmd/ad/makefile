include ${COMB}/src/main/include.mk
OBJECT=ad.o adt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../scan/C.h ../../main/machinedep.h\
	../../misc/C.h
CFLAGS=-O
#
test      : ad.o adt_.o
	../../../bin/LN_TO_OBJ $?
	
	touch test
#ad.o	:$(INCL)\
#	ad.dc.h\
#	ad.ec.h
#adt_.o	:$(INCLP)
ad.dc.h ad.ec.h adt_.c : ad.t
	../../../bin/entree -C ad.t
# The following is a kludge to allow scan number of type old so &+1 works - RWW
# more kludge added to make 'ss' be of type old. (Bob's sed script won't
# allow this).  -- MWP 1/92
#       sed '/_o/d' ad.ec.h > test; mv test ad.ec.h
	sed 's/ssn_o/SSNO/g' ad.ec.h| sed '/_o/d'|sed 's/SSNO/ssn_o/g'>test;mv test ad.ec.h
print:	ad.f ad.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies
