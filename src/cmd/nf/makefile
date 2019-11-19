include ${COMB}/src/main/include.mk
OBJECT=nf.o nft_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../scan/C.h ../../main/C.h
LINTLIBS=\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../scan/llib-lscan.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u nf.c nft_.c $(LINTLIBS)
#nf.o      : nf.dc.h nf.ec.h $(INCL)
#nft_.o    : $(INCLP)
nf.dc.h nf.ec.h nft_.c : nf.t
	../../../bin/entree -C nf.t
print:	nf.f nf.t
	@pr $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
