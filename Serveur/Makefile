#Server Makefile
OBJS = serveur.o http.o udp.o parity.o
LIBS = -lpthread
LOCAL_LIBS  = -L ../Communication -L ../Threads
LOCAL_LIBS += -lcom -lthrd
CFLAGS     += -I ../Communication -I ../Threads -std=gnu99

#General target
all: serveur

#Cleaning target

clean: 
	rm -f core *.o serveur

#Output target

serveur: $(OBJS)
	$(CC) $(CFLAGS) -o serveur $(OBJS) $(LOCAL_LIBS) $(LIBS)

serveur.o: serveur.c
http.o: http.c http.h
udp.o: udp.c udp.h
parity.o: parity.c parity.h
