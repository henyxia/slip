#Server Makefile
OBJS = changeTeam.o
LIBS = -lpthread
LOCAL_LIBS  = -L ../Communication
LOCAL_LIBS += -lcom
CFLAGS     += -I ../Communication -std=gnu99

#General target
all: changeTeam

#Cleaning target
clean:
	rm -f core *.o changeTeam

#Output target
changeTeam: $(OBJS)
	$(CC) $(CFLAGS) -o changeTeam $(OBJS) $(LOCAL_LIBS) $(LIBS)

changeTeam.o: changeTeam.c
