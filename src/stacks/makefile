include ${COMB}/src/main/include.mk
OBJECT=  S.routines.o  T.routines.o data.o value.o\
	default.o errors.o stk.global.o interp.o search.o
INCL=C.h include.h ../coordsys/C.h
SFCK=../../bin/sfck
CFLAGS=-g
#
test : ${OBJECT}
	../../bin/LN_TO_OBJ $?
	touch test
	

llib : llib_lstack.ln

llib_lstack.ln : S.routines.c  T.routines.c data.c value.c\
		default.c errors.c stk.global.c interp.c search.c
	lint -Cstack  S.routines.c  T.routines.c data.c value.c\
		default.c errors.c stk.global.c interp.c search.c
lint :
	lint -h -u  S.routines.c  T.routines.c data.c value.c\
		default.c errors.c stk.global.c interp.c search.c\
		../coordsys/llib-lcoord.ln

sfck : $(SFCK)
$(SFCK) : sfck.c
	cc -O -o $(SFCK) sfck.c

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies
