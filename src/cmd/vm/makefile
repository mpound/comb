include ${COMB}/src/main/include.mk
OBJECT=vm.o vmt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../main/dragon.h ../../stacks/C.h ../../stacks/include.h\
	../../coordsys/C.h ../../image/C.h\
	../../error/C.h ../../misc/C.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../graphics/llib-lgraph.ln\
	../../image/llib-limage.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-O
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u vm.c vmt_.c $(LINTLIBS)
#vm.o	: vm.dc.h vm.ec.h $(INCL)
#vmt_.o	: $(INCLP)
vm.dc.h vm.ec.h vmt_.c : vm.t
	../../../bin/entree -C vm.t
print:	vm.c vm.t
	@pr -l62 $?
	@touch print
depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
