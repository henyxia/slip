CC=gcc
CC_FLAGS=-c -Wall -Werror -std=gnu99 -g
CC_LIBS=
INCLUDES=

SOURCES=main.c udp.c parity.c
OBJECTS=$(SOURCES:.c=.o)
OUTPUT=slip

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) $(CC_LIBS) -o $@

%.o: %.c
	$(CC) $(INCLUDES) $(CC_FLAGS) $< -o $@

clear:
	rm -f $(OUTPUT) $(OBJECTS)
