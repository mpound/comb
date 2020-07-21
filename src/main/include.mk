CC=gcc
CFLAGS=-m32 -O
COMPILE.c=$(CC) -m32 -ansi $(CFLAGS) $(CPPFLAGS) -c

.c.o:
	$(COMPILE.c) $(OUTPUT_OPTION) $<
