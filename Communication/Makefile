#
# Makefile pour generer la bibliotheque de fonctions de communication
#

all: libcom.a

#
# La cible de nettoyage
#

clean: 
	rm -f core *.o libcom.a

#
# Les cibles pour la bibliotheque
#

libcom.o: libcom.c libcom.h
 
libcom.a: libcom.o
	rm -f $@
	$(CLIB) $@ $+
