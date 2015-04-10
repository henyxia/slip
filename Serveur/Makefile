#Server Makefile
OBJS = serveur.o http.o
LIBS = -lpthread
LOCAL_LIBS  = -L ../Communication -L ../Threads
LOCAL_LIBS += -lcom -lthrd
CFLAGS     += -I ../Communication -I ../Threads

#General target
all: serveur

#Cleaning target

clean: 
	rm -f core *.o serveur

#Output target

serveur: $(OBJS)
	$(CC) $(CFLAGS) -o serveur $(OBJS) $(LOCAL_LIBS) $(LIBS)

serveur.o: serveur.c
http.o: http.c
