#Constants
export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -Wall

#Working Dirs
DIRS=Communication Threads Serveur

#Main Target
all: $(patsubst %, _dir_%, $(DIRS))

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && make

#Cleaning target
clean: $(patsubst %, _clean_%, $(DIRS))

$(patsubst %,_clean_%,$(DIRS)):
	cd $(patsubst _clean_%,%,$@) && make clean

#Debug Target
debug: CFLAGS += -DDEBUG -g
debug: $(patsubst %, _dir_%, $(DIRS))
