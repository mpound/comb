include ${COMB}/src/main/include.mk
CFLAGS=-O
OBJECTS=${COMB}/bin/puthd ${COMB}/bin/unblock ${COMB}/bin/listhd ${COMB}/bin/gethd

test: $(OBJECTS)
	touch test

$(COMB)/bin/puthd: puthd.o
	${CC} $(CFLAGS) puthd.o -o $(COMB)/bin/puthd

$(COMB)/bin/unblock: unblock.o
	${CC} $(CFLAGS) unblock.o -o $(COMB)/bin/unblock

$(COMB)/bin/gethd: listhd
	rm -f $(COMB)/bin/gethd
	ln listhd $(COMB)/bin/gethd

$(COMB)/bin/listhd: listhd
	rm -f $(COMB)/bin/listhd
	ln listhd $(COMB)/bin/listhd

depend:	
	
include dependencies
