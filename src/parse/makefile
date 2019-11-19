include ${COMB}/src/main/include.mk
CFLAGS=-O 
OBJECT=P.default.o P.error.o P.lex.o P.pointer.o P.yacc.o P.doc.o P.headvars.o\
	P.parse.o P.uservars.o P.value.o
LINTSRC=P.default.c P.error.c P.lex.c P.pointer.c P.yacc.c P.doc.c P.headvars.c\
	P.parse.c P.uservars.c P.value.c
#
test : $(OBJECT)
	../../bin/LN_TO_OBJ $?
	touch test
	
lint :
	lint $(LINTSRC)
llib : llib-lparse.ln
llib-lparse.ln : $(LINTSRC)
	lint -Cparse $(LINTSRC)

P.lex.c : P.lex.x
	lex P.lex.x
	cat lex.yy.c | sed 's/= {stdin}//g' | sed 's/= {stdout}//g' > P.lex.c
	/bin/rm lex.yy.c
#       /*mv lex.yy.c P.lex.c */
# mwp 12/12/1999 - this is required to pass -ansi for gcc egcs-2.91.66
# under redhat 6.0.  (if "lex P.lex.x" is run under rh 6.0, it 
# creates uncompilable gobbleygook.)  Lines such as
#
# FILE *yyin = {stdin}
#
# cause the compile time error: "intializer element is not constant"
#
# Apparently, such declarations are indeed non-ANSI, but most compilers
# let you get away with it.  P.lex.c should be in the distribution tar
# file so that installers never run lex.
#

P.yacc.c : P.yacc.y
	yacc P.yacc.y
	mv y.tab.c P.yacc.c

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies

