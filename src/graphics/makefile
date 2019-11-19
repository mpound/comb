include ${COMB}/src/main/include.mk
OBJECT=graph.o grid.o guts.o hp.driver.o map.o options.o scale.o show.o\
	step.o\
	tek.driver.o\
	im.driver.o\
	lj.driver.o\
	laser.driver.o
# HP 7580 pen plotter no longer officially supported as of comb 1993.0
# However, the source is still distributed so uncomment next line and 
# the "hp7580" lines in d.routines and d.names to use the pen 
# plotter driver.
#	pp.driver.o hpplot_.o\
#
# Setlayer (5620) no longer supported as of comb 1992.0
#	sl.driver.o\
# ver.driver.o was removed from this list and d.routines and d.names
CFLAGS=-O
#
test : $(OBJECT)
	../../bin/LN_TO_OBJ $?
	touch test
llib : llib-lgraph.ln
llib-lgraph.ln : graph.c grid.c guts.c  map.c options.c scale.c show.c step.c
	lint -Cgraph graph.c grid.c guts.c  map.c options.c scale.c show.c\
		step.c
lint :
	lint -h -u graph.c grid.c guts.c  map.c options.c scale.c show.c \
	step.c ../error/llib-lerror.ln

depend :
	gcc -MM ${OBJECT:.o=.c} > dependencies

include dependencies
#graph.o: C.h include.h ../main/dragon.h
#grid.o:C.h include.h step.h
#guts.o: ../error/C.h
#guts.o: ../graphics/C.h include.h ../main/C.h ../main/dragon.h
#hp.driver.o: C.h include.h
#map.o: C.h include.h
#map.o: ../main/dragon.h
#options.o: C.h include.h ../main/dragon.h
#scale.o: C.h include.h
#scale.o: ../main/dragon.h
#show.o: C.h include.h ../error/C.h d.routines
#show.o: d.names
#step.o:  step.h
#tek.driver.o: C.h include.h
#tek.driver.o: ../main/C.h startqms.h endqms.h
#im.driver.o: C.h
#im.driver.o: include.h ../main/C.h imPcodes.h
#lj.driver.o: include.h ../main/C.h
#laser.driver.o: C.h
#laser.driver.o: include.h ../main/C.h

