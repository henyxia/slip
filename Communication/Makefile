#Communication lib
all: libcom.a

# La cible de nettoyage
clean: 
	rm -f core *.o libcom.a

#Lib target
libcom.o: libcom.c libcom.h
 
libcom.a: libcom.o
	rm -f $@
	$(CLIB) $@ $+
