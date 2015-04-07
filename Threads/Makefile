#Thread lib
all: libthrd.a

#Cleaning target
clean: 
	rm -f core *.o libthrd.a

#Lib target
libthrd.o: libthrd.c libthrd.h

libthrd.a: libthrd.o
	rm -f $@
	$(CLIB) $@ $+
