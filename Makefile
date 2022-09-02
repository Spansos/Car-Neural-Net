CC=gcc
CFLAGS=-g -Wall
SRCFILES = $(wildcard src/*.c)
OBJFILES = $(patsubst src/%.c,obj/%.o,$(SRCFILES))

all: bin/main

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -I include -o $@

bin/main: $(OBJFILES)
	$(CC) $(CFLAGS) $^ $(wildcard libs/*/*) -o $@