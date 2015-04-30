#Server Makefile
OBJS = flooder.o
LIBS = -lpthread
LOCAL_LIBS  = -L ../Communication -L ../Threads
LOCAL_LIBS += -lcom -lthrd
CFLAGS     += -I ../Communication -I ../Threads -std=gnu99

#General target
all: flooder

#Cleaning target
clean: 
	rm -f core *.o flooder

#Output target
flooder: $(OBJS)
	$(CC) $(CFLAGS) -o flooder $(OBJS) $(LOCAL_LIBS) $(LIBS)

flooder.o: flooder.c
