CC=gcc
CFLAGS=-g -Wall -lm

SRCFILES = $(wildcard src/*.c)
OBJFILES = $(patsubst src/%.c,obj/%.o,$(SRCFILES))

INCFOLDERS = include /usr/include/SFML
LIBS := $(wildcard /lib/libcsfml-*.so)
LIBS := $(patsubst /lib/lib%.so,-l%,$(LIBS))

.PHONY: clean

all: bin/main run

clean:
	rm obj/* -r
	rm bin/* -r

run: bin/main
	./bin/main

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< $(patsubst %,-I%,$(INCFOLDERS)) -o $@

bin/main: $(OBJFILES)
	$(CC) $(CFLAGS) -L/lib $(LIBS) $^ -o $@