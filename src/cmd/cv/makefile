include ${COMB}/src/main/include.mk
OBJECT=cv.o cvt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h ../../error/C.h
CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
#cv.o	:$(INCL)\
#	cv.dc.h\
#	cv.ec.h
#cvt_.o	:$(INCLP)
cv.dc.h cv.ec.h cvt_.c : cv.t
	../../../bin/entree -C cv.t
print:	cv.f cv.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
