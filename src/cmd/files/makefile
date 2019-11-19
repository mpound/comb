include ${COMB}/src/main/include.mk
OBJECT=rtst.o emt_.o rtt_.o stt_.o nst_.o upt_.o
INCLP=../../parse/P.tree.h  ../../main/machinedep.h
INCL=../../main/C.h ../../stacks/C.h  ../../stacks/include.h ../../coordsys/C.h
LINTLIBS=\
	../../coordsys/llib-lcoord.ln\
	../../error/llib-lerror.ln\
	../../main/llib-lmain.ln\
	../../misc/llib-lmisc.ln\
	../../stacks/llib-lstack.ln

CFLAGS=-g
#
test      : $(OBJECT) 
	../../../bin/LN_TO_OBJ $?
	
	touch test
lint:
	lint -u rtst.c emt_.c rtt_.c stt_.c nst_.c upt_.c $(LINTLIBS)

#emt_.o rtt_.o stt_.o nst_.o upt_.o : $(INCLP)
rt.dc.h rt.ec.h rt.uc.h rtt_.c : rt.t
	../../../bin/entree -C rt.t; dc_to_uc rt
st.dc.h st.ec.h st.uc.h stt_.c : st.t
	../../../bin/entree -C st.t; dc_to_uc st
ns.dc.h ns.ec.h ns.uc.h nst_.c : ns.t
	../../../bin/entree -C ns.t; dc_to_uc ns
em.dc.h em.ec.h em.uc.h emt_.c : em.t
	../../../bin/entree -C em.t; dc_to_uc em
up.dc.h up.ec.h up.uc.h upt_.c : up.t
	../../../bin/entree -C up.t; dc_to_uc up
print:	rtst.c rt.t st.t ns.t up.t
	@pr -l62 $?
	@touch print

depend:
	gcc -MM ${OBJECT:.o=.c} > dependencies
include dependencies
