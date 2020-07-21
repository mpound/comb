include ${COMB}/src/main/include.mk
OBJECT=E.error.o E.family.o E.flocs.o E.fprnt.o E.lex.o E.main.o E.var.o\
	E.wrdef.o E.wrex.o E.wrtre.o E.yacc.o
OOBJECT=E.error.o E.family.o E.flocs.o E.fprnt.o E.lex.o E.main.o E.var.o\
	E.owrdef.o E.owrex.o E.wrtre.o E.yacc.o
INCLF= E.constant.h E.extern.h E.global.h E.struct.h y.tab.h
#CFLAGS=-O
FFLAGS=-O
LINTSRC=
#
test: ../../../bin/entree
	touch test

../../../bin/entree : $(OBJECT)
	${CC} ${CFLAGS} $(OBJECT)
	mv a.out ../../../bin/entree
	

lint :
	lint $(LINTSRC)

E.lex.c : E.lex.x
	lex E.lex.x
#       /*mv lex.yy.c E.lex.c */
# mwp 12/12/1999 - this is required to pass -ansi for gcc egcs-2.91.66
# under redhat 6.0.  (although if "lex E.lex.x" is run under rh 6.0, it
# creates compilable code.)  Lines such as
#
# FILE *yyin = {stdin}
#
# cause the compile time error: "intializer element is not constant"
#
# Apparently, such declarations are indeed non-ANSI, but most compilers
# let you get away with it.  E.lex.c should be in the distribution tar
# file so that installers never run lex.
#
	cat lex.yy.c | sed 's/= {stdin}//g' | sed 's/= {stdout}//g' > E.lex.c
	/bin/rm lex.yy.c
E.yacc.c : E.yacc.y
	yacc E.yacc.y
	mv y.tab.c E.yacc.c
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies

#E.error.o : E.constant.h E.struct.h E.extern.h
#E.family.o : E.constant.h E.struct.h E.extern.h
#E.flocs.o : E.constant.h E.struct.h E.extern.h
#E.fprnt.o : E.constant.h E.struct.h E.extern.h
#E.lex.o : E.constant.h E.struct.h E.extern.h y.tab.h
#E.main.o : E.constant.h E.struct.h ../P.tree.h E.global.h
#E.var.o : E.constant.h E.struct.h E.extern.h
#E.wrdef.o : E.constant.h E.struct.h E.extern.h ../P.tree.h
#E.wrex.o : E.constant.h E.struct.h ../P.tree.h E.extern.h
#E.wrtre.o : E.constant.h E.struct.h E.extern.h ../P.tree.h
#E.yacc.o : ../P.tree.h E.constant.h E.struct.h E.extern.h
include dependencies
